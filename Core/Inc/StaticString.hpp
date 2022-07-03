/**
  ******************************************************************************
  * @file			: StaticString.hpp
  * @brief			: Structure for Stack-Based Static String with Helper Functions
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

#include <stdint.h>
#include <string.h>

#include <algorithm>

class StaticString{
private:
	const char * s;
	size_t l;


public:
	StaticString() : s(nullptr), l(0u){}

	template<typename C>
	StaticString(C * c_str, size_t len) : s(c_str), l(len){}

	template<typename C>
	StaticString(C * begin, C * end) : s((char *)begin), l(end-begin){}

	StaticString(const char * c_str) : s(c_str), l(strlen(c_str)){}
	StaticString(const StaticString &) = default;
	
	inline size_t size() const { return l; }
	inline size_t length() const { return size(); }
	inline const char * begin() const { return s; }
	inline const char * end() const { return s + l; }
	inline const char * c_str() const { return s; }
	inline bool empty() const { return l == 0; }
	inline char at(size_t n) const { return *(s + n); }


	StaticString substr(size_t begin, size_t len = npos) const { return StaticString(s + begin, ((len > l - begin) ? (l - begin) : len) ); }
	size_t find(char c) const;
	size_t rfind(char c) const;

	// inline const char * operator = (const StaticString & str){ return str.s; }
	StaticString operator + (StaticString & s);
	bool operator == (StaticString && str) const { return strncmp(s, str.s, l) == 0; }
	bool operator < (const StaticString & str) const { return strncmp(s, str.s, l) < 0; }
	bool operator > (const StaticString & str) const { return strncmp(s, str.s, l) > 0; }
	char operator [](unsigned int i) const { return ( (i < l) ? s[i] : 0 ); }
	
	static const size_t npos = -1;
};

/*** END OF FILE ***/