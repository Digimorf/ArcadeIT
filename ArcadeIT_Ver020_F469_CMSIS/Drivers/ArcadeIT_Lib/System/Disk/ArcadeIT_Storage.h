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
 *           (C)2018 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_Storage.h
 * @version V0.13
 * @date    26-07-2017
 * @last    03-07-2018
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


sd


 gnd 3 5 cs mosi clk miso gnd



 *******************************************************************************
*/

#ifndef _ARCADEIT_STORAGE_H_
#define _ARCADEIT_STORAGE_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// FATFS functions.
#include "ff.h"
#include "diskio.h"

// /////////////////////////////////////////////////////////////////////////////
// Defines.
// ////////////////////////////////////////////////////////////////////////////
#define USE_DETECT_PIN          FALSE
#define USE_WRITE_PROTECTION    FALSE

#define INDEX_DRIVE                       0
#define INDEX_ROOT                        1
#define INDEX_PATH                        2
#define INDEX_FILE                        3

// /////////////////////////////////////////////////////////////////////////////
// Macros.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Export.
// /////////////////////////////////////////////////////////////////////////////
extern const char gDriveName[];
extern const char gErrorMessages[];

// /////////////////////////////////////////////////////////////////////////////
// Types.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////

// These are the functions used by the core of FATFS library.
/*
File Access
  f_open - Open/Create a file
  f_close - Close an open file
  f_read - Read data from the file
  f_write - Write data to the file
  f_lseek - Move read/write pointer, Expand size
  f_truncate - Truncate file size
  f_sync - Flush cached data
  f_forward - Forward data to the stream
  f_expand - Allocate a contiguous block to the file
  f_gets - Read a string
  f_putc - Write a character
  f_puts - Write a string
  f_printf - Write a formatted string
  f_tell - Get current read/write pointer
  f_eof - Test for end-of-file
  f_size - Get size
  f_error - Test for an error

Directory Access
  f_opendir - Open a directory
  f_closedir - Close an open directory
  f_readdir - Read a directory item
  f_findfirst - Open a directory and read the first item matched
  f_findnext - Read a next item matched

File and Directory Management
  f_stat - Check existance of a file or sub-directory
  f_unlink - Remove a file or sub-directory
  f_rename - Rename/Move a file or sub-directory
  f_chmod - Change attribute of a file or sub-directory
  f_utime - Change timestamp of a file or sub-directory
  f_mkdir - Create a sub-directory
  f_chdir - Change current directory
  f_chdrive - Change current drive
  f_getcwd - Retrieve the current directory and drive

Volume Management and System Configuration
  f_mount - Register/Unregister the work area of the volume
  f_mkfs - Create an FAT volume on the logical drive
  f_fdisk - Create partitions on the physical drive
  f_getfree - Get free space on the volume
  f_getlabel - Get volume label
  f_setlabel - Set volume label
  f_setcp - Set active code page

 */



// /////////////////////////////////////////////////////////////////////////////
#endif // _ARCADEIT_STORAGE_H_
