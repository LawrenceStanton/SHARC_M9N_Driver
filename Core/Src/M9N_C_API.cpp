/**
  ******************************************************************************
  * @file           : M9N_C_API.c
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

// #include "main.h"
#include "M9N_C_API.hpp"

extern UART_HandleTypeDef huart4;
M9N m9n{ &huart4, UART4_IRQn, DMA1_Channel2_IRQn, DMA1_Channel1_IRQn };


GPS_Init_msg_t GPS_Init(){
	m9n.init();
	return GPS_Init_OK;
}


GPS_Status_t GPS_Update(GPS_Data_t * gpsDataInstance){
	m9n.scanMessages();
	
	// TODO: Populate data instance


	(void) gpsDataInstance;

	return GPS_MSG_RDY;
}


uint8_t GPS_Data_Ready(){
	return (m9n.dataReady() ? 1u : 0u);
}

/*** END OF FILE ***/
