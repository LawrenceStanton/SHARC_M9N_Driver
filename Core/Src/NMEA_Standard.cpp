/**
  ******************************************************************************
  * @file           : NMEA.cpp
  * @brief          : Header file for NMEA_Standard.hpp
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

/* --------------------------------------------------------------------------- */
/* Begin Private Includes */
#include "NMEA_Standard.hpp"

#include <algorithm>
#include <cstdio>
#include <map>

template<size_t N>
std::array<string, N> NMEA_Standard::parseFields(const string & nmea){
	std::array<string, N> fields;

	size_t i = 1, n = 0;
	if(!nmea.empty() && valid(nmea)){	// Only proceed if the NMEA sentence was valid.
		if(nmea.at(0) == '$'){
			std::size_t j;
			for(j = 1; j < nmea.length(); j++){
				if(nmea.at(j) == ','){	// Push each comma delimited item.
					fields[n++] = nmea.substr(i, j-i); 
					i = j+1;
					if(n >= N) goto InvalidNoFields;
				}
				else if(nmea.at(j) == '*'){
					fields[n++] = (nmea.substr(i, j-i));	// Last field before checksum
					break;
				}
			}
			if(j < nmea.length() - 3) fields[n] = (nmea.substr(j, 3));	// Checksum characters.
		}
	}
	return fields;

InvalidNoFields:
	for(auto k = 0u; ++k < N; ) fields[k] = "";
	return fields;
}


/* NMEA Address Methods */

/**
 * @brief Construct a new NMEA::Address::Address object
 * 
 * @param str
 * 
 * @note sss always filled with 3 characters. tt will assume the remainder.
 */
NMEA_Standard::Address::Address(const string & str){
	if(str.length() >= 3){
		tt = getTalkerId(str.substr(0, str.length() - 3));
		sss = getMessage(str.substr(2));
	}
}

NMEA_Standard::Address::Address(const string & tt, const string & sss) :
	tt(getTalkerId(tt)), sss(getMessage(sss)) {}

string NMEA_Standard::Address::toString(char buff[5]){
	auto ttStr = NMEA_Standard::toString(tt);
	auto sssStr = NMEA_Standard::toString(sss);
	std::copy(ttStr.begin(), ttStr.end(), buff);
	std::copy(sssStr.begin(), sssStr.end(), buff + ttStr.length());
	return string(buff, ttStr.length() + sssStr.length());
}

/* NMEA Checksum Methods */

NMEA_Standard::Checksum::Checksum(const string & s){
	std::sscanf(s.c_str(), "*%2hhX", &cs);
}

NMEA_Standard::Checksum::Checksum(uint8_t c){
	cs = c;
}

bool NMEA_Standard::Checksum::valid(const string & nmea){
	auto astI = nmea.rfind('*');

	if(nmea.empty() || astI == string::npos) return false;

	uint8_t givenC = 0;
	auto nConv = std::sscanf(nmea.substr(astI, 5).c_str(), "*%2hX\r\n", (uint16_t *)&givenC);
		// In sscanf using "*%2hhX\r\n" should have been appropriate for a uint8_t value but this formatter triggered reading
		// the checksum as a decimal number. This appears to be a legitimate bug with the compiler.
		// The above using the type cast remains safe because %2 guarantees the value will fit within givenC.
	if(nConv != 1) return false;	// Scan failed.
	else return givenC == checksum(nmea);
}

uint8_t NMEA_Standard::Checksum::checksum(const string & nmea){
	uint8_t chk = 0x00u;
	for(auto & c : nmea){
		if(c == '$') continue;
		else if (c == '*') break;
		else chk ^= c;
	}
	return chk;
}

void NMEA_Standard::setChecksum(){
	cs.cs = Checksum::checksum(this->toString(nullptr));
}

const string NMEA_Standard::toString(const Message msg){
	switch (msg){
		case Message::DTM : return "DTM";
		case Message::GAQ : return "GAQ";
		case Message::GBQ : return "GBQ";
		case Message::GBS : return "GBS";
		case Message::GGA : return "GGA";
		case Message::GLL : return "GLL";
		case Message::GLQ : return "GLQ";
		case Message::GNQ : return "GNQ";
		case Message::GNS : return "GNS";
		case Message::GPQ : return "GPQ";
		case Message::GRS : return "GRS";
		case Message::GSA : return "GSA";
		case Message::GST : return "GST";
		case Message::GSV : return "GSV";
		case Message::RLM : return "RLM";
		case Message::RMC : return "RMC";
		case Message::TXT : return "TXT";
		case Message::VLW : return "VLW";
		case Message::VTG : return "VTG";
		case Message::ZDA : return "ZDA";
		default: return "";
	}
}

