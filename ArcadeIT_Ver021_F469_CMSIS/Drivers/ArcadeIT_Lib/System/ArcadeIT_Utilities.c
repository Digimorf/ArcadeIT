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
 * @file    ArcadeIT_Utilities.c
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

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <string.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Global elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
/*
void ArcadeIT_Utility_DMA_Init(void)
{
    NVIC_InitTypeDef         lNVIC_InitStructure;
    DMA_InitTypeDef          lDMA_InitStructure;

    DMA_Cmd(DMA2_Stream0, DISABLE);
    DMA_DeInit(DMA2_Stream0);

    if ((RCC_AHB1ENR & RCC_AHB1Periph_DMA2) == FALSE)
      RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_DMA2, ENABLE);

    DMA_StructInit(&lDMA_InitStructure);

    // Set the parameters to be configured
    lDMA_InitStructure.DMA_Channel = DMA_Channel_7;
    lDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    lDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    lDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    lDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    lDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
    lDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    lDMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    lDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    lDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_3QuartersFull;
    lDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    lDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &lDMA_InitStructure);

    lNVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    lNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_VIDEO_PRIORITY;
    lNVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    lNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&lNVIC_InitStructure);

} // End ArcadeIT_Utility_DMA_Init
*/
// -----------------------------------------------------------------------------
void ArcadeIT_Utility_Uppercase
(
  char *pString   // The pointer to the string.
)
{
  /*
  * DESCRIPTION: This function modify the string provided to upper case.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Temporary character.
  char lTempCharacter = *pString;

  // Go through the characters of the string provided and transform them to upper case.
  while (lTempCharacter)
  {
    // Upper case only alpha lower case.
    if (CHR_IS_LOWCASEALPHA(lTempCharacter))
      *pString = lTempCharacter - 32;

    // Next characters.
    lTempCharacter = *((char*)++pString);

  } // End while.

} // End ArcadeIT_Utility_Upper case.

// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Get_String_ID
(
  uint32_t pId,   // The id of the string.
  char *pBuffer   // The pointer to the buffer.
)
{
  /*
  * DESCRIPTION: This function returns the pointer to the in string found in a buffer.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Temporary character.
  uint32_t lTempCharacter = 0;

  char *msg = pBuffer;

  // Go through the characters of the string provided and transform them to upper case.
  while (lTempCharacter != pId)
  {
    if (*msg++ == '\0') lTempCharacter++;

  } // End while.

  return msg;

} // End ArcadeIT_Utility_Get_String_ID.

// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Skip_Blank_Chars
(
  char *pString  // The pointer to the string.
)
{
  /*
  * DESCRIPTION: Update the pointer of a string to the pFirst non blank character.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Go through the characters of the string provided until a non space character is found.
  while (*pString == ASCII_SPACE)
  {
    pString++;

  } // End while.

  return pString;

} // End ArcadeIT_Utility_Skip_Blank_Chars.

// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Skip_Line
(
  char *pString  // The pointer to the string.
)
{
  /*
  * DESCRIPTION: Update the pointer of a string to the first character after a new line..
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Go through the characters of the string provided until a non space character is found.
  while ((*pString != ASCII_CR) && (*pString != ASCII_LF))
  {
    //ArcadeIT_Terminal_Char_Send(*pString);

    if (*pString == 0)
      break;

    pString++;

  } // End while.

  if (
      ((*pString == ASCII_CR) && (*(pString+1) == ASCII_LF)) ||
      ((*pString == ASCII_LF) && (*(pString+1) == ASCII_CR))
     )
  {
    pString += 2;

  } // End if.

  //ArcadeIT_Terminal_Debug(CURSOR_NEWLINE);

  return pString;

} // End ArcadeIT_Utility_Skip_Line.

// -----------------------------------------------------------------------------
void ArcadeIT_Utility_Goto_Char
(
  char *pString,   // The pointer to the string.
  char pCharacter  // The character of the string to point to.
)
{
  /*
  * DESCRIPTION: update the pointer of a string to the pFirst blank character.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Go through the characters of the string provided until a non space character is found.
  while (*pString++ != pCharacter) {};

} // End ArcadeIT_Utility_Goto_Char.

// -----------------------------------------------------------------------------
char *ArcadeIT_Utility_Get_Token
(
  char **pLineTokenPtr,   // The pointer to the string.
  char pNextDivider       // The pointer to the extracted token.
)
{
  /*
  * DESCRIPTION: Get one subsequent token from a string.
  * PARAMETERS:  See above.
  * RETURNS:     The updated position into the string.
  */

    char *lLineTokenEnd, *lLineTokenStart, lNextChar = 0;

    lLineTokenStart = *pLineTokenPtr;
    lLineTokenEnd = *pLineTokenPtr;

    // Set the string delimiter.
    if (*lLineTokenStart == '"')
    {
        // Set the next break character for the token.
        lNextChar = '"';

        // Update the start point.
        lLineTokenStart++;
        lLineTokenEnd = lLineTokenStart;
    }
    else if (*lLineTokenStart == 0)
    {
      *pLineTokenPtr = 0;

      return lLineTokenStart;
    }
    else
    {
        lNextChar = pNextDivider;

    } // End if.

    // Extract token.
    while (*lLineTokenEnd != lNextChar && *lLineTokenEnd != 0 && *lLineTokenEnd != ASCII_CR && *lLineTokenEnd != ASCII_LF)
    {
      lLineTokenEnd++;

    } // End while.

    // Align to the next token start.
    switch(*lLineTokenEnd)
    {
      case ASCII_CR:
      case ASCII_LF:
        *lLineTokenEnd = 0;
      break;

      case 0:
        *pLineTokenPtr = 0;

        return lLineTokenStart;
      break;

      case '"':
        if (lNextChar == '"')
        {
          /* "text string"token...     -> ..tring\x00token
           * "text string" token...    -> ..tring\x00\x20token
           * "text string"    token... -> ..tring\x00\x20...\x20token
           */
            lLineTokenEnd++;
            *lLineTokenEnd = 0;

            // Skip eventual blank spaces after the token.
            lLineTokenEnd++;
            if (*lLineTokenEnd == ASCII_SPACE)
              while (*lLineTokenEnd++ != ASCII_SPACE) {};

        } // End if.
      break;

      default:
        if ((*lLineTokenEnd == pNextDivider) && (lNextChar == pNextDivider))
        {
            *lLineTokenEnd = 0;

            // Skip eventual blank spaces after the token.
            while (*lLineTokenEnd++ == ASCII_SPACE) {};

        } // End if.
      break;

    } // End switch.

    *pLineTokenPtr = lLineTokenEnd;

    return lLineTokenStart;

} // End ArcadeIT_Utility_Get_Token.

