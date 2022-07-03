/**
  ******************************************************************************
  * @file			: UBX_INF.hpp
  * @brief			: Information Message Classes for UBX Driver.
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

class UBX::INF : public UBX{
public:
	class DBG;		// "DEBUG" conflicts with a common define.
	class ERROR;
	class NOTICE;
	class TEST;
	class WARNING;
};

class UBX::INF::DBG : public UBX::INF{
public:	
	DBG(const vect & ubx);
};

class UBX::INF::ERROR : public UBX::INF{
public:	
	ERROR(const vect & ubx);
};

class UBX::INF::NOTICE : public UBX::INF{
public:	
	NOTICE(const vect & ubx);
};

class UBX::INF::TEST : public UBX::INF{
public:	
	TEST(const vect & ubx);
};

class UBX::INF::WARNING : public UBX::INF{
public:	
	WARNING(const vect & ubx);
};

/*** END OF FILE ***/