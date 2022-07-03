/**
  ******************************************************************************
  * @file			: UBX_MGA.hpp
  * @brief			: GNSS Assistance Message Classes for UBX Driver.
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

class UBX::MGA : public UBX{
public:
	class ACK;
	class ANO;
	class BDS;
	class DBD;
	class GAL;
	class GLO;
	class GPS;
	class INI;
	class QZSS;
	
};

class UBX::MGA::ACK : public UBX::MGA{
public:
	ACK(const vect & ubx);
};

class UBX::MGA::ANO : public UBX::MGA{
public:
	ANO(const vect & ubx);
};

class UBX::MGA::BDS : public UBX::MGA{
public:
	BDS(const vect & ubx);
};

class UBX::MGA::DBD : public UBX::MGA{
public:
	DBD(const vect & ubx);
};

class UBX::MGA::GAL : public UBX::MGA{
public:
	GAL(const vect & ubx);
};

class UBX::MGA::GLO : public UBX::MGA{
public:
	GLO(const vect & ubx);
};

class UBX::MGA::GPS : public UBX::MGA{
public:
	GPS(const vect & ubx);
};

class UBX::MGA::INI : public UBX::MGA{
public:
	INI(const vect & ubx);
};

class UBX::MGA::QZSS : public UBX::MGA{
public:
	QZSS(const vect & ubx);
};


/*** END OF FILE ***/