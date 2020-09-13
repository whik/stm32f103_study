#include "drv_led.h"

/*
LED0-PB5
LED1-PE5
*/

void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void led_set(uint8_t num, ledStatus status)
{
    switch(num)
    {
        case 0: LED0 = status; break;
        case 1: LED1 = status; break;
    }
}

void led_blink(uint16_t period)
{
    led_set(0, ON);
    led_set(1, ON);
    delay_ms(period);

    led_set(0, OFF);
    led_set(1, OFF);
    delay_ms(period);
}
