/**
  ******************************************************************************
  * @file			: UART.hpp
  * @brief			: Application specific UART Port Controller with Dynamic Data Queues.
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

/**
 * Warning:
 * This controller makes use of standard library containers for dynamic memory allocation.
 * The standard library containers are however not thread safe, which includes issues with interrupt service routines.
 * It is absolutely necessary to test code where dynamically allocated containers are involved in interrupt events.
 */

#pragma once

#include <array>
#include <vector>
#include <deque>

#include "stm32l4xx_hal.h"
#include "StaticString.hpp"

using string = StaticString;

class UART{
private:
	UART_HandleTypeDef * hUart;	// STM32 HAL UART Handle
	const IRQn_Type uartIrq;	// IRQ related to hUart
	const IRQn_Type dmaTxIrq;	// IRQ related to Rx DMA Process
	const IRQn_Type dmaRxIrq;	// IRQ related to Tx DMA Process

	void errorCallback(UART_HandleTypeDef * huart);
	friend void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

public:
	class Tx{
		UART_HandleTypeDef * hUart;

		static const uint16_t buffSize = 512u;
		std::array<uint8_t, buffSize>buff;
				 uint8_t * alHead = buff.data();	// Head Buffer Allocation
				 uint8_t * scHead = buff.data();	// Head Scheduled Transmission
				 uint8_t * lpHead = buff.data();	// Head Maximum Allocated before Buffer Looping Occurred
		volatile uint8_t * txHead = buff.data();	// Head Active Transmission
		volatile uint8_t * tail;

		static const uint16_t delayTime = 50;	// Unit period where alloc will wait for transmission to.
		static const uint16_t timeout = 10*delayTime;	// Timeout on waiting for memory to free.

		void txCmpltCallback();	// Must be called upon UART transmission complete.
		friend void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
		friend void errorCallback(UART_HandleTypeDef * huart);

	public:
		Tx(UART_HandleTypeDef * hUart) : hUart(hUart) {}

		uint8_t * alloc(uint16_t size) noexcept;	// Allocate memory in the buffer.
		void transmit(uint16_t n);					// Transmit n characters that have been allocated and set.

		void transmit(const StaticString & msg);
		template<typename T>
		void transmit(const T * first, const T * last);
		
		void nextTransmission();

		bool txBusy() const;	// Checks for a current UART Transmission Process.
		bool txScheduled() const;

	} tx;

	class Rx{	// Data Structure for asynchronously receiving UART data.
	private:
		UART_HandleTypeDef * hUart;

		template<size_t S, typename tB = uint8_t, typename tS = uint16_t>
		struct Buffer{
			const tS size{S};		// Size of buffer.
			tB buff[S];				// Static Storage
			volatile tS tail{0};	// Data Written to Buffer. Will be advanced in an ISR.

			// Buffer() = default;
			static_assert(S > 0, "Buffer size must be non-zero.");
			static_assert(sizeof(tB) == 1, "Only byte-sized types allowed for buffer elements (STM32 Configuration Constraint).");
		};

		static const size_t buffSize = 254u;
		Buffer<buffSize> dmaBuff{};			// Main Circular Buffer given to HAL DMA Process
		Buffer<8*buffSize> offloadBuff{};	// Large static offloading buffer. Rx Event Callbacks will move data from rxBuff to here.
		bool receiving = false;	// Notes if currently in receiving mode. Will be used to re-enable if there is and error requiring peripheral reset.

		friend class M9N;	// Temporary for testing
		
		void rxEventCallback(uint16_t size);	// Must be called upon UART idle, half and full complete data reception.
		friend void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
		friend void UART::errorCallback(UART_HandleTypeDef * huart);
	
	public:
		Rx(UART_HandleTypeDef * hUart) : hUart(hUart){}

		void beginReceive();	// Begins data reception. Does not initialise the STM32 UART peripheral device.
		void endReceive();		// Ends data reception. Does not de-initialise the STM32 UART peripheral device.
		bool dataReady();
	} rx;
	
	UART(UART_HandleTypeDef * h, IRQn_Type uartIrq, IRQn_Type dmaTxIrq, IRQn_Type dmaRxIrq);

	void setBaudrate(uint32_t baud) const;

	void interruptsOff() const;	// Disables all UART related interrupts.
	void interruptsOn() const;	// Enables all UART related interrupts.
};

template<typename T>
void UART::Tx::transmit(const T * first, const T * last){
	int len = last - first;
	if( (len > 0) && (len < (int)buff.size()) ) {
		auto p = alloc(len);
		if(p){
			std::copy(first, last, p);
			transmit(len);
		}
	}
}

/*** END OF FILE ***/