const string NMEA_Standard::toString(const TalkerID tId){
	switch(tId){
		case TalkerID::GP : return"GP";
		case TalkerID::GL : return"GL";
		case TalkerID::GA : return"GA";
		case TalkerID::GB : return"GB";
		case TalkerID::GQ : return"GQ";
		case TalkerID::GN : return"GN";
		default: return "";
	}
}

NMEA_Standard::Message NMEA_Standard::getMessage(const StaticString & s){
	if(s.size() != 3) return Message::UNKNOWN;

	static std::map<StaticString, Message> m{
		{"DTM", Message::DTM},
		{"GAQ", Message::GAQ},
		{"GBQ", Message::GBQ},
		{"GBS", Message::GBS},
		{"GGA", Message::GGA},
		{"GLL", Message::GLL},
		{"GLQ", Message::GLQ},
		{"GNQ", Message::GNQ},
		{"GNS", Message::GNS},
		{"GPQ", Message::GPQ},
		{"GRS", Message::GRS},
		{"GSA", Message::GSA},
		{"GST", Message::GST},
		{"GSV", Message::GSV},
		{"RLM", Message::RLM},
		{"RMC", Message::RMC},
		{"TXT", Message::TXT},
		{"VLW", Message::VLW},
		{"VTG", Message::VTG},
		{"ZDA", Message::ZDA},
	};

	if(m.find(s) == m.end()) return Message::UNKNOWN;
	else return m[s];
}

NMEA_Standard::TalkerID NMEA_Standard::getTalkerId(const StaticString & s){
	if(s.size() != 2) return TalkerID::UNKNOWN;

	static std::map<StaticString, TalkerID> m{
		{"GP", TalkerID::GP},
		{"GL", TalkerID::GL},
		{"GA", TalkerID::GA},
		{"GB", TalkerID::GB},
		{"GQ", TalkerID::GQ},
		{"GN", TalkerID::GN}
	};

	if(m.find(s) == m.end()) return TalkerID::UNKNOWN;
	else return m[s];
}



string NMEA_Standard::Checksum::toString(char cStr[4]){
	auto nConv = std::snprintf(cStr, 4, "*%2X", cs);
	return (nConv == 1) ? string(cStr) : "";
}

/* NMEA UTC Time Methods */

NMEA_Standard::UTC_Time::UTC_Time(const string & tStr){
	std::sscanf(tStr.c_str(), "%2hu%2hu%5f", (uint16_t*)&hh, (uint16_t*)&mm, &ss);
}

time_t NMEA_Standard::UTC_Time::daytime() const{
	return (hh * 60 + mm) * 60 * 1000 + ss * 1000;
}

/*  NMEA Coordinate Methods */

NMEA_Standard::Coordinate::Coordinate(const string & s, char nsew) : nsew(nsew){
	auto const decI = s.find('.');
	if(
		(decI != string::npos) &&
		(s.length() >= 6) &&
		(decI > 3)
	){
		char fmt[8];
		std::snprintf(fmt, 8, "%%%1dhd%%8f", decI-2);
		std::sscanf(s.c_str(), fmt, &deg, &min);
	}
}

string NMEA_Standard::Coordinate::toString(char cStr[12]){
	auto nConv = std::snprintf(cStr, 12, "%3d%8.4f", deg, min);
	return (nConv == 2) ? string(cStr) : "";
}

/**
 * @brief Assignes the Coordinate in terms of fractional degrees.
 * 
 * @param c	The Coordinate being assigned.
 * @return float The Coordinate in fractional degrees.
 */
NMEA_Standard::Coordinate::operator float() const{
	return (static_cast<float>(deg)*60 + min) * (( nsew == 'S') || (nsew == 'W') ? -1.0 : 1.0);
}

