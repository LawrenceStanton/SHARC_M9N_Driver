/**
  ******************************************************************************
  * @file			: UART.cpp
  * @brief			: Source for UART.hpp
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

#include "UART.hpp"

#include <cstring>


UART::UART(UART_HandleTypeDef * h, IRQn_Type uartIrq, IRQn_Type dmaTxIrq, IRQn_Type dmaRxIrq) :
	hUart(h), uartIrq(uartIrq), dmaTxIrq(dmaTxIrq), dmaRxIrq(dmaRxIrq), tx(hUart), rx(hUart) {}

void UART::interruptsOff() const{
	HAL_NVIC_DisableIRQ(uartIrq);
	HAL_NVIC_DisableIRQ(dmaTxIrq);
	HAL_NVIC_DisableIRQ(dmaRxIrq);
}

void UART::interruptsOn() const{
	HAL_NVIC_EnableIRQ(uartIrq);
	HAL_NVIC_EnableIRQ(dmaTxIrq);
	HAL_NVIC_EnableIRQ(dmaRxIrq);
}

void UART::Tx::transmit(const StaticString & msg){
	auto p = alloc(msg.size());
	if(p){
		std::copy(msg.begin(), msg.end(), p);
		transmit(msg.size());
		nextTransmission();	// Will start transmission if idle. Else message will be sent later.
	}
}

uint8_t * UART::Tx::alloc(uint16_t size) noexcept {
	if(size > (buffSize)) return nullptr;
	
	uint16_t free;
	if(alHead >= tail){								// Allocation has not looped.
		free = buff.end() - alHead;
		if(free >= size){							// Can allocate in front of current allocation.
			auto p = const_cast<uint8_t *>(alHead);
			alHead += size;
			lpHead = alHead;	// Save this as the new maximum head before looping.
			return p;
		} 
		else{								// Must loop to beginning of buffer.
			alHead = buff.begin();			// Reset the allocation pointer.
			goto PreTailAllocation;
		}
	}
	else{								// Allocation is to be before tail.
PreTailAllocation:
		auto tik = HAL_GetTick();		// Save time for timeout detection.
		do{
				free = tail - buff.begin();
				if(free >= size){				// Can allocate before tail.
					auto p = alHead;
					alHead += size;				// Loop head
					return p;
				}
				else if(txBusy()){				// Transmission should free memory soon.
					HAL_Delay(delayTime);
				}
				else return nullptr;			// Tx not active therefore no memory will be freed anytime soon.
		} while(HAL_GetTick() - tik < timeout);
		return nullptr;	// Timed out
	}
}

void UART::Tx::transmit(uint16_t n){
	scHead += n;
	if(scHead > lpHead)	scHead = buff.begin() + n;	// Allocation has looped
	if(scHead > lpHead) scHead = lpHead;			// Erroneous condition. Only schedule remainder before the looping head.

	nextTransmission();		// Instantiate a new transmission if one is not already occuring.
}

void UART::Tx::nextTransmission(){
	if(!txBusy()){		// Only act if not currently transmitting.
		tail = txHead;

		if(txHead >= lpHead){	// Last transmission was final transmission in current buffer loop. Loop variables.
			tail = buff.begin();
			txHead = buff.begin();
			lpHead = alHead;
		}

		if(!txScheduled()) return;	// No further transmission scheduled.
		else{
			txHead = (scHead > txHead) ? scHead : lpHead;	// Transmit either scheduled characters in front of the tail or the remaining loop.
			HAL_UART_Transmit_DMA(hUart, const_cast<uint8_t *>(tail), txHead - tail);	// Begin transmission
			return;
		}
	}
}

bool UART::Tx::txBusy() const {
	const auto halTxState = HAL_UART_GetState(hUart);

	return !( 
		( (halTxState & HAL_UART_STATE_READY) == HAL_UART_STATE_READY ) &&
		( (halTxState & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX) );
}

bool UART::Tx::txScheduled() const{
	return !(txHead == scHead);
}


bool UART::Rx::dataReady(){
	static uint16_t lastTail = 0;
	bool retval = ((offloadBuff.tail > 0) && (lastTail != offloadBuff.tail)) ? true : false;
	lastTail = offloadBuff.tail;
	return retval;
}


/**
 * @brief Initialises DMA UART Reception.
 * 
 * @note The UART and DMA Peripherals must already be initialised by the time this function is called.
 * @note The DMA peripheral must be configured in circular mode.
 */
void UART::Rx::beginReceive(){
	HAL_UARTEx_ReceiveToIdle_DMA(hUart, dmaBuff.buff, dmaBuff.size);
	receiving = true;
}

void UART::Rx::endReceive(){
	HAL_UART_DMAStop(hUart);
	receiving = false;
}

void UART::setBaudrate(uint32_t baud) const{
	if(HAL_UART_DeInit(hUart) == HAL_OK){
		hUart->Init.BaudRate = baud;
		HAL_UART_Init(hUart);
	}
}


/**
 * @brief Transmission Complete Callback
 * 
 * @note This function must be called by HAL_UART_TxCmpltCallback upon completion of a transmission for the relevant UART interface.
 */
void UART::Tx::txCmpltCallback(){
	// Interrupt Enable/Disable not necessary because this ISR is to begin the next transmission.
	// interruptsOff();
	nextTransmission();
	// interruptsOn();
}


/**
 * @brief 
 * 
 * @param size The new head position reported from the UART peripheral.
 */
void UART::Rx::rxEventCallback(uint16_t size){
	if(size == dmaBuff.tail) return;	// Do nothing. Unknown event of zero size.
	
	// Offload buffer overflow. Reduce size to fill only available capacity. All further characters will be discarded.
	uint16_t head = (offloadBuff.tail + (size - dmaBuff.tail) > offloadBuff.size) ?  // Will the offload buffer overflow if [size] characters are added.
		(offloadBuff.size - offloadBuff.tail) + dmaBuff.tail : 	// Only move head to as many characters forward as will fit in offload buffer.
		size; 	// Move head to new head
	
	/**
	 * If an interrupt is missed, or suspended during debug, a condition may arise where the tail was not reset and is now greater than the head.
	 * If so, the tail must be reset. The data at the end of the buffer should not be retrieved as this may create a race condition with the DMA controller.
	 */
	if( (dmaBuff.tail > size) || (dmaBuff.tail > head) ) dmaBuff.tail = 0;
	
	std::memcpy(offloadBuff.buff + offloadBuff.tail, dmaBuff.buff + dmaBuff.tail, head - dmaBuff.tail);
	offloadBuff.tail += (head - dmaBuff.tail);
	
	dmaBuff.tail = (head == dmaBuff.size) ? 0 : head;	// Loop if at end else move tail to head.
}

void UART::errorCallback(UART_HandleTypeDef * huart){
	HAL_UART_DeInit(huart);
	HAL_UART_Init(huart);
	if(rx.receiving){
		rx.beginReceive();	// Restart if receiving was on.
	}
	if(tx.txScheduled()) tx.nextTransmission();	// Restart scheduled transmission
}

/*** END OF FILE ***/