#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#include "soc_system.h"

int main(int argc, char** argv) {
  int fd = 0;
  int max = 512;
  int delay = 1000000; //ms

  fd = open("/dev/mem", O_RDWR | O_SYNC);

  if (fd < 0) {
    perror("Couldn't open /dev/mem\n");
    return -1;
  }
  int* esl_demo_map = NULL;
  esl_demo_map = (int*)mmap(NULL, HPS_0_ARM_A9_0_ESL_BUS_DEMO_0_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HPS_0_ARM_A9_0_ESL_BUS_DEMO_0_BASE);
  if (esl_demo_map == MAP_FAILED) {
    perror("Couldn't map bridge.");
    close(fd);
    return -1;
  }

  //reset
  esl_demo_map[0] = 1; //*esl_demo_map || 0x10000;
  esl_demo_map[0] = 0;
  
  //print pitch and yaw
  int index = 0;
  while (true)
  {
    int pitch = esl_demo_map[1];
    int yaw   = esl_demo_map[2];
    printf("%4d  pitch: %8d yaw: %8d\n", index, yaw, pitch);
    usleep(delay);
    index = index + 1;
  }

  close(fd);
  return 0;
}
