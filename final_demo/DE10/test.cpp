#include <stdio.h>
#include <unistd.h>

#include "gst/GstProcessor.h"

void sigint(int a)
{
  //reset
  printf("Existing program, triggering software reset\n");
  exit(-1);
}

int main(int argc, char *argv[]) {
    GstProcessor processor;

    if (!processor.initialize(argc, argv)) {
        return -1;
    }

    printf("Staring loop\n");

    int YDistance = 0;
    while(true)
    {
      if (processor.bus_call() == false)
      {
        printf("Help\n");
        break;
      }
     
      processor.set_YDistance(YDistance);
      printf("dx = %d, dy = %d\nradx = %f, rady = %f\n", processor.get_dx(), processor.get_dy(), processor.get_radx(), processor.get_rady());
      YDistance = (YDistance < 1000) ? YDistance + 1 : 0;

      usleep(100000);
    }

    printf("Returned, stopping playback\n");
    return 0;
}
