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

UBX::UBX(const vect & ubx) :
	len(getPayloadLen(ubx)){
	if(ubx.size() >= 8u){	// Minimum required size for valid UBX frame.
		msgClass = ubx[2];
		msgID = ubx[3];

		if(ubx.size() == (len + 8u) ){		// Check the len. Will also reject inputs with trailing excess elements.
			cs.ckA = ubx[ubx.size() - 2];	// Second to last.
			cs.ckB = ubx[ubx.size() - 1];	// Last
		}
		else{				// Same outcome as below.
			msgClass = msgID = 0;
		}
	}
	else{				// Insufficient data to be valid ubx. Default values to 0.
		msgClass = msgID = 0;
	}
}

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





/* End Source Code */


/*** END OF FILE ***/
