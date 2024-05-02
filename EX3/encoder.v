// parameter CLOCKWISE   = 'd 0;
// parameter COUNTERCLOCKWISE = 'd 1;
// parameter PULSES_PER_ROTATION = 2000;   // should be power of 2 in order to save division.
// parameter CLOCKSPEED = 100*10^6;

module quadrature_decoder #(parameter CLOCKSPEED = 2*10^6, PULSES_PER_ROTATION = 2000, COUNTERCLOCKWISE = 'd 1, CLOCKWISE = 'd 0) (
    clock,
    // reset,

    input_A,
    input_B,
    RPS,
    direction
);

input clock;

input input_A;
input input_B;
output reg[31:0] RPS = 0;
output reg direction = 0;

integer clk_count = 0;
integer pulse_count = 0;


always @ (posedge input_A) begin
// if B is already high:
    if (input_B)
        direction = CLOCKWISE;

// if B is not already high:
    else
        direction = COUNTERCLOCKWISE;

// counter for input_A
pulse_count = pulse_count + 1;

end

always @ (posedge clock) begin;
    clk_count = clk_count + 1;

    if (clk_count == CLOCKSPEED) begin
        RPS = pulse_count / PULSES_PER_ROTATION;
        clk_count = 0;
        pulse_count = 0;
    end
end

endmodule


module testbench;
parameter CLOCKWISE   = 'd 0;
parameter COUNTERCLOCKWISE = 'd 1;
parameter PULSES_PER_ROTATION = 2000;   // should be power of 2 in order to save division.
parameter CLOCKSPEED = 100*10^6;

parameter PERIOD = 10;

// initial rotation speed
integer cycles_per_pulse = 4 * CLOCKSPEED; // TODO fix, cant be zero
// initial direction
reg expected_direction = CLOCKWISE;

// instantiate the quadrature decoder module
reg clk = 0;
reg input_A = 0;
reg input_B = 0;
integer RPS;
reg direction;

quadrature_decoder DUT (clk, input_A, input_B, RPS, direction);

// generate the clock
always begin clk = #(PERIOD/2)~clk; end

// begin of simulation
initial begin
    $display($time, " >>> Simulation starting");
    clk = 0;
end

// end of simulation
initial begin #(10 * CLOCKSPEED + 1); // total number of clock cycles to simulate
    $display($time, " >>> Simulation finished");
    $dumpfile("quadrature_decoder_output.vcd");
    $dumpvars;
end


// called every CLOCKSPEED interval
always begin #CLOCKSPEED

    // print the RPS and direction
    $display("RPS=%d, direction=%b", RPS, direction);

    // double the rotation speed
    cycles_per_pulse = cycles_per_pulse / 2;

    // reverse the direction
    expected_direction=~expected_direction;

    // control the A and B wires
    if (expected_direction==CLOCKWISE) begin
        // if CLOCKWISE
        forever input_A = #(cycles_per_pulse/2.0) ~input_A;  // hopefully the forever gets reset every CLOCKSPEED
        #(cycles_per_pulse/4); //to make it 90degrees out of phase with input_A
        forever input_B = #(cycles_per_pulse/2.0) ~input_B;
    end 
    else begin
        // if COUNTERCLOCKWISE
        forever input_B= #(cycles_per_pulse/2.0) ~input_B;  // hopefully the forever gets reset every CLOCKSPEED
        #(cycles_per_pulse/4); //to make it 90degrees out of phase with input_B
        forever input_A= #(cycles_per_pulse/2.0) ~input_A;
    end

end

endmodule