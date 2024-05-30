`timescale 1 ps / 1 ps
module esl_bus_demo_example #(
        parameter DATA_WIDTH = 32
    ) (
		input wire clk,
		input wire rst,
		
		input wire [DATA_WIDTH-1:0] mem_in,
		output wire [DATA_WIDTH-1:0] pitch_out,
		output wire [DATA_WIDTH-1:0] yaw_out,
		
		input wire  PITCH_ENC_A,
		input wire  PITCH_ENC_B,
		input wire  YAW_ENC_A,
		input wire  YAW_ENC_B,
		
		output reg PITCH_PWM_VAL,	
		output reg PITCH_DIRA,	
		output reg PITCH_DIRB,	
		output reg YAW_PWM_VAL,	
		output reg YAW_DIRA,	
		output reg YAW_DIRB
    );
	 

	 
wire reset;

reg [DATA_WIDTH-1:0] pitch_cnt = 0;
reg [DATA_WIDTH-1:0] yaw_cnt = 0;
assign pitch_out[DATA_WIDTH-1:0] = pitch_cnt;
assign yaw_out[DATA_WIDTH-1:0] = yaw_cnt;

assign reset = (mem_in[0] || rst);
  
always @ (posedge PITCH_ENC_A, posedge reset) begin
	if (reset == 1)
		pitch_cnt = 0;
	
		// pitch counter
	else if (PITCH_ENC_B)
		// if B is already high:
	  pitch_cnt = pitch_cnt + 1;
	else
		// if B is not already high:
		pitch_cnt = pitch_cnt - 1;
	end
	
always @ (posedge YAW_ENC_A, posedge reset) begin
	if (reset == 1)
		yaw_cnt = 0;
	
	// pitch counter
	else if (YAW_ENC_A)
		// if B is already high:
		yaw_cnt = yaw_cnt + 1;
	else
		// if B is not already high:
		yaw_cnt = yaw_cnt - 1;
	end


endmodule

