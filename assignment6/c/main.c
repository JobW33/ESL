#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#include "soc_system.h"

void run_cycles(uint8_t* esl_demo_map, int amount, bool direction);

int main(int argc, char** argv) {
  int fd = 0;
  int max = 512;

  fd = open("/dev/mem", O_RDWR | O_SYNC);

  printf("%d\n", fd);

  if (fd < 0) {
    perror("Couldn't open /dev/mem\n");
    return -1;
  }
  uint8_t* esl_demo_map = NULL;
  esl_demo_map = (uint8_t*)mmap(NULL, HPS_0_ARM_A9_0_ESL_BUS_DEMO_0_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HPS_0_ARM_A9_0_ESL_BUS_DEMO_0_BASE);
  if (esl_demo_map == MAP_FAILED) {
    perror("Couldn't map bridge.");
    close(fd);
    return -1;
  }

  *esl_demo_map = 0b100;
  printf("Count up halfway!\n");
  run_cycles(esl_demo_map, max/2, true);
  while (true)
  {
    printf("Count down!\n");
    run_cycles(esl_demo_map, max, false);
    printf("Count up!\n");
    run_cycles(esl_demo_map, max, true);
  }

  //*((uint32_t *)esl_demo_map) = 1 << 31 | 0x13; // count down from 0xnn

  printf("%8x\n", *((uint32_t *)esl_demo_map));

  close(fd);
  return 0;
}

void run_cycles(uint8_t* esl_demo_map, int amount, bool direction)
{
  int delay = 5000; //ms
  for (int i = 0; i < amount; i = i + 1)
  {
    if (direction)
    {
      *esl_demo_map = 0b10;
    }
    else
    {
      *esl_demo_map = 0b01;
    }
    
    printf("%02b\n", *esl_demo_map);
    usleep(delay);
    *esl_demo_map = 0b11;
    printf("%02b\n", *esl_demo_map);
    usleep(delay);
    
    if (direction)
    {
      *esl_demo_map = 0b01;
    }
    else
    {
      *esl_demo_map = 0b10;
    }
    
    printf("%02b\n", *esl_demo_map);
    usleep(delay);
    *esl_demo_map = 0b00;
    printf("%02b\n", *esl_demo_map);
    usleep(delay);
  }
}
