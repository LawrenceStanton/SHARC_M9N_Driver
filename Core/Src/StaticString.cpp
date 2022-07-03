/**
  ******************************************************************************
  * @file			: StaticString.cpp
  * @brief			: Source for StaticString.hpp
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

#include "StaticString.hpp"

size_t StaticString::find(char c) const{
	auto i = std::find(begin(), end(), c);
	return (i == end()) ? npos : i - begin();
}

size_t StaticString::find_last_of(char c) const{
	auto i = std::find_end(begin(), end(), &c, &c);
	return (i == end()) ? npos : i - begin();
}


/*** END OF FILE ***/