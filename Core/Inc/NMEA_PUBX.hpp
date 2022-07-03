/**
  ******************************************************************************
  * @file			: NMEA_PUBX.hpp
  * @brief			: uBlox NMEA PUBX Protocol Extension
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

#include "M9N_Base.hpp"
#include "NMEA_Standard.hpp"

using NMEA = M9N_Base::NMEA_PUBX;

enum class NMEA::Message{
	/* Standard NMEA Sentences. Identical to NMEA_Base::Message */
	DTM,	// Datum Reference
	GAQ,	// GA Poll
	GBQ,	// GB Poll
	GBS,	// Satellite Fault Detection
	GGA,	// GPS Fix Data
	GLL,	// Location with Time and Fix Status
	GLQ,	// GL Poll
	GNQ,	// GN Poll
	GNS,	// GNSS Fix Data
	GPQ,	// GP Poll
	GRS,	// GNSS Range Residuals
	GSA,	// GNSS Dilution of Precision
	GST,	// GNSS Pseudorange Error Statistics
	GSV,	// GNSS Satellites in View
	RLM,	// Return Link Message
	RMC,	// Recommended Minimum Data
	TXT,	// Test transmission
	VLW,	// Dual Ground/Water Distance
	VTG,	// Course over Ground and Speed
	ZDA, 	// Time and Date

	/* uBlox PUBX Extensions  */
	PUBX_CONFIG,	// Protocols and Rate Configuration
	PUBX_POSITION,	// Positional Data
	PUBX_RATE,		// Message Output Rate
	PUBX_SVSTATUS,	// Satellite Status
	PUBX_TIME,		// Time Information

	/* Unknown Case*/
	UNKNOWN
};

class NMEA::PUBX : public NMEA_PUBX{
protected:
	struct Address{
		const string pubx{"PUBX"};
		uint8_t msgId;

		Address(uint8_t msgId);
	} addr;

	PUBX(uint8_t msgId);
	// virtual string toString() = 0;
};

class NMEA::Config : public PUBX{
public:
	M9N_Base::PortID portId;
	M9N_Base::InProto inProto;
	M9N_Base::OutProto outProto;
	M9N_Base::Baud baudrate;
	bool autobauding;

	Config(
		M9N_Base::PortID portId, 
		M9N_Base::InProto inProto, 
		M9N_Base::OutProto outProto, 
		M9N_Base::Baud baudrate, 
		bool autobauding );

	virtual string toString(char [36]) final;
};

class NMEA::Rate : public PUBX{
public:
	Message ID;		// NMEA Message Identifier
					// Note: Interface Description overloaded "msgId". 
					// Swapped "msgId" for "ID" to preserve naming with PUBX.
	uint8_t rddc;	// Rate on DDC Port
	uint8_t rus1;	// Rate on UART1 Port
	uint8_t rus2;	// Rate on UART2 Port
	uint8_t rusb;	// Rate on USB Port
	uint8_t rspi;	// Rate on SPI Port
	uint8_t reserved;	// Always 0

	Rate(
		Message ID,
		uint8_t rus1 = 0u,
		uint8_t rddc = 0u,
		uint8_t rus2 = 0u,
		uint8_t rusb = 0u,
		uint8_t rspi = 0u);

	virtual string toString(char buff[30]) final;
};

/*** END OF FILE ***/