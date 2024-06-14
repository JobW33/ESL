
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

/* 20-sim submodel class include file */
#include "pan/PositionControllerPan.h"
#include "tilt/PositionControllerTilt.h"

class YawController{
public:
    PositionControllerPan model;
    XXDouble inputs [2 + 1];
    XXDouble outputs [2 + 1];

  YawController(){
    /* initialize the inputs and outputs with correct initial values */
    inputs[0] = 0.0;		/* in */
    inputs[1] = 0.0;		/* position */

    outputs[0] = 0.0;		/* corr */
    outputs[1] = 0.0;		/* out */

    /* initialize the submodel itself and calculate the outputs for t=0.0 */
    model.Initialize(inputs, outputs, 0.0);
    printf("Time: %f\n", model.GetTime());
  } 

  void step(){
    model.Calculate (inputs, outputs);
  }

  void print(){
    printf("<<yaw>>     input: %f, position: %f, output: %f, time: %f\n", inputs[0],  inputs[1], outputs[1], model.GetTime());
  }

  void setInput(double in){
    inputs[0] = in;
  }

  void setPosition(double pos){
    inputs[1] = pos;
  }

  double getOutput(){
    return outputs[1];
  }

  double getTime(){
    return model.GetTime();
  }

};

class PitchController{
public:
    XXDouble inputs [3 + 1];
    XXDouble outputs [1 + 1];
    PositionControllerTilt model;

  PitchController(){
    /* initialize the inputs and outputs with correct initial values */
    inputs[0] = 0.0;		/* corr */
    inputs[1] = 0.0;		/* in */
    inputs[2] = 0.0;		/* position */

    outputs[0] = 0.0;		/* out */

    /* initialize the submodel itself and calculate the outputs for t=0.0 */
    model.Initialize(inputs, outputs, 0.0);
  } 

  void step(){
    model.Calculate (inputs, outputs);
  }

  void print(){
    printf("<<pitch>>  input: %f, position: %f, output: %f, time: %f\n", inputs[1],  inputs[2], outputs[0], model.GetTime());
  }

  void setInput(double in){
    inputs[1] = in;
  }

  void setPosition(double pos){
    inputs[2] = pos;
  }

  double getOutput(){
    return outputs[0];
  }

  double getTime(){
    return model.GetTime();
  }

};

class PID{
public: 

  PitchController *pitch;
  YawController *yaw;

  PID(){
    this->pitch = new PitchController();
    this->yaw = new YawController();

    this->pitch->model.SetFinishTime(1e12);
    this->yaw->model.SetFinishTime(1e12);
  }

  /**
   * Step function combined for convenience
  */
  void step(){
    this->pitch->step();
    this->yaw->step();
  }
};