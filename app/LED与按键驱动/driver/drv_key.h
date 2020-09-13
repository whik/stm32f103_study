#ifndef __DRV_KEY_H__
#define __DRV_KEY_H__

/*
- KEY0-PE4
- KEY1-PE3
- KEY2-PE2
*/

#include "sys.h"
#include "delay.h"

#define KEY0    PEin(4)
#define KEY1    PEin(3)
#define KEY2    PEin(2)

void key_init(void);
uint8_t key_get(uint8_t mode);

#endif

