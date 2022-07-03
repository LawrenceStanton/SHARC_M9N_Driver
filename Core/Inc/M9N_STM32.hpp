/**
  ******************************************************************************
  * @file           : M9N_STM32.hpp
  * @brief          : Source / Header file for M9N_STM32.c/.h
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

#include "M9N_Base.hpp"
#include "UART.hpp"

#include "stm32l4xx_hal.h"

#include <string>

using string = StaticString;

class M9N : public M9N_Base{
public:
	M9N(UART_HandleTypeDef * h, IRQn_Type uartIrq, IRQn_Type dmaTxIrq, IRQn_Type dmaRxIrq);
	void init();

	void scanMessages();
	inline bool dataReady(){ return uart.rx.dataReady(); }

	inline void interruptsOn(){ uart.interruptsOn(); }
	inline void interruptsOff(){ uart.interruptsOff(); }
	
private:
	UART uart;

	inline void interpretNmea(const StaticString & s);
	inline void interpretUBX(std::pair<uint8_t *, uint8_t *> v);

	using M9N_Base::transmit;
	virtual void transmit(const uint8_t * first, const uint8_t * last) final;

	virtual void delay(uint32_t delay) final;
	virtual void setBaudrate(Baud baud = Baud::B38400, PortID portId = PortID::UART1) final;


	friend void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
	friend void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
	friend void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
};

/* Static Structures for Fetching Scanned Values */


/*** END OF FILE ***/
