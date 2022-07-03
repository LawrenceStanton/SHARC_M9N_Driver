/**
  ******************************************************************************
  * @file			: UBX_ACK.hpp
  * @brief			: Acknowledge Classes for UBX Driver
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

#include "UBX.hpp"

class UBX::ACKNAK : public UBX, UBX::OTP{
public:
	class ACK;
	class NAK;
protected:
	uint8_t ackClsID;	//Class ID of Acknowledged/Not Acknowledged Message.
	uint8_t ackMsgID;	//Message ID of Acknowledged/Not Acknowledged Message.

	ACKNAK(const vect & ubx);
};

class UBX::ACKNAK::ACK : public ACKNAK{
public:
	ACK(const vect & ubx);
};

class UBX::ACKNAK::NAK : public ACKNAK{
public:
	NAK(const vect & ubx);
};


/*** END OF FILE ***/