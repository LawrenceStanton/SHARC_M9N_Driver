/**
  ******************************************************************************
  * @file			: UBX_CFG.hpp
  * @brief			: Configuration classes for UBX Driver.
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


class UBX::CFG {
public:
	// class ANT;			// Depricated
	// class BATCH;			// Depricated
	// class CCFG;			// Depricated
	// class DAT;			// Depricated
	// class GEOFENCE;		// Depricated
	// class GNSS;			// Depricated
	// class INF;			// Depricated
	// class ITFM;			// Depricated
	// class LOGFILTER;		// Depricated
	// class MSG;			// Depricated	
	// class NAV5;			// Depricated
	// class NAVX5;			// Depricated
	// class NMEA;			// Depricated
	// class ODO;			// Depricated
	// class PM2;			// Depricated
	// class PM5;			// Depricated
	// class PMS;			// Depricated
	// class PRT;			// Depricated
	// class POWER;			// Depricated	// PWR. PWR was overloaded by a define.
	// class RATE;			// Depricated
	// class RINV;			// Depricated
	class RST;
	class RXM;
	// class SBAS;			// Depricated
	// class TP5;			// Depricated
	// class USB;			// Depricated
	class VAL;

	struct KeyID;

private:


};

struct UBX::CFG::KeyID{
	enum class Size : uint8_t{
		BIT = 0x01u,	// 1-bit (will use 1 byte of storage, LSB significant)
		BYTE = 0x02,	// 1-byte
		WORD = 0x03,	// 2-bytes
		DOUBLE = 0x04,	// 4-bytes
		QUAD = 0x05		// 8-bytes
	} size;

	U1 groupID;
	U2 itemID;

	X4 toKey();
	
	constexpr KeyID(Size size, U1 groupID, U2 itemID) : 
		size(size), groupID(groupID), itemID(itemID) {}
	constexpr KeyID(U4 && key) : 
		size(static_cast<Size>( (key & (0b111u << 28)) >> 28 )),
		groupID((key & (0xFFu << 16)) >> 16),
		itemID((key & 0xFFFu)) {}
};

class UBX::CFG::RST : public UBX{
public:
	RST(const vect & ubx);
};

class UBX::CFG::RXM : public UBX{
public:
	RXM(const vect & ubx);
};

class UBX::CFG::VAL {
public:
	class DEL;
	class GET;
	class SET;

	enum class Action : U1 {
		TRANSACTIONLESS = 0x00u,
		RE_START		= 0x01u,
		ONGOING			= 0x02u,
		APPLY			= 0x03u
	};

private:
	VAL() = delete;
};

class UBX::CFG::VAL::DEL : public UBX, public UBX::SET {
public:
	class TRANSACTION;	// Extension for transaction-based deletion.

private:
	const U1 version = 0x00u;
protected:
	enum class Layer : X1{
		BBR 	= 0x01u,
		FLSH 	= 0x02u,	// "FLASH" was used by a macro.
		ALL 	= 0x03u
	} layers;
private:
	const U1 res0[2] = {0x00u, 0x00u};
protected:
	std::vector<KeyID> keys;

public:

};

class UBX::CFG::VAL::DEL::TRANSACTION : public UBX::CFG::VAL::DEL {
private:
	const U1 version = 0x01u;
	Action action;
	const U1 res0 = 0x00u;
};

class UBX::CFG::VAL::GET : public UBX {
public:
	class POLL_REQ;
	class POLLED;

	enum class Layer : U1 {
		RAM 	= 0u,
		BBR 	= 1u,
		FLSH 	= 2u,	// "FLASH" was used by a macro.
		DEFAULT = 7u
	};

protected:
	GET() : UBX(0x06, 0x8B) {}
};

class UBX::CFG::VAL::GET::POLL_REQ : public UBX::CFG::VAL::GET, public UBX::POLL_REQ {
private:
	const U1 version = 0x00;
	Layer layer;
	U2 position;
	std::vector<KeyID> keys{};

public:
	POLL_REQ(KeyID key);
	POLL_REQ(std::vector<KeyID> keys);

	virtual vect toPollReq() final;
};

class UBX::CFG::VAL::GET::POLLED : public UBX::CFG, public UBX::POLLED {
private:
	const U1 version = 0x01;
	Layer layer;
	U2 position;
	std::vector< std::pair<KeyID, std::vector<uint8_t>> > cfgData;	// Data will still be in binary format within the pair's vector.
	// Maximum 64 key-value pairs.

public:
	POLLED(const vect & ubx);
};

class UBX::CFG::VAL::SET : public UBX::CFG, public UBX::SET {
public:
	class TRANSACTION;	// Extension for setting via a transaction.

	enum class Layers : X1{
		RAM 	= 0x01,
		BBR 	= 0x02,
		FLSH 	= 0x04,	// "FLASH" was used by a macro
	};

protected:
	const U1 version = 0x00u;
	Layers layers;
private:
	U1 reserved0[2] = {0x00u, 0x00u};
protected:
	std::vector< std::pair<KeyID, vect> > cfgData;	// Multi-byte data to be cast to hex vector.
	// Maximum 64 key-value pairs.

private:
	SET() = delete;

public:
	SET(Layers layers, std::vector< std::pair<KeyID, vect> > cfgData = {});

	void push(std::pair<KeyID, vect> cfg);
	virtual vect toSetterHex();
};

class UBX::CFG::VAL::SET::TRANSACTION : public UBX::CFG::VAL::SET {
private:
	Action action;

	TRANSACTION(Layers layers, Action action ,std::vector< std::pair<KeyID, vect> > cfgData = {});
};

#include "UBX_CFG_KEYID.hpp"

/*** END OF FILE ***/