module Steering #(parameter COUNT_SIZE = 11) (
    input wire clk,
    input wire reset,

    input wire direction,
    input wire [COUNT_SIZE-1:0] duty_cycle,

    output reg output_A,
    output reg output_B,
    output reg pwm
);

reg [COUNT_SIZE-1:0] pwm_cnt = 0;

always @ (posedge clk, posedge reset) begin
    pwm_cnt = pwm_cnt + 1;
    
	if (reset == 1) begin
        output_A  = 0;
        output_B  = 0;
		pwm       = 0;
		pwm_cnt   = 0;
    end else begin
		pwm = (pwm_cnt < duty_cycle);
        output_A = direction;
        output_B = ~direction;
    end
end


endmodule

