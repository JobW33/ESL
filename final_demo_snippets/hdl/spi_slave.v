module spi_slave #(parameter MSG_WIDTH = 8, PWM_DATA_WIDTH = 16, QD_DATA_WIDTH = 16) (
    input wire SPI_CLK,
    input wire SPI_CS,
    input wire SPI_MOSI,
    output reg SPI_MISO = 0,

    output reg led = 1,
    output reg led2 = 1,
    output reg led3 = 1,
    output reg rst = 1,

    input wire [QD_DATA_WIDTH-1:0] YAW_COUNT,
    input wire [QD_DATA_WIDTH-1:0] PITCH_COUNT,
    output reg [PWM_DATA_WIDTH-1:0] YAW_PWM = 0,
    output reg [PWM_DATA_WIDTH-1:0] PITCH_PWM = 0
  );

  
  reg [MSG_WIDTH-1:0] serial_data_in = 0;    // data received from the master device
  reg [QD_DATA_WIDTH-1:0] serial_data_out = 0;   // data to output
  
  // stores up to 4 words we received so it can handle all commands data
  reg [MSG_WIDTH-1:0] command_data [3:0];

  // counts the amount of bits we have received from the current word
  reg [3:0] bit_counter = 0;

  // counts the amount of words we have received for the current command
  reg [3:0] word_counter = 0;

  // flag whether we hould output data
  reg write_enable = 0;

  // states
  localparam [MSG_WIDTH-1:0] RESET_CMD      ='hFF;
  localparam [MSG_WIDTH-1:0] SET_PITCH_CMD  ='h11;
  localparam [MSG_WIDTH-1:0] GET_PITCH_CMD  ='h12;
  localparam [MSG_WIDTH-1:0] SET_YAW_CMD    ='h21;
  localparam [MSG_WIDTH-1:0] GET_YAW_CMD    ='h22;
  localparam [MSG_WIDTH-1:0] TOGGLE_LED_CMD ='h31;
  reg [MSG_WIDTH-1:0] commandState = 1;

  always @(posedge SPI_CLK)
  begin

      // reset bit and word counter when slave select goes high
      if (SPI_CS==1) begin
        bit_counter <= 0;
      end

      else if (SPI_CS==0) begin

        // every new clock cycle
        // increment word counter and read input
        bit_counter <= bit_counter + 1'b1;
        serial_data_in[MSG_WIDTH-1 - bit_counter[2:0]] = SPI_MOSI;

        // handle writing data to the output
        // if write_enable, output the data in serial_data_out
        if ((write_enable == 1) && (bit_counter != QD_DATA_WIDTH-1)) begin
          SPI_MISO <= serial_data_out[QD_DATA_WIDTH-2 - bit_counter];
        end
        
        // every 8 clock cycles
        // handle newly received bytes
        if (bit_counter[2:0] == MSG_WIDTH-1) begin
          command_data[word_counter] = serial_data_in;


          // if this is the first word, it indicates the command
          if(word_counter == 0) begin
            commandState = serial_data_in;
          end
          
          word_counter = word_counter + 1'b1;
          rst = 0;
          led2 = 0;
          led3 = 0;

          case (commandState)
            /*
            *   Command 0xFF
            *   Reset the hardware
            *   1 byte received
            */
            RESET_CMD:
              if(word_counter == 1) begin
                rst <= 1;
                write_enable <= 0;
                word_counter = 0;
		led <= 0;
		led3 <= 1;
              end

            /*
            *   Command 0x11
            *   Set the duty cycle and direction of the pitch PWM controller
            *   4 bytes received. 1:CommandID, 2:direction, 3: upper PWM value, 4: lower PWM value
            */
            SET_PITCH_CMD:
              if(word_counter == 4) begin
                // rst=0, dir=command_data[1][0], duty_cycle=command_data[2][5:0] + command_data[3]
                PITCH_PWM <= {1'b0, command_data[1][0], command_data[2][5:0], command_data[3]};
                write_enable <= 0;
                word_counter = 0;
              end

            /*
            * Command 0x12
            * Get the current pitch count from the pitch QD
            * 1 byte received.  1: CommandID
            * 2 byte send.      1: upper count value, 2: lower PWM value
            */
            GET_PITCH_CMD:
              if(word_counter == 1) begin
                serial_data_out <= PITCH_COUNT;
                SPI_MISO <= serial_data_out[MSG_WIDTH-1];
                write_enable <= 1;
                word_counter = 0;
              end

            /*
            *   Command 0x21
            *   Set the duty cycle and direction of the yaw PWM controller
            *   4 bytes received. 1:CommandID, 2:direction, 3: upper PWM value, 4 lower PWM value
            */
            SET_YAW_CMD:
              if(word_counter == 4) begin
                // rst=0, dir=command_data[1][0], duty_cycle=command_data[2][5:0] + command_data[3]
                YAW_PWM <= {1'b0, command_data[1][0], command_data[2][5:0], command_data[3]};
                write_enable <= 0;
                word_counter = 0;
              end

            /*
            * Command 0x22
            * Get the current yaw count from the yaw QD
            * 1 byte received.  1: CommandID
            * 2 byte send.      1: upper count value, 2: lower PWM value
            */
            GET_YAW_CMD:
              if(word_counter == 1) begin
                serial_data_out <= YAW_COUNT;
                SPI_MISO <= serial_data_out[MSG_WIDTH-1];
                write_enable <= 1;
                word_counter = 0;
              end

            /*
            * Command 0x31
            * Toggles the on board LED
            * 1 byte received.  1: CommandID
            */
            TOGGLE_LED_CMD:
              if(word_counter == 1) begin
                led <= ~led;
                write_enable <= 0;
                word_counter = 0;
              end

            /*
            * You're not supposed to be here
            */
            default: begin
              led2 <= 1;
              word_counter = 0;
            end
          endcase
        end
      end
    end

endmodule
