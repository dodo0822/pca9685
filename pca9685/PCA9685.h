#pragma once

#include "mbed-drivers/mbed.h"

class PCA9685 {
public:
	PCA9685(I2C&, uint8_t = 0x80);
	void set_frequency(int);
	void set_pwm(uint8_t, uint16_t, uint16_t);
	void set_all_pwm(uint16_t, uint16_t);

private:
	void write(uint8_t, uint8_t);
	uint8_t read(uint8_t);

	I2C &i2c;
	uint8_t address;
};