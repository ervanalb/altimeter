#include "hal.h"
#include "stm32f0xx.h"

// Bring up all hardware
void init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    USART_InitTypeDef USART_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
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
