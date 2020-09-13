#include "main.h"

uint8_t rx_ok_flag = 0;
int main(void)
{
    delay_init();
    led_init();
    uart1_init(115200);
    Jansson2_Demo();
    Jansson3_Demo();
    
    LOG1("SystemCoreClock: %d\r\n", SystemCoreClock);
    
    while(1)
    {
        #ifdef JASSON_PASE_IN_MAIN
        //主函数中进行解析
        if(rx_ok_flag == 1)
        {
            Jansson1_Demo(uart1_rx_buf);
            rx_ok_flag = 0;
            memset(uart1_rx_buf, 0, sizeof(uart1_rx_buf));
            uart1_rx_idx = 0;
        }
        #endif
    }
    return 0;
}


