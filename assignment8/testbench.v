module testbench; 

parameter TEST_COUNT_SIZE = 4;
parameter SIM_END = 4;
parameter PERIOD = 1 << (2 * TEST_COUNT_SIZE + 2) - 1; // 2^(TEST_COUNT_SIZE + 1) using bit shift
parameter QUARTER = (1 << TEST_COUNT_SIZE) / 4; // 0.25 * 2^(TEST_COUNT_SIZE) as an integer division


reg clk = 1;
reg reset = 0;
reg dir = 0;

reg [TEST_COUNT_SIZE - 1:0] percentage_high = 0;
wire [31:0] mem_in;

assign mem_in[31] = reset;
assign mem_in[30] = dir;
assign mem_in[TEST_COUNT_SIZE - 1:0] = percentage_high;

Steering #(.COUNT_SIZE(TEST_COUNT_SIZE)) DUT (clk, reset, mem_in, output_A, output_B, pwm);

// begin of simulation
initial begin
    $display(" >>> Simulation starting");
    reset = 1;
    #1
    reset = 0;
end

initial begin #(SIM_END * PERIOD) // total number of clock cycles to simulate
    $display(" >>> Simulation finished");
    $dumpvars;
    $finish;
end

always begin #(PERIOD)
    // update and print the percentage_high value
    percentage_high = percentage_high + QUARTER;
    dir = ~dir;
    $display("percentage_high=%d", percentage_high);
end

always begin #1
  clk = ~clk;
end

endmodule
