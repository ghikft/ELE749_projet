--------------------------------------------------------------------------------
-- Title       : cursor controller 
-- Project     : ELE749
--------------------------------------------------------------------------------
-- File        : cursor.vhdl
-- Author      : Robin
-- Created     : Tue Jul  20 2023
-- Last update : Sun Aug  6 14:31:26 2023
-- Standard    : <VHDL-2008>
--------------------------------------------------------------------------------
-- 
--------------------------------------------------------------------------------
-- Description: Hardware description for cursor controller
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cursor is
	port (
		clk   : in std_logic;
		reset : in std_logic;

		-- interface avalon slave from CPU
		rf_address   : in  std_logic_vector(3 downto 0); -- 4 bits = 16 addr
		rf_write     : in  std_logic;
		rf_writedata : in  std_logic_vector(31 downto 0);
		rf_read      : in  std_logic;
		rf_readdata  : out std_logic_vector(31 downto 0);

		-- interface avalon master to OnChipMEM
		fb_address     : out std_logic_vector(31 downto 0);
		fb_write       : out std_logic;
		fb_writedata   : out std_logic_vector( 7 downto 0);
		fb_waitrequest : in  std_logic;
		fb_read        : out  std_logic;
		fb_readdata    : in std_logic_vector(7 downto 0)
	);
end entity cursor;

architecture rtl of cursor is
	-- signals for control registers
	signal ready    	: std_logic_vector(31 downto 0);
	signal fb_base  	: unsigned(31 downto 0);
	signal x_max    	: unsigned(15 downto 0);
	signal y_max    	: unsigned(15 downto 0);
	signal x_coordinate	: unsigned(15 downto 0);
	signal y_coordinate : unsigned(15 downto 0);
	signal command		: unsigned(7 downto 0);
	signal start    	: std_logic_vector(31 downto 0);

	-- signals for state machine
	type state_type is (idle, setup1, setup2, draw, save, erase);
	signal state_reg : state_type;

	-- Internal signals
	type pixel_array is array (24 downto 0) of unsigned(7 downto 0);
	signal cursor_memory: pixel_array := (  x"FF",x"FF",x"FF",x"FF",x"FF",
											x"FF",x"FF",x"FF",x"FF",x"FF",
											x"FF",x"FF",x"FF",x"FF",x"FF",
											x"FF",x"FF",x"FF",x"FF",x"FF",
											x"FF",x"FF",x"FF",x"FF",x"FF" );

	signal cursor_sprite: pixel_array := (  x"01",x"00",x"00",x"00",x"00",
											x"01",x"01",x"00",x"00",x"01",
											x"00",x"01",x"01",x"00",x"01",
											x"00",x"00",x"01",x"01",x"01",
											x"01",x"01",x"01",x"01",x"01" );

	signal addr  : unsigned(31 downto 0);
	signal x     : unsigned(15 downto 0);
	signal y     : unsigned(15 downto 0);	
	signal x_iter: unsigned(15 downto 0);
	signal y_iter: unsigned(15 downto 0);	
	signal total : unsigned(31 downto 0);
	signal color : unsigned(7 downto 0);
	

