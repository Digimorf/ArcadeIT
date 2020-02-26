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

 16-10-2018:
   Cleaned code and optimized it for portability.

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

// Storage devices layers.
#include <Devices/ArcadeIT_SD_Card_LL.h>

// ArcadeIT! Peripherals and buses.
#include <Devices/ArcadeIT_Serial_Port.h>
#include <Devices/ArcadeIT_SPI_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements.
// /////////////////////////////////////////////////////////////////////////////
extern DSTATUS gStatus[FF_VOLUMES];

extern BYTE gCardType;

// /////////////////////////////////////////////////////////////////////////////
// Global variables.
// /////////////////////////////////////////////////////////////////////////////
FATFS gFileSystemSD;

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
static int32_t wait_ready
(
  uint32_t pTimeOut   // Timeout in milliseconds.
)
{
  // Wait for card ready
  // pTimeOut:  Timeout [ms]
  // lReturnValue : 1:Ready, 0:Timeout

  uint8_t lData;

  gSecondaryTimer = pTimeOut;

  do {
    lData = ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);
    // This loop takes a time. Insert rot_rdq() here for multi task environment.

  } while (lData != DUMMY_BYTE && gSecondaryTimer); // Wait for card goes ready or timeout

  return (lData == DUMMY_BYTE) ? 1 : 0;

} // End wait_ready.

// /////////////////////////////////////////////////////////////////////////////
static void sd_deselect(void)
{
  // Deselect card and release SPI.
  SYS_SPI_CS_HIGH();

  // Dummy clock (force DO hi-z for multiple slave SPI).
  ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

} // End sd_deselect.

// /////////////////////////////////////////////////////////////////////////////
static int sd_select (void) /* 1:OK, 0:Timeout */
{
  // Select card and wait for ready
  // lReturnValue :  1:OK, 0:Timeout
  SYS_SPI_CS_LOW();

  // Dummy clock (force DO enabled).
  ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

  if (wait_ready(ARCADEIT_MILLISECOND)) return TRUE; // OK

  sd_deselect();

  return FALSE; // Timeout.

} // End sd_select.

// /////////////////////////////////////////////////////////////////////////////
static int sd_read_block
(
  BYTE *pBuffer,       // Data buffer
  UINT pNumberOfBytes  // Data block length (byte)
                       // 1:OK, 0:Error
)
{
  BYTE lToken;

  gSecondaryTimer = 200;
  do {
    // Wait for DataStart token in timeout of 200ms.
    lToken = ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

    // This loop will take a time. Insert rot_rdq() here for multi task environment.

  } while ((lToken == DUMMY_BYTE) && gSecondaryTimer);

  // Function fails if invalid DataStart token or timeout.
  if(lToken != 0xFE) return 0;

  // Store trailing data to the buffer.
  ArcadeIT_SPI_Port_Read_Buffer(pBuffer, pNumberOfBytes);

  // Discard CRC.
  ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);
  ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

  // Function succeeded.
  return 1;

} // End sd_read_block.

// /////////////////////////////////////////////////////////////////////////////
static int sd_write_block
(
  BYTE *pBuffer, // Pointer to 512 byte data to be sent.
  BYTE pToken    // Token
                 // 1:OK, 0:Error
)
{
  BYTE lResponse;

  // Wait for card ready.
  if (!wait_ready(500)) return 0;

  // Send token.
  ArcadeIT_SPI_Port_RW_Byte(pToken);

  if (pToken != 0xFD)
  {
    // Send data if token is other than StopTran.
    ArcadeIT_SPI_Port_Write_Buffer(pBuffer, 512);

    // Discard CRC.
    ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);
    ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

    // Receive data response.
    lResponse = ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

    // Function fails if the data packet was not accepted.
    if ((lResponse & 0x1F) != 0x05) return 0;

  } // End if.

  return 1;

} // End sd_write_block.

