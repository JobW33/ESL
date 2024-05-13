module quadrature_decoder #(parameter CLOCKSPEED = 1_000_000, PULSES_PER_ROTATION = 2000) (
    reset,
    input_A,
    input_B,
    direction_count
);

input reset;

input input_A;
input input_B;
output integer direction_count;

always @ (posedge input_A, posedge reset) begin
	 if (reset == 1)
		  direction_count = 0;
    else if (input_B)
        // if B is already high:
        direction_count = direction_count + 1;
    else
        // if B is not already high:
        direction_count = direction_count - 1;
end


endmodule
