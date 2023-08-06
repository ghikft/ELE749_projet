library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity hwellipse is
	port (
		clk   : in std_logic;
		reset : in std_logic;

		-- interface avalon slave depuis CPU
		rf_address   : in  std_logic_vector(3 downto 0); -- 4 bits = 16 addr
		rf_write     : in  std_logic;
		rf_writedata : in  std_logic_vector(31 downto 0);
		rf_read      : in  std_logic;
		rf_readdata  : out std_logic_vector(31 downto 0);

		-- interface avalon master vers MEM
		fb_address     : out std_logic_vector(31 downto 0);
		fb_write       : out std_logic;
		fb_writedata   : out std_logic_vector( 7 downto 0);
		fb_waitrequest : in  std_logic
	--fb_byteenable  : out std_logic_vector(3 downto 0)
	);
end entity hwellipse;

architecture rtl of hwellipse is
	-- signaux pour registres de controle
	signal ready    	: std_logic_vector(31 downto 0);
	signal fb_base  	: unsigned(31 downto 0);
	signal x_max    	: unsigned(15 downto 0);
	signal y_max    	: unsigned(15 downto 0);
	signal x_center   	: unsigned(15 downto 0);
	signal y_center    	: unsigned(15 downto 0);
	signal x_radius  	: unsigned(15 downto 0);
	signal y_radius 	: unsigned(15 downto 0);
	signal color    	: std_logic_vector( 7 downto 0);
	signal start    	: std_logic_vector(31 downto 0);

	-- signaux pour machine a etat
	type state_type is (idle, setup1P1, setup2P1, drawP1_1, drawP1_2, drawP1_3, drawP1_4, setup1P2, setup2P2, drawP2_1, drawP2_2, drawP2_3, drawP2_4);
	signal state_reg : state_type;

	-- signaux internes
	signal addr  : unsigned(31 downto 0);
	signal x     : unsigned(15 downto 0);
	signal y     : unsigned(15 downto 0);
	signal xChange : unsigned(15 downto 0);
	signal yChange : unsigned(15 downto 0);
	signal ellipseError : unsigned(15 downto 0);
	signal twoASquare : unsigned(15 downto 0);
	signal twoBSquare : unsigned(15 downto 0);
	signal stoppingX : unsigned(15 downto 0);
	signal stoppingY : unsigned(15 downto 0);
	signal total : unsigned(31 downto 0);

