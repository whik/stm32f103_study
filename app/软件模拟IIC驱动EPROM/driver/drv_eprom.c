#include "drv_eprom.h"

eprom_type EPROM_TYPE;
extern void LOG1(char *fmt, ...);

/*
0: OK
1: ERROR
*/
uint8_t eprom_init(eprom_type type)
{
    uint8_t ret = 0;
    uint8_t dat = 0;
    
    EPROM_TYPE = type;
    eprom_iic_init();
    dat = eprom_read_byte(0xff); 
    
    if(dat == 0X55)
        return ret;
    else
    {
        ret = eprom_write_byte(0xff, 0x55);
        dat = eprom_read_byte(255);
        if(dat == 0X55)
            ret = 0;
        else
            ret = 1;
    }
    return ret;
}
/*
0: OK
1: ERROR
*/
uint8_t eprom_read_byte(uint8_t addr)
{
    uint8_t dat;
    eprom_iic_start();
    if(EPROM_TYPE > AT24C16)
    {
        eprom_iic_write_byte(0XA0);	   //发送写命令
        eprom_iic_wait_ack();
        eprom_iic_write_byte(addr >> 8); //发送高地址
        eprom_iic_wait_ack();
    }
    else
        eprom_iic_write_byte(0XA0 + ((addr / 256) << 1)); 

    eprom_iic_wait_ack();
    eprom_iic_write_byte(addr % 256); 
    eprom_iic_wait_ack();
    eprom_iic_start();
    eprom_iic_write_byte(0XA1);      
    eprom_iic_wait_ack();
    dat = eprom_iic_read_byte(0);
    eprom_iic_stop();
    
    return dat;
}

/*
0: OK
1: ERROR
*/
uint8_t eprom_write_byte(uint8_t addr, uint8_t dat)
{
    uint8_t ret = 0;
    
    eprom_iic_start();
    if(EPROM_TYPE > AT24C16)
    {
        eprom_iic_write_byte(0XA0);	 
        eprom_iic_wait_ack();
        eprom_iic_write_byte(addr >> 8);
    }
    else
        eprom_iic_write_byte(0XA0 + ((addr / 256) << 1)); 

    ret = eprom_iic_wait_ack();
    eprom_iic_write_byte(addr % 256);
    ret = eprom_iic_wait_ack();
    eprom_iic_write_byte(dat); 
    ret = eprom_iic_wait_ack();
    eprom_iic_stop();
    delay_ms(10);
    
    return ret;
}

uint8_t eprom_test(uint8_t start, uint8_t end)
{
    uint8_t addr = 0;
    uint8_t dat;
    uint8_t ret = 0;
    
    for(addr = start; addr < end; addr++)
    {
        eprom_write_byte(addr, addr);
        delay_ms(100);
    }
    for(addr = start; addr < end; addr++)
    {
        dat = eprom_read_byte(addr);
        LOG1("read success: %3d: %3d\r\n", addr, dat);
        delay_ms(100);
        if(dat != addr)
        {
            LOG1("EPROM Verification failed: %d\r\n", addr);
            return addr;
        }
    }
    LOG1("EPROM Verification success!\r\n");
    return 0;
}

