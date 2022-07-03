/**
  ******************************************************************************
  * @file			: UBX_NAV.hpp
  * @brief			: Navigational Message Classes for UBX Driver.
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

class UBX::NAV : public UBX{
public:
	class CLOCK;
	class COV;
	class DOP;
	class EOE;
	class GEOFENCE;
	class ODO;
	class ORB;
	class POS;
	class PVT;
	class RESET_ODO;
	class SAT;
	class SIG;
	class STATUS;
	class TIME;
	class VEL;

};

class UBX::NAV::CLOCK : public UBX::NAV{
public:
	CLOCK(const vect & ubx);
};

class UBX::NAV::COV : public UBX::NAV{
public:
	COV(const vect & ubx);
};

class UBX::NAV::DOP : public UBX::NAV{
public:
	DOP(const vect & ubx);
};

class UBX::NAV::EOE : public UBX::NAV{
public:
	EOE(const vect & ubx);
};

class UBX::NAV::GEOFENCE : public UBX::NAV{
public:
	GEOFENCE(const vect & ubx);
};

class UBX::NAV::ODO : public UBX::NAV{
public:
	ODO(const vect & ubx);
};

class UBX::NAV::ORB : public UBX::NAV{
public:
	ORB(const vect & ubx);
};

class UBX::NAV::PVT : public UBX::NAV{
public:
	PVT(const vect & ubx);
};

class UBX::NAV::RESET_ODO : public UBX::NAV{
public:
	RESET_ODO(const vect & ubx);
};

class UBX::NAV::SAT : public UBX::NAV{
public:
	SAT(const vect & ubx);
};

class UBX::NAV::SIG : public UBX::NAV{
public:
	SIG(const vect & ubx);
};

class UBX::NAV::STATUS : public UBX::NAV{
public:
	STATUS(const vect & ubx);
};


class UBX::NAV::POS : public UBX::NAV{
public:
	class ECEF;
	class LLH;
protected:
	POS(const vect & ubx);
};

class UBX::NAV::POS::ECEF : public UBX::NAV::POS{
public:
	ECEF(const vect & ubx);
};

class UBX::NAV::POS::LLH : public UBX::NAV::POS{
public:
	LLH(const vect & ubx);
};

class UBX::NAV::TIME : public UBX::NAV{
public:
	class BDS;
	class GAL;
	class GLO;
	class GPS;
	class ELS;
	class QZSS;
	class UTC;
protected:
	TIME(const vect & ubx);
};

class UBX::NAV::TIME::BDS : public UBX::NAV::TIME{
public:
	BDS(const vect & ubx);
};

class UBX::NAV::TIME::GAL : public UBX::NAV::TIME{
public:
	GAL(const vect & ubx);
};

class UBX::NAV::TIME::GLO : public UBX::NAV::TIME{
public:
	GLO(const vect & ubx);
};

class UBX::NAV::TIME::GPS : public UBX::NAV::TIME{
public:
	GPS(const vect & ubx);
};

class UBX::NAV::TIME::ELS : public UBX::NAV::TIME{
public:
	ELS(const vect & ubx);
};

class UBX::NAV::TIME::QZSS : public UBX::NAV::TIME{
public:
	QZSS(const vect & ubx);
};

class UBX::NAV::TIME::UTC : public UBX::NAV::TIME{
public:
	UTC(const vect & ubx);
};


class UBX::NAV::VEL : public UBX::NAV{
public:
	class ECEF;
	class NED;
protected:
	VEL(const vect & ubx);
};

class UBX::NAV::VEL::ECEF : public UBX::NAV::VEL{
public:
	ECEF(const vect & ubx);
};

class UBX::NAV::VEL::NED : public UBX::NAV::VEL{
public:
	NED(const vect & ubx);
};


/*** END OF FILE ***/