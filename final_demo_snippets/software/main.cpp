#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#include <signal.h>
#include <chrono>

// select 1
//#include "hw/hardware_manager_RPI.h"
#include "hardware/hardware_manager_DE10.h"

// the PID controller for the Yaw and Pitch
#include "controller/controller.h"

void sigint(int a)
{
	//reset
	HardwareManager justReset = HardwareManager();
	printf("Existing program, triggering software reset\n");
    exit(-1);
}

// Accounts for processing time. Sleeps until uSleepTime microseconds after the starting time. 
auto sleepRemainder(std::chrono::time_point<std::chrono::system_clock> start, int uSleepTime){
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  usleep(uSleepTime - duration.count());
  return stop;
}

int main(int argc, char** argv) {

  // set the signal interrupt
  signal(SIGINT, sigint);

  // create the hardware manager
  HardwareManager manager = HardwareManager();
  PID pid = PID();

  // set the duty cycle limits for the pitch and yaw 
  manager.pitch->setLimiter(0.2);
  manager.yaw->setLimiter(0.8);

  // set a position to move to
  pid.yaw->setInput(0.1);
  pid.pitch->setInput(0.1);

  int usleep_time = 10'000;
  int index = 0;

  auto start = std::chrono::high_resolution_clock::now();

  while (true)
  {

    //get pitch and yaw angles
    double pitch_angle = manager.pitch->getAngle();
    double yaw_angle   = manager.yaw->getAngle();
    
    pid.pitch->setPosition(pitch_angle);
    pid.yaw->setPosition(yaw_angle);

    //print status update
    if(index % 50){
      printf("pitch: %f yaw: %f\n", pitch_angle, yaw_angle);
    }

    // get the output value of the controllers and update the duty cycles accordingly
    manager.pitch->setDutyCycle(pid.pitch->getOutput());
    manager.yaw->setDutyCycle(pid.yaw->getOutput());

    //update the controller
    pid.step();

    // sleep and update index
    sleepRemainder(start, usleep_time);
    index = index + 1;
  }

  return 0;
}
