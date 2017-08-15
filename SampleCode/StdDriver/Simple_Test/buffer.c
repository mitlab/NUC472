

#include "buffer.h"


uint8_t U0RxBuf[UART_BUF_MAX];
uint8_t U0RxCmdBuf[UART_BUF_MAX];

volatile uint16_t U0RxHead;
volatile uint16_t U0RxTail;


/* EOF */

