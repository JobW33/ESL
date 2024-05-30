`timescale 1 ps / 1 ps
module esl_bus_demo #(
        parameter DATA_WIDTH = 32,
		parameter MEM_WIDTH = 16
	) (
		input  wire [7:0]  					slave_address,     //      avs_s0.address
		input  wire        					slave_read,        //            .read
		output reg  [DATA_WIDTH-1:0] 		slave_readdata,    //            .readdata
		input  wire        					slave_write,       //            .write
		input  wire [DATA_WIDTH-1:0] 		slave_writedata,   //            .writedata
		input  wire        					clk,          //       clock.clk
		input  wire        					reset,        //       reset.reset
        input  wire [(DATA_WIDTH/8)-1:0] 	slave_byteenable,
		
		input wire 							PITCH_ENC_A,         // user_input.new_signal
		input wire 							PITCH_ENC_B,         // user_input.new_signal
		input wire 							YAW_ENC_A,         // user_input.new_signal
		input wire 							YAW_ENC_B,         // user_input.new_signal
		
		output wire 						PITCH_PWM_VAL,         // user_output.new_signal
		output wire 						PITCH_DIRA,         // user_output.new_signal
		output wire 						PITCH_DIRB,         // user_output.new_signal
		output wire 						YAW_PWM_VAL,         // user_output.new_signal
		output wire 						YAW_DIRA,         // user_output.new_signal
		output wire 						YAW_DIRB         // user_output.new_signal
	);

	// our memory array
	reg [DATA_WIDTH-1:0] data [0:MEM_WIDTH-1];
	
	// Create named wires for ease of use
	wire [DATA_WIDTH-1:0] mem_in;
	wire [DATA_WIDTH-1:0] pitch_out;
	wire [DATA_WIDTH-1:0] yaw_out;
	
	// assign the read_only data to the correct wire
	assign mem_in[DATA_WIDTH-1:0] = data[0];
	
    // Definition of the counter
    esl_bus_demo_example #(
        .DATA_WIDTH(DATA_WIDTH)
    ) my_ip (
		.clk(clk),
		.rst(reset),
		
		.mem_in(mem_in),
		.pitch_out(pitch_out),
		.yaw_out(yaw_out),
		
		.PITCH_ENC_A(PITCH_ENC_A),
		.PITCH_ENC_B(PITCH_ENC_B),
		.YAW_ENC_A(YAW_ENC_A),
		.YAW_ENC_B(YAW_ENC_B),
		.PITCH_PWM_VAL(PITCH_PWM_VAL),
		.PITCH_DIRA(PITCH_DIRA),
		.PITCH_DIRB(PITCH_DIRB),
		.YAW_PWM_VAL(YAW_PWM_VAL),
		.YAW_DIRA(YAW_DIRA),
		.YAW_DIRB(YAW_DIRB)
    );


	integer j;
	
    always @(posedge clk or posedge reset) begin
        if (reset) begin
			for (j=0; j < MEM_WIDTH; j=j+1) begin
				data[j] <= 0;
			end
        end else begin
		  
				// update the registers to the ne output values
				data[1] = pitch_out;
				data[2] = yaw_out;

		  
		  
            if (slave_read) begin
                slave_readdata <= data[slave_address];
            end
            if (slave_write) begin
                data[slave_address] <= slave_writedata;
            end;
        end;
		
    end

endmodule