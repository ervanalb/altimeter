#include "stm32f0xx.h"
#include "hal.h"
#include "barometer.h"

// Bring up all hardware
void init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    USART_InitTypeDef USART_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16 | RCC_APB2Periph_USART1, ENABLE);

    // PWR_HOLD
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LEDs
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
        GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
        GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Button
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Charging
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Piezo
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);

    TIM_TimeBaseInitStruct.TIM_Prescaler = 47;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 0;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseInitStruct);

    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM16, &TIM_OCInitStruct);
    TIM_CtrlPWMOutputs(TIM16, ENABLE);
    TIM_Cmd(TIM16, ENABLE);

    // GPS
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    USART_DeInit(USART1);
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);

    // Barometer
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_1);

    I2C_InitStruct.I2C_Timing = 0x50330309; // How's that for magic constants
    I2C_InitStruct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    I2C_InitStruct.I2C_DigitalFilter = 0;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = 0;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStruct);

    I2C_Cmd(I2C1, ENABLE);
}

void leds(uint8_t colors)
{
    uint32_t gpioa = 0;
    uint32_t gpiob = 0;

    if(colors & 1)   gpioa |= GPIO_Pin_0; else gpioa |= (GPIO_Pin_0 << 16);
    if(colors & 2)   gpioa |= GPIO_Pin_1; else gpioa |= (GPIO_Pin_1 << 16);
    if(colors & 4)   gpioa |= GPIO_Pin_2; else gpioa |= (GPIO_Pin_2 << 16);
    if(colors & 8)   gpioa |= GPIO_Pin_3; else gpioa |= (GPIO_Pin_3 << 16);
    if(colors & 16)  gpioa |= GPIO_Pin_6; else gpioa |= (GPIO_Pin_6 << 16);
    if(colors & 32)  gpioa |= GPIO_Pin_7; else gpioa |= (GPIO_Pin_7 << 16);
    if(colors & 64)  gpiob |= GPIO_Pin_0; else gpiob |= (GPIO_Pin_0 << 16);
    if(colors & 128) gpiob |= GPIO_Pin_1; else gpiob |= (GPIO_Pin_1 << 16);

    GPIOA->BSRR = gpioa;
    GPIOB->BSRR = gpiob;
}

void off()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void on()
{
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t button()
{
    return GPIO_ReadInputData(GPIOA) & GPIO_Pin_5;
}

uint8_t charging()
{
    return !(GPIO_ReadInputData(GPIOB) & GPIO_Pin_7);
}

void tone(uint16_t period)
{
    TIM16->ARR = period;
    TIM16->CCR1 = period / 2;
}

uint8_t getchar()
{
    while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
    return USART1->RDR;
}

void putchar(uint8_t c)
{
    USART1->TDR = c;
    while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
}

int i2c_read(uint8_t i2c_address, uint8_t reg_address, uint8_t* data, uint16_t n_bytes)
{
    __IO uint32_t timeout;

    I2C_TransferHandling(I2C1, i2c_address, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    timeout = I2C_TIMEOUT;
    while(!I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS))
    {
        if(!(timeout--)) return -1;
    }

    I2C_SendData(I2C1, reg_address);

    timeout = I2C_TIMEOUT;
    while(!I2C_GetFlagStatus(I2C1, I2C_ISR_TC))
    {
        if(!(timeout--)) return -2;
    } 

    I2C_TransferHandling(I2C1, i2c_address, n_bytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    while(n_bytes--)
    {
        timeout = I2C_TIMEOUT;
        while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)    
        {
            if(!(timeout--)) return -3;
        }

        *data++ = I2C_ReceiveData(I2C1);
    }    

    timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)   
    {
        if(!(timeout--)) return -4;
    }

    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
    return 0;
}

int i2c_write(uint8_t i2c_address, uint8_t reg_address, uint8_t* data, uint16_t n_bytes)
{
    __IO uint32_t timeout;

    I2C_TransferHandling(I2C1, i2c_address, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    timeout = I2C_TIMEOUT;
    while(!I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS))
    {
        if(!(timeout--)) return -1;
    }

    I2C_SendData(I2C1, reg_address);

    timeout = I2C_TIMEOUT;
    while(!I2C_GetFlagStatus(I2C1, I2C_ISR_TC))
    {
        if(!(timeout--)) return -2;
    } 

    I2C_TransferHandling(I2C1, i2c_address, n_bytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    while(n_bytes--)
    {
        timeout = I2C_TIMEOUT;
        while(!I2C_GetFlagStatus(I2C1, I2C_ISR_TXE))    
        {
            if(!(timeout--)) return -3;
        }

        I2C_SendData(I2C1, *data++);
    }    

    timeout = I2C_TIMEOUT;
    while(!I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF))
    {
        if(!(timeout--)) return -4;
    }

    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
    return 0;
}
