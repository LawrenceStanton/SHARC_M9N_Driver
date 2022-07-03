/**
  ******************************************************************************
  * @file           : M9N_C_API.hpp
  * @brief          : Source / Header file for M9N_C_API.c/.h
  * @author			: Lawrence Stanton
  ******************************************************************************
  * @attention
  *
  * © LD Stanton 2022
  * 
  * This file and its content are the copyright property of the author. All 
  * rights are reserved. No warranty is given. No liability is assumed.
  * Confidential unless licensed otherwise. If licensed, refer to the 
  * accompanying file "LICENCE" for license details.
  * 
  ******************************************************************************
  */

#pragma once

#include "M9N_STM32.hpp"
#include "GPS_Struct.h"

//Initialisation/config enum
extern "C" typedef enum{
	GPS_Init_OK = 1,
	GPS_Init_Ack_Error = 2,
	GPS_Init_Baud_Config_Error = 3,
	GPS_Init_MSG_Config_Error = 4,
	GPS_Init_Ack_Tx_Error = 5,
	GPS_Init_Periph_Config_Error = 6,
	GPS_Init_Offline_Error = 7
}GPS_Init_msg_t;


typedef enum{
	GPS_MSG_RDY = 1,
	GPS_BUSY= 2,
	GPS_OFFLINE = 3,
	GPS_ERROR = 4,
}GPS_Status_t;


typedef enum{
	UBX_OK = 2,
	UBX_ACK_ACK = 1,
	UBX_ACK_NACK = 0,
	UBX_ERROR = -1,
	UBX_TIMEOUT_Tx = -2,
	UBX_TIMEOUT_Rx = -3,

} UBX_MSG_t;

/**
 * @brief Initialize GPS with initial configuration and interfaces
 *
 * @return
 */
GPS_Init_msg_t GPS_Init();

/**
 * @brief Configure update rate
 *
 * @param UpdateRate
 * @return GPS_Init_msg_t
 */
extern "C" UBX_MSG_t GPS_UpdateRate_Config(int UpdateRate);



/**
 * @brief Enable Low power GPS mode
 *
 * @return UBX_MSG_t
 */
extern "C" UBX_MSG_t GPS_LP_Enable();

/***
 * @brief Disable Low Power Mode
 *
 * @return UBX_MSG_t
 */
extern "C" UBX_MSG_t GPS_LP_Disable();


/**
 * @brief Retrieve latest NMEA string
 *
 * @return GPS_Status_t
 */
extern "C" GPS_Status_t GPS_Update(GPS_Data_t * gpsDataInstance);


extern "C" uint8_t GPS_Data_Ready();

extern "C" void GPS_InterruptsOff();
extern "C" void GPS_InterruptsOn();
/*** END OF FILE ***/
