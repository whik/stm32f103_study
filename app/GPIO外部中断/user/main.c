#include "main.h"

int main(void)
{
    delay_init();
    led_init();
    key_init();
    LOG1("SystemCoreClock: %d\r\n", SystemCoreClock);
    while(1)
    {
    }
    return 0;
}


