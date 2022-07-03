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

size_t StaticString::rfind(char c) const{
	auto rBegin = std::make_reverse_iterator(end());
	auto rEnd = std::make_reverse_iterator(begin());
	auto i = std::find(rBegin, rEnd, c);
	return (i == rEnd) ? npos : (l-1) - (i - rBegin);
}


/*** END OF FILE ***/