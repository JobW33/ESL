
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <sys/mman.h>

// Memory mapping
#include "soc_system.h"

// generic hardware manager class
#include "hardware_manager_generic.h"

class DE10_AxisDevice : public AxisDevice{
  int *readReg;
  int *writeReg;

  public:

  DE10_AxisDevice(int *rReg, int *wReg, int steps_per_rotation){
    readReg = rReg;
    writeReg = wReg;

    StepSize = (2 * M_PI) / float(steps_per_rotation);
  }

  double getAngle(){
    return (StartingAngle + (*readReg) * StepSize);
  }

  int getCount(){
    return *readReg;
  }

  void setDutyCycle(float dutyCycle){
    // just return if the value is invalid
    if (dutyCycle < -1.0 or dutyCycle > 1.0){
      return;
    }

    // set the direction
    *writeReg = (*writeReg) & 0x3FFFFFFF | ((dutyCycle >= 0.0 ? 1 : 0) << 30);
    dutyCycle = abs(dutyCycle);
    
    // if the duty cycle exceeds the limitor, set it to the max allowed value
    dutyCycle = (dutyCycle > limiter? limiter : dutyCycle);

    // calculate the correct duty cycle value with 11 bits
    uint32_t duty_val = uint32_t(dutyCycle * 2047);

    // Set the lower 11 bits to zero and bitwise or the new duty cycle
    duty_val = duty_val & 0x000007FF;  
    *writeReg = ((*writeReg) & 0xFFFFF800) | duty_val;
  }

};

class HardwareManager : public AbstractHardwareManager{

  // QUADRATURE DECODER REGISTERS/WIRES
	//  esl_map[0] QD qd_control					[31]=software_reset
	//  esl_map[1] QD pitch_out					[DATA_WIDTH:0]=counter_output
	//  esl_map[2] QD yaw_out						[DATA_WIDTH:0]=counter_output
  
  // STEERING REGISTERS/WIRES
	//  esl_map[3] Steering pitch_control   		[31]=software_reset, [30]=dir, [COUNT_SIZE:0]=duty_cycle
	//  esl_map[4] Steering yaw_control 			[31]=software_reset, [30]=dir, [COUNT_SIZE:0]=duty_cycle

private:
  int mapfd = 0;
  int* esl_map = NULL;

public:
  HardwareManager(){

    // open the memory device thingy
    mapfd = open("/dev/mem", O_RDWR | O_SYNC);

    if (mapfd < 0) {
      perror("Couldn't open /dev/mem\n");
      exit(-1);
    }


    // get the memory map
    esl_map = (int*)mmap(NULL, HPS_0_ARM_A9_0_ESL_BUS_DEMO_0_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, mapfd, HPS_0_ARM_A9_0_ESL_BUS_DEMO_0_BASE);
    if (esl_map == MAP_FAILED) {
      perror("Couldn't map bridge.");
      close(mapfd);
      exit(-1);
    }

    //clear any previous values
    reset();

    // Create the Axis Devices
    this->pitch = new DE10_AxisDevice(&esl_map[1], &esl_map[3], 510);
    this->yaw = new DE10_AxisDevice(&esl_map[2], &esl_map[4], 1254);
  }

 //Fully resets the hardware
  void reset(){
    esl_map[0] = 1 << 31;
    esl_map[0] = 0;
    esl_map[3] = 1 << 31;
    esl_map[3] = 0;
    esl_map[4] = 1 << 31;
    esl_map[4] = 0;
  }

  void deviceInfo(){
    printf("DE10 hardware manager with memory mapped interface\n");
  }

};
