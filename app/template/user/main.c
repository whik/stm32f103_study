#include "main.h"

int main(void)
{
    delay_init();
    led_init();
    LOG1("SystemCoreClock: %d\r\n", SystemCoreClock);
    while(1)
    {
        led_blink(100);
    }
    return 0;
}


