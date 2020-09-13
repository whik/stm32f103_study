#include "drv_key.h"
#include "drv_led.h"

/*
- KEY0-PE4
- KEY1-PE3
- KEY2-PE2
*/


void key_exti_init(void)
{
    EXTI_InitTypeDef EXTI_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line4;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    
    EXTI_Init(&EXTI_InitStruct);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
    {
        delay_ms(10);
        if(KEY0 == 0)
            LED0 = !LED0;
        EXTI_ClearITPendingBit(EXTI_Line4); //清除 LINE 上的中断标志位
    }
}

void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOE, &GPIO_InitStruct);
    key_exti_init();
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
