    module testbench;

`timescale 1ns/1ns  

parameter CLOCKWISE   = 'd 0;
parameter COUNTERCLOCKWISE = 'd 1;

parameter PULSES_PER_ROTATION = 2000;   // should be power of 2 in order to save division.
parameter CLOCKSPEED = 1_000_000;       // hz/period
parameter SIM_END = 10 * CLOCKSPEED + 1;// clock cycles

parameter PERIOD = 2;

// log testbench values
// integer fd;
// initial begin
//   fd = $fopen("log.txt","w");
// end

// initial rotation speed
integer cycles_per_pulse = CLOCKSPEED; // Simulate a speed of 0 by having no clock pulses in the span of a second.
// initial direction
reg expected_direction = CLOCKWISE;

// instantiate the quadrature decoder module
reg clk = 0;

reg input_A = 0;
reg input_B = 0;
wire [31:0] RPS;
wire direction;

quadrature_decoder DUT (clk, input_A, input_B, RPS, direction);

// generate the clock
always begin clk = #(PERIOD/2)~clk; end

// begin of simulation
initial begin
    $display(" >>> Simulation starting");
    clk = 0;
end

// called every CLOCKSPEED interval
always begin #(CLOCKSPEED * PERIOD)
    // $display(" >>> Simulation existing");
    // print the RPS and direction
    //$fwrite(fd, "RPS=%d, direction=%b\n", RPS, direction);
    $display("RPμS=%d, direction=%b", RPS, direction);
    // double the rotation speed
    cycles_per_pulse = cycles_per_pulse / 2;

    // reverse the direction
    expected_direction=~expected_direction;
end

// end of simulation
initial begin #(SIM_END * PERIOD) // total number of clock cycles to simulate
    $display(" >>> Simulation finished");
    $dumpfile("quadrature_decoder_output.vcd");
    $dumpvars;
    $finish;
end

// implement A and B direction logic
integer clk_counter = 0;
reg temp1 = 0, temp2 = 0; // Only used in the next procedural block

always begin #PERIOD
    
    // toggle every half period
    if (clk_counter >= cycles_per_pulse / 2) begin
        temp2=~temp2;
        clk_counter = 0;
    end else begin
        clk_counter = clk_counter + 1;
    end

    // 90 degree delay
    if (clk_counter == cycles_per_pulse / 4)
        temp1=~temp1;

    // assign the correct wire to the correct input
    if (expected_direction==CLOCKWISE) begin
        input_A = temp1;
        input_B = temp2;
    end else begin
        input_A = temp2;
        input_B = temp1;
    end

end

endmodule