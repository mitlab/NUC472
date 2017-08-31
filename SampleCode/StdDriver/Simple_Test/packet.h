#ifndef __PACKET_H__
#define __PACKET_H__

#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif

	
/* ICT packet type

{
	"MSGTIME":"20170816134622",		
	"DEVICEID":"D01223464185",
	"OPCODE":400,
	"RSSI":-74 ,
	"IN24V":15,
	"INB24V":18,
	"S1":01,
	"mA1":00,
	"mA2":04,
	"mA3":04,
	"mA4":04
} */

typedef struct {
	char cur_time[14];
	char opcode[3];
	char rssi[4];
	char power[2];
	char battery[2];
	uint16_t sw;
	uint16_t ma1[4];
} msg_t ;

typedef struct {
	dev_t dev;
	msg_t msg;
} packet_t ;


/* QAT packet type
{
	"extrSysId":"OPEN_TCP_001PTL001_1000001962",
	"devColecDataVOs":
		[{"deviceId":"E01223584698",
			"colecRowVOs":
				[{"snsnDataInfoVOs":
					[{"dataTypeCd":"S1","snsnVal":"0"},
					 {"dataTypeCd":"S2","snsnVal":"0"},
					 {"dataTypeCd":"S3","snsnVal":"0"},
					 {"dataTypeCd":"S4","snsnVal":"0"},
					 {"dataTypeCd":"S5","snsnVal":"0"},
					 {"dataTypeCd":"IN24V","snsnVal":"024"},
					 {"dataTypeCd":"EX24V","snsnVal":"023"},
					 {"dataTypeCd":"EXB24V","snsnVal":"022"},
					 {"dataTypeCd":"I4TO20mA","snsnVal":"10"},
					 {"dataTypeCd":"RSSI","snsnVal":"-080"}
					]
				}]
		}]
}

*/
	
	
#ifdef __cplusplus
}
#endif

#endif /* __PACKET_H__ */

/* EOF */

