#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <signal.h>
#include <chrono>
#include <math.h>

// select 1
//#include "hardware/hardware_manager_RPI.h"
//#include "hardware/hardware_manager_DE10.h"
#include "hardware/hardware_manager_sim.h"

// the PID controller for the Yaw and Pitch
#include "controller/controller.h"

void sigint(int a)
{
	//reset
	HardwareManager().reset();
	printf("Existing program, triggering software reset\n");
    exit(-1);
}

// Accounts for processing time. Sleeps until uSleepTime microseconds after the starting time. 
auto sleepRemainder(std::chrono::time_point<std::chrono::system_clock> start, int uSleepTime){
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  if(duration.count() < uSleepTime){
    usleep(uSleepTime - duration.count());
  }
  else {
    printf("Operation took longer than 10ms, %ldms\n", duration.count()/1000);
  }
  return stop;

}

int main(int argc, char** argv) {

  // set the signal interrupt
  signal(SIGINT, sigint);

  // create the hardware manager
  HardwareManager manager = HardwareManager();
  PID pid = PID();

  pid.pitch->model.SetFinishTime(1e12);
  pid.yaw->model.SetFinishTime(1e12);

  // set a position to move to
  pid.yaw->setInput(0.0);
  pid.pitch->setInput(0.0);

  int usleep_time = 10'000;
  int index = 0;

  auto start = std::chrono::high_resolution_clock::now();

  double var = 7.0;

  while (true)
  {

    start = std::chrono::high_resolution_clock::now();

    //get pitch and yaw angles
    double pitch_angle = manager.pitch->getAngle();
    double yaw_angle   = manager.yaw->getAngle();
    
    // set the positions it has to move to
    pid.pitch->setPosition(pitch_angle);
    pid.yaw->setPosition(yaw_angle);

    // get the output value of the controllers and update the duty cycles accordingly
    manager.pitch->setDutyCycle(pid.pitch->getOutput()*0.3);
    manager.yaw->setDutyCycle(pid.yaw->getOutput());

    double val = sin((index*M_PI)/180);
    pid.yaw->setInput(0.9*val);
    pid.pitch->setInput(val);

    //print status update
    if(not(index % 25)){
      printf("pitch: %f yaw: %f\n", pitch_angle, yaw_angle);
      printf("desired pitch: %f yaw: %f\n", val, 0.9*val);

//      printf("pitch: %x yaw: %x\n", manager.pitch->getCount(), manager.yaw->getCount());
    }

    //update the controller
    pid.step();

    // sleep and update index
    sleepRemainder(start, usleep_time);
    index = index + 1;
  }

  return 0;
}
