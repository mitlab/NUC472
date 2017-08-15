


#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif


	
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <types.h>

//#include "NUC472_442.h"
//#include <stdbool.h>


#define UART_BUF_MAX			(1024)

extern uint8_t U0RxBuf[UART_BUF_MAX];
extern uint8_t U0RxCmdBuf[UART_BUF_MAX];
extern volatile uint16_t U0RxHead;
extern volatile uint16_t U0RxTail;

#define UART0_RXBUF_CLEAR()									\
	do {													\
		memset((void *)U0RxBuf, '\0', sizeof(U0RxBuf));		\
		 U0RxHead = U0RxTail;								\
	} while(0);


__STATIC_INLINE bool UART0_BufIsFull(void)
{
	if(U0RxHead == (U0RxTail+1) % UART_BUF_MAX) {
		printf("queue is full\r\n");
		UART0_RXBUF_CLEAR();
		return true;
	}
	
	return false;
}

__STATIC_INLINE uint8_t UART0_PutRxBuffer(uint8_t c)
{
	if(!UART0_BufIsFull()) {
		U0RxBuf[U0RxTail] = c;
		U0RxTail = ++U0RxTail % UART_BUF_MAX;
	}
	
	return c;
}

__STATIC_INLINE bool UART0_BufIsEmpty(void)
{
	if(U0RxTail == U0RxHead) {
		return TRUE;
	}
	
	return FALSE;
}

__STATIC_INLINE uint8_t UART0_GetRxBuffer(void)
{
	uint8_t c = NULL;
#if 0
	if(U0RxTail == U0RxHead) {
		//queue is empty
		return NULL;
	}
#endif
	
	c =  U0RxBuf[U0RxHead++ % UART_BUF_MAX];
	
	return c;
}

__STATIC_INLINE void UART0_RxBuf_Init(void) 
{
	U0RxHead = U0RxTail = 0;
	memset((void *)U0RxBuf, '\0', sizeof(U0RxBuf));
}

__STATIC_INLINE void UART0_RxCmdBuf_Init(void)
{
	memset((void *)U0RxCmdBuf, '\0', sizeof(U0RxCmdBuf));
}

#ifdef __cplusplus
}
#endif

#endif /* __BUFFER_H__ */
	
/* EOF */