begin
	process(clk)
	begin
		if rising_edge(clk) then
			if reset = '1' then
				ready     		<= (others => '0');
				fb_base   		<= (others => '0');
				x_max     		<= (others => '0');
				y_max     		<= (others => '0');
				x_center    	<= (others => '0');
				y_center     	<= (others => '0');
				x_radius   		<= (others => '0');
				y_radius  		<= (others => '0');
				color     		<= (others => '0');
				start     		<= (others => '0');
				addr      		<= (others => '0');
				x         		<= (others => '0');
				y         		<= (others => '0');
				total    		<= (others => '0');
				state_reg 		<= idle;
			else
				rf_readdata  <= ready;
				fb_write     <= '0';
				fb_writedata <= color;
				--fb_byteenable <= "1111"; -- octet 1 actif (transfer 8 bits) 

				case state_reg is
					when idle =>
						ready <= x"00000001";
						if start = x"00000001" then
							state_reg <= setup1P1;
							ready     <= x"00000000";
						else
							if rf_write = '1' then
								case rf_address is
									when x"0"   => null;
									when x"1"   => fb_base  	<= unsigned(rf_writedata);
									when x"2"   => x_max    	<= unsigned(rf_writedata(15 downto 0));
									when x"3"   => y_max    	<= unsigned(rf_writedata(15 downto 0));
									when x"4"   => x_center   	<= unsigned(rf_writedata(15 downto 0));
									when x"5"   => y_center    	<= unsigned(rf_writedata(15 downto 0));
									when x"6"   => x_radius  	<= unsigned(rf_writedata(15 downto 0));
									when x"7"   => y_radius 	<= unsigned(rf_writedata(15 downto 0));
									when x"8"   => color    	<= rf_writedata( 7 downto 0);
									when x"9"   => start    	<= rf_writedata;
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

					when setup1P1 =>
						start <= x"00000000";
						
						twoASquare <= resize(2*x_radius*x_radius,16);
						twoBSquare <= resize(2*y_radius*y_radius,16);
						x <= x_radius;
						y <= x"0000";
						xChange <= resize(y_radius*y_radius*(1-2*x_radius),16);
						yChange <= resize(x_radius*x_radius,16);
						ellipseError <= x"0000";
						stoppingX <= resize(twoBSquare*x_radius,16);
						stoppingY <= x"0000";

						addr      <= fb_base + (y_center+y) * x_max;
						state_reg <= setup2P1;

					when setup2P1 =>
						ready      <= x"00000000";
						fb_write   <= '1';
						fb_address <= std_logic_vector(addr + (x_center+x));
						state_reg  <= drawP1_1;
						total      <= (others => '0');


					when drawP1_1 =>
						ready      <= x"00000000";
						fb_write   <= '1';

						if fb_waitrequest = '0' then -- previous write accepted, move to next
							if stoppingX < stoppingY then
								state_reg <= setup1P2;
							else
								y <= y + 1;
								stoppingY <= stoppingY+twoASquare;
								ellipseError <= ellipseError+yChange;
								yChange <= yChange+twoASquare;

								if (2*ellipseError+xChange) > 0 then
									x <= x - 1;
									stoppingX <= stoppingX-twoBSquare;
									ellipseError <= ellipseError+xChange;
									xChange <= xChange+twoBSquare;
								end if;
								addr <= fb_base + (y_center+y) * x_max;
								fb_address <= std_logic_vector(addr + (x_center+x));
								state_reg <= drawP1_2;
							end if;
						end if;
					
					when drawP1_2 =>
						state_reg <= drawP1_3;
						addr <= fb_base + (y_center+y) * x_max;
						fb_address <= std_logic_vector(addr + (x_center-x));
					when drawP1_3 =>
						state_reg <= drawP1_4;
						addr <= fb_base + (y_center-y) * x_max;
						fb_address <= std_logic_vector(addr + (x_center-x));
					when drawP1_4 =>
						state_reg <= drawP1_1;
						addr <= fb_base + (y_center-y) * x_max;
						fb_address <= std_logic_vector(addr + (x_center+x));
						
					--Point set 2
					when setup1P2 =>
						start <= x"00000000";
						
						x <= x"0000";
						y <= y_radius;
						xChange <= resize(y_radius*y_radius,16);
						yChange <= resize(x_radius*x_radius*(1-2*y_radius),16);
						ellipseError <= x"0000";
						stoppingX <= x"0000";
						stoppingY <= resize(twoASquare*y_radius,16);

						addr      <= fb_base + (y_center+y) * x_max;
						state_reg <= setup2P2;

					when setup2P2 =>
						ready      <= x"00000000";
						fb_write   <= '1';
						fb_address <= std_logic_vector(addr + (x_center+x));
						state_reg  <= drawP2_1;
						total      <= (others => '0');

					when drawP2_1 =>
						ready      <= x"00000000";
						fb_write   <= '1';

						if fb_waitrequest = '0' then -- previous write accepted, move to next
							if stoppingX > stoppingY then
								state_reg <= idle;
							else
								x <= x + 1;
								stoppingX <= stoppingX+twoBSquare;
								ellipseError <= ellipseError+xChange;
								xChange <= xChange+twoBSquare;

								if (2*ellipseError+yChange) > 0 then
									y <= y - 1;
									stoppingY <= stoppingY-twoASquare;
									ellipseError <= ellipseError+yChange;
									yChange <= yChange+twoASquare;
								end if;
								state_reg <= drawP1_1;
								addr <= fb_base + (y_center+y) * x_max;
								fb_address <= std_logic_vector(addr + (x_center+x));
							end if;
						end if;
					
					when drawP2_2 =>
						state_reg <= drawP1_3;
						addr <= fb_base + (y_center+y) * x_max;
						fb_address <= std_logic_vector(addr + (x_center-x));
					when drawP2_3 =>
						state_reg <= drawP1_4;
						addr <= fb_base + (y_center+y) * x_max;
						fb_address <= std_logic_vector(addr + (x_center-x));
					when drawP2_4 =>
						state_reg <= drawP1_1;
						addr <= fb_base + (y_center+y) * x_max;
						fb_address <= std_logic_vector(addr + (x_center-x));
				end case;
			end if;
		end if;
	end process;
end architecture; -- rtl of hwellipse
