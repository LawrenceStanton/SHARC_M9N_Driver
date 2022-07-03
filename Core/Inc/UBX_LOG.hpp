/**
  ******************************************************************************
  * @file			: UBX.LOG.hpp
  * @brief			: Logging Classes for UBX Driver.
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

class UBX::LOG : public UBX{
public:
	class BATCH;
	class CREATE;
	class ERASE;
	class FINDTIME;
	class INFO;
	class RETRIEVE;
	class RETRIEVE_BATCH;
	class RETRIEVE_POS;
	class RETRIEVE_POS_EXTRA;
	class RETRIEVE_STRING;
	class STRING;
	
};

class UBX::LOG::BATCH : public UBX::LOG{
public:
	BATCH(const vect & ubx);
};
class UBX::LOG::CREATE : public UBX::LOG{
public:
	CREATE(const vect & ubx);
};
class UBX::LOG::ERASE : public UBX::LOG{
public:
	ERASE(const vect & ubx);
};
class UBX::LOG::FINDTIME : public UBX::LOG{
public:
	FINDTIME(const vect & ubx);
};
class UBX::LOG::INFO : public UBX::LOG{
public:
	INFO(const vect & ubx);
};
class UBX::LOG::RETRIEVE : public UBX::LOG{
public:
	RETRIEVE(const vect & ubx);
};
class UBX::LOG::RETRIEVE_BATCH : public UBX::LOG{
public:
	RETRIEVE_BATCH(const vect & ubx);
};
class UBX::LOG::RETRIEVE_POS : public UBX::LOG{
public:
	RETRIEVE_POS(const vect & ubx);
};
class UBX::LOG::RETRIEVE_POS_EXTRA : public UBX::LOG{
public:
	RETRIEVE_POS_EXTRA(const vect & ubx);
};
class UBX::LOG::RETRIEVE_STRING : public UBX::LOG{
public:
	RETRIEVE_STRING(const vect & ubx);
};
class UBX::LOG::STRING : public UBX::LOG{
public:
	STRING(const vect & ubx);
};

/*** END OF FILE ***/