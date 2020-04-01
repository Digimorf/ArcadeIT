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
 *                  S T O R A G E   S Y S T E M S
 * -----------------------------------------------------------------
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_RAMDisk_LL.h
 * @version V0.13
 * @date    26-07-2017
 * @last    27-02-2020
 * @brief   This library is used to drive a RAM disk on the SRAM module at Low level.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

  ArcadeIT! Motherboard CN2 - PortG - Memory

  Pin  GPIO Function                     External SRAM Memory Interface
  ---------------------------------------------------------------------
  GND  VSS  GND                          CN2-40
  3.3V VDD  3.3V                         CN2-39
  119  PC6  GPIO, MEM_SIZE_0             CN2-26
  120  PC7  GPIO, MEM_SIZE_1             CN2-28
  121  PC8  GPIO, MEM_SIZE_2             CN2-30
  -    -    -, CS_SRAM                   CN2-32
  ?    ?    ?, OE_SRAM                   CN2-34
  146 +PD4  FMC_NOE, RD                  CN2-36
  147 +PD5  FMC_NWE, WR                  CN2-38
  016 +PF0  FMC_A0, BUS_A0               CN2-23
  017 +PF1  FMC_A1, BUS_A1               CN2-21
  018 +PF2  FMC_A2, BUS_A2               CN2-19
  019 +PF3  FMC_A3, BUS_A3               CN2-17
  020 +PF4  FMC_A4, BUS_A4               CN2-15
  021 +PF5  FMC_A5, BUS_A5               CN2-13
  060 +PF12 FMC_A6, BUS_A6               CN2-1
  063 +PF13 FMC_A7 , BUS_A7              CN2-3
  064 +PF14 FMC_A8 , BUS_A8              CN2-5
  065 +PF15 FMC_A9 , BUS_A9              CN2-7
  066 +PG0  FMC_A10, BUS_A10             CN2-9
  067 +PG1  FMC_A11, BUS_A11             CN2-11
  110 +PG2  FMC_A12, BUS_A12             CN2-25
  111 +PG3  FMC_A13, BUS_A13             CN2-27
  112 +PG4  FMC_A14, BUS_A14             CN2-29
  113 +PG5  FMC_A15, BUS_A15             CN2-31
  092 +PD11 FMC_A16, BUS_A16             CN2-33
  093 +PD12 FMC_A17, BUS_A17             CN2-35
  094 +PD13 FMC_A18, BUS_A18             CN2-37
  002 +PE3  FMC_A19, BUS_A19             CN2-22  *
  003 +PE4  FMC_A20, BUS_A20             CN2-20  * Select 8 512KB banks for a total of 4MB
  004 +PE5  FMC_A21, BUS_A21             CN2-18  *
  097 +PD14 FMC_D0 BUS_D0                CN2-2
  098 +PD15 FMC_D1 BUS_D1                CN2-4
  142 +PD0  FMC_D2 BUS_D2                CN2-6
  143 +PD1  FMC_D3 BUS_D3                CN2-8
  68  +PE7  FMC_D4 BUS_D4                CN2-10
  69  +PE8  FMC_D5 BUS_D5                CN2-12
  70  +PE9  FMC_D6 BUS_D6                CN2-14
  73  +PE10 FMC_D7 BUS_D7                CN2-16

 ******************************************************************************
 HISTORY
 26-07-2017:
 - Creation date of the library structure.

 11-09-2017:
 - Several fixes to the code layout and definitions.

 27-02-2020:
 - Ported to STM32CubeIDE and stored into GitHUB.

********************************************************************************
*/

#ifndef _ARCADEIT_RAMDISK_LL_H_
#define _ARCADEIT_RAMDISK_LL_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT devices.
#include <System/Devices/ArcadeIT_SRAM_LL.h>

// FATFS functions.
#include <diskio.h>
#include <ff.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////

#define ARCADEIT_TEXT_RAMDISK_NAME "RAMDISK"

// /////////////////////////////////////////////////////////////////////////////
// Macros.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
DSTATUS ArcadeIT_RAMDisk_Status(void);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_RAMDisk_Initialize(void);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_RAMDisk_Read
(
  BYTE *pBuffer,   // Pointer to the data buffer to store read data
  DWORD pSector,   // Start pSector number (LBA)
  UINT  pCount     // Number of sectors to read (1..128)
);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_RAMDisk_Write
(
  BYTE *pBuffer,   // Pointer to the data to write
  DWORD pSector,   // Start pSector number (LBA)
  UINT  pCount     // Number of sectors to write (1..128)
);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_RAMDisk_Ioctl
(
  BYTE pCommand,   // Control command code
  void *pBuffer    // Pointer to the control data
);
// -----------------------------------------------------------------------------
FRESULT ArcadeIT_RAMDisk_Start (void);

// /////////////////////////////////////////////////////////////////////////////
#endif // _ARCADEIT_RAMDISK_LL_H_
