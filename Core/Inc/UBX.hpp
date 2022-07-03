/**
  ******************************************************************************
  * @file           : UBX.hpp
  * @brief          : uBlox Proprietary UBX Protocol Driver
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
/* --------------------------------------------------------------------------- */
/* Begin Public Includes */

#include "stdio.h"

#include <array>
#include <string>
#include <vector>

using vect = std::vector<uint8_t>;

class UBX{
public:
	/* General Message Classes */
	class ACKNAK;	// "ACK" is used by a derivative.
	class CFG;
	class INF;
	class LOG;
	class MGA;
	class MON;
	class NAV;
	class RXM;
	class SEC;
	class TIM;
	class UPD;

	class BAD;	//	NOT a real UBX object. Returned by factory when an invalid UBX is passed.

	/* Communication Type Interface Specifiers */
	class INP;
	class OTP;
	class INOUT;
	class PERIOD;
	class PERIOD_POLLED;
	class COMMAND;
	class GET;
	class SET;
	class GETSET;
	class POLLED;
	class POLL_REQ;

	/* UBX Type Definitions */
	typedef uint8_t 	U1;
	typedef int8_t 		I1;
	typedef uint8_t 	X1;
	typedef uint16_t 	U2;
	typedef int16_t 	I2;
	#ifdef __ARMEL__
	typedef uint16_t 	X2;
	typedef uint32_t	U4;
	typedef int32_t		I4;
	typedef uint32_t	X4;
	#else
	#error "UBX Data formats not implemented for non-little endian local system."
	#endif
	typedef float	R4;
	typedef double	R8;
	typedef char	CH;

	static U2 getPayloadLen(const std::vector<uint8_t> & ubx);	// Extracts a UBX frame length specifier value from a frame hex vector.

	template<typename T>
	static std::vector<uint8_t>  toHex(const T & n);		// General purpose variable to little-endian hex vector function.
	
protected:
	static const U1 sync1 = 0xB5;	// 'mu'
	static const U1 sync2 = 0x62;	// 'b'
	U1 msgClass;		// UBX Message Class
	U1 msgID;			// UBX Message Class Identifier
	U2 len;				// Length of UBX frame.

	struct Checksum{
		U1 ckA;
		U1 ckB;

		Checksum() = default;
		Checksum(const vect & ubxProto);
		Checksum(U1 _ckA, U1 _ckB);

		inline vect toHex() { return vect{ckA, ckB}; }
	} cs;

	UBX() = delete;
	UBX(U1 msgClass, U1 msgID, U2 len = 0);
	UBX(const vect & ubx);
};

template<typename T>
vect UBX::toHex(const T & n){
	#ifdef __ARMEL__	// If ARM Little Endian. This should be the default for all STM32 devices.
	return vect(&n, &n + sizeof(T));
	#else
	// Unimplemented
	#error "UBX::toHex not implemented for Big Endian formatting."
	#endif
}

class UBX::INP{
public:
	virtual vect toInpHex() = 0;
};

class UBX::OTP{
protected:
	OTP(const vect & ubx) {(void)ubx;}
};

class UBX::INOUT : INP, OTP {};

class UBX::PERIOD : OTP {};

class UBX::COMMAND : INP {};

class UBX::GET : COMMAND{
public:
	virtual vect toGetterHex() = 0;	// Hex vector for getting a particular frame.
};

class UBX::SET {
public:
	virtual vect toSetterHex() = 0;	// Hex vector for issuing command.
};

class UBX::GETSET : GET, SET {};

class UBX::POLLED : GET {};

class UBX::PERIOD_POLLED : POLLED {};

class UBX::POLL_REQ {
public:
	virtual vect toPollReq() = 0;
};

class UBX::SEC : public UBX{
public:
	class UNIQID;
};

class UBX::SEC::UNIQID : public UBX::SEC{
public:
	UNIQID(const vect & ubx);
};

class UBX::UPD : public UBX{
public:
	class SOS;
};

class UBX::UPD::SOS : public UBX::UPD{
public:
	SOS(const vect & ubx);
};

#include "UBX_CFG.hpp"

class BAD : public UBX{
private:
	vect ubx;
public:
	BAD(const vect & ubx) : UBX(0, 0), ubx(vect(ubx.begin(), ubx.end())) {}
};

/*** END OF FILE ***/
