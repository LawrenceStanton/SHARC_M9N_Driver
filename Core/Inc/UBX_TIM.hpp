/**
  ******************************************************************************
  * @file			: UBX_TIM.hpp
  * @brief			: Timing Message Classes for UBX Driver.
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

class UBX::TIM : public UBX{
public:
	class TM2;
	class TP;
	class VRFY;
};

class UBX::TIM::TM2 : public UBX::TIM{
public:
	TM2(const vect & ubx);
};

class UBX::TIM::TP : public UBX::TIM{
public:
	TP(const vect & ubx);
};

class UBX::TIM::VRFY : public UBX::TIM{
public:
	VRFY(const vect & ubx);
};


/*** END OF FILE ***/