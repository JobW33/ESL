
#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <math.h>

// generic hardware manager class
#include "hardware_manager_generic.h"

class SIM_AxisDevice : public AxisDevice{
  double lastDutyCycle = 0.0;
  double lastCount = 0.0;
  // double minCount, MaxCount;

  std::chrono::time_point<std::chrono::system_clock> lastDutyCycleChange = std::chrono::high_resolution_clock::now();

  public:

  SIM_AxisDevice(int steps_per_rotation){
    StepSize = (2 * M_PI) / float(steps_per_rotation);
  }

  double getAngle(){
    return (StartingAngle + (getCount() * StepSize));
  }

  int getCount(){
    lastCount += lastDutyCycle;
    return lastCount;
  }

  void setDutyCycle(float dutyCycle){
    lastDutyCycle = dutyCycle;
  }
};

class HardwareManager : public AbstractHardwareManager{

public:
  HardwareManager(){
    reset();
  }

 //Fully resets the hardware
  void reset(){
    this->pitch = new SIM_AxisDevice(510);
    this->yaw = new SIM_AxisDevice(1254);
  }

  void deviceInfo(){
    printf("Simulated hardware manager with memory mapped interface\n");
  }

};
