/****************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 9 $
 * $Date: 14/10/06 9:56a $
 * @brief    Transmit and receive data from PC terminal through RS232 interface.
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/

#include "system.h"
#include "buffer.h"
#include "cmd.h"
#include "uart.h"
//#include "rtc.h"




/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t g_u8SendData[12] = {0};
uint8_t g_u8RecData[UART_BUF_MAX]  = {0};

volatile uint32_t g_u32comRbytes = 0;
volatile uint32_t g_u32comRhead  = 0;
volatile uint32_t g_u32comRtail  = 0;
volatile int32_t g_bWait         = TRUE;
volatile int32_t g_i32pointer = 0;
volatile bool bCmdEnable = false;
static uint16_t rxBytes = 0;
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

volatile uint32_t  g_u32TICK = 0;

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void);
void UART0_TEST_HANDLE(void);
void UART1_TEST_HANDLE(void);
void UART2_TEST_HANDLE(void);
void UART4_TEST_HANDLE(void);
void UART5_TEST_HANDLE(void);
void UART_FunctionTest(void);

void UART0_Process(void);

void RTC_Init(void);
void Delay(uint32_t ucnt);

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    //CLK->PWRCTL |= CLK_PWRCTL_HXTEN_Msk; // XTAL12M (HXT) Enabled
	CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
	
    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Switch HCLK clock source to XTAL */
    //CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLKSEL_Msk;
    //CLK->CLKSEL0 |= CLK_CLKSEL0_HCLKSEL_HXT;
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));
	
	CLK->PLLCTL |= CLK_PLLCTL_PD_Msk; 					       	   //1Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.
    CLK->PLLCTL = CLK_PLLCTL_84MHz_HXT ;          					         //1Set PLL frequency  (  CLK_PLLCTL_84MHz_HXT  )
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);  			       	   //1 Waiting for clock ready 
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(1));
    
	/* Enable IP clock */
    //CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk; // UART0 Clock Enable
    //CLK->APBCLK0 |= CLK_APBCLK0_UART1CKEN_Msk; // UART1 Clock Enable
	//CLK->APBCLK0 |= CLK_APBCLK0_UART2CKEN_Msk; // UART2 Clock Enable

	/* Enable IP clock */
    CLK_EnableModuleClock(UART0_MODULE);
	CLK_EnableModuleClock(UART1_MODULE);
	CLK_EnableModuleClock(UART2_MODULE);
	CLK_EnableModuleClock(UART3_MODULE);
	CLK_EnableModuleClock(UART4_MODULE);
	CLK_EnableModuleClock(UART5_MODULE);
	
	// RTC
	CLK_EnableModuleClock(RTC_MODULE);
	
   	// Select IP clock source 
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));
	CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));
	CLK_SetModuleClock(UART4_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));
	CLK_SetModuleClock(UART5_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));
	/* Select IP clock source */
    //CLK->CLKSEL1 &= ~CLK_CLKSEL1_UARTSEL_Msk;
    //CLK->CLKSEL1 |= (0x0 << CLK_CLKSEL1_UARTSEL_Pos);// Clock source from external 12 MHz or 32 KHz crystal clock

	CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLKSEL_HXT);
	CLK_EnableSysTick(CLK_CLKSEL0_STCLKSEL_HXT, 0xFFFFFFUL);
	

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();
	
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PG multi-function pins for UART0 RXD, TXD */
    SYS->GPG_MFPL &= ~(SYS_GPG_MFPL_PG1MFP_Msk | SYS_GPG_MFPL_PG2MFP_Msk);
    SYS->GPG_MFPL |= (SYS_GPG_MFPL_PG1MFP_UART0_RXD | SYS_GPG_MFPL_PG2MFP_UART0_TXD);


#if 0 // UART1

	/* Set PB multi-function pins for UART1 RXD, TXD, RTS and CTS */
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB2MFP_Msk | SYS_GPB_MFPL_PB3MFP_Msk);
    SYS->GPB_MFPL |=  (SYS_GPB_MFPL_PB2MFP_UART1_RXD | SYS_GPB_MFPL_PB3MFP_UART1_TXD);
#else

	/* Set PB multi-function pins for UART1 RXD, TXD */
    //SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB2MFP_Msk | SYS_GPB_MFPL_PB3MFP_Msk);
    //SYS->GPB_MFPL |= (SYS_GPB_MFPL_PB2MFP_UART1_RXD | SYS_GPB_MFPL_PB3MFP_UART1_TXD)
	
	/* Set PB multi-function pins for UART1 RXD, TXD */
    SYS->GPF_MFPH &= ~(SYS_GPF_MFPH_PF13MFP_Msk | SYS_GPF_MFPH_PF14MFP_Msk);
   SYS->GPF_MFPH |= (SYS_GPF_MFPH_PF14MFP_UART1_RXD | SYS_GPF_MFPH_PF13MFP_UART1_TXD);
