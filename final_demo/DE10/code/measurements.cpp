
#include <stdio.h>
#include <unistd.h> 

#include <signal.h>
#include <chrono>
#include <algorithm>

// Select 1
#include "hardware/hardware_manager_DE10.h"
//#include "hardware/hardware_manager_RPI.h"
//#include "hardware/hardware_manager_sim.h"

void sigint(int a)
{
	//reset
	HardwareManager justReset = HardwareManager();
	printf("Existing program, triggering software reset\n");
    exit(-1);
}



/**
 * Measures the communication speed by sending a number of messages as fast as possible and then measures how long it took.
 */
void testCommuncationSpeed(int repeats){
  HardwareManager manager = HardwareManager();

  printf("\n===================================\nCommunication speed test: \n");

  double yawAngle, pitchAngle;

  // get the starting time for the measurements
  auto start = std::chrono::high_resolution_clock::now();

  // repeat the comands this many times and measure the time
  for(int i = 0; i < repeats; i++){
    manager.pitch->setDutyCycle(double(i)/1000);
    manager.yaw->setDutyCycle(double(-i)/1000);

    yawAngle = manager.yaw->getAngle();
    pitchAngle = manager.pitch->getAngle();
  }

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  printf(" >>> Took a total of %ld microseconds to run %d tests\n", duration.count(), repeats);
  printf(" >>> Transfer speed: %.0f commands per second\n", (double(repeats*4)/duration.count())*1'000'000);
}



/**
 * Helper function to get the Quadrature Decoder count value at the maximum positions
 */
int _getCount(AxisDevice *dev,  double dutyCycle, int uSleepTime){
  int count = dev->getCount();
  
  dev->setDutyCycle(dutyCycle);
  usleep(uSleepTime);
 
 // while the motor position is still changing
  while(count != dev->getCount()){
    count = dev->getCount();
    usleep(uSleepTime);
  }
  
  dev->setDutyCycle(0.0);
  return count;
}

/**
 * Moves the pitch and yaw to its minimum and maximum values and determines the range it can move in counts.
 */
void determineCounts(){
  int uSleepTime = 50'000;
  //double dutyCycle = 0.05;

  HardwareManager manager = HardwareManager();

  printf("\n===================================\nCount range test: \n");

  // Get the QD count value at both extremes of the pitch
  int minPitchCount = _getCount(manager.pitch, -0.1, uSleepTime); 
  int maxPitchCount = _getCount(manager.pitch, 0.1, uSleepTime);
  printf(" >>> Pitch count range: %d\n", maxPitchCount-minPitchCount);

  // Get the QD count value at both extremes of the yaw
  int minYawCount = _getCount(manager.yaw, -0.2, uSleepTime); 
  int maxYawCount = _getCount(manager.yaw, 0.2, uSleepTime); 
  printf(" >>> Yaw count range: %d\n", maxYawCount-minYawCount);
}



/**
 * Helper function to calculate how long it takes from one side to the other
 */
long int _getDuration(AxisDevice *dev, double dutyCycle, int uSleepTime){
    int count = dev->getCount();
    auto start = std::chrono::high_resolution_clock::now();

    // set the given duty cycle
    dev->setDutyCycle(dutyCycle);
    usleep(uSleepTime);
    
    // while the motor keeps moving, keep looping
    while(count != dev->getCount()){
      count = dev->getCount();
      usleep(uSleepTime);
    }

    // calculate how long this took
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    // set the duty cycle to zero just to be safe
    dev->setDutyCycle(0.0);
    usleep(1'00'000);
}

/**
 * Determines the speed at which the motors rotate from one side to the other and back for a given duty cycle
 */
void determineRotationSpeed(double dutyCycle){
  HardwareManager manager = HardwareManager();
  int uSleepTime = 100'000;

  printf("\n===================================\nRotation speed test: \n");

  // move both motors to the starting positions
  manager.pitch->setDutyCycle(-0.2);
  manager.yaw->setDutyCycle(-0.2);

  // sleep long enough for them to have moved to the correct position
  usleep(1'000'000);

  // reset the duty cycle
  manager.pitch->setDutyCycle(0.0);
  manager.yaw->setDutyCycle(0.0); 

  // get the time it takes the yaw motor to move from one side to the other and back again
  long int yawDurationForwards = _getDuration(manager.yaw, dutyCycle, uSleepTime);
  long int yawDurationBackwards = _getDuration(manager.yaw, -dutyCycle, uSleepTime);
  printf(" >>> Yaw @%.2f Duty cycle: Took %.5fms to move to the other side and %.5fms to move back.\n", dutyCycle, double(yawDurationForwards)/1000.0, double(yawDurationBackwards)/1000.0);

  // get the time it takes the pitch motor to move from one side to the other and back again
  long int pitchDurationForwward =  _getDuration(manager.pitch, dutyCycle, uSleepTime);
  long int pitchDurationBackwards =  _getDuration(manager.pitch, -dutyCycle, uSleepTime);
  printf(" >>> Pitch @%.2f Duty cycle: Took %.5fms to move to the other side and %.5fms to move back.\n", dutyCycle, double(pitchDurationForwward)/1000.0, double(pitchDurationBackwards)/1000.0);
}



/**
 * Measures the jitter present while sending commands
 */
void determineJitter(){
  HardwareManager manager = HardwareManager();

  printf("\n===================================\nCommunication Jitter test: \n");

  int repeats = 1000;
  float sum = 0.0;
  float usDuration[repeats];
  
  auto start = std::chrono::high_resolution_clock::now();
  auto stop = std::chrono::high_resolution_clock::now();

  // execute the command
  for(int i = 0; i < repeats; i++){
    start = std::chrono::high_resolution_clock::now();

    double yawAngle = manager.yaw->getAngle();

    stop = std::chrono::high_resolution_clock::now();
    usDuration[i] = float(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());

    sum += usDuration[i];
  }

  //sort the whole array
  float* first(&usDuration[0]);
  float* last(first + repeats);
  std::sort(first, last);

  // print nerd stuff
  float median = usDuration[int(repeats/2)]/1000.0;
  float average = (sum/repeats)/1000.0;
  float worstTenPercent = usDuration[int(repeats/10)]/1000.0;
  float fastest = usDuration[0]/1000.0;
  float slowest = usDuration[repeats-1]/1000.0;
  printf(" >>> Tested %d commands. Average duration: %.3fms, Median duration: %.3fms\n", repeats, average, median);
  printf(" >>> All commands took between %.3f and %.3f milliseconds. The slowest 10 percent took longer than %.3fms\n", fastest, slowest, worstTenPercent);
}



int main(int argc, char** argv) {

  printf("Running test using: ");
  HardwareManager().deviceInfo();
  HardwareManager().reset();

  // set the signal interrupt
  signal(SIGINT, sigint);

  // measure how long it takes to repeat this many sets of commands
  testCommuncationSpeed(10000);

  // determine how much variation there is in the communication
  determineJitter();

  // determine the value range while moving the motors
  determineCounts();

  // determine how fast the motors rotate from one side to the other at the given duty cycle
  determineRotationSpeed(0.3);

  HardwareManager().reset();


  return 0;
}
