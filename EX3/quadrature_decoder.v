module quadrature_decoder #(parameter CLOCKSPEED = 1_000_000, PULSES_PER_ROTATION = 2000, COUNTERCLOCKWISE = 'b 1, CLOCKWISE = 'b 0) (
    clock,
    // reset,

    input_A,
    input_B,
    RPMS,
    direction
);

input clock;

input input_A;
input input_B;
output integer RPMS = 0;
output reg direction = 0;

integer clk_count = 0;
integer pulse_count = 0;
reg reset_pulse_count = 0;

always @ (posedge input_A) begin
    // if B is already high:
        if (input_B)
            direction = CLOCKWISE;

    // if B is not already high:
        else
            direction = COUNTERCLOCKWISE;

    // counter for input_A
    pulse_count = pulse_count + 1;
    if (reset_pulse_count)
        pulse_count = 1;
end

always @ (posedge clock) begin
    clk_count = clk_count + 1;
    

    if (clk_count == CLOCKSPEED) begin
        RPMS = pulse_count / (PULSES_PER_ROTATION/1000);
        clk_count = 0;
        reset_pulse_count = 1;
    end else if (pulse_count == 1)
        reset_pulse_count = 0;
end

endmodule