#endif


#if 1 // UART2
		/* Set PB multi-function pins for UART2 RXD, TXD */
    SYS->GPC_MFPH &= ~(SYS_GPC_MFPH_PC10MFP_Msk | SYS_GPC_MFPH_PC11MFP_Msk);
    SYS->GPC_MFPH |=  (SYS_GPC_MFPH_PC10MFP_UART2_RXD | SYS_GPC_MFPH_PC11MFP_UART2_TXD);

#else
		/* Set PB multi-function pins for UART2 RXD, TXD */
    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF6MFP_Msk | SYS_GPF_MFPL_PF7MFP_Msk);
    SYS->GPF_MFPL |= (SYS_GPF_MFPL_PF6MFP_UART2_RXD | SYS_GPF_MFPL_PF7MFP_UART2_TXD);
#endif

	// UART4
	SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB4MFP_Msk | SYS_GPB_MFPL_PB5MFP_Msk );         
   	SYS->GPB_MFPL |=  (SYS_GPB_MFPL_PB4MFP_UART4_RXD | SYS_GPB_MFPL_PB5MFP_UART4_TXD) ;
   
   // SYS->GPG_MFPH &= ~(SYS_GPG_MFPH_PG15MFP_Msk             | SYS_GPG_MFPH_PG9MFP_Msk  );
   // SYS->GPG_MFPH |=    (SYS_GPG_MFPH_PG8MFP_UART4_RTS | SYS_GPG_MFPH_PG9MFP_UART4_CTS);
    
	// UART5
	SYS->GPD_MFPH &= ~(SYS_GPD_MFPH_PD15MFP_Msk);
	SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF0MFP_Msk);         
   	SYS->GPD_MFPH |=  (SYS_GPD_MFPH_PD15MFP_UART5_TXD);
	SYS->GPF_MFPL |=  (SYS_GPF_MFPL_PF0MFP_UART5_RXD);
   
    //SYS->GPG_MFPH &= ~(SYS_GPG_MFPH_PG8MFP_Msk             | SYS_GPG_MFPH_PG9MFP_Msk  );
    //SYS->GPG_MFPH |=    (SYS_GPG_MFPH_PG8MFP_UART4_RTS | SYS_GPG_MFPH_PG9MFP_UART4_CTS);
	
	/* Lock protected registers */
    SYS_LockReg();

}

void UART_Init(UART_T *uartport, uint32_t u32baudrate, uint32_t u32intsel, IRQn_Type irqn)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
	
    UART_Open(uartport, u32baudrate);
	UART_ENABLE_INT(uartport, u32intsel);
    NVIC_EnableIRQ(irqn);
}

#if 0
void UART1_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    UART_Open(UART1, UART_BAUD_RATE);
    UART_ENABLE_INT(UART1, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));
	NVIC_EnableIRQ(UART1_IRQn);
}

void UART2_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    UART_Open(UART2, UART_BAUD_RATE);
    UART_ENABLE_INT(UART2, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));
    NVIC_EnableIRQ(UART2_IRQn);
}

void UART4_Init()
{
	UART_Open(UART4, UART_BAUD_RATE);
	UART_ENABLE_INT(UART4, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));
	NVIC_EnableIRQ(UART4_IRQn);
}

void UART5_Init()
{
	UART_Open(UART5, UART_BAUD_RATE);
	UART_ENABLE_INT(UART5, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));
	NVIC_EnableIRQ(UART5_IRQn);
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* UART Test Sample                                                                                        */
/* Test Item                                                                                               */
/* It sends the received data to Hyper Terminal.                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();
	
	
    //CLK_SysTickDelay(5000); 
	
	//NVIC_EnableIRQ(SysTick_IRQn);
	
	/* Init UART0 for printf */
    UART_Init(UART0, UART0_BAUD_RATE, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk), UART0_IRQn);
	UART_Init(UART1, UART1_BAUD_RATE, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk), UART1_IRQn);
	UART_Init(UART2, UART2_BAUD_RATE, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk), UART2_IRQn);
	UART_Init(UART3, UART3_BAUD_RATE, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk), UART3_IRQn);
    UART_Init(UART4, UART4_BAUD_RATE, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk), UART4_IRQn);
	UART_Init(UART5, UART5_BAUD_RATE, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk), UART5_IRQn);
	
	UART0_RxBuf_Init();
	UART0_RxCmdBuf_Init();
	
	
    /*---------------------------------------------------------------------------------------------------------*/
    /* SAMPLE CODE                                                                                             */
    /*---------------------------------------------------------------------------------------------------------*/

    printf("\n\nCPU @ %dMHz\n", SystemCoreClock/1000000);

    printf("+---------------------+\n");
    printf("| UART function test  |\n");
    printf("+---------------------+\n");

    UART_FunctionTest();


	

