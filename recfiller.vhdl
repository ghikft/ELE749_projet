library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity recfiller is
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
end entity recfiller;

architecture rtl of recfiller is
	-- signaux pour registres de controle
	signal ready    : std_logic_vector(31 downto 0);
	signal fb_base  : unsigned(31 downto 0);
	signal x_max    : unsigned(15 downto 0);
	signal y_max    : unsigned(15 downto 0);
	signal x_left   : unsigned(15 downto 0);
	signal y_top    : unsigned(15 downto 0);
	signal x_right  : unsigned(15 downto 0);
	signal y_bottom : unsigned(15 downto 0);
	signal color    : std_logic_vector( 7 downto 0);
	signal start    : std_logic_vector(31 downto 0);

	-- signaux pour machine a etat
	type state_type is (idle, setup1, setup2, fill);
	signal state_reg : state_type;

	-- signaux internes
	signal addr  : unsigned(31 downto 0);
	signal x     : unsigned(15 downto 0);
	signal y     : unsigned(15 downto 0);
	signal total : unsigned(31 downto 0);

begin
	process(clk)
	begin
		if rising_edge(clk) then
			if reset = '1' then
				ready     <= (others => '0');
				fb_base   <= (others => '0');
				x_max     <= (others => '0');
				y_max     <= (others => '0');
				x_left    <= (others => '0');
				y_top     <= (others => '0');
				x_right   <= (others => '0');
				y_bottom  <= (others => '0');
				color     <= (others => '0');
				start     <= (others => '0');
				addr      <= (others => '0');
				x         <= (others => '0');
				y         <= (others => '0');
				total     <= (others => '0');
				state_reg <= idle;
			else
				rf_readdata  <= ready;
				fb_write     <= '0';
				fb_writedata <= color;
				--fb_byteenable <= "1111"; -- octet 1 actif (transfer 8 bits) 

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
									when x"1"   => fb_base  <= unsigned(rf_writedata);
									when x"2"   => x_max    <= unsigned(rf_writedata(15 downto 0));
									when x"3"   => y_max    <= unsigned(rf_writedata(15 downto 0));
									when x"4"   => x_left   <= unsigned(rf_writedata(15 downto 0));
									when x"5"   => y_top    <= unsigned(rf_writedata(15 downto 0));
									when x"6"   => x_right  <= unsigned(rf_writedata(15 downto 0));
									when x"7"   => y_bottom <= unsigned(rf_writedata(15 downto 0));
									when x"8"   => color    <= rf_writedata( 7 downto 0);
									when x"9"   => start    <= rf_writedata;
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

					when setup1 =>
						start <= x"00000000";
						x     <= x_left;
						y     <= y_top;

						if y_bottom > y_max then
							y_bottom <= y_max;
						end if;
						if x_right > x_max then
							x_right <= x_max;
						end if;

						addr      <= fb_base + y_top * x_max;
						state_reg <= setup2;

					when setup2 =>
						ready      <= x"00000000";
						fb_write   <= '1';
						fb_address <= std_logic_vector(addr + x);
						state_reg  <= fill;
						total      <= (others => '0');


					when fill =>
						ready      <= x"00000000";
						fb_write   <= '1';

						if fb_waitrequest = '0' then -- previous write accepted, move to next
							if y = y_bottom and x = x_right then
								state_reg <= idle;
							else
								if x = x_right then
									x    <= x_left;
									y    <= y + 1;
									addr <= addr + x_max;
								else
									x <= x + 1;
								end if;
							end if;
							total <= total + 1;
							fb_address <= std_logic_vector(addr + x);
						end if;
				end case;
			end if;
		end if;
	end process;
end architecture; -- rtl of recfiller
