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
//#include "hw/hardware_manager_RPI.h"
#include "hardware/hardware_manager_DE10.h"
//#include "hardware/hardware_manager_sim.h"

// the PID controller for the Yaw and Pitch
#include "controller/controller.h"

// the video processor
#include "gst/GstProcessor.h"

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
  if(duration.count() < uSleepTime){
    usleep(uSleepTime - duration.count());
    //printf("Took %ld ms\n", duration.count());
  }
  else
  {
	  printf("Missed deadline\n");
  }
  return stop;

}

int main(int argc, char** argv) {

  // set the signal interrupt
  signal(SIGINT, sigint);

  // create the hardware manager, pid controller and video processor
  HardwareManager manager = HardwareManager();
  PID pid = PID();
  GstProcessor processor;

  // start the video processor
  if (!processor.initialize(argc, argv)) {
    return -1;
  }

  //Prevent the controller from stopping before we think it should stop.
  printf("%f\n", pid.pitch->model.GetFinishTime());
  printf("%f\n", pid.yaw->model.GetFinishTime());


  int usleep_time = 10'000;
  int index = 0;

  while (true)
  {
    auto start = std::chrono::high_resolution_clock::now();

    // listen to the video processor bus
    if (processor.bus_call() == false) { break; }
     
    // get target angles from the camera
    double pitch_dif = processor.get_rady(); 
    double yaw_dif = processor.get_radx();
    
    //get current pitch and yaw positions from the hardware
    double pitch_angle = manager.pitch->getAngle();
    double yaw_angle   = manager.yaw->getAngle();

    // tell the PID controller what the current position is
    pid.pitch->setPosition(pitch_angle);
    pid.yaw->setPosition(yaw_angle);
   
    // tell the PID controller what position to go to
    pid.pitch->setInput(pitch_angle - pitch_dif);
    pid.yaw->setInput(yaw_angle + yaw_dif);

    // get the pid controller outputs
    double pitch_pwm = pid.pitch->getOutput()*0.15;
    double yaw_pwm = pid.yaw->getOutput()*0.6;
    
    // update the duty cycles based on the pid controller output
    manager.pitch->setDutyCycle(pitch_pwm);
    manager.yaw->setDutyCycle(yaw_pwm);

    //print status update
    if(not(index % 25)){
      printf("Current         pitch: %f yaw: %f\n", pitch_angle, yaw_angle);
      printf("Difference          pitch: %f yaw: %f\n", pitch_dif, yaw_dif);
      printf("Desired                 pitch: %f yaw: %f\n", pitch_angle - pitch_dif, yaw_angle + yaw_dif);
      printf("PWM                         pitch: %f yaw: %f\n", pitch_pwm, yaw_pwm);
    }

    //update the controller
    pid.step();

    // sleep and update index
    sleepRemainder(start, usleep_time);

    index = index + 1;
  }

  return 0;
}
