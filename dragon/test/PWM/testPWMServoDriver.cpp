#include <iostream> 
#include <exception>
#include <stdio.h> 
#include <stdint.h> 
#include "MRAA_PWMDriver.hpp"
#include <unistd.h>
//#include "mraa/common.hpp"
#include "mraa.hpp"
//#include <gpio.h>
//#include "mraa/i2c.hpp"
using namespace mraa; 
using namespace std;

//Sorry this code is really messy. But basically what it does is
//initializes the frequency to a somewhat low value (1000 in this case, 
//where the range is 0 to 4095). Then it sleeps for 3 seconds. Then
//wakes up at 2000 and increments by 100 every second or so, also
//oscillating the dragonboard's pin 23 and 25 on and off. After it reaches
//3000, it will increment down by 100 again continuing to oscilate the 
//GPIO pins. 
MRAA_PWMDriver* pwm;
#define LOW 0
#define HIGH 1

#define MOTOR_A_1_PIN 23
#define MOTOR_A_2_PIN 25

#define SERVOMIN 2000
#define SERVOMAX 3000

unsigned int usecs = 999999;

uint8_t servonum = 0; 



void setServoPulse(uint8_t n, double pulse) {
    double pulselength; 

    pulselength = 1000000; // 1,000,000 us per second
    pulselength /= 60; // 60Hz

    pulselength /= 4096; // 12 bits of resolution

    pulse *= 1000; 
    pulse /= pulselength; 
    pwm->setPWM(n, 0, pulse); 
}

int main() {
 /* mraa_result_t init_result = mraa_init();
  if (init_result != MRAA_SUCCESS)
  {
    cout << "ERROR in intialization" <<endl;
    return -1;
  }

  Platform platform_result = getPlatformType();
  if (platform_result != 9)
  {
    cout << "Not supported platform" << endl;
    return -1;
  }*/
//  string i2c_name = "I2C0";
//  int i2c_lookup = getI2cBusId(0); 
//  cout << "getI2cLookup: " << i2c_lookup << endl; 
//  return 0;


  mraa::Gpio* motorA_1_gpio = new mraa::Gpio(MOTOR_A_1_PIN); 
  mraa::Gpio* motorA_2_gpio = new mraa::Gpio(MOTOR_A_2_PIN); 
  motorA_1_gpio->dir(DIR_OUT_LOW);
  motorA_2_gpio->dir(DIR_OUT_LOW);

  mraa::I2c* i2c;

  i2c = new mraa::I2c(0);

   pwm = new MRAA_PWMDriver(0x40); 

   pwm->begin(); 

   pwm->setPWMFreq(60); // Analog servos run at ~60 Hz updates
   
   printf("servo: %d \n\n", servonum); 
   
   pwm->setPWM(servonum, 0, 1000);
   printf("sleeping for 5 sec. \n");
   usleep(5000000);
   printf("awake now! \n"); 
   
   for(uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen = pulselen+100) {
       printf("pulselen:  %d \n", pulselen); 
       pwm->setPWM(servonum, 0, pulselen); 
       motorA_1_gpio->write(HIGH);
       motorA_2_gpio->write(HIGH); 
       printf("sleep \n"); 
       usleep(1000000);
       motorA_1_gpio->write(LOW);
       motorA_2_gpio->write(LOW); 
       usleep(1000000);
   } 
   
   //usleep(usecs);
    // http://www.strictlyforgeeks.xyz/2013/05/using-delay-function-in-c.html

   for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen=pulselen-100) {
       printf("pulselen:  %d \n", pulselen); 
       pwm->setPWM(servonum, 0, pulselen);
       motorA_1_gpio->write(HIGH);
       motorA_2_gpio->write(HIGH); 
       printf("sleep \n"); 
       usleep(1000000);
       motorA_1_gpio->write(LOW);
       motorA_2_gpio->write(LOW);
       usleep(1000000);
   }

  delete(i2c); 
  delete(pwm);
  delete(motorA_1_gpio);
  delete(motorA_2_gpio);
   //usleep(usecs);

    return 0; 
}


