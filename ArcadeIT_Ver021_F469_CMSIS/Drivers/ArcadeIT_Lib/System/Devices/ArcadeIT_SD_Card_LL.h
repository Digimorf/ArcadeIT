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
 *           (C)2018 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_SD_Card_LL.h
 * @version V0.13
 * @date    26-07-2017
 * @last    03-07-2018
 * @brief   This library is used to drive the SD card at low level using SPI.
 *          ArcadeIT! uses SPI for the SD drive to keep the driver simple and
 *          portable to other platform.
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

  1.2 - SD Card

  ArcadeIT! Motherboard CN10 - Port B - SD-CARD

  Pin  GPIO Function                     SD Card interface
  ---------------------------------------------------------------------
  3.3V VDD  SD 3.3V                      CN10-1
  GND  VSS  SD GND                       CN10-2
  051  PA5  SD CLK    (SPI1_SCK)         CN10-3
  -    -    -                            CN10-4
  053  PA7  SD MOSI   (SPI1_MOSI)        CN10-5
  052  PA6  SD MISO   (SPI1_MISO)        CN10-6
  055  PC5  SD DETECT (GPIO SD Inserted) CN10-7
  054  PC4  SD CS     (GPIO Chip Select) CN10-8


  SPI port, SD Card / CN10
               .--------.
   2 GND       |  o  o  |   1 3.3V
   4 NC        |  o  o |    3 SPI1_SCK
   6 SPI1_MISO |  o  o |    5 SPI1_MOSI
   8 CS        |  o  o  |   7 SD DETECT
               '--------'

 ******************************************************************************
*/

#ifndef _ARCADEIT_SD_CARD_LL_H_
#define _ARCADEIT_SD_CARD_LL_H_

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

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////

// MMC/SD command.
#define CMD0   (0)        // GO_IDLE_STATE
#define CMD1   (1)        // SEND_OP_COND (MMC)
#define ACMD41 (0x80+41)  // SEND_OP_COND (SDC)
#define CMD8   (8)        // SEND_IF_COND
#define CMD9   (9)        // SEND_CSD
#define CMD10  (10)       // SEND_CID
#define CMD12  (12)       // STOP_TRANSMISSION
#define ACMD13 (0x80+13)  // SD_STATUS (SDC)
#define CMD16  (16)       // SET_BLOCKLEN
#define CMD17  (17)       // READ_SINGLE_BLOCK
#define CMD18  (18)       // READ_MULTIPLE_BLOCK
#define CMD23  (23)       // SET_BLOCK_COUNT (MMC)
#define ACMD23 (0x80+23)  // SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24  (24)       // WRITE_BLOCK
#define CMD25  (25)       // WRITE_MULTIPLE_BLOCK
#define CMD32  (32)       // ERASE_ER_BLK_START
#define CMD33  (33)       // ERASE_ER_BLK_END
#define CMD38  (38)       // ERASE
#define CMD55  (55)       // APP_CMD
#define CMD58  (58)       // READ_OCR

#define MEDIA_PRESENT     ((uint8_t)0x01)
#define MEDIA_NOT_PRESENT ((uint8_t)0x00)

#define ARCADEIT_TEXT_SDCARD_NAME  "ARCADEIT-CARD"

// /////////////////////////////////////////////////////////////////////////////
// Types.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External globals.
// /////////////////////////////////////////////////////////////////////////////
extern FATFS gFileSystemSD;

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
DSTATUS ArcadeIT_SDCard_Disk_Initialize(void);
// -----------------------------------------------------------------------------
DSTATUS ArcadeIT_SDCard_Disk_Status (void);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_SDCard_Disk_Read
(
  BYTE *pBuffer,   // Pointer to the data buffer to store read data
  DWORD pSector,   // Start pSector number (LBA)
  UINT  pCount     // Number of sectors to read (1..128)
);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_SDCard_Disk_Write
(
  const BYTE *pBuffer,   // Pointer to the data to write
  DWORD pSector,   // Start pSector number (LBA)
  UINT  pCount     // Number of sectors to write (1..128)
);
// -----------------------------------------------------------------------------
DRESULT ArcadeIT_SDCard_Disk_Ioctl
(
  BYTE pCommand,   // Control command code
  void *pBuffer    // Pointer to the conrtol data
);
// -----------------------------------------------------------------------------
FRESULT ArcadeIT_SDCard_Disk_Start (void);
// /////////////////////////////////////////////////////////////////////////////

#endif // _ARCADEIT_SD_CARD_LL_H_
