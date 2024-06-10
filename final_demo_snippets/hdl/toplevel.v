module toplevel #(parameter 
    MSG_WIDTH = 8,
    PWM_DATA_WIDTH = 16,
    QD_DATA_WIDTH = 16,
    PWM_COUNT_SIZE = 11,
    QD_CLOCK_SPEED = 1_000_000,
    QD_PULSES_PER_ROTATION = 2000
) (
    // system clock
    input wire clk,
    
    // SPI interface
    input wire SPI_CLK,
    input wire SPI_CS,
    input wire SPI_MOSI,
    output wire SPI_MISO,

    // debug LED
    output wire led,
    
    // External signals
    input wire PITCH_ENC_A,
    input wire PITCH_ENC_B,
    output wire PITCH_PWM_VAL,
    output wire PITCH_DIRA,
    output wire PITCH_DIRB,

    input wire YAW_ENC_A,
    input wire YAW_ENC_B,
    output wire YAW_PWM_VAL,
    output wire YAW_DIRA,
    output wire YAW_DIRB
);

    // internal signals
    wire [QD_DATA_WIDTH-1:0] QD_YAW_direction_count;
    wire [QD_DATA_WIDTH-1:0] QD_PITCH_direction_count;
    wire [PWM_DATA_WIDTH-1:0] PWM_YAW_mem_in;
    wire [PWM_DATA_WIDTH-1:0] PWM_PITCH_mem_in;
    wire rst;

spi_slave #(
    .MSG_WIDTH          (MSG_WIDTH),
    .PWM_DATA_WIDTH     (PWM_DATA_WIDTH),
    .QD_DATA_WIDTH      (QD_DATA_WIDTH) 
) u_SPI (
    .SPI_CLK            (SPI_CLK),
    .SPI_CS             (SPI_CS),
    .SPI_MOSI           (SPI_MOSI),
    .SPI_MISO           (SPI_MISO),

    .led                (led),
    .rst                (rst),

    .YAW_COUNT          (QD_YAW_direction_count),
    .PITCH_COUNT        (QD_PITCH_direction_count),
    .YAW_PWM            (PWM_YAW_mem_in),
    .PITCH_PWM          (PWM_PITCH_mem_in)
);

quadrature_decoder #(
    .DATA_WIDTH         (QD_DATA_WIDTH)
) u_QD_YAW (
    .clk                (clk),
    .rst                (rst),
    .input_A            (YAW_ENC_A),
    .input_B            (YAW_ENC_B),
    .cnt_out            (QD_YAW_direction_count)
);

quadrature_decoder #(
    .DATA_WIDTH         (QD_DATA_WIDTH)
) u_QD_PITCH (
    .clk                (clk),
    .rst                (rst),
    .input_A            (PITCH_ENC_A),
    .input_B            (PITCH_ENC_B),
    .cnt_out            (QD_PITCH_direction_count)
);

Steering #(
    .COUNT_SIZE         (PWM_COUNT_SIZE),
    .DATA_WIDTH         (PWM_DATA_WIDTH)
) u_PWM_YAW (
    .clk                (clk),
    .rst                (rst),
    .mem_in             (PWM_YAW_mem_in),
    .output_A           (YAW_DIRA),
    .output_B           (YAW_DIRB),
    .pwm                (YAW_PWM_VAL)
);

Steering #(
    .COUNT_SIZE         (PWM_COUNT_SIZE),
    .DATA_WIDTH         (PWM_DATA_WIDTH)
) u_PWM_PITCH (
    .clk                (clk),
    .rst                (rst),
    .mem_in             (PWM_PITCH_mem_in),
    .output_A           (PITCH_DIRA),
    .output_B           (PITCH_DIRB),
    .pwm                (PITCH_PWM_VAL)
);

endmodule