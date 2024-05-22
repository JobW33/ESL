module pwm #(parameter COUNT_SIZE = 8) (
    clk,
    reset,

    percentage_high,

    output_A,
    output_B,
    pwm
);

input clk;
input reset;

input [COUNT_SIZE - 1:0] percentage_high;

output reg output_A;
output reg output_B;
output reg pwm;

reg [COUNT_SIZE - 1:0] pwm_count = 0;

always @ (posedge clk, posedge reset) begin
  pwm_count = pwm_count + 1;
	output_A = ~output_A;
	output_B = ~output_B;
  
  if (reset == 1) begin
		  output_A  = 0;
		  output_B  = 0;
		  pwm       = 0;
		  pwm_count = 0;
  end else if (pwm_count == percentage_high)
      pwm = 0;
  else if (pwm_count == 0)
      pwm = 1;
end


endmodule
