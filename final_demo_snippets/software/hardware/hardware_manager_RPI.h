
#include <stdio.h>
#include <math.h>

// SPI library for the raspberry pi
#include "spidev/spidev_lib.h"

// generic hardware manager class
#include "hardware_manager_generic.h"

enum CommandID {  
    RESET = 0xFF, 
    SET_PITCH = 0x11, 
    GET_PITCH = 0x12,
    SET_YAW = 0x21, 
    GET_YAW = 0x22
};

/**
 * 0xFF Reset command
 *    1 byte send
 * 0x11 Set pitch duty cycle and direction 
 *    4 bytes send
 * 0x12 Get the current pitch angle
 *    1 byte send
 *    1 byte received
 * 0x21 Set yaw duty cycle and direction
 *    4 bytes send
 * 0x22 Get the current yaw angle
 *    1 byte send
 *    1 byte received 
*/
class RPI_AxisDevice : public AxisDevice{
public: 

  // IDs of the commands
  uint8_t getCMD;
  uint8_t setCMD;

  // the device
  int device;

  RPI_AxisDevice(int dev, int steps_per_rotation, uint8_t setCMD_ID, uint8_t getCMD_ID){
      StepSize = (2 * M_PI) / float(steps_per_rotation);
      getCMD = getCMD_ID;
      setCMD = setCMD_ID;
      device = dev;
  }

  double getAngle(){
    uint8_t rx;
    spi_xfer(device, &getCMD, 1, &rx, 1);
    return  StartingAngle + (rx * StepSize);
  }

  int getCount(){
    uint8_t rx;
    spi_xfer(device, &getCMD, 1, &rx, 1);
    return int(rx);
  }

  void setDutyCycle(float dutyCycle){
    bool forward = true;

    // just return if the value is invalid
    if (dutyCycle < -1.0 or dutyCycle > 1.0){
      return;
    }

    // handle negative values
    if(dutyCycle < 0.0){
      forward = false;
      dutyCycle = abs(dutyCycle);
    }

    // limit the duty cycle if needed
    dutyCycle = (dutyCycle > limiter? limiter : dutyCycle);

    // calculate the value to send
    uint16_t duty_val = uint16_t(dutyCycle * 2047);

    /**
     *  tx[0] command ID
     *  tx[1] direction
     *  tx[2] lower byte of the duty_val
     *  tx[3] upper byte of the duty_val
    */
    uint8_t tx[4]; 
    tx[0] = setCMD;
    tx[1] = (forward? 1 : 0);
    tx[2] = *(&duty_val);
    tx[3] = *(&duty_val+1);

    //send
    spi_write(device, tx, 3);
  }
};

class HardwareManager : public AbstractHardwareManager{
  spi_config_t spi_config;
  int devfd;

public:

  HardwareManager(){
    // init the  SPI device
    spi_config.mode=0;
    spi_config.speed=1;
    spi_config.delay=0;
    spi_config.bits_per_word=8;
    char dev[] = "/dev/spidev0.1";
    devfd=spi_open(dev, spi_config);

    // reset the hardware
    reset();

    // Create the Axis Devices
    this->pitch = new RPI_AxisDevice(devfd, 540, CommandID::SET_PITCH, CommandID::GET_PITCH);
    this->yaw = new RPI_AxisDevice(devfd,  1240, CommandID::SET_YAW, CommandID::GET_YAW);
  }

  // reset all the devices
  void reset(){
    uint8_t cmd = CommandID::RESET;
    spi_write(devfd, &cmd, 1);
  }

  void deviceInfo(){
    // print some info
    printf("RPI hardware manager with SPI interface\n");
  }
};
