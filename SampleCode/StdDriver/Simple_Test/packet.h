#ifndef __PACKET_H__
#define __PACKET_H__

#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum {
	PKT_TYPE_A,
	PKT_TYPE_B
};

typedef struct {
	uint8_t gas;	// Gas sonsor
	uint8_t temp;	// Temperature sonsor
	uint8_t hum;	// Humidity Sensor
	uint8_t	ult; 	// Ultrasonic sensor
	uint8_t dust;	// Dust
} sensor_t;

typedef struct {
	uint8_t adc[5];
	uint8_t sw[10];
} input_t;

typedef struct {
	char date[10];
	char time[8];
	uint32_t seqNum;
	char phoneNumber[11];
	char modemId[10];
	char boardId[10];
	uint32_t fwVer;  // 0xFFFFFFFF
	char fwDate[10]; // build date
	char gps[10];
	sensor_t sensor;
	input_t input;
} packet_t;

	
	
#ifdef __cplusplus
}
#endif

#endif /* __PACKET_H__ */

/* EOF */

