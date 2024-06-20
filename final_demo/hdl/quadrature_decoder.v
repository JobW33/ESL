// `timescale 1 ps / 1 ps
module quadrature_decoder #(
        parameter DATA_WIDTH = 32
    ) (
		input wire rst,
		
    input wire  input_A,
		input wire  input_B,
	
		output wire [DATA_WIDTH-1:0] cnt_out
    );
	 

// create the counter register and assing it to the cnt_out wire
reg [DATA_WIDTH-1:0] cnt = 0;
assign cnt_out[DATA_WIDTH-1:0] = cnt;

// combine the software and hardware reset
  
always @ (posedge input_A, posedge rst) begin
	if (rst == 1)
		cnt = 0;
	
		// pitch counter
	else if (input_B)
		// if B is already high:
		cnt = cnt + 1'b1;
	else
		// if B is not already high:
		cnt = cnt - 1'b1;
	end

endmodule

