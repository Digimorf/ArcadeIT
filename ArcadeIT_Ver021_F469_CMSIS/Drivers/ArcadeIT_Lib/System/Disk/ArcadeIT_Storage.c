/*
 ******************************************************************************
 *   ###                               ##          ###### ###### ###
 *  ## ##                              ##            ##     ##   ###
 * ##   ## ## ###  ######  #####   ######  ######    ##     ##   ###
 * ##   ## ###    ##           ## ##   ## ##    ##   ##     ##   ##
 * ####### ##     ##       ###### ##   ## ########   ##     ##   ##
 * ##   ## ##     ##      ##   ## ##   ## ##         ##     ##
 * ##   ## ##      ######  ######  ######  ######  ######   ##   ##
 * -----------------------------------------------------------------
 *                   S T O R A G E   I T E R F A C E
 * -----------------------------------------------------------------
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_Storage.c
 * @version V0.13
 * @date    26-07-2017
 * @last    26-02-2020
 * @brief   This library is used to manage the main routines of the storage system.
 *          Initialization, read, write, ioctl and status for each media installed.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY
 26-07-2017:
 - Creation date of the library structure.

 26-02-2020:
 - converted to CMSIS and ported to GitHUB

 *******************************************************************************
*/

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// FATFS functions.
#include <ff.h>
#include <diskio.h>

// ArcadeIT! Storage libraries.
#include <System/Disk/ArcadeIT_Storage.h>
#include <System/Disk/ArcadeIT_SD_Card.h>
#include <System/Disk/ArcadeIT_RAMDisk.h>

// ArcadeIT! System clocks and counters.
#include <System/Devices/ArcadeIT_RTC.h>
//#include "System/Peripherals/ArcadeIT_DMM.h"

// ArcadeIT! Applications.
#include <Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// External elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

// Memory space for the file systems.
DSTATUS gStatus[FF_VOLUMES] = { STA_NOINIT, STA_NOINIT };

extern FATFS *gFileSystemReferenceTemp;

uint8_t gCardType, gCurrentDrive = 0xFF;

const char gDriveName[] = {
    "RAMDISK:\0"
    "SDCARD:\0"
};

// Error messages strings.
const char gErrorMessages[] = {
    "* (0) Succeeded\0"
    "* (1) A hard error occurred in the low level disk I/O layer\0"
    "* (2) Assertion failed\0"
    "* (3) The physical drive cannot work\0"
    "* (4) Could not find the file\0"
    "* (5) Could not find the path\0"
    "* (6) The path name format is invalid\0"
    "* (7) Access denied due to prohibited access or directory full\0"
    "* (8) Access denied due to prohibited access\0"
    "* (9) The file/directory object is invalid\0"
    "* (10) The physical drive is write protected\0"
    "* (11) The logical drive number is invalid\0"
    "* (12) The volume has no work area\0"
    "* (13) There is no valid FAT volume\0"
    "* (14) The f_mkfs() aborted due to any problem\0"
    "* (15) Could not get a grant to access the volume within defined period\0"
    "* (16) The operation is rejected according to the file sharing policy\0"
    "* (17) LFN working buffer could not be allocated\0"
    "* (18) Number of open files > FF_FS_LOCK\0"
    "* (19) Given parameter is invalid\0"
};

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
DWORD get_fattime (void)
{
  rtc_time_t lTempTime = ArcadeIT_RTC_TimeDate_Get();

  return  (  (uint32_t)(2000 + lTempTime.year - 1980) << 25)
          | ((uint32_t)lTempTime.month << 21)
          | ((uint32_t)lTempTime.mday << 16)
          | ((uint32_t)lTempTime.hour << 11)
          | ((uint32_t)lTempTime.min << 5)
          | ((uint32_t)lTempTime.sec >> 1
          );

} // end get_fattime

// /////////////////////////////////////////////////////////////////////////////
// Get Drive Status
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
// Initialize a Drive
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Read Sector(s)
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Write Sector(s)
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Miscellaneous Functions
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
