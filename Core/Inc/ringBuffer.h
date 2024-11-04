/**
  ******************************************************************************
  * @file    ringBuffer.h
  * @brief   Communication received ring-buffer
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#ifdef __cplusplus
	extern "C"	{
#endif

	#include "stm32f7xx_hal.h"
	//#include "define.h"
	// #include "common.h"
	#include "string.h"


	#define SERIAL_BUF_SIZE     512

	

	//링버퍼 구조체 선언
	typedef struct {    // creation ring buffer
	    int tail;       // read data pointer
	    int head;       // write data pointer
	    uint8_t data[SERIAL_BUF_SIZE]; // receive buffer
	} t_rb;

	uint8_t rb_put( t_rb *rb , uint8_t d );
	uint8_t rb_get( t_rb *rb, uint8_t *err);
	uint8_t finder ( t_rb *rb, uint8_t ch );

	uint16_t rb_removeAt( uint8_t* buf, uint16_t count);
	uint16_t rb_removeRange( uint8_t* buf, uint16_t count, uint16_t length);

	extern t_rb OledRecvBuf, WifiRecvBuf, bcRecvBuf, BarcodeRecvBuf;

#ifdef __cplusplus
	}
#endif


#endif /* __RING_BUFFER_H */