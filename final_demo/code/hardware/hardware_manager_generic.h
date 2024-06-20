#include <error.h>
#include <fcntl.h>
// #include <stdint.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <sys/mman.h>
// #include <unistd.h>
// #include <stdbool.h>
// #include <math.h>
// #include <signal.h>
// #include <stdio.h>


class AxisDevice{
public:
  double StepSize;
  double StartingAngle = 0.0;
  double limiter = 1.0;

  void setStartingAngle(double angle){
    StartingAngle = angle;
  }

  void setLimiter(double limit){
    if (limit < 0.0 or limit > 1.0){ 
      printf("Invalid limiter value, should be between 0.0 and 1.0\n"); 
      return;
    }
    limiter = limit;
  }

  virtual double getAngle(){
    return 0.0;
  }

  virtual int getCount(){
    return 0;
  }

  virtual void setDutyCycle(float dutyCycle){
    return;
  }
};

class AbstractHardwareManager{
public:
  AxisDevice *pitch;
  AxisDevice *yaw;

  virtual void reset(){
    return;
  }

  virtual void deviceInfo(){
    printf("Fake generic devce");
  }
};