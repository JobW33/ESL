module testbench; 

parameter CLOCKWISE   = 'd 0;
parameter COUNTERCLOCKWISE = 'd 1;
parameter SIM_END = 22;
parameter PERIOD = 4;

reg [7:0] sim_pulse_count = 0;

reg input_A = 0;
reg input_B = 0;
reg reset = 0;

wire [31:0] direction_count;

quadrature_decoder DUT (reset, input_A, input_B, direction_count);

// begin of simulation
initial begin
    $display(" >>> Simulation starting");
    reset = 1;
    #1
    reset = 0;
end

initial begin #((SIM_END * (PERIOD + 2))) // total number of clock cycles to simulate
    $display(" >>> Simulation finished");
    $dumpfile("quadrature_decoder_output.vcd");
    $dumpvars;
    $finish;
end

always begin #(PERIOD + 2)
    // print the current direction count value
    $display("direction_count=%d", direction_count);
    sim_pulse_count = sim_pulse_count + 1;
end

// implement A and B direction logic
integer clk_counter = 0;
reg temp1 = 0, temp2 = 0; // Only used in the next procedural block

always begin #1
    // toggle every half period
    if (clk_counter >= PERIOD / 2) begin
        temp2=~temp2;
        clk_counter = 0;
    end else begin
        clk_counter = clk_counter + 1;
    end

    // 90 degree delay
    if (clk_counter == PERIOD / 4)
        temp1=~temp1;
    
    // reverse the direction halfway through the sim
    if (sim_pulse_count < SIM_END/2) begin
        input_A = temp2;
        input_B = temp1;
    end else begin
        input_A = temp1;
        input_B = temp2;
    end

end

endmodule