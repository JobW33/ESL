
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// select one, they have the exact same interface
// #include "hardware_manager_DE10.h"
#include "hardware_manager_RPI.h"

void sigint(int a)
{
	// reset
	HardwareManager justReset = HardwareManager();
	printf("Existing program, triggering software reset\n");
    exit(-1);
}

int main(int argc, char** argv) {

  // set the signal interrupt
  signal(SIGINT, sigint);

  // time to wait between each loop
  int delay = 1000000; //us
  
  // create the hardware manager
  HardwareManager manager = HardwareManager();
  manager.deviceInfo();

  // set the duty cycle limits
  manager.pitch->setLimiter(0.2);
  manager.yaw->setLimiter(0.8);

  int index = 0;
  float dutyCycle = 0.0;
  int direction = 1;

  while (true)
  {
    // set the new duty cycle
    manager.pitch->setDutyCycle(dutyCycle);
    manager.yaw->setDutyCycle(dutyCycle);

    // update the next duty cycle and reverse the direction
    dutyCycle = (dutyCycle >= 0.99 ? 0.0 : (dutyCycle + 0.01));
    direction = -direction;

    //get pitch and yaw angle
    double pitch = manager.pitch->getAngle();
    double yaw   = manager.yaw->getAngle();
    
    //print status update
    printf("%4d  pitch: %f yaw: %f duty cycle: %3f\n", index, yaw, pitch, (direction * dutyCycle));
	
    // sleep and update index
    usleep(delay);
    index = index + 1;
  }

  return 0;
}