#if 0
	RTC_Init();
	 /* Set Tick Period */
    RTC_SetTickPeriod(RTC_TICK_1_SEC);
	
	/* Enable RTC Tick Interrupt */
    RTC_EnableInt(RTC_INTEN_TICKIEN_Msk);
    NVIC_EnableIRQ(RTC_IRQn);
	
	g_u32TICK = 0;
    while(g_u32TICK < 5);

    printf("\n RTC Time Display Test End !!\n");

	/* Disable RTC Tick Interrupt */
    RTC_DisableInt(RTC_INTEN_TICKIEN_Msk);
    NVIC_DisableIRQ(RTC_IRQn);
#endif
    while(1)
	{
		//UART0_Process();
		
		if(bCmdEnable) {
			bCmdEnable = false;
			
			//if(U0RxCmdBuf[0] == 'A' && U0RxCmdBuf[1] == 'T') {
			if(!strncmp((const char*)U0RxCmdBuf, AT_CMD, strlen(AT_CMD))) {
				printf("%s\n", U0RxCmdBuf);
			}
			memset(U0RxCmdBuf, '\0', sizeof(U0RxCmdBuf));
		}
		
		if(sysTickCount%1000UL) {
			if(sysTickCountOld != sysTickCount) {
				printf("%ld\r\n", sysTickCount);
				sysTickCountOld = sysTickCount;
			}
		}
			
	}
}

void SysTick_Handler(void)
{
	
	sysTickCount++;
	
}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
    uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART0->INTSTS;

	if(u32IntSts & UART_INTSTS_RDAINT_Msk) {
		while(UART_IS_RX_READY(UART0)) 
		{
			u8InChar = UART_READ(UART0);           /* Rx trigger level is 1 byte*/
			
			if((u8InChar == '\r' || u8InChar == '\n') && rxBytes > 1) {
				memcpy(U0RxCmdBuf, U0RxBuf, strlen((const char*)U0RxBuf));
				rxBytes = 0;
				memset(U0RxBuf, '\0', sizeof(U0RxBuf));
				bCmdEnable = true;
			} else {
				U0RxBuf[rxBytes++] = u8InChar;
			}
		}
	}
}

void UART1_IRQHandler(void)
{
    UART1_TEST_HANDLE();
}

void UART2_IRQHandler(void)
{
    UART2_TEST_HANDLE();
}


void UART4_IRQHandler(void)
{
    UART4_TEST_HANDLE();
}

void UART5_IRQHandler(void)
{
    UART5_TEST_HANDLE();
}
/*---------------------------------------------------------------------------------------------------------*/
/* UART Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_TEST_HANDLE()
{
    //uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART0->INTSTS;

	if(u32IntSts & UART_INTSTS_RDAINT_Msk) {
		while(UART_IS_RX_READY(UART0)) 
		{
			//u8InChar = UART_READ(UART0);           /* Rx trigger level is 1 byte*/
			UART0_PutRxBuffer(UART_READ(UART0));
			//UART_WRITE(UART0, u8InChar); 
			//UART_WRITE(UART1, u8InChar);
			//UART_WRITE(UART2, u8InChar);
			//UART_WRITE(UART3, u8InChar);
			//UART_WRITE(UART4, u8InChar);
			//UART_WRITE(UART5, u8InChar);
		}
	}
}

void UART1_TEST_HANDLE()
{
    uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART1->INTSTS;

		if(u32IntSts & UART_INTSTS_RDAINT_Msk) {
				while(UART_IS_RX_READY(UART1)) {
					u8InChar = UART_READ(UART1);           /* Rx trigger level is 1 byte*/
					
					UART_WRITE(UART0, u8InChar);
					UART_WRITE(UART1, u8InChar);
					UART_WRITE(UART2, u8InChar);
					UART_WRITE(UART3, u8InChar);
					UART_WRITE(UART4, u8InChar);
					UART_WRITE(UART5, u8InChar);
				}
		}
}

