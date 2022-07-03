/**
  ******************************************************************************
  * @file			: UBX_CFG.cpp
  * @brief			: Source for UBX_CFG.hpp
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

#include "UBX_CFG.hpp"

UBX::X4 UBX::CFG::KeyID::toKey(){
	return ( ( ((X4)size) << 28 ) | ( ((X4)groupID) << 16 ) | ( ((X4)itemID) ) );
}

UBX::CFG::VAL::GET::POLL_REQ::POLL_REQ(KeyID key) : 
	keys(std::vector<UBX::CFG::KeyID>({key})) {}

UBX::CFG::VAL::GET::POLL_REQ::POLL_REQ(std::vector<KeyID> keys) : 
	keys(keys){}


vect UBX::CFG::VAL::GET::POLL_REQ::toPollReq(){
	vect v;
	v.reserve(4*(keys.size() + 1));

	v.push_back(version);
	v.push_back(static_cast<U1>(layer));
	
	auto posHex = UBX::toHex(position);
	v.insert(v.end(), posHex.begin(), posHex.end());
	
	for(auto & key : keys){
		auto keyHex = UBX::toHex(key.toKey());
		v.insert(v.end(), keyHex.begin(), keyHex.end());
	}

	return v;
}

/*** END OF FILE ***/