/* NMEA GNS Message */
extern unsigned long strtoul(const char *, char ** = nullptr, int = 10);
NMEA_Standard::GNS::GNS(const std::array<StaticString, 15> & fields){
		// Class constructors will handle empty cases.
							addr 		= fields[0];
							time		= fields[1];
							lat			= Coordinate(fields[2], (!fields[3].empty() ? fields[3].at(0) : ' ') );
							lon			= Coordinate(fields[4], (!fields[5].empty() ? fields[5].at(0) : ' ') );
							posMode		= fields[6];
	if(!fields[7].empty())	numSV		= std::strtoul(fields[7].c_str(), nullptr, 10);
	if(!fields[8].empty())	hdop		= std::strtof(fields[8].c_str(), nullptr);
	if(!fields[9].empty())	alt			= std::strtof(fields[9].c_str(), nullptr);
	if(!fields[10].empty())	sep			= std::strtof(fields[10].c_str(), nullptr);
	if(!fields[11].empty())	diffAge		= std::strtof(fields[11].c_str(), nullptr);
	if(!fields[12].empty())	diffStation	= std::strtoul(fields[12].c_str(), nullptr, 10);
	if(!fields[13].empty()) navStatus 	= fields[13].at(0);
							cs			= fields[14];
}

NMEA_Standard::GNS::GNS(const string & msg) :
	GNS(parseFields<15>(msg)){}

/* NMEA GLL Message */

NMEA_Standard::GLL::GLL(const std::array<StaticString, 9> & fields){	
							addr 	= fields[0];
							lat 	= Coordinate(fields[1], (!fields[2].empty() ? fields[2].at(0) : ' '));
							lon 	= Coordinate(fields[3], (!fields[4].empty() ? fields[4].at(0) : ' '));
							time 	= fields[5];
	if(!fields[6].empty()) 	status 	= fields[6].at(0);
	else					status	= ' ';
	if(!fields[7].empty()) 	posMode = fields[7].at(0);
	else					posMode	= ' ';
							cs 		= fields[8];
}

NMEA_Standard::GLL::GLL(const string & nmea):
	GLL(parseFields<9>(nmea)){}

/* NMEA GSA Message */

NMEA_Standard::GSA::GSA(const std::array<StaticString, 20> & fields){
							addr 		= fields[0];
	if(!fields[1].empty())	opMode 		= fields[1].at(0);
	if(!fields[2].empty()) 	navMode 	= std::strtoul(fields[2].c_str(), nullptr, 10);
	
	for(int i = 0; i < 12; i++)
							svid[i] = (!fields[3+i].empty()) ? std::strtoul(fields[3+i].c_str(), nullptr, 10) : 0u;
	
	if(!fields[15].empty()) pdop 		= std::strtof(fields[15].c_str(), nullptr);
	if(!fields[16].empty()) hdop 		= std::strtof(fields[16].c_str(), nullptr);
	if(!fields[17].empty()) vdop 		= std::strtof(fields[17].c_str(), nullptr);
	if(!fields[18].empty()) systemId 	= std::strtoul(fields[18].c_str(), nullptr, 16);
							cs 			= fields[19];
}

NMEA_Standard::GSA::GSA(const string & nmea) :
	GSA(parseFields<20>(nmea)){}

/* NMEA ZDA Message */


NMEA_Standard::ZDA::ZDA(const std::array<StaticString, 9> & fields){
	addr		= fields[0];
	time		= UTC_DateTime(
					fields[1],
					std::strtoul(fields[2].c_str(), nullptr, 10), std::strtoul(fields[3].c_str(), nullptr, 10), std::strtoul(fields[4].c_str(), nullptr, 10),
					std::strtoul(fields[5].c_str(), nullptr, 10), std::strtoul(fields[6].c_str(), nullptr, 10) );
	cs			= fields[7];
}

NMEA_Standard::ZDA::ZDA(const string & nmea) :
	ZDA(parseFields<9>(nmea)){}


NMEA_Standard::ZDA::UTC_DateTime::UTC_DateTime(const string & time, 
				uint8_t day, uint8_t month, uint16_t year, 
				uint8_t ltzh, uint8_t ltzm) :
	UTC_Time(time), 
	day(day), month(month), year(year),
	ltzh(ltzh), ltzm(ltzm) {}

time_t NMEA_Standard::ZDA::UTC_DateTime::epoch() const {
	struct tm date{
		.tm_sec 	= static_cast<int>(ss),
		.tm_min 	= mm,
		.tm_hour 	= hh,
		.tm_mday 	= day,
		.tm_mon 	= month,
		.tm_year 	= year - 1990,
		.tm_isdst = -1
	};
	return mktime(&date);
}

time_t NMEA_Standard::ZDA::UTC_DateTime::midnight() const{
	return epoch() - daytime();
}

inline NMEA_Standard::ZDA::UTC_DateTime::operator time_t() const{ return epoch(); }


/*** END OF FILE ***/