void UART2_TEST_HANDLE()
{
    uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART2->INTSTS;

		if(u32IntSts & UART_INTSTS_RDAINT_Msk) {
				while(UART_IS_RX_READY(UART2)) {
					u8InChar = UART_READ(UART2);           /* Rx trigger level is 1 byte*/
					
					UART_WRITE(UART0, u8InChar);
					UART_WRITE(UART1, u8InChar);
					UART_WRITE(UART2, u8InChar);
					UART_WRITE(UART3, u8InChar);
					UART_WRITE(UART4, u8InChar);
					UART_WRITE(UART5, u8InChar);
				}
		}
}


void UART4_TEST_HANDLE()
{
    uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART4->INTSTS;

		if(u32IntSts & UART_INTSTS_RDAINT_Msk) {
				while(UART_IS_RX_READY(UART4)) {
					u8InChar = UART_READ(UART4);           /* Rx trigger level is 1 byte*/
					
					UART_WRITE(UART0, u8InChar);
					UART_WRITE(UART1, u8InChar);
					UART_WRITE(UART2, u8InChar);
					UART_WRITE(UART3, u8InChar);
					UART_WRITE(UART4, u8InChar);
					UART_WRITE(UART5, u8InChar);
				}
		}
}

void UART5_TEST_HANDLE()
{
    uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART5->INTSTS;

		if(u32IntSts & UART_INTSTS_RDAINT_Msk) {
				while(UART_IS_RX_READY(UART5)) {
					u8InChar = UART_READ(UART5);           /* Rx trigger level is 1 byte*/
					
					UART_WRITE(UART0, u8InChar);
					UART_WRITE(UART1, u8InChar);
					UART_WRITE(UART2, u8InChar);
					UART_WRITE(UART3, u8InChar);
					UART_WRITE(UART4, u8InChar);
					UART_WRITE(UART5, u8InChar);
				}
		}
}
/*---------------------------------------------------------------------------------------------------------*/
/*  UART Function Test                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void UART_FunctionTest()
{
    printf("+-----------------------------------------------------------+\n");
    printf("|  UART Function Test                                       |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  Description :                                            |\n");
    printf("|    The sample code will print input char on terminal      |\n");
    printf("|    Please enter any to start     (Press '0' to exit)      |\n");
    printf("+-----------------------------------------------------------+\n");
}

void RTC_Init(void)
{
	S_RTC_TIME_DATA_T sInitTime;
	
	/* Time Setting */
    sInitTime.u32Year       = 2017;
    sInitTime.u32Month      = 8;
    sInitTime.u32Day        = 13;
    sInitTime.u32Hour       = 06;
    sInitTime.u32Minute     = 49;
    sInitTime.u32Second     = 0;
    sInitTime.u32DayOfWeek  = RTC_SUNDAY;
    sInitTime.u32TimeScale  = RTC_CLOCK_24;

    RTC_Open(&sInitTime);

    printf("\n RTC Time Display Test (Exit after 5 seconds)\n\n");

}

void RTC_TickHandle(void)
{
    S_RTC_TIME_DATA_T sCurTime;

    /* Get the current time */
    RTC_GetDateAndTime(&sCurTime);

    printf(" Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32Month,sCurTime.u32Day,sCurTime.u32Hour,sCurTime.u32Minute,sCurTime.u32Second);

    g_u32TICK++;
}

/**
  * @brief  RTC ISR to handle interrupt event
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{

    if ( (RTC->INTEN & RTC_INTEN_TICKIEN_Msk) && (RTC->INTSTS & RTC_INTSTS_TICKIF_Msk) ) {      /* tick interrupt occurred */
        RTC->INTSTS = 0x2;

        RTC_TickHandle();
    }

}

void Delay(uint32_t ucnt)
{
    volatile uint32_t i = ucnt;

    while (i--);
}



void UART0_Process(void)
{
	uint8_t inChar = NULL;
	
	if(!UART0_BufIsEmpty()) {
		
		inChar = UART0_GetRxBuffer();
		U0RxCmdBuf[rxBytes] = inChar;
		
		if(inChar == '\r' || inChar == '\n') {
			
			//inChar = '\n';
			//if(rxBytes > 0) {
				//UART_WRITE(UART0, inChar);
			//}
			if(rxBytes > 2) {
				if(U0RxCmdBuf[0] == 'A' && U0RxCmdBuf[1] == 'T') {
					bCmdEnable = true;
				}
			}
			rxBytes = 0;
		}
	}
}


