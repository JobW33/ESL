//https://learnembeddedsystems.co.uk/bmp280-and-pi-pico-over-spi
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define MISO 16
#define CS   17
#define SCLK 18
#define MOSI 19

#define SPI_PORT spi0

int main(){
//    stdio_init_all(); // Initialise I/O for USB Serial

    spi_init(SPI_PORT, 500000); // Initialise spi0 at 500kHz
    
    //Initialise GPIO pins for SPI communication
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);

    // Configure Chip Select
    gpio_init(CS); // Initialise CS Pin
    gpio_set_dir(CS, GPIO_OUT); // Set CS as output
    gpio_put(CS, 1); // Set CS High to indicate no currect SPI communication

    uint8_t toggle = 1;
    uint8_t read = 2;
    uint8_t read_data = 0;
    int index = 0;

    while(1){
        gpio_put(CS, 0);
        spi_write_blocking(SPI_PORT, &toggle, 1);
        spi_write_blocking(SPI_PORT, &toggle, 1);
        spi_write_blocking(SPI_PORT, &toggle, 1);
        spi_write_blocking(SPI_PORT, &read, 1);
        spi_read_blocking(SPI_PORT, 0, read_data, 1);
        gpio_put(CS, 1);

        printf("%4d  Amount of times toggled: %4d\n", index, read_data);

        sleep_ms(1000);
        index = index + 1;
    }
}
