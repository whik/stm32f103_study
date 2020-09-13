#include "drv_eprom_iic.h"
/*
I2C_SCL - PB6
I2C_SDA - PB7
*/
void eprom_iic_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void eprom_iic_sda_set(sda_direction dir)
{
    /* B7 Clear */
    GPIOB->CRL &= 0x0FFFFFFF;

    /* B7 Set */
    if(dir == DIR_IN)
        GPIOB->CRL |= 8 << 28;
    else if(dir == DIR_OUT)
        GPIOB->CRL |= 3 << 28;
}

void eprom_iic_start(void)
{
    eprom_iic_sda_set(DIR_OUT);
    SDA_OUT = 1;
    SCL_OUT = 1;
    delay_us(4);
    SDA_OUT = 0;
    delay_us(4);
    SCL_OUT = 0;
}

void eprom_iic_stop(void)
{
    eprom_iic_sda_set(DIR_OUT);
    SCL_OUT = 0;
    SDA_OUT = 0;
    delay_us(4);
    SCL_OUT = 1;
    SDA_OUT = 1;
    delay_us(4);
}

uint8_t eprom_iic_wait_ack(void)
{
    uint8_t errCnt = 0;
    eprom_iic_sda_set(DIR_IN);
    SDA_OUT = 1;
    delay_us(1);
    SCL_OUT = 1;
    delay_us(1);
    while(SDA_IN)
    {
        errCnt++;
        if(errCnt > 250)
        {
            eprom_iic_stop();
            return 1;
        }
    }
    SCL_OUT = 0;
    return 0;
}

void eprom_iic_ack(void)
{
    SCL_OUT = 0;
    eprom_iic_sda_set(DIR_OUT);
    SDA_OUT = 0;
    delay_us(2);
    SCL_OUT = 1;
    delay_us(2);
    SCL_OUT = 0;
}

void eprom_iic_nack(void)
{
    SCL_OUT = 0;
    eprom_iic_sda_set(DIR_OUT);
    SDA_OUT = 1;
    delay_us(2);
    SCL_OUT = 1;
    delay_us(2);
    SCL_OUT = 0;
}

void eprom_iic_write_byte(uint8_t dat)
{
    uint8_t idx;
    eprom_iic_sda_set(DIR_OUT);
    SCL_OUT = 0;
    for(idx = 0; idx < 8; idx++)
    {
        SDA_OUT = (dat & 0x80) >> 7;
        dat <<= 1;
        delay_us(2);
        SCL_OUT = 1;
        delay_us(2);
        SCL_OUT = 0;
        delay_us(2);
    }
}

uint8_t eprom_iic_read_byte(uint8_t ack)
{
    uint8_t idx, dat = 0;
    eprom_iic_sda_set(DIR_IN);
    for(idx = 0; idx < 8; idx++ )
    {
        SCL_OUT = 0;
        delay_us(2);
        SCL_OUT = 1;
        dat <<= 1;
        if(SDA_IN)
            dat++;
        delay_us(1);
    }
    if (!ack)
        eprom_iic_nack();
    else
        eprom_iic_ack();
    return dat;
}

