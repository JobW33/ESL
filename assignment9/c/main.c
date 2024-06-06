#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spidev-lib/src/spidev_lib.h"
#include <unistd.h>

spi_config_t spi_config;
uint8_t tx_buffer;
uint8_t rx_buffer;


void main( void)
{
  int spifd;
  spi_config.mode=0;
  spi_config.speed=1;
  spi_config.delay=0;
  spi_config.bits_per_word=8;

  spifd=spi_open("/dev/spidev0.1", spi_config);

  for (int _ = 0; _ < 10; _++)
  {
	//toggle led
  	tx_buffer = 1;
  	rx_buffer = 0;

//  	printf("sending %d, to spidev2.0 in full duplex \n ",tx_buffer);

  	spi_xfer(spifd, &tx_buffer, 1, &rx_buffer, 1);

  	printf("rx_buffer=%d\n",rx_buffer);

	//read data
  	tx_buffer = 2;
  	rx_buffer = 0;

//  	printf("sending %d, to spidev2.0 in full duplex \n ",tx_buffer);

  	spi_xfer(spifd, &tx_buffer, 1, &rx_buffer, 1);

//  	printf("rx_buffer=%d\n",rx_buffer);
	usleep(1000000);
  }

  spi_close(spifd);
}
