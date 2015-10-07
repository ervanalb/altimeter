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
    if(i2c_read(I2C_BAROMETER_ADDR, 0x0C, &id, sizeof(id))) return -3;
    if(id != 0xC4) return -4;

    return 0;
}

