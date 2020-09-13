#ifndef __DRV_24C02_IIC_H__
#define __DRV_24C02_IIC_H__

/*
I2C_SCL - PB6
I2C_SDA - PB7
*/
#include "sys.h"

typedef enum {
    DIR_IN = 0,
    DIR_OUT
}sda_direction;

#define SCL_OUT PBout(6)
#define SDA_OUT PBout(7)
#define SDA_IN  PBin(7)
 
void eprom_iic_init(void);
void eprom_iic_sda_set(sda_direction dir);
void eprom_iic_start(void);
void eprom_iic_stop(void);
uint8_t eprom_iic_wait_ack(void);
void eprom_iic_ack(void);
void eprom_iic_nack(void);
void eprom_iic_write_byte(uint8_t dat);
uint8_t eprom_iic_read_byte(uint8_t ack);

extern void delay_us(uint32_t nus);


#endif
