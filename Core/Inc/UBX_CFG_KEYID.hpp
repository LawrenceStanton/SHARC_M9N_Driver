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

using KeyID = UBX::CFG::VAL::KeyID;

/* CFG-HW Hardware Configuration Keys */
static const constexpr KeyID CFG_HW_ANT_CFG_VOLTCTRL		{0x10A3002E};
static const constexpr KeyID CFG_HW_ANT_CFG_SHORTDET		{0x10A3002F};
static const constexpr KeyID CFG_HW_ANT_CFG_SHORTDET_POL	{0x10A30030};
static const constexpr KeyID CFG_HW_ANT_CFG_OPENDET			{0x10A30031};
static const constexpr KeyID CFG_HW_ANT_CFG_OPENDET_POL		{0x10A30032};
static const constexpr KeyID CFG_HW_ANT_CFG_PWRDOWN			{0x10A30033};
static const constexpr KeyID CFG_HW_ANT_CFG_PWRDOWN_POL		{0x10A30034};
static const constexpr KeyID CFG_HW_ANT_CFG_RECOVER			{0x10A30035};

static const constexpr KeyID CFG_HW_ANT_SUP_SWITCH_PIN		{0x20A30036};
static const constexpr KeyID CFG_HW_ANT_SUP_SHORT_PIN		{0x20A30037};
static const constexpr KeyID CFG_HW_ANT_SUP_OPEN_PIN		{0x20A30038};
static const constexpr KeyID CFG_HW_ANT_SUP_ENGINE			{0x20A30054};
static const constexpr KeyID CFG_HW_ANT_SUP_SHORT_THR		{0x20A30055};
static const constexpr KeyID CFG_HW_ANT_SUP_OPEN_THR		{0x20A30056};


/* CFG-NMEA NMEA Protocol Configuration Keys */
static const constexpr KeyID CFG_NMEA_PROTVER				{0x20930001};
static const constexpr KeyID CFG_NMEA_MAXSVS				{0x20930002};
static const constexpr KeyID CFG_NMEA_COMPAT				{0x10930003};
static const constexpr KeyID CFG_NMEA_CONSIDER				{0x10930004};
static const constexpr KeyID CFG_NMEA_LIMIT82				{0x10930005};
static const constexpr KeyID CFG_NMEA_HIGHPREC				{0x10930006};
static const constexpr KeyID CFG_NMEA_SVNUMBERING			{0x20930007};

static const constexpr KeyID CFG_NMEA_FILT_GPS				{0x10930011};
static const constexpr KeyID CFG_NMEA_FILT_SBAS				{0x10930012};
static const constexpr KeyID CFG_NMEA_FILT_GAL				{0x10930013};
static const constexpr KeyID CFG_NMEA_FILT_QZSS				{0x10930015};
static const constexpr KeyID CFG_NMEA_FILT_GLO				{0x10930016};
static const constexpr KeyID CFG_NMEA_FILT_BDS				{0x10930017};

static const constexpr KeyID CFG_NMEA_OUT_INVFIX			{0x10930021};
static const constexpr KeyID CFG_NMEA_OUT_MSKFIX			{0x10930022};
static const constexpr KeyID CFG_NMEA_OUT_INVTIME			{0x10930023};
static const constexpr KeyID CFG_NMEA_OUT_INVDATE			{0x10930024};
static const constexpr KeyID CFG_NMEA_OUT_ONLYGPS			{0x10930025};
static const constexpr KeyID CFG_NMEA_OUT_FROZENCOG			{0x10930026};

static const constexpr KeyID CFG_NMEA_MAINTALKERID			{0x20930031};
static const constexpr KeyID CFG_NMEA_GSVTALKERID			{0x20930032};
static const constexpr KeyID CFG_NMEA_BDSTALKERID			{0x20930033};

/* CFG_PM Receiver Power Management */
static const constexpr KeyID CFG_PM_OPERATEMODE 			{0x20D00001};
static const constexpr KeyID CFG_PM_POSUPDATEPERIOD 		{0X40D00002};
static const constexpr KeyID CFG_PM_ACQPERIOD 				{0x40D00003};
static const constexpr KeyID CFG_PM_GRIDOFFSET 				{0x40D00004};
static const constexpr KeyID CFG_PM_ONTIME 					{0x30D00005};
static const constexpr KeyID CFG_PM_MINACQTIME 				{0x20D00006};
static const constexpr KeyID CFG_PM_MAXACQTIME 				{0x20D00007};
static const constexpr KeyID CFG_PM_DONOTENTEROFF 			{0x10D00008};
static const constexpr KeyID CFG_PM_WAITTIMEFIX 			{0x10D0000A};
static const constexpr KeyID CFG_PM_UPDATEEPH 				{0x20D0000B};
static const constexpr KeyID CFG_PM_EXTINTSEL 				{0x10D0000C};
static const constexpr KeyID CFG_PM_EXTINTWAKE 				{0x10D0000D};
static const constexpr KeyID CFG_PM_EXTINTBACKUP 			{0x10D0000E};
static const constexpr KeyID CFG_PM_EXTINTINACTIVE 			{0x10D0000E};
static const constexpr KeyID CFG_PM_EXTINTACTIVITY 			{0x40D0000F};
static const constexpr KeyID CFG_PM_LIMITPEAKCURR 			{0x10D00010};

/* CFG-UART1 UART1 Configuration */
static const constexpr KeyID CFG_UART1_BAUDRATE				{0x50420001};
static const constexpr KeyID CFG_UART1_STOPBITS				{0x20520002};
static const constexpr KeyID CFG_UART1_DATABITS				{0x20520003};
static const constexpr KeyID CFG_UART1_PARITY				{0x20520004};
static const constexpr KeyID CFG_UART1_ENABLED				{0x10520005};

/*** END OF FILE ***/