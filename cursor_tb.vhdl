--------------------------------------------------------------------------------
-- Title       : cursor controller testbench
-- Project     : ELE749
--------------------------------------------------------------------------------
-- File        : cursor_tb.vhdl
-- Author      : Robin
-- Created     : Tue Jul  20 2023
-- Last update : Sun Aug  6 14:31:26 2023
-- Standard    : <VHDL-2008>
--------------------------------------------------------------------------------
-- 
--------------------------------------------------------------------------------
-- Description: Testbench for cursor controller
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library vunit_lib;
context vunit_lib.vunit_context;
library rf_lib;
-----------------------------------------------------------
entity cursor_tb is
	generic (runner_cfg : string);
end entity cursor_tb;
-----------------------------------------------------------
architecture testbench of cursor_tb is

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
	signal fb_readdata    : std_logic_vector( 7 downto 0);
	signal fb_read 	      : std_logic;

	-- Other constants
	constant C_CLK_PERIOD : real := 10.0e-9; -- NS

	--simulation of onchip memory
	type pixel_array is array (24 downto 0) of unsigned(7 downto 0);
	signal on_chip_mem: pixel_array := (    x"18",x"17",x"16",x"15",x"14",
											x"13",x"12",x"11",x"10",x"0F",
											x"0E",x"0D",x"0C",x"0B",x"0A",
											x"09",x"08",x"07",x"06",x"05",
											x"04",x"03",x"02",x"01",x"00" );

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

		-- test scenario 1
		-- draw the cursor at x:160, y: 100
		-- dessin d'une ellipse à x:160, y: 100 de rayon x: 4, y:4
		-- in memory at 0x00000000 (for debug) for pixel buffer
		-- de 640 x 480
		-- expected addres from fb_address are (in hex) if ther is a (x)
		-- they are not drawn in GTKwave and the previous valid address 
		-- will be stretched
		-- FAA0, FAA1, FAA2, FAA3, FAA4,
		-- FD20, FD21, FD22, (x)FD23, (x)FD24,
		-- FFA0, (x)FFA1, FFA2, FFA3, FFA4,
		-- 10220, (x)10221, (x)10222, 10223, 10224,
		-- (x)104A0, v104A1, (x)104A2, (x)104A3, 104A4
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
		rf_writedata <= x"000000A0"; -- x_coordinate = 160
		wait for 10 ns;

		rf_address   <= x"5";
		rf_writedata <= x"00000064"; -- y_coordinate = 100
		wait for 10 ns;

		rf_address   <= x"6";
		rf_writedata <= x"00000000"; -- command = 0 = draw command
		wait for 10 ns;

		rf_address   <= x"7";
		rf_writedata <= x"00000001"; -- start = 1
		wait for 10 ns;

		rf_write <= '0';
		wait for 400 ns;
		

		rf_read    <= '1';
		rf_address <= x"0"; -- read ready
		wait for 10 ns;

		wait for 10 ns; -- read ready again

		rf_address <= x"1"; -- read total
		wait for 10 ns;
		rf_read <= '0';

		--test senario 2
		-- save the value of a 5x5 pixel square in the module memory
		-- x: 160 and y: =1000
		-- in memory at 0x00000000 (for debug) for pixel buffer
		-- of size 640 x 480
		-- the expected value in the cursor_mem array is 
		-- 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
		-- the expected address at fb_address where the read is expected to occure 
		-- are in (hex):
		-- FAA0, FAA1, FAA2, FAA3, FAA4,
		-- FD20, FD21, FD22, FD23, FD24,
		-- FFA0, FFA1, FFA2, FFA3, FFA4,
		-- 10220, 10221, 10222, 10223, 10224,
		-- 104A0, 104A1, 104A2, 104A3, 104A4
		rf_write     <= '1';
		rf_address   <= x"4";
		rf_writedata <= x"000000A0"; -- x_coordinate = 160
		wait for 10 ns;

		rf_address   <= x"5";
		rf_writedata <= x"00000064"; -- y_coordinate = 100
		wait for 10 ns;

		rf_address   <= x"6";
		rf_writedata <= x"00000002"; -- command = 2 = draw command
		wait for 10 ns;

		rf_address   <= x"7";
		rf_writedata <= x"00000001"; -- start = 1
		wait for 10 ns;
		
		rf_write <= '0';
		wait for 10 ns;--wait setup 1

		rf_write <= '0';
		wait for 10 ns;--wait setup 2

		for i in 0 to 24 loop
	    	fb_readdata <= std_logic_vector(on_chip_mem(i));
	    	wait for 10 ns;
	    end loop;
	    wait for 400 ns;
	    rf_read    <= '1';
		rf_address <= x"0"; -- read ready
		wait for 10 ns;

		wait for 10 ns; -- read ready again

		--test senario 3
		-- erase the cursor with the data in its internal memory
		-- x: 160 and y: =1000
		-- in memory at 0x00000000 (for debug) for pixel buffer
		-- of size 640 x 480
		-- the expected value in the cursor_mem array is (we will read on fb_writedata)
		-- 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
		-- the expected address at fb_address where the read is expected to occure 
		-- are in (hex):
		-- FAA0, FAA1, FAA2, FAA3, FAA4,
		-- FD20, FD21, FD22, FD23, FD24,
		-- FFA0, FFA1, FFA2, FFA3, FFA4,
		-- 10220, 10221, 10222, 10223, 10224,
		-- 104A0, 104A1, 104A2, 104A3, 104A4
		rf_write     <= '1';
		rf_address   <= x"4";
		rf_writedata <= x"000000A0"; -- x_coordinate = 160
		wait for 10 ns;

		rf_address   <= x"5";
		rf_writedata <= x"00000064"; -- y_coordinate = 100
		wait for 10 ns;

		rf_address   <= x"6";
		rf_writedata <= x"00000001"; -- command = 2 = erase command
		wait for 10 ns;

		rf_address   <= x"7";
		rf_writedata <= x"00000001"; -- start = 1
		wait for 10 ns;

		rf_write <= '0';
		wait for 400 ns;
		

		rf_read    <= '1';
		rf_address <= x"0"; -- read ready
		wait for 10 ns;

		wait for 10 ns; -- read ready again

		rf_address <= x"1"; -- read total
		wait for 10 ns;
		rf_read <= '0';
			
		test_runner_cleanup(runner);
	end process;

	-----------------------------------------------------------
	-- Entity Under Test
	-----------------------------------------------------------
	DUT : entity work.cursor
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
			fb_waitrequest => fb_waitrequest,
			fb_readdata	   => fb_readdata,
			fb_read 	   => fb_read
		);

end architecture testbench;