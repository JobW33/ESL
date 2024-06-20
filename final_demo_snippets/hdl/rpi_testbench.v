module testbench; 

parameter WORD_TIME = 16;
parameter SIM_END = 22;
parameter QD_PERIOD = 4;
parameter SPI_PERIOD = WORD_TIME * 8;

reg dir = 0;

// Logic that dictates controls the QD
reg [7:0] sim_pulse_count = 0;
integer clk_counter = 0;
reg temp1 = 0, temp2 = 0;

// system clock
reg clk = 0;

// SPI interface
reg SPI_CLK = 0;
reg SPI_CS;
reg SPI_MOSI = 1;
wire SPI_MISO;

// debug LED
// reg led;

// External signals
reg PITCH_ENC_A;
reg PITCH_ENC_B;
// wire PITCH_PWM_VAL;
// wire PITCH_DIRA;
// wire PITCH_DIRB;

reg YAW_ENC_A;
reg YAW_ENC_B;
// wire YAW_PWM_VAL;
// wire YAW_DIRA;
// wire YAW_DIRB;


rpi_toplevel DUT (  clk, 
                SPI_CLK, SPI_CS, SPI_MOSI, SPI_MISO, 
                led, led2, led3,
                PITCH_ENC_A, PITCH_ENC_B, PITCH_PWM_VAL, PITCH_DIRA, PITCH_DIRB, 
                YAW_ENC_A, YAW_ENC_B, YAW_PWM_VAL, YAW_DIRA, YAW_DIRB
            );

// 50MHz clock
always begin #1
  clk = ~clk;
end

// begin of simulation
initial begin
    $display(" >>> Simulation starting");
end

// end of simulation
initial begin #(SIM_END * SPI_PERIOD) // total number of clock cycles to simulate
    $display(" >>> Simulation finished");
    $dumpvars;
    $finish;
end

always begin #(QD_PERIOD + 2)
    // print the current direction count value
    sim_pulse_count = sim_pulse_count + 1;
end

// simulate input QD
always begin #1
    // toggle every half period
    if (clk_counter >= QD_PERIOD / 2) begin
        temp2=~temp2;
        clk_counter = 0;
    end else begin
        clk_counter = clk_counter + 1;
    end

    // 90 degree delay
    if (clk_counter == QD_PERIOD / 4)
        temp1=~temp1;
    
    // reverse the direction halfway through the sim
    if (sim_pulse_count < SIM_END) begin
        YAW_ENC_A = temp2;
        YAW_ENC_B = temp1;
        PITCH_ENC_A = temp2;
        PITCH_ENC_B = temp1;
    end else begin
        YAW_ENC_A = temp1;
        YAW_ENC_B = temp2;
        PITCH_ENC_A = temp1;
        PITCH_ENC_B = temp2;
    end
end

// simulate SPI input.
always begin
    $display("Period 1\n");
    
    SPI_MOSI = 1;
    SPI_CS = 0;
    // 1 word 0xFF RESET command
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


//    SPI_CS = 1;
//    #WORD_TIME;
//    SPI_CS = 0;
//    // 2 word 0x11 Set pitch duty cycle command ID
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 0;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 0;
//    
//
//    SPI_CS = 1;
//    #WORD_TIME;
//    SPI_CS = 0;
//    // 3 word 0x01 Set pitch direction
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 0;
//
//    SPI_CS = 1;
//    #WORD_TIME;
//    SPI_CS = 0;
//    // 4 word 0x02 upper duty cycle value
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 0;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//
//
//    SPI_CS = 1;
//    #WORD_TIME;
//    SPI_CS = 0;
//    // 5 word 0x40 lower duty cycle value
//    SPI_MOSI = 1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_MOSI = 0;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;
//    SPI_CLK = ~SPI_CLK;
//    #1;

    SPI_CS = 1;
    #WORD_TIME;
    SPI_CS = 0;
    // 6 word 0x12 Get pitch count command ID
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


    #WORD_TIME;
    SPI_CS = 0;
    // 7 word 0x12
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


    #WORD_TIME;
    SPI_CS = 0;
    // 8 word 0x12
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;

    SPI_CS = 1;
    #WORD_TIME;
    SPI_CS = 0;
    // 6 word 0x12 Get pitch count command ID
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


    #WORD_TIME;
    SPI_CS = 0;
    // 7 word 0x12
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


    #WORD_TIME;
    SPI_CS = 0;
    // 8 word 0x12
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;

    SPI_CS = 1;
    #WORD_TIME;
    SPI_CS = 0;
    // 6 word 0x12 Get pitch count command ID
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


    #WORD_TIME;
    SPI_CS = 0;
    // 7 word 0x12
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;


    #WORD_TIME;
    SPI_CS = 0;
    // 8 word 0x12
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_CLK = ~SPI_CLK;
    #1;
    SPI_MOSI = 0;
    
    #WORD_TIME;
end

endmodule
