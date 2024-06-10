module Steering #(parameter COUNT_SIZE = 11, parameter DATA_WIDTH = 32) (
    input wire clk,
    input wire rst,

    input wire [DATA_WIDTH-1:0] mem_in,
	output wire debug_out,
    output reg output_A,
    output reg output_B,
    output reg pwm
);

// wires to retreive from mem_in
wire reset;
wire [COUNT_SIZE-1:0] duty_cycle;
wire direction;

assign reset = (mem_in[DATA_WIDTH-1] || rst);
assign duty_cycle = mem_in[COUNT_SIZE-1:0];
assign direction = mem_in[DATA_WIDTH-2];

assign debug_out = pwm_cnt;

reg [COUNT_SIZE-1:0] pwm_cnt = 0;

always @ (posedge clk, posedge reset) begin
  
	if (reset == 1) begin
        output_A  = 0;
        output_B  = 0;
		pwm       = 0;
    end else begin
		pwm_cnt = pwm_cnt + 1;
		pwm = (pwm_cnt < duty_cycle);
        output_A = direction;
        output_B = ~direction;
    end
end


endmodule