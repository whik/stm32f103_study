#include "main.h"

int main(void)
{
    delay_init();
    led_init();
    
    while(1)
    {
        led_blink(100);
    }
    return 0;
}


