#ifndef __CMD_H__
#define __CMD_H__

#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif


	
typedef enum {
	AT_CMD,
	AT_CPIN
} atCmd_t;



#ifdef USE_TELIT_LTE_M

// For Telit LTE Modem

#define AT_CMD			"AT"
#define AT_E			"ATE"			// Command Echo 0: disable, 1: enable(factory default)
// AT&C - DCD , AT&D - DTR , AT&K - Flow Control , AT&S - DSR Control
#define AT_CGMR			"AT+CGMR"		// Check the Software Versions
#define AT_CGMM 		"AT+CGMM"		// Check the Module Identification
#define AT_CPIN 		"AT+CPIN?"		// USIM Pin Code and PUK Code Configurations
#define AT_QSS			"AT#QSS"		// USIM Status
#define AT_QSS_R		"#QSS"			// response
#define AT_CNUM 		"AT+CNUM"		// GET_PhoneNumber
#define AT_CEREG		"AT+CEREG?"		// 
#define AT_CEREG_R		"+CEREG:"		// CEREG Respons
#define AT_CGDCONT 		"AT+CGDCONT"	// APN AT+CGDCONT=1,"IPV4V6","www.ktf.com"
#define AT_CGDCONT_R 	"+CGPADDR:"		// respons 
#define AT_CCLK 		"AT#CCLK?"		// GetTime_ME_RTC
#define AT_GPIO			"AT#GPIO"		// GPIO
#define AT_SIMINCFG		"AT#SIMINCFG"	// USIM_DETECT_Pin

#define AT_CNUM			"AT+CNUM"		// Get_Phone_Number
#define AT_SD			"AT#SD"			// Socket_Dial
#define AT_SSEND		"AT#SSEND"		// TCP_Send
#define AT_SCFGEXT		"AT#SCFGEXT"	// Set_Socket_Configuration
#define AT_SGACT_Q		"AT#SGACT?"		// Get_TCP_IP_LTE_MODULE
#define AT_CMEE			"AT+CMEE"		// Report_Mobile_Equipment_Error
#define AT_CSCS			"AT+CSCS"		// Select_TE_Character_Set
#define AT_IPR_Q		"AT+IPR=?"		// Main Serial Port Speed Configuration
#define AT_IPR_R		"+IPR:"			// response +IPR: 38400 \r\n OK
#define AT_W			"AT&W0"			// store the setting on profile 0
#define AT_P			"AT&P0"			// at power on use profile 0
#define AT_OK			"OK"			// response OK

// For Telit LTE Modem
#endif

#define PLUS		"+"
#define EQAL		"="
#define COMMA		","
#define POINT		"."


#define LF			0x0A
#define CR			0x0D
#define CTRL_Z		0x1A
#define CR_LF		"\r\n"

#define IP_RX_CMD     "AT#SGACT=1,1\r\n"
#define    Socket_Dial_KT_CMD     "AT#SD=1,0,10020,\"220.90.216.90\",0,0,1\r\n"
#define    StringData_Send_CMD     "AT#SSEND=1\r\n"
#define    Kt_Sever_Connect_CMD     "000000831101002360E00000014E055FDFCE0000000000000000000000000000000000000300007B22657874725379734964223A224F50454E5F5443505F30303150544C3030315F31303030303031393632222C226465766963654964223A2254657374444C50303032222C226174686E5271744E6F223A226B6D73343336303064303031227D\x1A"
#define KeepALIVE_KMS_CMD  "000000231101002360E70000014E055FDFCE675F9C3B00000000FAD19A3B000000000000030000\x1A"

#define fire_Sensor   "000000F811010023619B00000153CFA6F9E1000000003B9AD1FA000000003B9C5F4A00000300007B22657874725379734964223A224F50454E5F5443505F30303150544C3030315F31303030303031393632222C22646576436F6C656344617461564F73223A5B7B226465766963654964223A2254657374444C50303032222C22636F6C6563526F77564F73223A5B7B2273747244617461496E666F564F73223A5B7B22736E736E5461674364223A22747373303032222C2273747256616C223A2246697265227D2C7B22736E736E5461674364223A224E616D65222C2273747256616C223A22506F68616E6720312041726561227D5D7D5D7D5D7D\x1A" 

#define    USIM_Test_CMD     "AT+CPIN?\r\n"
#define    GetPhoneNumber_CMD     "AT+CNUM\r\n"
#define    Get_APN_CMD     "AT+CGDCONT?\r\n"

#define    Set_16_bit_character_CMD     "AT+CSCS=UCS2\r\n"

#define    ee_CMD     "\r\n"

#ifdef __cplusplus
}
#endif

#endif /* __CMD_H__ */

/* EOF */
