
#include "time_api.h"

uint16_t	uTimeTick;	// in ms uint
uint32_t	uTimeCurrent;
uint32_t	uTimeBase;

//----| Private Function Prototype |--------------------------------------------
//====| Function Start |========================================================
 
/**
  * @brief  Get the current time
  * @param  None
  * @retval current time
  */
uint32_t TIME_Get()
{
	uTimeCurrent	= ((uint32_t)uTimeTick << 16) /* | TIME_TIMER->CNT */;
	return uTimeCurrent;
}

/**
  * @brief  catch the current time
  * @param  None
  * @retval current time
  */
void TIME_Base()
{
	uTimeBase = TIME_Get();
}

/**
  * @brief  Get the difference of two time point
  * @param  None
  * @retval Difference time
  */
uint32_t TIME_Diff()
{
	uint32_t uTimeDiff;

	TIME_Get();
	uTimeDiff = DiffTime(uTimeCurrent, uTimeBase);
	uTimeBase = uTimeCurrent;

	return uTimeDiff;
}

/**
  * @brief  Delay for the specified us
  * @param  uDelay	: the delay in microseconds
  * @retval None
  */
void TIME_Delay(uint32_t uDelay)
{
	uint32_t uTime = TIME_Get();
	while(DiffTime(uTimeCurrent, uTime) < uDelay) TIME_Get();
}


/* EOF */

