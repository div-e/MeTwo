

#ifndef MRAA_PWMDRIVER_H
#define MRAA_PWMDRIVER_H

#include <stdint.h>
#include <math.h>
#include <time.h> 
#include <unistd.h>
#include "mraa.hpp"
#include "mraa/i2c.hpp"
	
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD



class MRAA_PWMDriver {
    public: 
            MRAA_PWMDriver(uint8_t addr);
            ~MRAA_PWMDriver();
            void begin(); 
            void reset(); 
            void setPWMFreq(float freq); 
            void setPWM(uint8_t num, uint16_t on, uint16_t off); 
            void setPin(uint8_t num, uint16_t val, bool invert); 
            
    private:
            mraa::I2c* i2c;
            uint8_t _i2caddr;             
            uint8_t read8(uint8_t addr);
            void write8(uint8_t addr, uint8_t d); 

};

#endif