// /////////////////////////////////////////////////////////////////////////////
static BYTE send_cmd
(
  BYTE  pCommand,   // Command index
  DWORD pArgument   // Argument
                    // Return value: R1 resp (bit7==1:Failed to send)
)
{
  BYTE n, lResult;

  uint8_t lData;

  if (pCommand & 0x80)
  {
    // Send a CMD55 prior to ACMD<n>.
    pCommand &= 0x7F;

    lResult = send_cmd(CMD55, 0);
    if (lResult > 1) return lResult;

  } // End if.

  // Select the card and wait for ready except to stop multiple block read.
  if (pCommand != CMD12)
  {
    sd_deselect();

    if (!sd_select()) return DUMMY_BYTE;

  } // End if.

  // Send command packet.
  ArcadeIT_SPI_Port_RW_Byte(0x40 | pCommand);          // Start + command index
  ArcadeIT_SPI_Port_RW_Byte((pArgument >> 24) & 0xFF); // Argument[31..24]
  ArcadeIT_SPI_Port_RW_Byte((pArgument >> 16) & 0xFF); // Argument[23..16]
  ArcadeIT_SPI_Port_RW_Byte((pArgument >> 8) & 0xFF);  // Argument[15..8]
  ArcadeIT_SPI_Port_RW_Byte(pArgument & 0xFF);         // Argument[7..0]

  lData = 0x01;                        // Dummy CRC + Stop.
  if (pCommand == CMD0) lData = 0x95;  // Valid CRC for CMD0(0).
  if (pCommand == CMD8) lData = 0x87;  // Valid CRC for CMD8(0x1AA).

  ArcadeIT_SPI_Port_RW_Byte(lData);

  // Receive command response.
  lData = DUMMY_BYTE;

  // Discard following one byte when CMD12.
  if (pCommand == CMD12) ArcadeIT_SPI_Port_RW_Byte(lData);

  // Wait for response (10 bytes max).
  n = 10;
  do {
    lResult = ArcadeIT_SPI_Port_RW_Byte(lData);

  } while ((lResult & 0x80) && --n);

  // Return received response.
  return lResult;

} // End send_cmd.

// /////////////////////////////////////////////////////////////////////////////
DSTATUS ArcadeIT_SDCard_Disk_Initialize(void)
{
  BYTE n, cmd, ty, ocr[4];

  BYTE lData = DUMMY_BYTE;


  //ArcadeIT_SPI_Port_Init();

  // Check if drive is ready.
  //if (gStatus[DEV_SD] & STA_NOINIT) return RES_NOTRDY;

  SYS_SPI_SLOW();

  // Send 80 dummy clocks.
  for (n = 10; n; n--) ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

  ty = 0;
  if (send_cmd(CMD0, 0) == 1)
  {
    // Put the card SPI/Idle state.
    if (send_cmd(CMD8, 0x1AA) == 1)
    {
      // SDv2?
      for (n = 0; n < 4; n++)
      {
        lData = ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

        // Get 32 bit return value of R7 response.
        ocr[n] = lData;

      } // End for.

      if (ocr[2] == 0x01 && ocr[3] == 0xAA)
      {
        // Initialization timeout = 1 sec.
        gSecondaryTimer = ARCADEIT_MILLISECOND;

        // Is the card supports vcc of 2.7-3.6V?
        while (gSecondaryTimer && send_cmd(ACMD41, 1UL << 30)); // Wait for end of initialization with ACMD41(HCS)

        if (gSecondaryTimer && send_cmd(CMD58, 0) == 0)
        {
          // Check CCS bit in the OCR.
          for (n = 0; n < 4; n++)
          {
            lData = ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

            // Get 32 bit return value of R7 response.
            ocr[n] = lData;

          } // End for.

          ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;  // Card id SDv2

        } // End if.

      } // End if.
    }
    else
    {
      // Not SDv2 card.
      if (send_cmd(ACMD41, 0) <= 1)
      {
        // SDv1 or MMC?
        ty = CT_SD1;
        cmd = ACMD41;  // SDv1 (ACMD41(0))
      }
      else
      {
        ty = CT_MMC;
        cmd = CMD1;  // MMCv3 (CMD1(0))

      } // End if.

      // Initialization timeout = 1 sec.
      gSecondaryTimer = ARCADEIT_MILLISECOND;
      while (gSecondaryTimer && send_cmd(cmd, 0));    // Wait for end of initialization

      if (!gSecondaryTimer || send_cmd(CMD16, 512) != 0) // Set block length: 512
        ty = 0;

    } // End if.

  } // End if.

  gCardType = ty;  // Card type

  sd_deselect();

  if (ty)
  {
    // OK
    SYS_SPI_FAST();                      // Set fast clock
    gStatus[DEV_SD] &= ~STA_NOINIT;  // Clear STA_NOINIT flag
  }
  else
  {
    // Failed
    gStatus[DEV_SD] = STA_NOINIT;

  } // End if.

  return gStatus[DEV_SD];

} // End ArcadeIT_SDCard_Disk_Initialize.

// /////////////////////////////////////////////////////////////////////////////
DSTATUS ArcadeIT_SDCard_Disk_Status (void)
{
  // Return disk status.
  return gStatus[DEV_SD];

} // End ArcadeIT_SDCard_Disk_Status.

