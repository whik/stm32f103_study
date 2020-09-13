#ifndef __DRV_UART1_H__
#define __DRV_UART1_H__

#include "sys.h"
#include "delay.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "def.h"

#define UART1_RX_MAX_SIZE 1024

void uart1_init(uint32_t baud_value);
void LOG1(char *fmt,...);

extern uint16_t uart1_rx_idx;
extern uint8_t uart1_rx_buf[UART1_RX_MAX_SIZE];


#endif

