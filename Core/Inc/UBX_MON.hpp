/**
  ******************************************************************************
  * @file			: UBX_MON.hpp
  * @brief			: Monitoring Message Classes for UBX Driver
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

class UBX::MON : public UBX{
public:
	class BATCH;
	class COMMS;
	class GNSS;
	class HW;
	class HW2;
	class HW3;
	class IO;
	class MSGPP;
	class PATCH;
	class RF;
	class RXBUF;
	class RXR;
	class SPAN;
	class TXBUF;
	class VER;

};

class UBX::MON::BATCH : public UBX::MON{
public:
	BATCH(const vect & ubx);
};

class UBX::MON::COMMS : public UBX::MON{
public:
	COMMS(const vect & ubx);
};

class UBX::MON::GNSS : public UBX::MON{
public:
	GNSS(const vect & ubx);
};

class UBX::MON::HW : public UBX::MON{
public:
	HW(const vect & ubx);
};

class UBX::MON::HW2 : public UBX::MON{
public:
	HW2(const vect & ubx);
};

class UBX::MON::HW3 : public UBX::MON{
public:
	HW3(const vect & ubx);
};

class UBX::MON::IO : public UBX::MON{
public:
	IO(const vect & ubx);
};

class UBX::MON::MSGPP : public UBX::MON{
public:
	MSGPP(const vect & ubx);
};

class UBX::MON::PATCH : public UBX::MON{
public:
	PATCH(const vect & ubx);
};

class UBX::MON::RF : public UBX::MON{
public:
	RF(const vect & ubx);
};

class UBX::MON::RXBUF : public UBX::MON{
public:
	RXBUF(const vect & ubx);
};

class UBX::MON::RXR : public UBX::MON{
public:
	RXR(const vect & ubx);
};

class UBX::MON::SPAN : public UBX::MON{
public:
	SPAN(const vect & ubx);
};

class UBX::MON::TXBUF : public UBX::MON{
public:
	TXBUF(const vect & ubx);
};

class UBX::MON::VER : public UBX::MON{
public:
	VER(const vect & ubx);
};


/*** END OF FILE ***/