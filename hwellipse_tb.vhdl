library ieee;
use ieee.std_logic_1164.all;
library vunit_lib;
context vunit_lib.vunit_context;
library rf_lib;
-----------------------------------------------------------
entity hwellipse_tb is
	generic (runner_cfg : string);
end entity hwellipse_tb;
-----------------------------------------------------------
architecture testbench of hwellipse_tb is

	-- Testbench DUT generics


	-- Testbench DUT ports
	signal clk            : std_logic;
	signal reset          : std_logic;
	signal rf_address     : std_logic_vector(3 downto 0);
	signal rf_write       : std_logic;
	signal rf_writedata   : std_logic_vector(31 downto 0);
	signal rf_read        : std_logic;
	signal rf_readdata    : std_logic_vector(31 downto 0);
	signal fb_address     : std_logic_vector(31 downto 0);
	signal fb_write       : std_logic;
	signal fb_writedata   : std_logic_vector( 7 downto 0);
	signal fb_waitrequest : std_logic;

	-- Other constants
	constant C_CLK_PERIOD : real := 10.0e-9; -- NS

begin
	-----------------------------------------------------------
	-- Clocks and Reset
	-----------------------------------------------------------
	CLK_GEN : process
	begin
		clk <= '1';
		wait for C_CLK_PERIOD / 2.0 * (1 SEC);
		clk <= '0';
		wait for C_CLK_PERIOD / 2.0 * (1 SEC);
	end process CLK_GEN;

	RESET_GEN : process
	begin
		reset <= '1',
			'0' after 5.0*C_CLK_PERIOD * (1 SEC);
		wait;
	end process RESET_GEN;

	-----------------------------------------------------------
	-- Testbench Stimulus
	-----------------------------------------------------------
	main : process
	begin
		test_runner_setup(runner, runner_cfg);
		fb_waitrequest <= '0';
		rf_read        <= '0';
		rf_write       <= '0';

		wait for 50 ns; -- attente du reset

		-- scenario de test 1
		-- dessin d'une ellipse à x:160, y: 100 de rayon x: 4, y:4
		-- dans une memoire situee a 0x00000000 (pour deboguer) pour un pixel buffer
		-- de 640 x 480
		-- les addresses attendues sont (en hex):
		-- faa4, fa9c, fa9c, faa4
		-- fd24, fd1c, f81c, f824
		-- ffa3, ff9d, f59d, f5a3
		-- 104a0, 104a0, f0a0, f0a0
		-- 104a1, 1049f, f09f, f0a1
		-- 10222, 1021e, f31e, f322


		rf_write     <= '1';
		rf_address   <= x"1";
		rf_writedata <= x"00000000";
		wait for 10 ns;

		rf_address   <= x"2";
		rf_writedata <= x"00000280"; -- x_max = 640
		wait for 10 ns;

		rf_address   <= x"3";
		rf_writedata <= x"000001E0"; -- y_max = 480
		wait for 10 ns;

		rf_address   <= x"4";
		rf_writedata <= x"00000064"; -- x_center = 100
		wait for 10 ns;

		rf_address   <= x"5";
		rf_writedata <= x"00000064"; -- y_center = 100
		wait for 10 ns;

		rf_address   <= x"6";
		rf_writedata <= x"00000004"; -- x_radius = 4
		wait for 10 ns;

		rf_address   <= x"7";
		rf_writedata <= x"00000004"; -- y_radius = 4
		wait for 10 ns;

		rf_address   <= x"8";
		rf_writedata <= x"00000055"; -- color = 0x55
		wait for 10 ns;

		rf_address   <= x"9";
		rf_writedata <= x"00000001"; -- start = 1
		wait for 10 ns;

		rf_write <= '0';
		wait for 900 ns;

		rf_read    <= '1';
		rf_address <= x"0"; -- read ready
		wait for 10 ns;

		wait for 10 ns; -- read ready again

		rf_address <= x"1"; -- read total
		wait for 10 ns;
		rf_read <= '0';

		-- scenario de test 1
		-- dessin d'une ellipse à x:100, y: 100 de rayon x: 6, y:4
		-- dans une memoire situee a 0x00000000 (pour deboguer) pour un pixel buffer
		-- de 640 x 480
		-- les addresses attendues sont (en hex):
		--fa6a, fa5e, fa5e, fa6a
		--fcea, fcde, f7de, f7ea
		--ff69, ff5f, f55f, f569
		--10464, 10464, f064, f064
		--410465, 10463, f063, f065
		--10466, 10462, f062, f066
		--101e7, 101e1, f2e1, f2e7
		--101e8, 101e0, f2e0, f2e8


		rf_write     <= '1';
		rf_address   <= x"4";
		rf_writedata <= x"00000064"; -- x_center = 100
		wait for 10 ns;

		rf_address   <= x"5";
		rf_writedata <= x"00000064"; -- y_center = 100
		wait for 10 ns;

		rf_address   <= x"6";
		rf_writedata <= x"00000006"; -- x_radius = 6
		wait for 10 ns;

		rf_address   <= x"7";
		rf_writedata <= x"00000004"; -- y_radius = 4
		wait for 10 ns;

		rf_address   <= x"8";
		rf_writedata <= x"00000088"; -- color = 0x88
		wait for 10 ns;

		rf_address   <= x"9";
		rf_writedata <= x"00000001"; -- start = 1
		wait for 10 ns;
		rf_write <= '0';

		fb_waitrequest <= '1';
		wait for 30 ns;
		fb_waitrequest <= '0';
		wait for 10 ns;
		fb_waitrequest <= '1';
		wait for 20 ns;
		fb_waitrequest <= '0';
		wait for 30 ns;
		fb_waitrequest <= '1';
		wait for 10 ns;
		fb_waitrequest <= '0';

		wait for 100 ns;

		test_runner_cleanup(runner);
	end process;

	-----------------------------------------------------------
	-- Entity Under Test
	-----------------------------------------------------------
	DUT : entity work.hwellipse
		port map (
			clk            => clk,
			reset          => reset,
			rf_address     => rf_address,
			rf_write       => rf_write,
			rf_writedata   => rf_writedata,
			rf_read        => rf_read,
			rf_readdata    => rf_readdata,
			fb_address     => fb_address,
			fb_write       => fb_write,
			fb_writedata   => fb_writedata,
			fb_waitrequest => fb_waitrequest
		);

end architecture testbench;