// -----------------------------------------------------------------------------
char ArcadeIT_Utility_Character_At
(
  char *pString,      // The pointer to the string.
  uint32_t pCounter   // The position counter of the character.
)
{
  /*
  * DESCRIPTION: Returns a character from a string at a given position.
  *              The position is always calculated wrapping around the length of
  *              the string.
  * PARAMETERS:  See above.
  * RETURNS:     the character.
  */

  return (pString[pCounter % strlen(pString)]);

} // End ArcadeIT_Utility_Animation_Charfacters.

// -----------------------------------------------------------------------------
uint8_t ArcadeIT_Utility_String_Match
(
  char *pFirst,   // The pFirst string.
  char *pSecond   // The pSecond string.
)
{
  /*
  * DESCRIPTION: Checks if two given strings match. The pFirst string may contain wildcard characters
  * PARAMETERS:  See above.
  * RETURNS:     TRUE/FALSE.
  */

  // If we reach at the end of both strings, we are done
  if (*pFirst == '\0' && *pSecond == '\0')
    return TRUE;

  // Make sure that the characters after '*' are present
  // in pSecond string. This function assumes that the pFirst
  // string will not contain two consecutive '*'
  if (*pFirst == '*' && *(pFirst + 1) != '\0' && *pSecond == '\0')
    return FALSE;

  // If the pFirst string contains '?', or current characters
  // of both strings match
  if (*pFirst == '?' || toupper(*pFirst) == toupper(*pSecond))
    return ArcadeIT_Utility_String_Match(pFirst + 1, pSecond + 1);

  // If there is *, then there are two possibilities
  // a) We consider current character of pSecond string
  // b) We ignore current character of pSecond string.
  if (*pFirst == '*')
    return ArcadeIT_Utility_String_Match(pFirst + 1, pSecond) || ArcadeIT_Utility_String_Match(pFirst, pSecond + 1);

  return FALSE;

} // End ArcadeIT_Utility_String_Match.

