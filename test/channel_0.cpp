#include "mbed-drivers/mbed.h"

#include "pca9685/PCA9685.h"

static I2C i2c(YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SDA, YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SCL);

void app_start(int, char**) {
	PCA9685 pca(i2c);
	pca.set_frequency(500);
	pca.set_pwm(0, 0, 4095);
	wait_ms(2000);
	pca.set_pwm(0, 0, 1000);
	wait_ms(2000);
	wait_ms(2000);
	pca.set_pwm(0, 0, 4095);
}