/**
  ******************************************************************************
  * @file			: UBX_CFG_KEYID.hpp
  * @brief			: UBX Configuration Key ID Constants
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

#include "UBX_CFG.hpp"

/* CFG-HW Hardware Configuration Keys */
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_VOLTCTRL		{0x10A3002E};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_SHORTDET		{0x10A3002F};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_SHORTDET_POL	{0x10A30030};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_OPENDET		{0x10A30031};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_OPENDET_POL	{0x10A30032};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_PWRDOWN		{0x10A30033};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_PWRDOWN_POL	{0x10A30034};
constexpr UBX::CFG::KeyID CFG_HW_ANT_CFG_RECOVER		{0x10A30035};

constexpr UBX::CFG::KeyID CFG_HW_ANT_SUP_SWITCH_PIN		{0x20A30036};
constexpr UBX::CFG::KeyID CFG_HW_ANT_SUP_SHORT_PIN		{0x20A30037};
constexpr UBX::CFG::KeyID CFG_HW_ANT_SUP_OPEN_PIN		{0x20A30038};
constexpr UBX::CFG::KeyID CFG_HW_ANT_SUP_ENGINE			{0x20A30054};
constexpr UBX::CFG::KeyID CFG_HW_ANT_SUP_SHORT_THR		{0x20A30055};
constexpr UBX::CFG::KeyID CFG_HW_ANT_SUP_OPEN_THR		{0x20A30056};


/* CFG-NMEA NMEA Protocol Configuration Keys */
constexpr UBX::CFG::KeyID CFG_NMEA_PROTVER				{0x20930001};
constexpr UBX::CFG::KeyID CFG_NMEA_MAXSVS				{0x20930002};
constexpr UBX::CFG::KeyID CFG_NMEA_COMPAT				{0x10930003};
constexpr UBX::CFG::KeyID CFG_NMEA_CONSIDER				{0x10930004};
constexpr UBX::CFG::KeyID CFG_NMEA_LIMIT82				{0x10930005};
constexpr UBX::CFG::KeyID CFG_NMEA_HIGHPREC				{0x10930006};
constexpr UBX::CFG::KeyID CFG_NMEA_SVNUMBERING			{0x20930007};

constexpr UBX::CFG::KeyID CFG_NMEA_FILT_GPS				{0x10930011};
constexpr UBX::CFG::KeyID CFG_NMEA_FILT_SBAS			{0x10930012};
constexpr UBX::CFG::KeyID CFG_NMEA_FILT_GAL				{0x10930013};
constexpr UBX::CFG::KeyID CFG_NMEA_FILT_QZSS			{0x10930015};
constexpr UBX::CFG::KeyID CFG_NMEA_FILT_GLO				{0x10930016};
constexpr UBX::CFG::KeyID CFG_NMEA_FILT_BDS				{0x10930017};

constexpr UBX::CFG::KeyID CFG_NMEA_OUT_INVFIX			{0x10930021};
constexpr UBX::CFG::KeyID CFG_NMEA_OUT_MSKFIX			{0x10930022};
constexpr UBX::CFG::KeyID CFG_NMEA_OUT_INVTIME			{0x10930023};
constexpr UBX::CFG::KeyID CFG_NMEA_OUT_INVDATE			{0x10930024};
constexpr UBX::CFG::KeyID CFG_NMEA_OUT_ONLYGPS			{0x10930025};
constexpr UBX::CFG::KeyID CFG_NMEA_OUT_FROZENCOG		{0x10930026};

constexpr UBX::CFG::KeyID CFG_NMEA_MAINTALKERID			{0x20930031};
constexpr UBX::CFG::KeyID CFG_NMEA_GSVTALKERID			{0x20930032};
constexpr UBX::CFG::KeyID CFG_NMEA_BDSTALKERID			{0x20930033};




/*** END OF FILE ***/