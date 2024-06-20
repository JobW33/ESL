#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#include <signal.h>
#include <stdio.h>
#include <math.h>

#include "controller.h"

double degree_to_rad(double degree){
    return  (2 * M_PI) * (degree / 360);
}

void sigint(int a)
{
    exit(-1);
}


int main(int argc, char** argv) {

  double pos = 0.0;
  // set the signal interrupt
  signal(SIGINT, sigint);

  int usleep_time = 10'000;

  YawController panPid = YawController();
  PitchController tiltPid = PitchController();

  panPid.setInput(0.1);
  tiltPid.setInput(0.1);

	while (panPid.getTime() < 20.0)
	{
    panPid.step();
    tiltPid.step();

    usleep(usleep_time);

    //panPid.print();
    tiltPid.print();

    panPid.setPosition(pos);
    tiltPid.setPosition(pos);
    pos+=0.001;
  }

  return 0;
}
