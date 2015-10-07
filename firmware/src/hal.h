#ifndef __HAL_H
#define __HAL_H

#include <stdint.h>

#define I2C_TIMEOUT 0x3FFF

void init();
void leds(uint8_t colors);
void off();
void on();
uint8_t button();
uint8_t charging();
void tone(uint16_t period);

uint8_t getchar();
void putchar(uint8_t c);

int i2c_read(uint8_t i2c_address, uint8_t reg_address, uint8_t* data, uint16_t n_bytes);
int i2c_write(uint8_t i2c_address, uint8_t reg_address, uint8_t* data, uint16_t n_bytes);

#endif
