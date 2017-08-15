#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "NUC472_442.h"

#ifdef RTC_ENABLE

#include <time.h>

#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define UART0_BAUD_RATE		(115200)
#define UART1_BAUD_RATE		(115200)
#define UART2_BAUD_RATE		(115200)
#define UART3_BAUD_RATE		(115200)
#define UART4_BAUD_RATE		(115200)
#define UART5_BAUD_RATE		(115200)

#ifdef RTC_ENABLE
struct nu_modinit_s {
    int modname;
    uint32_t clkidx;
    uint32_t clksrc;
    uint32_t clkdiv;
    uint32_t rsetidx;
    
    IRQn_Type irq_n;
    void *var;
};
#endif


extern volatile unsigned long sysTickCount;
extern volatile unsigned long sysTickCountOld;

extern void SysTick_Handler(void);

#ifdef RTC_ENABLE
const struct nu_modinit_s *get_modinit(uint32_t modname, const struct nu_modinit_s *modprop_tab);
static const struct nu_modinit_s rtc_modinit = {RTC_BASE, RTC_MODULE, 0, 0, 0, RTC_IRQn, NULL};
static int rtc_inited = 0;
#define YEAR0 	1900
#endif


#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_H__ */

/* EOF */
