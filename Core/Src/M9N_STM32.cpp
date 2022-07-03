/**
  ******************************************************************************
  * @file           : M9N_STM32.cpp
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


#include "M9N_STM32.hpp"
#include <array>
#include <algorithm>
#include <string>
#include <vector>

M9N::M9N(UART_HandleTypeDef * h, IRQn_Type uartIrq, IRQn_Type dmaTxIrq, IRQn_Type dmaRxIrq) :
	uart(h, uartIrq, dmaTxIrq, dmaRxIrq) {}

void M9N::init(){
	uart.rx.beginReceive();
}

void M9N::transmit(const uint8_t * first, const uint8_t * last){
	uart.tx.transmit(first, last);	// Delegate
}


inline void M9N::delay(uint32_t delay){ HAL_Delay(delay); }

void M9N::setBaudrate(Baud baud, PortID portId){
	switch (portId){
		case PortID::UART1:
			if(this->baud != baud){
				uart.setBaudrate(static_cast<uint32_t>(baud));
				this->baud = baud;
			}
			break;
		case PortID::USB:
			// Unimplemented
			break;
		default:
			break;
	}
}

void M9N::scanMessages(){
	std::pair<uint8_t *, uint16_t> s;	// {array, size}
	s.second =  uart.rx.offloadBuff.tail;
	if(s.second == 0) return;	// Nothing to do
	uint8_t buffCopy[s.second];
	s.first = buffCopy;

	// The later processing will take a while. Pay copy expense in return for flushing the buffer ASAP.
	uart.interruptsOff();
	std::copy(uart.rx.offloadBuff.buff, uart.rx.offloadBuff.buff + s.second, s.first);
	uart.rx.offloadBuff.tail = 0;
	uart.interruptsOn();

	uint16_t lenD = std::count_if(s.first, s.first + s.second, [](uint8_t c) -> bool{ return c == '$'; });
	uint16_t lenL = std::count_if(s.first, s.first + s.second, [](uint8_t c) -> bool{ return c == '\r'; });
	uint16_t lenU = std::count_if(s.first, s.first + s.second, [](uint8_t c) -> bool{ return c == 0xB5; });
	uint16_t dollars[(lenD > 0) ? lenD : 1u];	// Will crash if zero-sized.
	uint16_t lfcrs	[(lenL > 0) ? lenL : 1u];	// Will crash if zero-sized.
	uint16_t mus	[(lenU > 0) ? lenU : 1u];	// Will crash if zero-sized.
	std::pair<uint16_t*, uint16_t> d = {dollars	, 0u};	// {array, working index}
	std::pair<uint16_t*, uint16_t> l = {lfcrs	, 0u};	// {array, working index}
	std::pair<uint16_t*, uint16_t> u = {mus		, 0u};	// {array, working index}

	for(uint16_t i = 0; i < s.second - 1; i++){
		switch(s.first[i]){
			case '$': {
				d.first[d.second++] = i;
				break;
			}
			case '\r':{
				if(s.first[i+1] == '\n'){
					l.first[l.second++] = i;
				}
				break;
			} 
			case 0xB5: {
				if(s.first[i+1] == 0x62){
					u.first[u.second++] = i;
				}
				break;
			}
			default: continue;
		}
	}

	/* UBX Frame Identification */
	// Drop any "μb" indicies which are too close to the end of v such that the length specifier is not received.
	for(auto i = u.second - 1; (u.second > 1u) && (i >= 0); i-- ){
		if(u.first[i] > (s.second - 5)) lenU--;
		else break;
	}

	// Construct pairs of UBX indicies and the length of that frame.
	std::pair<uint16_t, uint16_t> ubxRanges[lenU];	// {Start, Length}

	for(uint16_t i = 0u; i < lenU; i++){
		// ubxRanges[i] = { u.first[i], UBX::getPayloadLen(std::vector<uint8_t>(s.first + u.first[i], s.first + u.first[i] + 6)) + 8 };	// +6 safe because of prior filter.
	}

	// Filter pairs for which the entire frame is not within v.
	// This will also happen to erase pairs with a corrupted and had a large length specifier.
	for(size_t i = 0; (lenU > 0) && (i < lenU); i++) if(ubxRanges[i].first + ubxRanges[i].second > s.second) lenU--;


	// TODO: Scan UBX Frames


	/* NMEA Sentence Identification */
	// Construct pairs of alternating dollar and lfcr characters.
	// Filter non-alternating sequences of dollar and lfcr indecies.
	size_t lenR = (d.second > l.second) ? d.second : l.second;
	std::pair<uint16_t, uint16_t> nmeaRanges[lenR]; // {begin, end}[]
	lenD = d.second;
	lenL = l.second;
	d.second = l.second = 0u;
	uint16_t k = 0u;
	while( (d.second < lenD) && (l.second <lenL) ){
		if(d.first[d.second] > l.first[l.second]){	// Discard LFCR index if no dollar index is less than the pending lfcr index.
			l.second++;
		}
		else{	// Guaranteed that lfcr[j] > dollar[i]. lfcr[j] is asserted as a valid end index.
			nmeaRanges[k++] = {d.first[d.second++], l.first[l.second++] + 2};

			// Discard current dollar start index if there is a later dollar index that is also less than the end index.
			while((d.second < lenD) && (d.first[d.second] < nmeaRanges[k-1].second) ) {
				nmeaRanges[k-1].first = d.first[d.second++];
			}
		}
	}

	/* Scan NMEA Frames */
	for(uint16_t i = 0; i < k; i++){
		interpretNmea(StaticString(s.first + nmeaRanges[i].first, s.first + nmeaRanges[i].second));
		// __NOP();
	}

	/* Push remainder characters back to Rx */
	auto lastUBX  = (lenU > 0) ? ubxRanges[lenU].first + ubxRanges[lenU].second : 0;
	auto lastNmea = (k > 0) ? nmeaRanges[k-1].second + 2 : 0;
	auto lastC = (lastUBX > lastNmea) ? lastUBX : lastNmea;

	// If nothing was processed, or if all was processed, do nothing.
	if( (lastC == 0) || (lastC = s.second) ) return;
	// Add any characters that may have been received during the above processing.
	else if(uart.rx.offloadBuff.tail > 0){	// More characters have been added during this function's call.
		if(s.second + uart.rx.offloadBuff.tail > uart.rx.offloadBuff.size ) return;	// Buffer will overflow. Flush these remaining characters
		else{
			uart.interruptsOff();
			std::copy(uart.rx.offloadBuff.buff, uart.rx.offloadBuff.buff + uart.rx.offloadBuff.tail, s.first + lastC);
			s.second += uart.rx.offloadBuff.tail;
			std::copy(s.first + lastC, s.first + s.second, uart.rx.offloadBuff.buff);
			uart.rx.offloadBuff.tail = lastC;
			uart.interruptsOn();
		}
	}
	else{	// No characters were added. Copy directly into the buffer
		uart.interruptsOff();
		std::copy(s.first + lastC, s.first + s.second, uart.rx.offloadBuff.buff);
		uart.rx.offloadBuff.tail = s.second;
		uart.interruptsOn();
	}
}

inline void M9N::interpretNmea(const StaticString & s){	
	auto message = M9N_Base::NMEA_PUBX::getMessage(s);
	switch(message){
		case M9N_Base::NMEA_PUBX::Message::DTM :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GAQ :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GBQ :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GBS :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GGA :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GLL :{
			NMEA_Standard::GLL gll{s};
			

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GLQ :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GNQ :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GNS :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GPQ :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GRS :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GSA :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GST :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::GSV :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::RLM :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::RMC :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::TXT :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::VLW :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::VTG :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::ZDA :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::PUBX_CONFIG :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::PUBX_POSITION :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::PUBX_RATE :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::PUBX_SVSTATUS :{

			return;
		}

		case M9N_Base::NMEA_PUBX::Message::PUBX_TIME :{

			return;
		}
		default: return;
	}
}


extern M9N m9n;	// To be declared in main.
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == m9n.uart.tx.hUart->Instance) m9n.uart.tx.txCmpltCallback();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == m9n.uart.rx.hUart->Instance)
		m9n.uart.rx.rxEventCallback(Size);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == m9n.uart.hUart->Instance)
		m9n.uart.errorCallback(huart);
}

/*** END OF FILE ***/
