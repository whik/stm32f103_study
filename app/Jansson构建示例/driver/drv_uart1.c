#include "drv_uart1.h"

uint16_t uart1_rx_idx = 0;
uint8_t uart1_rx_buf[UART1_RX_MAX_SIZE];
#if 1
//{"status":true,"age":20,"score":78.4,"name":"whik1194","blog":"https://blog.csdn.net/whik1194","wechat":"mcu149"}";
extern uint8_t rx_ok_flag;
extern uint8_t Jansson1_Demo(uint8_t *text);

void USART1_IRQHandler(void)
{
    uint8_t dat;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(USART1); 
        //必须是回车换行结束
        uart1_rx_buf[uart1_rx_idx] = dat;
        if(dat == '\r')
        {
            rx_ok_flag = 1;
        #ifndef JASSON_PASE_IN_MAIN
            //中断函数中进行解析
            Jansson1_Demo(uart1_rx_buf);
            memset(uart1_rx_buf, 0, sizeof(uart1_rx_buf));
            uart1_rx_idx = 0;
        #endif
        }
        else
            uart1_rx_idx++;
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
#else
void USART1_IRQHandler(void)
{
    uint8_t dat;
    static uint8_t flag_head = 0;
    static uint8_t flag_tail = 0;

    // 数据必须是* + data + #格式: *124245643#
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(USART1);
        switch(dat)
        {
        case '*':
            flag_head = 1;
            flag_tail = 0;
            break;
        case '#':
            flag_tail = 1;
            if(flag_head && flag_tail)
            {
                LOG1("rx_buf: %d-%s\r\n", uart1_rx_idx, uart1_rx_buf);
                memset(uart1_rx_buf, 0, sizeof(uart1_rx_buf));
                uart1_rx_idx = 0;
                flag_head = 0;
                flag_tail = 0;
            }
            break;
        default:
            uart1_rx_buf[uart1_rx_idx] = dat;
            uart1_rx_idx++;
            break;
        }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
#endif

void LOG1(char *fmt, ...)
{
    unsigned char UsartPrintfBuf[296];
    va_list ap;
    unsigned char *pStr = UsartPrintfBuf;

    va_start(ap, fmt);
    vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
    va_end(ap);

    while(*pStr != 0)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, *pStr++);
    }
}
void uart1_init(uint32_t baud_value)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = baud_value;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}
