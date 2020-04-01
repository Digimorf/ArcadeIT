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
 * @file    ArcadeIT_SRAM_LL.c
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
 HISTORY
 26-07-2017:
 - Creation date of the library structure.

 11-09-2017:
 - Several fixes to the code layout and definitions.

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

********************************************************************************
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
#include <diskio.h>
#include <ff.h>

// ArcadeIT! Storage libraries.
#include <System/Disk/ArcadeIT_Storage.h>
#include <System/Disk/ArcadeIT_RAMDisk.h>

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>
#include <System/Devices/ArcadeIT_BUS_Port.h>

// ArcadeIT! System clocks and counters.
//#include "System/Peripherals/ArcadeIT_DMM.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements.
// /////////////////////////////////////////////////////////////////////////////
extern DSTATUS gStatus[FF_VOLUMES];

// /////////////////////////////////////////////////////////////////////////////
// Global variables.
// /////////////////////////////////////////////////////////////////////////////

FATFS gFileSystemRAM;
FATFS *gFileSystemReferenceRAM;

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////
DSTATUS ArcadeIT_SRAM_Init(void)
{
  /*
  * DESCRIPTION: This function initializes the SRAM module.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  char lString[ARCADEIT_STRING_LENGTH_MAX];
  uint8_t lRead = 0;
  uint8_t lMemoryBanks = 0;
  uint32_t lErrorBytes = 0, lBytes = 0;

  if (gStatus[DEV_RAM] == STA_NOINIT)
  {
    gStatus[DEV_RAM] &= ~STA_NOINIT;  // Clear STA_NOINIT flag

    // Test the first sectors of each memory bank and mark it as present or not.
    uint8_t lBanks = 0;
    for (lBanks = 0; lBanks < 4; lBanks++)
    {
      if (gDevices & ARCADEIT_DEVICE_SERIAL)
      {
        sprintf(lString, "Checking memory bank %d: ", lBanks);
        //ArcadeIT_Terminal_Debug(TERMINAL_DEBUG, lString);
        ArcadeIT_Serial_Port_String_Send(lString);
      }

      for (lBytes = lBanks*512*1024; lBytes < (uint32_t)((lBanks+1)*512*1024); lBytes++)
      {
        //*(__IO uint8_t *)(0x60000000 + lBytes) = lBytes & 0xFF;
        gRAMAddress[lBytes] = lBytes & 0xFF;

      } // End for.

      for (lBytes = lBanks*512*1024; lBytes < (uint32_t)((lBanks+1)*512*1024); lBytes++)
      {
        //lRead = *(__IO uint8_t *)(0x60000000 + lBytes);
        lRead = gRAMAddress[lBytes];

        if (lRead != (lBytes & 0xFF))
        {
            lErrorBytes++;

        } // End if.

      } // End for.

      if (lErrorBytes == 0)
      {
        lMemoryBanks |= (1<<lBanks);
        // FIXME Include only with a debug flag
        if (gDevices & ARCADEIT_DEVICE_SERIAL)
          //ArcadeIT_Terminal_Debug(TERMINAL_DEBUG, "OK");
          ArcadeIT_Serial_Port_String_Send("OK" CURSOR_NEWLINE);
      }
      else
      {
        // FIXME Include only with a debug flag
        if (gDevices & ARCADEIT_DEVICE_SERIAL)
          //ArcadeIT_Terminal_Debug(TERMINAL_DEBUG, "-");
          ArcadeIT_Serial_Port_String_Send("-" CURSOR_NEWLINE);
      } // End if.

      // Reset error conditions for the next bank check.
      lErrorBytes = 0;

    } // End for.

    // Configure the amount of RAM.
    switch (lMemoryBanks)
    {
      case SRAM_512K:
        gRAMSize = 1 * 512 * 1024;
        break;

      case SRAM_1024K:
        gRAMSize = 2 * 512 * 1024;
        break;

      case SRAM_2048K:
        gRAMSize = 4 * 512 * 1024;
        break;

      default:
        gRAMSize = 0;
        gStatus[DEV_RAM] = STA_NOINIT;

        // FIXME Include only with a debug flag
        if (gDevices & ARCADEIT_DEVICE_SERIAL)
          //ArcadeIT_Terminal_Debug(TERMINAL_DEBUG, "\n\rSRAM R/W Error or not present.");
          ArcadeIT_Serial_Port_String_Send("SRAM R/W Error or not present." CURSOR_NEWLINE);
        break;

    } // End switch.

    // FIXME Include only with a debug flag
    if (gDevices & ARCADEIT_DEVICE_SERIAL)
    {
      sprintf(lString, "RAM: %luKB." CURSOR_NEWLINE, (gRAMSize/1024));
      //ArcadeIT_Terminal_Debug(TERMINAL_DEBUG, lString);
      ArcadeIT_Serial_Port_String_Send(lString);

    } // End if.

  } // End if.

  return gStatus[DEV_RAM];

} // End ArcadeIT_SRAM_Init.

// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