// /////////////////////////////////////////////////////////////////////////////
DRESULT ArcadeIT_SDCard_Disk_Read
(
  BYTE *pBuffer,   // Pointer to the data buffer to store read data
  DWORD pSector,   // Start pSector number (LBA)
  UINT  pCount     // Number of sectors to read (1..128)
)
{
  if (!pCount) return RES_PARERR;   // Check parameter

  if (gStatus[DEV_SD] & STA_NOINIT) return RES_NOTRDY; // Check if drive is ready

  if (!(gCardType & CT_BLOCK)) pSector *= 512;  // LBA of BA conversion (byte addressing cards)

  if (pCount == 1)
  {
    // Single pSector read
    if ((send_cmd(CMD17, pSector) == 0) && sd_read_block(pBuffer, 512))
    {
      // READ_SINGLE_BLOCK
      pCount = 0;
    }
  }
  else
  {
    // Multiple pSector read
    if (send_cmd(CMD18, pSector) == 0)
    {
      // READ_MULTIPLE_BLOCK
      do {
        if (!sd_read_block(pBuffer, 512)) break;

        pBuffer += 512;

      } while (--pCount);

      // STOP_TRANSMISSION
      send_cmd(CMD12, 0);
    }
  }

  sd_deselect();

  // Return result
  return pCount ? RES_ERROR : RES_OK;

} // End ArcadeIT_SDCard_Disk_Read.

// /////////////////////////////////////////////////////////////////////////////
DRESULT ArcadeIT_SDCard_Disk_Write
(
  const BYTE *pBuffer,   // Pointer to the data to write
  DWORD pSector,   // Start pSector number (LBA)
  UINT  pCount     // Number of sectors to write (1..128)
)
{
  if (!pCount) return RES_PARERR;   // Check parameter
  if (gStatus[DEV_SD] & STA_NOINIT) return RES_NOTRDY; // Check if drive is ready
  if (gStatus[DEV_SD] & STA_PROTECT) return RES_WRPRT; // Check write protect

  if (!(gCardType & CT_BLOCK)) pSector *= 512;  // LBA ==> BA conversion (byte addressing cards)

  if (pCount == 1)
  {
    // Single pSector write
    if ((send_cmd(CMD24, pSector) == 0) && sd_write_block((BYTE*)pBuffer, 0xFE))
    {
      pCount = 0;

    } // End if.
  }
  else
  {        // Multiple pSector write
    if (gCardType & CT_SDC) send_cmd(ACMD23, pCount); // Predefine number of sectors

    if (send_cmd(CMD25, pSector) == 0)
    {
      // WRITE_MULTIPLE_BLOCK
      do {
        if (!sd_write_block((BYTE*)pBuffer, 0xFC)) break;

        pBuffer += 512;

      } while (--pCount);

      if (!sd_write_block(0, 0xFD)) pCount = 1;  // STOP_TRAN token

    } // End if.

  } // End if.

  sd_deselect();

  // Return result.
  return pCount ? RES_ERROR : RES_OK;

} // End ArcadeIT_SDCard_Disk_Write.

// /////////////////////////////////////////////////////////////////////////////
DRESULT ArcadeIT_SDCard_Disk_Ioctl
(
  BYTE pCommand,   // Control command code
  void *pBuffer    // Pointer to the conrtol data
)
{
  DRESULT res;
  BYTE n, csd[16];
  DWORD *dp, st, ed, csize;

  if (gStatus[DEV_SD] & STA_NOINIT) return RES_NOTRDY; // Check if drive is ready

  res = RES_ERROR;

  switch (pCommand)
  {
    case CTRL_SYNC :    // Wait for end of internal write process of the drive
      if (sd_select()) res = RES_OK;
      break;

    case GET_SECTOR_COUNT : // Get drive capacity in unit of sector (DWORD)
      if ((send_cmd(CMD9, 0) == 0) && sd_read_block(csd, 16))
      {
        if ((csd[0] >> 6) == 1)
        {
          // SDC ver 2.00
          csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;

          *(DWORD*)pBuffer = csize << 10;
        }
        else
        {
          // SDC ver 1.XX or MMC ver 3
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;

          csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;

          *(DWORD*)pBuffer = csize << (n - 9);

        } // End if.

        res = RES_OK;
      }
      break;

    case GET_BLOCK_SIZE : // Get erase block size in unit of sector (DWORD)
      if (gCardType & CT_SD2)
      {
        // SDC ver 2.00
        if (send_cmd(ACMD13, 0) == 0)
        {
          // Read SD status
          ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE);

          if (sd_read_block(csd, 16))
          {
            // Read partial block
            for (n = 64 - 16; n; n--) ArcadeIT_SPI_Port_RW_Byte(DUMMY_BYTE); // Purge trailing data

            *(DWORD*)pBuffer = 16UL << (csd[10] >> 4);

            res = RES_OK;

          } // End if.

        } // End if.
      }
      else
      {
        // SDC ver 1.XX or MMC
        if ((send_cmd(CMD9, 0) == 0) && sd_read_block(csd, 16))
        {
          // Read CSD
          if (gCardType & CT_SD1)
          {
            // SDC ver 1.XX
            *(DWORD*)pBuffer = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
          }
          else
          {
            // MMC
            *(DWORD*)pBuffer = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);

          } // End if.

          res = RES_OK;

        } // End if.
      }
      break;

    case CTRL_TRIM :  // Erase a block of sectors (used when _USE_ERASE == 1)
      if (!(gCardType & CT_SDC)) break;        // Check if the card is SDC

      if (disk_ioctl(DEV_SD, SD_GET_CSD, csd)) break; // Get CSD

      if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break; // Check if sector erase can be applied to the card

      dp = pBuffer; st = dp[0]; ed = dp[1];        // Load sector block

      if (!(gCardType & CT_BLOCK))
      {
        st *= 512; ed *= 512;

      } // End if.

      if (send_cmd(CMD32, st) == 0 && send_cmd(CMD33, ed) == 0 && send_cmd(CMD38, 0) == 0 && wait_ready(30000))
      {
        // Erase sector block
        res = RES_OK; // FatFs does not check result of this command

      } // End if.

      break;

    default: res = RES_PARERR; break;

  } // End switch.

  sd_deselect();

  return res;

} // End ArcadeIT_SDCard_Disk_Ioctl.

