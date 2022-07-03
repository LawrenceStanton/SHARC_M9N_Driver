/**
  ******************************************************************************
  * @file           : NMEA.hpp
  * @brief          : NMEA Protocol Standard Driver
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

#include <array>
#include <stdint.h>

#include "StaticString.hpp"

using string = StaticString;

class NMEA_Standard{
protected:
	struct Address;
	struct Checksum;

public:
	enum class TalkerID;
	enum class Message;

	class DTM;
	class GBS;
	class GGA;
	class GLL;
	class GNS;
	class GRS;
	class GSA;
	class GST;
	class GSV;
	class RLM;
	class RMC;
	class TXT;
	class VLW;
	class VTG;
	class ZDA;

	class BAD;	// NOT a real NMEA object. Returned by factory when an invalid NMEA sentence is passed.
	
	virtual ~NMEA_Standard() = default;

	virtual string toString(char * buff) = 0;	// Generates the NMEA Sentence representing teh NMEA object. NMEA is an Abstract Class.

	inline static bool valid(const string & nmea) {return Checksum::valid(nmea);}

protected:
	struct Address{
		string tt;	// Talker Identifier (string format)
		string sss;	// Sentence Formatter
		// Note: tt and sss are typically that many characters long but proprietary messages can be different.
		
		Address() = default;
		Address(const string & s);
		Address(const string & tt, const string & sss);
		string toString();
	};
	
	struct Checksum{
		static const char ast = '*';	// Checksum Prefix
		uint8_t cs;						// Checksum Value

		Checksum() = default;
		Checksum(const string & s);		// s is the checksum substring including the '*'.
		Checksum(uint8_t cs);			// Set a hexadecimal value for cs.
		
		static uint8_t checksum(const string & nmea);	// Calculates and sets the checksum based upon a NMEA sentence.
		static bool valid(const string & nmea);
		string toString(char cStr[4]);
	};

	struct UTC_Time{
		uint8_t hh;	// Hours
		uint8_t mm;	// Minutes
		float ss;	// Seconds (2 or 3 decimal points of precision)

		UTC_Time() = default;
		UTC_Time(const string & tStr);
		UTC_Time(const UTC_Time & t) = default;
		string toString();

		// uint32_t operator = (const UTC_Time & t);	// Time in ms since Jan 1 1980
	};

	struct Coordinate{
		uint16_t deg;
		float min;

		Coordinate() = default;
		Coordinate(const string & s);
		Coordinate(const Coordinate & c) = default;

		string toString(char cStr[11]);

		float operator = (const Coordinate c);

	};

	NMEA_Standard() = default;	// Derived classes shall be responsible for all base member initialisation.

	void setChecksum();	// Sets the checksum from the member toString representation.
	
	/* Static helper functions */
	template<size_t N>
	static std::array<StaticString, N> parseFields(const string & nmea);
	static string toString(const Message msg);
	static string toString(const TalkerID tId);

									// Address below.
	const char start = '$';	// Start Character
									// Payload defined in derived class.
	Checksum cs;					// Checksum
	const string crlf = "\r\n";		// Carriage Return, Line Forward
};

enum class NMEA_Standard::TalkerID {
	GP,	// GPS, SBAS
	GL, // GLONASS
	GA,	// Galileo
	GB,	// BeiDou
	GQ,	// QZSS
	GN,	// Any Combination
};

enum class NMEA_Standard::Message{
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
	ZDA 	// Time and Date
};

class NMEA_Standard::GNS : public NMEA_Standard{
private:
	Address addr;
	UTC_Time time;	// GNSS UTC Time
	Coordinate lat;	// Latitude
	char NS;	// North / South
	Coordinate lon;	// Longitude
	char EW;	// East / West
	string posMode;	// Positioning Mode
	uint8_t numSV;	// Number of Satellites
	float hdop;	// Horizontal Dilution of Precision
	float alt;	// Altitude
	float sep;	// Geoid Separation
	float diffAge;	// Age of Differential Corrections
	uint16_t diffStation;	// Differential Correction Station ID
	char navStatus;	// Navigational Status Indicator
	
public:
	GNS(const std::array<StaticString, 15> & fields);
	GNS(const string & nmea);
	virtual ~GNS() = default;

	virtual string toString() final { return ""; }	// Unimplemented
};

class NMEA_Standard::GLL : public NMEA_Standard{
private:
	Address addr;
	Coordinate lat;
	char NS;
	Coordinate lon;
	char EW;
	UTC_Time time;
	char status;
	char posMode;

public:
	GLL(const std::array<StaticString, 9> & fields);
	GLL(const string & nmea);
	virtual string toString(char *) final{return "";}
};

class NMEA_Standard::GSA : public NMEA_Standard{
private:
	Address addr;
	char opMode;	// Operational Mode ('M' or 'A')
	uint8_t navMode;	// Navigation Mode
	std::array<uint8_t, 12> svid;	// Satellite Numbers
	float pdop;	// Position DOP
	float hdop;	// Horizontal DOP
	float vdop;	// Vertical DOP
	uint8_t systemId;	// GNSS System ID

public:
	GSA(const std::array<StaticString, 20> & fields);
	GSA(const string & nmea);
	virtual ~GSA() = default;

	virtual string toString() final{return "";}	// Unimplemented
};

class NMEA_Standard::ZDA : public NMEA_Standard{
private:
	struct UTC_DateTime : UTC_Time{
		uint8_t day;
		uint8_t month;
		uint16_t year;
		uint8_t ltzh;
		uint8_t ltzm;

		UTC_DateTime() = default;
		UTC_DateTime(const string & time, 
			uint8_t day, uint8_t month, uint16_t year, 
			uint8_t ltzh, uint8_t ltzm);

		};

	Address addr;
	UTC_DateTime time;

public:
	ZDA(const std::array<StaticString, 9> & fields);
	ZDA(const string & nmea);
	virtual ~ZDA() = default;

	virtual string toString() final{return "";}	// Unimplemented
};

class NMEA_Standard::BAD : public NMEA_Standard{
public:
	string nmea;
	BAD(const string & nmea) : nmea(nmea) {}
	virtual ~BAD() = default;

	virtual string toString() final { return nmea; }
};

/*** END OF FILE ***/
