#include "main.h"

int main(void)
{
    uint8_t key;
    delay_init();
    led_init();
    key_init();
    
    while(1)
    {
        key = key_get(0);
        if(key == 1)
            led_blink(200);
        else if(key == 2)
            led_blink(800);
        else if(key == 0)
            led_blink(50);
    }
}


