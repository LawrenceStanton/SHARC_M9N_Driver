/**
  ******************************************************************************
  * @file           : UBX.cpp
  * @brief          : Source UBX.hpp
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

/* --------------------------------------------------------------------------- */
/* Begin Private Includes */
#include "UBX.hpp"
#include "UBX_ACK.hpp"
#include "UBX_CFG.hpp"
#include "UBX_INF.hpp"
#include "UBX_LOG.hpp"
#include "UBX_MGA.hpp"
#include "UBX_MON.hpp"
#include "UBX_NAV.hpp"
#include "UBX_RXM.hpp"
#include "UBX_TIM.hpp"

using string = std::string;
using vect = std::vector<uint8_t>;
/* End Private Includes */

/* --------------------------------------------------------------------------- */
/* Begin Private Defines */


/* End Private Defines */

/* --------------------------------------------------------------------------- */
/* Begin Private Macros */


/* End Private Macros */

/* --------------------------------------------------------------------------- */
/* Begin Private Typedefs */


/* End Private Typedefs */

/* --------------------------------------------------------------------------- */
/* Begin Private Function Prototypes */


/* End Private Function Prototypes */

/* --------------------------------------------------------------------------- */
/* Begin Private Variables */


/* End Private Variables */

/* --------------------------------------------------------------------------- */
/* Begin Source Code */

UBX::UBX(U1 msgClass, U1 msgID, U2 len) :
	msgClass(msgClass), msgID(msgID), len(len) {}

/**
 * @brief CalcuUBX::Checksum::Checksum object from a UBX frame prototype (without any checksum> attached.
 * 
 * @param ubxProto
 */
UBX::Checksum::Checksum(const vect & ubxProto){
	auto v = vect(ubxProto.begin()+2, ubxProto.end());	// Checksum string excludes preamble.
	ckA = ckB = 0;

	for(const auto c : v){
		ckA += c;
		ckB += ckA;
	}
}

UBX::Checksum::Checksum(U1 ckA, U1 ckB) :
	ckA(ckA), ckB(ckB) {}

UBX::U2 UBX::getPayloadLen(const std::vector<uint8_t> & ubx){
	if(ubx.size() >= 6){
		#ifdef __ARMEL__	// If ARM Little Endian. This should be the default for all STM32 devices.
		return *(uint16_t *)(ubx.data() + 4);
		#elif __ARMEB__		// If ARM Big Endian.
		return (*(uint16_t *)(
		ubx.data() + 4) << 8) | *(
		ubx.data() + 5);
		#else				// Unknown endianess.
		#error "Unknown Endianness"
		#endif
	}
	else return 0;
}

std::array<uint8_t, 6> UBX::header() const{
	std::array<uint8_t, 6> header;
	header[0] = sync1;
	header[1] = sync2;
	header[2] = msgClass;
	header[3] = msgID;
	header[4] = *(uint8_t *)&len;
	header[5] = *((uint8_t *)&len + 1);
	return header;
}



/* End Source Code */


/*** END OF FILE ***/
