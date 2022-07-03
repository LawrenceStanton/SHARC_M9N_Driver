/**
  ******************************************************************************
  * @file			: UBX_RXM.hpp
  * @brief			: Receiver Manager Message Classes for UBX Driver.
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

class UBX::RXM : public UBX{
public:
	class MEASX;
	class PMREQ;
	class RLM;
	class RTCM;
	class SFRBX;
	
};


class UBX::RXM::MEASX : public UBX::RXM{
public:
	MEASX(const vect & ubx);
};

class UBX::RXM::PMREQ : public UBX::RXM{
public:
	PMREQ(const vect & ubx);
};

class UBX::RXM::RLM : public UBX::RXM{
public:
	RLM(const vect & ubx);
};

class UBX::RXM::RTCM : public UBX::RXM{
public:
	RTCM(const vect & ubx);
};

class UBX::RXM::SFRBX : public UBX::RXM{
public:
	SFRBX(const vect & ubx);
};

/*** END OF FILE ***/