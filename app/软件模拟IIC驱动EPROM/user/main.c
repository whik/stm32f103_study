#include "main.h"

int main(void)
{
    delay_init();
    led_init();
    uart1_init(115200);
    
    if(!eprom_init(AT24C02))
        LOG1("EPROM initial success\r\n");
    else 
        LOG1("EPROM initial failed\r\n");
        
    if(!eprom_test(10, 50))
        LOG1("EPROM test success\r\n");
    
    LOG1("SystemCoreClock: %d\r\n", SystemCoreClock);
    
    while(1)
    {
        led_blink(100);
    }
    return 0;
}


