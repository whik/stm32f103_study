#ifndef __DRV_LED_H__
#define __DRV_LED_H__

#include "sys.h"
#include "delay.h"

/*
LED0-PB5
LED1-PE5
*/

typedef enum {
    ON = 0,
    OFF
}ledStatus;

#define LED0 PBout(5)
#define LED1 PEout(5)

void led_init(void);
void led_set(uint8_t num, ledStatus status);
void led_blink(uint16_t period);

#endif

