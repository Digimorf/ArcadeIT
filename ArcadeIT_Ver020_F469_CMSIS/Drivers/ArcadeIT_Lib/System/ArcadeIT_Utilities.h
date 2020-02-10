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
 *                   C O M M O N   U T I L I T I E S
 * -----------------------------------------------------------------
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_Utilities.h
 * @version V0.13
 * @date    17-07-2017
 * @last    02-07-2020
 * @brief   This library is used to handle several common tasks such as
 *          character conversion, operations on colors etc...
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY

 17-07-2017:
 - Creation date of the library structure.

 21-09-2017:
 - Cleaned the code from the specific memory location positioning of the
   functions.

 - Moved functions to different files for better reference.

 06-02-2020:
 - Converted to pure CMSIS.

 ******************************************************************************
*/

#ifndef __ARCADEIT_UTILITIES_H
#define __ARCADEIT_UTILITIES_H

#ifdef __cplusplus
 extern "C" {
#endif

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Firmware.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// ASCII CHARACTERS
#define ASCII_NULL          0x00 // Null Character
#define ASCII_BS            0x08 // Backspace
#define ASCII_TAB           0x09 // Tab
#define ASCII_LF            0x0A // Line Feed
#define ASCII_CR            0x0D // Carriage Return
#define ASCII_ESC           0x1B // Escape
#define ASCII_SPACE         0x20 // Space
#define ASCII_A             ('A')
#define ASCII_L             ('L')
#define ASCII_Z             ('Z')
#define ASCII_a             ('a')
#define ASCII_z             ('z')
#define ASCII_0             ('0')
#define ASCII_9             ('9')
#define ASCII_Q             ('Q')
#define ASCII_AT            ('@')
#define ASCII_SLASH         ('/')
#define ASCII_COLON         (':')
#define ASCII_DASH          ('-')
#define ASCII_HASH          ('#')
#define ASCII_DELETE        0x7F // Tab

#define HID_KBD_ENTER       ASCII_CR
#define HID_KBD_BS          ASCII_BS
#define HID_KBD_TAB         ASCII_TAB
#define HID_KBD_ESC         ASCII_ESC
#define HID_KBD_DEL         ASCII_DELETE

#define HID_KBD_CURSORUP    0x80
#define HID_KBD_CURSORDOWN  0x81
#define HID_KBD_CURSORLEFT  0x82
#define HID_KBD_CURSORRIGHT 0x83
#define HID_KBD_CTRL_Q      0x88
#define HID_KBD_CTRL_H      0x89
#define HID_KBD_CTRL_I      0x8a
#define HID_KBD_CTRL_N      0x8b
#define HID_KBD_CTRL_S      0x8c
#define HID_KBD_CTRL_L      0x8d
#define HID_KBD_CTRL_F      0x8e
#define HID_KBD_CTRL_1      0xa1

#define HID_KBD_PAGEUP      0x90
#define HID_KBD_PAGEDOWN    0x91
#define HID_KBD_END         0x92
#define HID_KBD_HOME        0x93
#define HID_KBD_CTRL_END    0x94
#define HID_KBD_CTRL_HOME   0x95

#define HID_KBD_PRTSCR      0x96

// Prints a new line.
#define CURSOR_NEWLINE      "\n\r"

// GFX characters
#define GFX_DOT             0x95  // Centered dot
#define GFX_TM              0x99  // Trademark
#define GFX_COPY            0xA9  // Copyright
#define GFX_REGISTERED      0xAE  // Registered
#define GFX_FRAME_LW_R      0x6A  // Lower right frame corner
#define GFX_FRAME_UP_R      0x6B  // Upper right frame corner
#define GFX_FRAME_UP_L      0x6C  // Upper left frame corner
#define GFX_FRAME_LW_L      0x6D  // Lower left frame corner
#define GFX_FRAME_H         0x71  // Horizontal line
#define GFX_FRAME_V         0x78  // Vertical line

#define DEBUG_BLINK_DELAY   500  // ms for the blinking LED

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////
// This palette is 11 bits + the msb is the h sync value.
                //           PB15-----PB12        PB11-----PB8       PB7-----PB4
                //           HS B3 B2 B1          G3 G2 G1 G0        R3 R2 R1 R0
#define RGB_TO_11_1_BIT(r,g,b)  ((((b >> 5) << 12) | ((g >> 4) << 8) | ((r >> 4) << 4)) | 0x8000)

                //           PB7-----PB4          PB11-----PB8       PB15-----PB12
                //           B3 B2 B1 B0          G3 G2 G1 G0        R3 R2 R1 R0
#define RGB_TO_12BIT(r,g,b)     ((((b) >> 4) << 12) | (((g) >> 4) << 8) | (((r) >> 4) << 4))
#define P12BIT_TO_RGB(c)        ((((c) & 0xF000)<< 8)|(((c) & 0x0F00) << 4)|(((c) & 0x00F0)))

   // DMA2D CLUT format
   // 4          210 43    210 432   105 43
   // AAAA AAAA XGGG BBXX XBBB RRRX XRRG GGXX
#define P12BIT_TO_RGB_DMA2D_VGA2(c)  (((\
   ( ((c) & 0xF000) << 8)\
 | ( ((c) & 0x0F00) << 4)\
 | ((((c) & 0x00F0))))) | 0xFF000000 )

   // DMA2D CLUT format
   // 4          210 43    210 432   105 43
   // AAAA AAAA XGGG BBXX XBBB RRRX XRRG GGXX
