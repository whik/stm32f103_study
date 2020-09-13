#include "main.h"

uint8_t rx_ok_flag = 0;
int main(void)
{
    delay_init();
    led_init();
    uart1_init(115200);
    
    Jansson1_Demo();
    Jansson2_Demo();
    Jansson3_Demo();
       
    while(1)
    {

    }
    return 0;
}


