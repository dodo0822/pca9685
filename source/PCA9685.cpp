#include <cmath>

#include "mbed-drivers/mbed.h"

#include "pca9685/PCA9685.h"

#define MODE1            0x00
#define MODE2            0x01
#define SUBADR1          0x02
#define SUBADR2          0x03
#define SUBADR3          0x04
#define PRESCALE         0xFE
#define LED0_ON_L        0x06
#define LED0_ON_H        0x07
#define LED0_OFF_L       0x08
#define LED0_OFF_H       0x09
#define ALL_LED_ON_L     0xFA
#define ALL_LED_ON_H     0xFB
#define ALL_LED_OFF_L    0xFC
#define ALL_LED_OFF_H    0xFD
#define RESTART          0x80
#define SLEEP            0x10
#define ALLCALL          0x01
#define INVRT            0x10
#define OUTDRV           0x04

PCA9685::PCA9685(I2C& _i2c, uint8_t _address) : i2c(_i2c), address(_address) {
	set_all_pwm(0, 0);

	write(MODE2, OUTDRV);
	write(MODE1, ALLCALL);

	wait_ms(5);

	uint8_t mode1 = read(MODE1);
	mode1 &= (~SLEEP);

	write(MODE1, mode1);

	wait_ms(5);
}

void PCA9685::set_frequency(int freq) {
	float preScaleLevel = 25000000;
	preScaleLevel /= 4096;
	preScaleLevel /= freq;
	preScaleLevel -= 1;
	uint8_t preScale = floor(preScaleLevel + 0.5);
	uint8_t oldMode = read(MODE1);
	uint8_t newMode = (oldMode & 0x7F) | 0x10;
	write(MODE1, newMode);
	write(PRESCALE, preScale);
	write(MODE1, oldMode);
	wait_ms(5);
	write(MODE1, oldMode | 0x80);
}

void PCA9685::set_pwm(uint8_t channel, uint16_t on, uint16_t off) {
	write(LED0_ON_L + 4*channel, on & 0xFF);
	write(LED0_ON_H + 4*channel, on >> 8);
	write(LED0_OFF_L + 4*channel, off & 0xFF);
	write(LED0_OFF_H + 4*channel, off >> 8);
}

void PCA9685::set_all_pwm(uint16_t on, uint16_t off) {
	write(ALL_LED_ON_L, on & 0xFF);
	write(ALL_LED_ON_H, on >> 8);
	write(ALL_LED_OFF_L, off & 0xFF);
	write(ALL_LED_OFF_H, off >> 8);
}

void PCA9685::write(uint8_t reg, uint8_t value) {
	char data[2] = { reg, value };
	i2c.write(address, data, sizeof(data), false);
}

uint8_t PCA9685::read(uint8_t reg) {
	char data = reg;
	i2c.write(address, &data, 1, false);
	char ret;
	i2c.read(address, &ret, 1);
	return ret;
}