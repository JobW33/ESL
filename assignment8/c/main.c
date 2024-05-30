#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#include "soc_system.h"

uint32_t percentage_to_pwm(int percentage);

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
  esl_demo_map[0] = 1 << 31;
  esl_demo_map[0] = 0;
  esl_demo_map[3] = 1 << 31;
  esl_demo_map[3] = 0;
  esl_demo_map[4] = 1 << 31;
  esl_demo_map[4] = 0;
  
  //set pwm signal to 0%
  esl_demo_map[3] = 0;
  
  //print pitch and yaw
  int index = 0;
  int percentage = 0;
  while (true)
  {
    //change the percentage the pwm module is high this also changes the direction just for testing puposes
    uint32_t esl_update = percentage_to_pwm(percentage);
    esl_demo_map[3] = esl_update;
    esl_demo_map[4] = esl_update;
    percentage = percentage >= 99 ? 0 : percentage + 1;

    //get pitch and yaw
    int pitch = esl_demo_map[1];
    int yaw   = esl_demo_map[2];
    
    //print status update
    printf("%4d  pitch: %8d yaw: %8d percentage: %3d\n", index, yaw, pitch, percentage);

    // sleep and update index
    usleep(delay);
    index = index + 1;
  }

  close(fd);
  return 0;
}

uint32_t percentage_to_pwm(int percentage)
{
  // big variable that reprisents 1%
  const uint32_t one_percent32_t = 0xFFFFFFFF / 100;

  // percentage
  uint32_t output = one_percent32_t * percentage;   // get percentage in 32 bits form
  output = output >> (32 - 11);   // shift right 21 bits
  output = output && 0x000007FF;  //mask with 11 bits just to be sure
  
  // update direction, get the direction depending on multiple of 2 and shift 30 bits
  output = output || ((percentage % 2 == 0 ? 1 : 0) << 30);

  return output;
}
