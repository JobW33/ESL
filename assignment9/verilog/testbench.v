module testbench; 

parameter SIM_DATA_WIDTH = 8;
parameter SIMULATION_PHASES = 4;
parameter WORD_TIME = 2 * SIM_DATA_WIDTH;
parameter PERIOD = WORD_TIME * 8;

reg clk = 0;
reg chip_select = 1;
reg MOSI = 0;

led_spi_slave #(.DATA_WIDTH(SIM_DATA_WIDTH)) DUT (clk, chip_select, MOSI, MISO, led, led2, led3);

// begin of simulation
initial begin
    $display(" >>> Simulation starting");
end

initial begin #(SIMULATION_PHASES * PERIOD) // total number of clock cycles to simulate
    $display(" >>> Simulation finished");
    $dumpvars;
    $finish;
end

always begin
    $display("Period 1\n");

    chip_select = 0;
    // 1 word 0x0
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;


    chip_select = 1;
    #WORD_TIME;
    chip_select = 0;
    // 2 word 0x1
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;
    

    chip_select = 1;
    #WORD_TIME;
    chip_select = 1;
    // 3 word 0x1
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;


    #WORD_TIME;
    chip_select = 0;
    // 4 word 0x1
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;


    chip_select = 1;
    #WORD_TIME;
    chip_select = 0;
    // 5 word 0x2
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;

    #WORD_TIME;
    // 6 word 0x1
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;


    #WORD_TIME;
    chip_select = 1;
    // 7 word 0x2
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;


    #WORD_TIME;
    chip_select = 0;
    // 8 word 0x2
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
  
    $display("Period 2\n");

    #WORD_TIME;
    chip_select = 1;
    // 1 word 0x1
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;

    #WORD_TIME;
    // 5 word 0x2
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;


    #WORD_TIME;
    // 6 word 0x1
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;


    
    $display("Period 3\n");

    #WORD_TIME;
    chip_select = 0;
    // 1 word 0xFF
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;


    #WORD_TIME;
    // 2 word 0xF0
    MOSI = 1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    MOSI = 0;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    clk = ~clk;
    #1;
    
    #WORD_TIME;
    #WORD_TIME;
    #WORD_TIME;
    #WORD_TIME;
    #WORD_TIME;
    #WORD_TIME;
  $display("Done\n");

  end
endmodule
