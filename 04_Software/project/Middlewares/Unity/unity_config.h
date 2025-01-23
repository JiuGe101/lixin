#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "SEGGER_RTT.h"
#define UNITY_OUTPUT_CHAR(a) SEGGER_RTT_printf(0, "%c", a);
#define UNITY_OUTPUT_START()
	
#ifdef __cplusplus
}
#endif
	
#endif
