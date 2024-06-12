
#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <math.h>

// generic hardware manager class
#include "hardware_manager_generic.h"

class SIM_AxisDevice : public AxisDevice{
  double lastDutyCycle = 0.0;
  
  double lastCount;
  double minCount, MaxCount;

  std::chrono::time_point<std::chrono::system_clock> lastDutyCycleChange = std::chrono::high_resolution_clock::now();

  public:

  SIM_AxisDevice(int steps_per_rotation){
    // set the lastCount to an arbitrary value
    lastCount = 200;
    minCount = lastCount - (steps_per_rotation/2);
    MaxCount = lastCount + (steps_per_rotation/2);

    StepSize = (2 * M_PI) / float(steps_per_rotation);
  }

  double getAngle(){
    return (StartingAngle + getCount() * StepSize);
  }

  int getCount(){
    // simulate communication delays
    usleep(10);

    // get the time between the last change in duty cycle and the current time
    auto now = std::chrono::high_resolution_clock::now();
    long int dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastDutyCycleChange).count();

    double calculatedCount = (double(dt)/1'000'000.0) * lastDutyCycle;
    int result;

    if(calculatedCount > MaxCount){
      result = int(MaxCount);
    }
    else if(calculatedCount < minCount){
      result = int(minCount);
    }
    else {
      // multiply the time difference with the duty cycle
      result = int(lastCount + calculatedCount);
    }

    //printf("count value: %d\n", result);
    return result;
  }

  void setDutyCycle(float dutyCycle){

    // update the count value
    lastCount = getCount();

    // store the duty cycle and the time at which the duty cycle was changed
    lastDutyCycle = dutyCycle;
    lastDutyCycleChange = std::chrono::high_resolution_clock::now();
  }
};

class HardwareManager : public AbstractHardwareManager{

public:
  HardwareManager(){
    reset();
  }

 //Fully resets the hardware
  void reset(){
    this->pitch = new SIM_AxisDevice(550);
    this->yaw = new SIM_AxisDevice(1240);
  }

  void deviceInfo(){
    printf("Simulated hardware manager with memory mapped interface\n");
  }

};
