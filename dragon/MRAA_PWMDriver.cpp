/*

 * Filename: MRAA_PWMDriver.cpp
 * Author: Geeling Chau
 * Description: Modified Adafruit Servo Driver code to work with libmraa
 * Date: May 08 2017
 * Sources of Help: 
 * https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library/blob/master/
 * Adafruit_PWMServoDriver.cpp
 */

#include "MRAA_PWMDriver.hpp"
#include "mraa.hpp"
#include "mraa/i2c.hpp"

/*
 * Function name: MRAA_PWMDriver()
 * Function prototype: MRAA_PWMDriver(uint8_t addr); 
 * Description: Initializes a MRAA_PWMDriver object with the i2c address. 
 * Parameters: 
 * 	arg 1: uint8_t addr -- the i2c address
 * Side Effects: Object created. I2C address set. 
 * Error Conditions: None. 
 * Return Value: Constructor. 
 */
MRAA_PWMDriver::MRAA_PWMDriver(uint8_t addr) : 
	i2c(new mraa::I2c(0))
{
	_i2caddr = addr; 
}

MRAA_PWMDriver::~MRAA_PWMDriver() {
   reset();
   delete i2c;
}

// Helps initialize things for the new driver class? 
void MRAA_PWMDriver::begin(void) {
    //any libmraa initialization stuff? 
    reset(); 
}

// Writes a reset code to something somewhere. Not sure what this is for. 
void MRAA_PWMDriver::reset(void) {
	write8(PCA9685_MODE1, 0x0); 
}

// Sets the frequency of the PWM signal (e.g. clock?)
void MRAA_PWMDriver::setPWMFreq(float freq) {
	freq *= 0.9; //Correct for overshoot for some issue (see original code)
	float prescaleval = 25000000; 
	prescaleval /= 4096; 
	prescaleval /= freq; 
	prescaleval -= 1; 
	
	uint8_t prescale = floor(prescaleval + 0.5); 

	uint8_t oldmode = read8(PCA9685_MODE1); 
	uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
	write8(PCA9685_MODE1, newmode); // go to sleep. okay. 
	write8(PCA9685_PRESCALE, prescale); // set the prescaler... idk
	write8(PCA9685_MODE1, oldmode); 
	usleep(5000); // sleep for 5000 microseconds (5 millis) 
	write8(PCA9685_MODE1, oldmode | 0xa1); // sets the MODE1 register to 
					       // turn on auto increment. 
}

// sets the PWM duty cycle on pin num given on off times. 
void MRAA_PWMDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
	i2c->address(_i2caddr); 
	uint8_t buff[5] = {LED0_ON_L + 4*num, (uint8_t)on, (uint8_t)(on>>8), 
    (uint8_t)off, (uint8_t)(off>>8)}; 
  
	i2c->write(buff, 5); // write all the data also ends the connection. 
	
}

// sets the PWM on-ness on pin num
void MRAA_PWMDriver::setPin(uint8_t num, uint16_t val, bool invert) {
	if(val > 4095) {
		val = 4095; 
	} else if (val < 0) {
		val = 0; 
	}

	if(invert) {
		setPWM(num, 0, 4095 - val); // may add code for "fully on" 4096
	} else {
		setPWM(num, 0, val); 
	} 
}

// reads a byte of data from _i2caddr
uint8_t MRAA_PWMDriver::read8(uint8_t addr) {
	i2c->address(_i2caddr);
	uint8_t addrBuff[1] = {addr}; 
	i2c->write(addrBuff,1); 

	uint8_t* data; 
	(void) i2c->read(data, 1); 
	return *data; 
}

// writes a byte of data to _i2caddr
void MRAA_PWMDriver::write8(uint8_t addr, uint8_t d) {
	i2c->address(_i2caddr); 
	uint8_t addrDataBuff[2] = {addr, d}; 
	i2c->write(addrDataBuff, 2);
}

