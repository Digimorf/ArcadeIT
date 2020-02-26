/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

// /////////////////////////////////////////////////////////////////////////////
// Added by Francesco De Simone to implement devices of ArcadeIT!
// This section is an interface layer between FATFS and ArcadeIT to use when
// updating this library.


// Include Low Level storage devices libraries
#include <System/Devices/ArcadeIT_SD_Card_LL.h>

// Prototypes for unused functions
DSTATUS RAM_disk_status();
DSTATUS MMC_disk_status();
DSTATUS USB_disk_status();
DSTATUS SD_disk_status()
{
  return ArcadeIT_SDCard_Disk_Status();
}

DSTATUS RAM_disk_initialize();
DSTATUS MMC_disk_initialize();
DSTATUS USB_disk_initialize();
DSTATUS SD_disk_initialize()
{
  return ArcadeIT_SDCard_Disk_Initialize();
}

DSTATUS RAM_disk_read(BYTE *buff, DWORD sector, UINT count);
DSTATUS MMC_disk_read(BYTE *buff, DWORD sector, UINT count);
DSTATUS USB_disk_read(BYTE *buff, DWORD sector, UINT count);
DSTATUS SD_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  return ArcadeIT_SDCard_Disk_Read(buff, sector, count);
}

DSTATUS RAM_disk_write(const BYTE *buff, DWORD sector, UINT count);
DSTATUS MMC_disk_write(const BYTE *buff, DWORD sector, UINT count);
DSTATUS USB_disk_write(const BYTE *buff, DWORD sector, UINT count);
DSTATUS SD_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  return ArcadeIT_SDCard_Disk_Write((BYTE*)buff, sector, count);
}

// /////////////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS result = 0;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_status();

		// translate the reslut code here

		return result;

	case DEV_MMC :
		//result = MMC_disk_status();

		// translate the reslut code here

		return result;

  // Added by Francesco De Simone to implement devices of ArcadeIT!
	case DEV_SD :
		result = SD_disk_status();

		// translate the reslut code here

		return result;

	case DEV_USB :
		//result = USB_disk_status();

		// translate the reslut code here

		return result;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS result = 0;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_initialize();

		// translate the reslut code here

		return result;

	case DEV_MMC :
		//result = MMC_disk_initialize();

		// translate the reslut code here

		return result;

	  // Added by Francesco De Simone to implement devices of ArcadeIT!
	  case DEV_SD :
	    result = SD_disk_initialize();

	    // translate the reslut code here

	    return result;

	case DEV_USB :
		//result = USB_disk_initialize();

		// translate the reslut code here

		return result;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
  DSTATUS result = 0;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return result;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return result;

  // Added by Francesco De Simone to implement devices of ArcadeIT!
  case DEV_SD :
    result = SD_disk_read(buff, sector, count);

    // translate the reslut code here

    return result;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return result;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive number to identify the drive */
	const BYTE *buff,	    /* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
  DSTATUS result = 0;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return result;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return result;

  // Added by Francesco De Simone to implement devices of ArcadeIT!
  case DEV_SD :
    result = SD_disk_write(buff, sector, count);

    // translate the reslut code here

    return result;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return result;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DSTATUS result = 0;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		return result;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return result;

	case DEV_SD :

	  result = ArcadeIT_SDCard_Disk_Ioctl(cmd, buff);

		// Process of the command for the MMC/SD card

		return result;

	case DEV_USB :

		// Process of the command the USB drive

		return result;
	}

	return RES_PARERR;
}

