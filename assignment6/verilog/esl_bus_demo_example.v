`timescale 1 ps / 1 ps
module esl_bus_demo_example #(
        parameter DATA_WIDTH = 8
    ) (
        input wire clk,
        input wire rst,
        input wire [DATA_WIDTH-1:0] in,
		input wire cnt_enable,
        output reg [DATA_WIDTH-1:0] out	
    );
	
wire input_A;
wire input_B;
wire reset;

assign input_A = in[0];
assign input_B = in[1];
assign reset = (in[2] || rst);
	
always @ (posedge input_A, posedge reset) begin
	 if (reset == 1)
		  out = 0;
    else if (input_B)
        // if B is already high:
        out = out + 1;
    else
        // if B is not already high:
        out = out - 1;
end

endmodule
