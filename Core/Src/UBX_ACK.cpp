/**
  ******************************************************************************
  * @file			: UBX_ACK.cpp
  * @brief			: Source for UBX_ACK.hpp
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
#include "UBX_ACK.hpp"

UBX::ACKNAK::ACKNAK(const vect & ubx) :
	UBX(ubx),
	UBX::OTP(ubx){
	if(ubx.size() == 10u){
		ackClsID = ubx[4];
		ackMsgID = ubx[5];
	}
	else ackClsID = ackMsgID = 0;
}

UBX::ACKNAK::ACK::ACK(const vect & ubx) :
	ACKNAK(ubx){}

UBX::ACKNAK::NAK::NAK(const vect & ubx) :
	ACKNAK(ubx){}

/*** END OF FILE ***/