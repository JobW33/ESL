`timescale 1 ps / 1 ps
module esl_interface #(
        parameter DATA_WIDTH = 32,
		parameter MEM_WIDTH = 16,
		parameter COUNT_SIZE = 11
		
	) (
		input  wire [7:0]  					slave_address,     //      avs_s0.address
		input  wire        					slave_read,        //            .read
		output reg  [DATA_WIDTH-1:0] 		slave_readdata,    //            .readdata
		input  wire        					slave_write,       //            .write
		input  wire [DATA_WIDTH-1:0] 		slave_writedata,   //            .writedata
		input  wire        					clk,          //       clock.clk
		input  wire        					reset,        //       reset.reset
        input  wire [(DATA_WIDTH/8)-1:0] 	slave_byteenable,
		
		// signals going in to the quadrature decoders
		input wire 							PITCH_ENC_A,         // user_input.new_signal
		input wire 							PITCH_ENC_B,         // user_input.new_signal
		input wire 							YAW_ENC_A,         // user_input.new_signal
		input wire 							YAW_ENC_B,         // user_input.new_signal
		
		// signals coming out of the steering modules
		output wire 						PITCH_PWM_VAL,         // user_output.new_signal
		output wire 						PITCH_DIRA,         // user_output.new_signal
		output wire 						PITCH_DIRB,         // user_output.new_signal
		output wire 						YAW_PWM_VAL,         // user_output.new_signal
		output wire 						YAW_DIRA,         // user_output.new_signal
		output wire 						YAW_DIRB         // user_output.new_signal
	);

	// our memory array
	reg [DATA_WIDTH-1:0] data [0:MEM_WIDTH-1];
	
	// QUADRATURE DECODER REGISTERS/WIRES
	//  data[0] QD qd_control					[31]=software_reset
	//  data[1] QD pitch_out					[DATA_WIDTH:0]=counter_output
	//  data[2] QD yaw_out						[DATA_WIDTH:0]=counter_output
	wire [DATA_WIDTH-1:0] qd_control;
	wire [DATA_WIDTH-1:0] pitch_out;
	wire [DATA_WIDTH-1:0] yaw_out;
	assign qd_control[DATA_WIDTH-1:0] = data[0];
	// NOTE: pitch_out and yaw_out are updated in the clocked section of the code below
	
	
	// STEERING REGISTERS/WIRES
	//  data[3] Steering pitch_control   		[31]=software_reset, [30]=dir, [COUNT_SIZE:0]=duty_cycle
	//  data[4] Steering yaw_control 			[31]=software_reset, [30]=dir, [COUNT_SIZE:0]=duty_cycle
	wire [DATA_WIDTH-1:0] pitch_control;
	wire [DATA_WIDTH-1:0] yaw_control;
	assign pitch_control[DATA_WIDTH-1:0] = data[3];
	assign yaw_control[DATA_WIDTH-1:0] = data[4];
	
	// DEBUG BECAUSE WE R STUPID
	wire [DATA_WIDTH-1:0] DEBUG_YAW;
	wire [DATA_WIDTH-1:0] DEBUG_PITCH;

	
	
    quadrature_decoder #( .DATA_WIDTH(DATA_WIDTH)
    ) pitch_decoder (
		.rst(reset),
		.mem_in(qd_control),
		.cnt_out(pitch_out),
		.input_A(PITCH_ENC_A),
		.input_B(PITCH_ENC_B)
    );
	
	quadrature_decoder #( .DATA_WIDTH(DATA_WIDTH)
    ) yaw_decoder (
		.rst(reset),
		.mem_in(qd_control),
		.cnt_out(yaw_out),
		.input_A(YAW_ENC_A),
		.input_B(YAW_ENC_B)
    );

	Steering #( .COUNT_SIZE(COUNT_SIZE), .DATA_WIDTH(DATA_WIDTH)
	) pitch_controller (
		.clk(clk), 
		.rst(reset),
		.mem_in(pitch_control),  
		.debug_out(DEBUG_PITCH),
		.output_A(PITCH_DIRA), 
		.output_B(PITCH_DIRB), 
		.pwm(PITCH_PWM_VAL)
	);
	
	Steering #( .COUNT_SIZE(COUNT_SIZE), .DATA_WIDTH(DATA_WIDTH)
	) yaw_controller (
		.clk(clk), 
		.rst(reset),
		.mem_in(yaw_control), 
		.debug_out(DEBUG_YAW),
		.output_A(YAW_DIRA), 
		.output_B(YAW_DIRB), 
		.pwm(YAW_PWM_VAL)
	);


	integer j;
	
    always @(posedge clk or posedge reset) begin
        if (reset) begin
			for (j=0; j < MEM_WIDTH; j=j+1) begin
				data[j] <= 0;
			end
        end else begin
		  
				// update the registers to the new output values
				data[1] = pitch_out;
				data[2] = yaw_out;
				data[5] = DEBUG_YAW;
				data[6] = DEBUG_PITCH;
				
            if (slave_read) begin
                slave_readdata <= data[slave_address];
            end
            if (slave_write) begin
                data[slave_address] <= slave_writedata;
            end;
        end;
		
    end

endmodule
