#include "drv_key.h"
/*
- KEY0-PE4
- KEY1-PE3
- KEY2-PE2
*/

void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t key_get(uint8_t mode)
{
    static uint8_t key_up = 1; 
    if(mode)
        key_up = 1; 
    if(key_up & (KEY0 && KEY1 && KEY2))
    {
        delay_ms(10);
        key_up = 0;
        if(KEY0 == 0)
            return 0;
        else if(KEY1 == 0)
            return 1;
        else if(KEY2 == 0)
            return 2;
    }
    else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1)
        key_up = 1;
    return 3;
}