begin
	process(clk)
	begin
		if rising_edge(clk) then
			if reset = '1' then--iniitalise signals on reset
				ready     		<= (others => '0');
				fb_base   		<= (others => '0');
				x_max     		<= (others => '0');
				y_max     		<= (others => '0');
				start     		<= (others => '0');
				addr      		<= (others => '0');
				x         		<= (others => '0');
				y         		<= (others => '0');
				x_iter         	<= (others => '0');
				y_iter         	<= (others => '0');
				total    		<= (others => '0');
				state_reg 		<= idle;
			else
				rf_readdata  <= ready;
				fb_write     <= '0';
				fb_read 	 <= '0';
				--start of state machine
				case state_reg is
					when idle =>
						ready <= x"00000001";
						if start = x"00000001" then
							state_reg <= setup1;
							ready     <= x"00000000";
						else
							if rf_write = '1' then
								case rf_address is
									when x"0"   => null;
									when x"1"   => fb_base  	<= unsigned(rf_writedata);
									when x"2"   => x_max    	<= unsigned(rf_writedata(15 downto 0));
									when x"3"   => y_max    	<= unsigned(rf_writedata(15 downto 0));
									when x"4"   => x_coordinate <= unsigned(rf_writedata(15 downto 0));
									when x"5"   => y_coordinate <= unsigned(rf_writedata(15 downto 0));
									when x"6"   => command    	<= unsigned(rf_writedata( 7 downto 0));
									when x"7"   => start    	<= rf_writedata;
									when others => null;
								end case;
							end if;

							if rf_read = '1' then
								case rf_address is
									when x"0"   => rf_readdata <= std_logic_vector(ready);
									when x"1"   => rf_readdata <= std_logic_vector(total);
									when others => null;
								end case;
							end if;
						end if;
					--first setup
					when setup1 =>
						start <= x"00000000";						
						addr      <= fb_base + y_coordinate*x_max;
						x <= x_coordinate;
						y <= y_coordinate;
						x_iter <= x"0000";
						y_iter <= x"0000";
						total      <= (others => '0');
						state_reg <= setup2;
					--second settup
					when setup2 =>
						ready      <= x"00000000";
						fb_address <= std_logic_vector(addr + x);
						case command is	
							when x"00" => state_reg <= draw;
							when x"01" => state_reg <= erase;
							when x"02" => state_reg <= save;
							when others => state_reg <= idle;
						end case;
						
					when draw =>
						ready      <= x"00000000";
						fb_write   <= '1';
						-- previous read accepted, move to next
						if fb_waitrequest = '0' then 
							--at the end of the 5x5 cursor sprite
							if y_iter = 4 and x_iter = 4 then-
								state_reg <= idle;
							else
								--loop on the 5 by 5 square to draw the cursor
								if x_iter = 4 then
									x    <= x_coordinate;
									x_iter <= x"0000";
									y    <= y_coordinate + 1;
									y_iter <= y_iter + 1;
									addr <= addr + x_max;
								else
									x <= x + 1;
									x_iter <= x_iter + 1;
								end if;
							end if;
							total <= total + 1;
							--signal for debug purpose	in test bench
							color <= cursor_sprite(to_integer((x_iter+(y_iter*x"05"))));
							--only update the output to on chip mem if there is a pixel
							--to draw fron the cursor
							if cursor_sprite(to_integer((x_iter+(y_iter*x"05")))) = x"01" then
						 		fb_address <= std_logic_vector(addr + x);
						 		fb_writedata <= (others => '0');--black cursor
							end if;							
						end if;

					when erase =>
						ready      <= x"00000000";
						fb_write   <= '1';
						-- previous write accepted, move to next
						if fb_waitrequest = '0' then 
							--at the end of the 5x5 cursor sprite
							if y_iter = 4 and x_iter = 4 then
								state_reg <= idle;
							else
								--loop on the 5 by 5 square to erase the cursor
								if x_iter = 4 then
									x    <= x_coordinate;
									x_iter <= x"0000";
									y    <= y_coordinate + 1;
									y_iter <= y_iter + 1;
									addr <= addr + x_max;
								else
									x <= x + 1;
									x_iter <= x_iter + 1;
								end if;
							end if;
							total <= total + 1;
							--signal for debug purpose
							color <= cursor_memory(to_integer((x_iter+(y_iter*x"05"))));							
					 		--draw the content of the saved memory bloc of 25 pixels
					 		--which was filled by a previous call with command save
					 		fb_address <= std_logic_vector(addr + x);
					 		fb_writedata <= std_logic_vector(cursor_memory(to_integer((x_iter+(y_iter*x"05")))));
							end if;
					
					when save =>
						ready      <= x"00000000";
						fb_read   <= '1';
						-- previous write accepted, move to next
						if fb_waitrequest = '0' then 
							--at the end of the 5x5 cursor sprite
							if y_iter = 4 and x_iter = 4 then
								state_reg <= idle;
							else
								--loop on the 5 by 5 square to save pixel value
								--in the internal memory
								if x_iter = 4 then
									x    <= x_coordinate;
									x_iter <= x"0000";
									y    <= y_coordinate + 1;
									y_iter <= y_iter + 1;
									addr <= addr + x_max;
								else
									x <= x + 1;
									x_iter <= x_iter + 1;
								end if;
							end if;
							total <= total + 1;													
					 		--read the data of the onChipMemory and save it in the
					 		-- memory bloc of 25 pixels (cursor_memory)
					 		fb_address <= std_logic_vector(addr + x);
					 		cursor_memory(to_integer((x_iter+(y_iter*x"05")))) <= unsigned(fb_readdata);
							end if;
				end case;
			end if;
		end if;
	end process;
end architecture; -- rtl of cursor