// -----------------------------------------------------------------------------
void ArcadeIT_Utility_String_Match_Rename
(
  char *pFirst,    // The pFirst string.
  char *pSecond,   // The pSecond string.
  char *pThird     // The new name that results from
)
{
  /*
  * DESCRIPTION: Matches two strings, one of those may contain wildcards, and
  *              fill a third string with the matched string.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  while (*pFirst != '\0' && *pSecond != '\0')
  {
    switch (*pFirst)
  {
    case '*':
      *pThird = *pSecond++;
      if (*pSecond == '.') pFirst++;
      break;

    case '?':
      *pThird = *pSecond++;
      pFirst++;
      break;

    default:
      *pThird = *pFirst++;
      pSecond++;
      break;

  } // End switch.

    pThird++;

  } // End while.

  *pThird = 0;

} // End ArcadeIT_Utility_String_Match_Rename.

// -----------------------------------------------------------------------------
void ArcadeIT_Memory_FillCopy
(
    uint8_t *pBufferTo,    // The buffer where to copy into.
    uint8_t *pBufferFrom,  // The buffer where to copy from.
    uint32_t pSize,          
    uint8_t pValue,          
    uint8_t pMode         
)
{
  /*
  * DESCRIPTION: Copy or Fill the content of a block of memory.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing
  */

  uint32_t lData = pSize;

  if (pMode == COPY)
  {
    while (lData)
    {
      *pBufferTo++ = *pBufferFrom++;

      lData--;

    } // End while.
  }
  else if (pMode == FILL)
  {
    while (lData)
    {
      *pBufferTo++ = pValue;

      lData--;

    } // End while.
    
  } // End if.

} // End ArcadeIT_Memory_Copy

// -----------------------------------------------------------------------------
void ArcadeIT_Utility_Show_Bytes
(
  uint8_t pId,
  void *pMemoryPointer,   // The pointer to any memory location to start showing the bytes from.
  uint32_t pNumberOfBytes // The amount of bytes to show.
)
{
  /*
  * DESCRIPTION: This function is used to fill the memory with the provided byte.
  *              of the memory and change its size.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  int32_t lBytes = pNumberOfBytes, lByteIndex = 0;
  uint8_t *lAddress = (uint8_t *)pMemoryPointer;
  char lString[ARCADEIT_STRING_LENGTH_MAX];

  sprintf((char*)lString, "\n\rShowing %lu bytes from %p:\n\r", pNumberOfBytes, pMemoryPointer);
  //ArcadeIT_Terminal_Debug(pId, lString);
  ArcadeIT_Serial_Port_String_Send(lString);

  // Read all lines and display it.
  while (lBytes > 0)
  {
    sprintf((char*)lString, "%p: ", lAddress);
    //ArcadeIT_Terminal_Debug(pId, lString);
    ArcadeIT_Serial_Port_String_Send(lString);

    // Print hex values.
    for (lByteIndex = 0; lByteIndex < 8; lByteIndex++)
    {
      sprintf((char*)lString, "%02X ", lAddress[lByteIndex]);
      //ArcadeIT_Terminal_Debug(pId, lString);
      ArcadeIT_Serial_Port_String_Send(lString);

      lBytes--;

    } // End for.

    //ArcadeIT_Terminal_Debug(pId, " | ");
    ArcadeIT_Serial_Port_String_Send(" | ");

    for (lByteIndex = 0; lByteIndex < 8; lByteIndex++)
    {
      sprintf((char*)lString, "%c", (lAddress[lByteIndex] >= ASCII_SPACE && lAddress[lByteIndex] < ASCII_DELETE) ? lAddress[lByteIndex] : '.');
      //ArcadeIT_Terminal_Debug(pId, lString);
      ArcadeIT_Serial_Port_String_Send(lString);

    } // End for.

    //ArcadeIT_Terminal_Debug(pId, lString);
    ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);

    lAddress += 8;

  } // End while.

} // ArcadeIT_Utility_Show_Bytes.

// /////////////////////////////////////////////////////////////////////////////