// /////////////////////////////////////////////////////////////////////////////
FRESULT ArcadeIT_SDCard_Disk_Start (void)
{
  FRESULT lResult, lActive;
  BYTE lBuffer[4096];
  uint32_t lFreeCluster;
  char lString[ARCADEIT_STRING_LENGTH_MAX];
  FATFS *lFileSystemReferenceTemp;

  lActive = FR_NOT_READY;

  lResult = f_mount(&gFileSystemSD, ArcadeIT_Utility_Get_String_ID(DEV_SD, (char*)gDriveName), 1);
  switch (lResult)
  {
    case FR_NO_FILESYSTEM:
      sprintf(lString, "No file system on SD-Card");
      // Shows a message to serial port as debug
      if (gDevices & ARCADEIT_DEVICE_SERIAL)
      {
        // Starts and configure the serial port.
        ArcadeIT_Serial_Port_String_Send(lString);

      } // End if.

      lResult = f_mkfs(ArcadeIT_Utility_Get_String_ID(DEV_SD, (char*)gDriveName), NULL, lBuffer, sizeof(lBuffer));
      sprintf(lString, "Preparing SD-Card Disk Drive: %s", ArcadeIT_Utility_Get_String_ID(lResult, (char*)gErrorMessages));
      // Shows a message to serial port as debug
      if (gDevices & ARCADEIT_DEVICE_SERIAL)
      {
        // Starts and configure the serial port.
        ArcadeIT_Serial_Port_String_Send(lString);

      } // End if.

      f_chdrive(ArcadeIT_Utility_Get_String_ID(DEV_SD, (char*)gDriveName));
      f_setlabel(ARCADEIT_TEXT_SDCARD_NAME);
      break;

    case FR_OK:
      sprintf(lString, "SD-Card Disk device: [%s] ",ArcadeIT_Utility_Get_String_ID(DEV_SD, (char*)gDriveName));
      // Shows a message to serial port as debug
      if (gDevices & ARCADEIT_DEVICE_SERIAL)
      {
        // Starts and configure the serial port.
        ArcadeIT_Serial_Port_String_Send(lString);

      } // End if.

      lResult = f_getfree(ArcadeIT_Utility_Get_String_ID(DEV_SD, (char*)gDriveName), &lFreeCluster, &lFileSystemReferenceTemp);

      // Print the free space.
      sprintf(lString,
             "%lu/%lu KiB free.\n\r",
             (lFreeCluster * ((lFileSystemReferenceTemp->csize * SECTOR_SIZE) / 1024)),
             ((lFileSystemReferenceTemp->n_fatent - 2) * ((lFileSystemReferenceTemp->csize * SECTOR_SIZE) / 1024))
             );
      // Shows a message to serial port as debug
      if (gDevices & ARCADEIT_DEVICE_SERIAL)
      {
        // Starts and configure the serial port.
        ArcadeIT_Serial_Port_String_Send(lString);

      } // End if.

      lActive = FR_OK;
      break;

    default:
      sprintf(lString, "SD-Card not present or hardware error.\n\r");
      // Shows a message to serial port as debug
      if (gDevices & ARCADEIT_DEVICE_SERIAL)
      {
        // Starts and configure the serial port.
        ArcadeIT_Serial_Port_String_Send(lString);

      } // End if.

      break;

  } // End switch.

  return lActive;

} // End ArcadeIT_SDCard_Disk_Start.

// /////////////////////////////////////////////////////////////////////////////
