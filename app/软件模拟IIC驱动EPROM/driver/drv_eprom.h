#ifndef __DRV_24C02_H__
#define __DRV_24C02_H__

#include "sys.h"
#include "drv_eprom_iic.h"

typedef enum {
    AT24C01 = 127,
    AT24C02 = 255,
    AT24C04 = 511,
    AT24C08 = 1023,
    AT24C16 = 2047,
    AT24C32 = 4095,
    AT24C64 = 8191,
    AT24C128 = 16383,
    AT24C256 = 32767 , 
}eprom_type;

uint8_t eprom_init(eprom_type type);
uint8_t eprom_read_byte(uint8_t addr);
uint8_t eprom_write_byte(uint8_t addr, uint8_t dat);
uint8_t eprom_test(uint8_t start, uint8_t end);

extern void delay_ms(uint16_t nms);

#endif
