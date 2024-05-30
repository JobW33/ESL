module led_spi_slave #(parameter DATA_WIDTH = 8) (
    input wire rst,
    input wire clk,
    input wire chip_select,
    input wire MOSI,
    
    output reg MISO = 0,
    output reg led = 0
  );

  reg [DATA_WIDTH-1:0] serial_data_in = 0;    // data received from the master device
  reg [DATA_WIDTH-1:0] serial_data_out = 0;   // data to output
  reg [DATA_WIDTH-1:0] cnt = 0;               // counter that tracks the amount of toggles

  reg [2:0] word_counter;
  reg write_enable;

  always @(posedge clk, posedge rst)
  begin

    if (rst == 1) begin
      serial_data_in <= 0;
      serial_data_out <= 0;
      word_counter <= 0;
      write_enable <= 0;
    end else begin
      // reset word counter (just in case)
      if (chip_select==1) begin
        word_counter <= 0;  
      end

      else if (chip_select==0) begin
        // increment word counter and read input
        word_counter <= word_counter + 1;
        serial_data_in[DATA_WIDTH-1 - word_counter] <= MOSI;

        
        // if write_enable, output the data in serial_data_out
        if ((write_enable == 1) && (word_counter != DATA_WIDTH-1)) begin
          MISO <= serial_data_out[DATA_WIDTH-2 - word_counter];
        end
        
        // every 8 clock cycles
        if (word_counter == DATA_WIDTH-1) begin

          // if the serial data we received was 1: toggle the LED
          if ({serial_data_in[DATA_WIDTH-1:1], MOSI} == 1) begin
            led <= ~led;
            cnt <= cnt + 1;
            write_enable <= 0;
          end

          // if the serial data we received was 2: output the counter value over the next 8 clock cycles
          if ({serial_data_in[DATA_WIDTH-1:1], MOSI} == 2) begin
            serial_data_out <= cnt;
            write_enable <= 1;
            MISO <= serial_data_out[DATA_WIDTH-1];
          end
          
          else begin
            // every other case
            serial_data_out <= 0;
            write_enable <= 0;
          end
        end
      end
    end
  end

endmodule
