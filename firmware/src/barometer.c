#include "barometer.h"
#include "hal.h"
#include "stm32f0xx.h"

#define I2C_BAROMETER_ADDR 0xC0

int init_barometer()
{
    __IO uint32_t timeout;

    I2C_TransferHandling(I2C1, I2C_BAROMETER_ADDR, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);
    I2C_ClearFlag(I2C1, I2C_ICR_NACKCF | I2C_ICR_STOPCF);

    I2C_GenerateSTART(I2C1, ENABLE);

    timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)
    {
        if(!(timeout--)) return -1;
    }

    if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) != RESET) return -2;
    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

    uint8_t id;
    if(i2c_read_byte(I2C_BAROMETER_ADDR, 0x0C, &id)) return -3;
    if(id != 0xC4) return -4;

    if(i2c_write_byte(I2C_BAROMETER_ADDR, 0x26, 0x88)) return -1; // Write to CTRL_REG1 OS=128
    if(i2c_write_byte(I2C_BAROMETER_ADDR, 0x13, 0x07)) return -2; // Notify on new altitude data

    return 0;
}

int begin_alititude_measurement()
{
    if(i2c_write_byte(I2C_BAROMETER_ADDR, 0x26, 0x8A)) return -1; // One-shot
    return 0;
}

int get_altitude_measurement(int32_t* altitude)
{
    uint8_t status;
    uint8_t result[3];
    if(i2c_read_byte(I2C_BAROMETER_ADDR, 0x00, &status)) return -2;
    if(!(status & 0x08)) return -1; // Data unavailable

    if(i2c_read(I2C_BAROMETER_ADDR, 0x01, result, sizeof(result))) return -3;
    *altitude = ((uint32_t)result[0] << 24) | ((uint32_t)result[1] << 16) | ((uint32_t)result[2] << 8);
    return 0;
}

