#ifndef __TIME_API_H__
#define __TIME_API_H__

#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
	
//----| Time conversion
#define	us2ms(t)	((t) / 1000)
#define	us2s(t)		((t) / 1000000)
#define	ms2s(t)		((t) / 1000)
#define	ms2us(t)	((t) * 1000)
#define	s2us(t)		((t) * 1000000)
#define	s2ms(t)		((t) * 1000)

//----| Differnece Time
#define	DiffTime(t1, t2)	((t1) >= (t2) ? (t1) - (t2) : (t2) + (~t1 + 1))
#define	TIME_DelayUs(uUs)	TIME_Delay(uUs)
#define	TIME_DelayMs(uMs)	TIME_Delay(uMs*1000)

//----| Exported Type Definition |----------------------------------------------
//----| Exported Variable Declaration |-----------------------------------------
//----| Exported Function Prototype |-------------------------------------------
void TIME_Init(void);
uint32_t TIME_Get(void);
void TIME_Base(void);
uint32_t TIME_Diff(void);
void TIME_Delay(uint32_t uDelay);
	
	
#ifdef __cplusplus
}
#endif

#endif /* __TIME_API_H__ */

/* EOF */

