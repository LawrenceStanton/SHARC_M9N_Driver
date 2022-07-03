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
		tt = str.substr(0, str.length() - 3);
		sss = str.substr(2);
	}
}

NMEA_Standard::Address::Address(const string & tt, const string & sss) :
	tt(tt), sss(sss){}

string NMEA_Standard::Address::toString(){
	return tt + sss;
}

/* NMEA Checksum Methods */

NMEA_Standard::Checksum::Checksum(const string & s){
	std::sscanf(s.c_str(), "*%2hhX", &cs);
}

NMEA_Standard::Checksum::Checksum(uint8_t c){
	cs = c;
}

bool NMEA_Standard::Checksum::valid(const string & nmea){
	auto astI = nmea.find_last_of('*');

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

string NMEA_Standard::toString(const Message msg){
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

string NMEA_Standard::toString(const TalkerID tId){
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


string NMEA_Standard::Checksum::toString(char cStr[4]){
	auto nConv = std::snprintf(cStr, 4, "*%2X", cs);
	return (nConv == 1) ? string(cStr) : "";
}

/* NMEA UTC Time Methods */

NMEA_Standard::UTC_Time::UTC_Time(const string & tStr){
	std::sscanf(tStr.c_str(), "%2hhu%2hhu%f", &hh, &mm, &ss);
}

/*  NMEA Coordinate Methods */

NMEA_Standard::Coordinate::Coordinate(const string & s){
	auto const decI = s.find('.');
	if(
		(decI != string::npos) &&
		(s.length() >= 6) &&
		(decI > 3)
	){
		char fmt[7];
		std::snprintf(fmt, 7, "%%%1dhd%%f", decI-2);
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
float NMEA_Standard::Coordinate::operator = (const Coordinate c){
	return static_cast<float>(c.deg)*60 + c.min;
}

/* NMEA GNS Message */
extern unsigned long strtoul(const char *, char ** = nullptr, int = 10);
NMEA_Standard::GNS::GNS(const std::array<StaticString, 15> & fields){
		// Class constructors will handle empty cases.
							addr 		= fields[0];
							time		= fields[1];
							lat			= fields[2];
	if(!fields[3].empty())	NS 			= fields[3].at(0);
							lon			= fields[4];
	if(!fields[5].empty())	EW 			= fields[5].at(0);
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
							lat 	= fields[1];
	if(!fields[2].empty())	NS 		= fields[2].at(0);
	else					NS 		= ' ';
							lon 	= fields[3];
	if(!fields[4].empty())	EW 		= fields[4].at(0);
	else					EW 		= ' ';
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
	if(!fields[3+i].empty()) svid[i] 	= std::strtoul(fields[3+i].c_str(), nullptr, 10);
	
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



/*** END OF FILE ***/
