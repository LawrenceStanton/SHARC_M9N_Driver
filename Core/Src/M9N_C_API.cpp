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

#include <time.h>

extern UART_HandleTypeDef huart4;
M9N m9n{ &huart4, UART4_IRQn, DMA1_Channel2_IRQn, DMA1_Channel1_IRQn };
uint32_t midnight = 0;

GPS_Init_msg_t GPS_Init(){
	m9n.init();
	return GPS_Init_OK;
}


void GPS_Update(){
	m9n.scanMessages();
}


uint8_t GPS_Data_Ready(){
	return (m9n.dataReady() ? 1u : 0u);
}

void GPS_InterruptsOff(){
	m9n.interruptsOff();
}

void GPS_InterruptsOn(){
	m9n.interruptsOn();
}

void receiveGLL(const NMEA_Standard::GLL & gll){
	gpsData.coordinates.tic = HAL_GetTick();
	gpsData.coordinates.lat = gll.lat;
	gpsData.coordinates.longi = gll.lon;
	gpsData.coordinates.time = midnight + gll.time;
}

void receiveGSA(const NMEA_Standard::GSA & gsa){
	static const uint32_t ticTimeDelayThreshold = 5000u;

	gpsData.diag.HDOP.digit = static_cast<int>(gsa.hdop);
	gpsData.diag.HDOP.precision = static_cast<int>( (gsa.hdop - gpsData.diag.HDOP.digit)*100 );
	gpsData.diag.VDOP.digit = static_cast<int>(gsa.vdop);
	gpsData.diag.VDOP.precision = static_cast<int>((gsa.vdop - gpsData.diag.VDOP.digit) * 100);
	gpsData.diag.PDOP.digit = static_cast<int>(gsa.pdop);
	gpsData.diag.PDOP.precision = static_cast<int>((gsa.pdop - gpsData.diag.PDOP.digit) * 100);
	
	gpsData.diag.num_sats = std::count_if(gsa.svid.begin(), gsa.svid.end(), [](uint8_t n) -> bool { return n!=0; } );	// TODO: GSA Receives Satellite IDs, not the number of sattelites
	
	gpsData.diag.fix_type = gsa.navMode;
	
	auto delaySinceLocation = (int32_t)HAL_GetTick() - gpsData.coordinates.tic;

	if(delaySinceLocation < 0) 							gpsData.diag.time = 0;
	else if(delaySinceLocation < ticTimeDelayThreshold) gpsData.diag.time = gpsData.coordinates.time;
	else 												gpsData.diag.time = gpsData.coordinates.time + delaySinceLocation;
}

void receiveZDA(const NMEA_Standard::ZDA & zda){
	midnight = zda.time.midnight();
}


/*** END OF FILE ***/