#define P12BIT_TO_RGB_DMA2D_VGA(c)  (((\
   ( (((c) & 0xC000) << 4)  | (((c) & 0x3000) << 1))\
 | ( (((c) & 0x0E00) >> 7)  | (((c) & 0x0100) << 14))\
 | ( (((c) & 0x00E0) << 4)  | (((c) & 0x0010) << 2)))/*<< 8*/) | 0xFF000000\
    )
   // DMA2D CLUT format
   //   210 43    210 432   105 43
   //  XGGG BBXX XBBB RRRX XRRG GGXX
#define P12BIT_TO_RGB_DMA2D_LCD_FG(c)  (\
   ( (((c) & 0xC000) << 4)  | (((c) & 0x3000) << 1))\
 | ( (((c) & 0x0E00) >> 7)  | (((c) & 0x0100) << 14))\
 | ( (((c) & 0x00E0) << 4)  | (((c) & 0x0010) << 2))\
    )
#define P12BIT_TO_RGB_DMA2D_LCD_BG(c)  (\
   ( (((c) & 0xC000) << 4)  | (((c) & 0x3000) << 1))\
 | ( (((c) & 0x0E00) >> 7)  | (((c) & 0x0100) << 14))\
 | ( (((c) & 0x00E0) << 4)  | (((c) & 0x0010) << 2))\
    )

//#define P12BIT_TO_RGB_DMA2D(c)  ((((c) & 0xF000)<<8)|(((c) & 0x0F00)<<4)|(((c) & 0x00F0)))
#define RGB_TO_16BIT565(r,g,b)  ((((r) >> 3) << 11) | (((g) >> 2) << 5) | (((b) >> 3)))
#define P12BIT_TO_16BIT565(c)   ((((c) & 0xF000)>>3)|(((c) & 0x0F00)>>9)|(((c) & 0x0100)<<7)|(((c) & 0x00F0)))
   /*  P12BIT_TO_16BIT565
       RRRRGGGG BBBB1xxx
       GGGRRRRR BBBBBGGG
   */

// LCD
// RRGGBB 12B
// F000F0 0FF8
// 00F000 F008

#define RGB_TO_12_1_BIT(r,g,b)  (((((b) >> 4) << 12) | (((g) >> 4) << 8) | (((r) >> 4) << 4)) | 0x0008)
#define C12_1_BIT_TO_RGB(color) (((color & 0xF000) << 8) | ((color & 0x0F00) << 4) | (color & 0x00F0))

#define RGB_TO_8BIT(r,g,b)      (((((b) >> 6) << 6) | (((g) >> 5) << 3) | ((r) >> 5)) << 4)

// Color conversion macros.
#define HEX_TO_BLUE(hexcolor)     (hexcolor & 0xff)
#define HEX_TO_GREEN(hexcolor)    ((hexcolor >> 8) & 0xff)
#define HEX_TO_RED(hexcolor)      ((hexcolor >> 16) & 0xff)

// Character conversion macros.
#define CHR_IS_LOWCASEALPHA(ch)   ((ch >= ASCII_a)&&(ch <= ASCII_z))

// /////////////////////////////////////////////////////////////////////////////
// Operations on variables.
/*
* DESCRIPTION: This function swaps the value of two variables.
* PARAMETERS:  See above.
* RETURNS:     Nothing.
*/
#define ArcadeIT_Utility_Swap_Values( pValue1, pValue2) \
{ \
  int16_t lValueTemp = pValue1; \
  pValue1 = pValue2; \
  pValue2 = lValueTemp; \
\
} // End ArcadeIT_Utility_Swap_Values.

#define ArcadeIT_Utility_Swap_Values2( pValue1, pValue2) \
{ \
  pValue1 = pValue1 ^ pValue2; \
  pValue2 = pValue1 ^ pValue2; \
  pValue1 = pValue1 ^ pValue2; \
\
} // End ArcadeIT_Utility_Swap_Values2.

#define ASSEMBLE_ARGB(A,R,G,B) (A << 24 | R << 16 | G << 8 | B)
#define ABS(x)                 ((x) > 0 ? (x) : -(x))
#define MIN(x, y)              ((x) > (y) ? (y) : (x))
#define MAX(x, y)              ((x) < (y) ? (y) : (x))
#define ROUND(x)               ((x)>=0?(int32_t)((x)+0.5):(int32_t)((x)-0.5))

#define COPY 1
#define FILL 2

// /////////////////////////////////////////////////////////////////////////////
// Global elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////

char *ArcadeIT_Utility_Get_String_ID
(
  uint32_t pId,   // The id of the string.
  char *pBuffer   // The pointer to the buffer.
);
// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Skip_Blank_Chars
(
  char *pString  // The pointer to the string.
);
// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Skip_Line
(
  char *pString  // The pointer to the string.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_Goto_Char
(
  char *pString,   // The pointer to the string.
  char pCharacter  // The character of the string to point to.
);
// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Get_Token
(
  char **pLineTokenPtr,   // The pointer to the string.
  char pNextDivider       // The pointer to the extracted token.
);
// -----------------------------------------------------------------------------
char ArcadeIT_Utility_Character_At
(
  char *pString,      // The pointer to the string.
  uint32_t pCounter   // The position counter of the character.
);
// -----------------------------------------------------------------------------
uint8_t ArcadeIT_Utility_String_Match
(
  char *pFirst,   // The pFirst string.
  char *pSecond   // The pSecond string.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_String_Match_Rename
(
  char *pFirst,    // The pFirst string.
  char *pSecond,   // The pSecond string.
  char *pThird     // The new name that results from
);
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_Uppercase
(
  char *pString   // The pointer to the string.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_DMA_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_DMA_Copy
(
  void *pSource,        // The pointer to the source buffer.
  void *pDestination,   // The pointer to the destination buffer.
  uint32_t pSize        // The size of the memory to copy.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Memory_FillCopy
(
    uint8_t *pBufferTo,    // The buffer where to copy into.
    uint8_t *pBufferFrom,  // The buffer where to copy from.
    uint32_t pSize,          
    uint8_t pValue,          
    uint8_t pMode         
);
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_Show_Bytes
(
  uint8_t pId,
  void *pMemoryPointer,   // The pointer to any memory location to start showing the bytes from.
  uint32_t pNumberOfBytes // The amount of bytes to show.
);
// -----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif //__ARCADEIT_UTILITIES_H
