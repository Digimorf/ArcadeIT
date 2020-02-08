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
 *                       P E R I P H E R A L S
 * -----------------------------------------------------------------
 *           (C)2018 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_Status-LEDs.h
 * @version V0.13
 * @date    15-06-2017
 * @last    08-02-2020
 * @brief   This library is used to drive two status LEDs for any visual use.
 *          Nothing special to say about this, just two simple output signals
 *          assigned to two GPIOs.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.13 - Status LEDs

 Pin  GPIO      Component     Function
 ---------------------------------------------
 045  PH4 GPIO, Output LED 2, Status LED 1
 046  PH5 GPIO, Output LED 3, Status LED 2

 ******************************************************************************
 HISTORY

 15-06-2017:
 - Creation date of the library structure.
 - Created basic functions for the Status LEDs initialization and value
 set. The LEDs could be used "visually" separately or together by
 sending the proper value:

       | value
       | 0 1 2 3
 ---------------
 LED 2 | 0 0 1 1
 LED 1 | 0 1 0 1

 16-06-2017:
 - Added two separated function to drive the LEDS independently.

 21-06-2017:
 - Ported the project on Atollic TrueStudio.

 21-09-2017:
 - Cleaned the code from the specific memory location positioning of the
   functions.
 - Ported the project on STM32CubeIDE

 16-10-2018:
 - Cleaned code and optimized it for portability.

 08-02-2020:
 - Converted to pure CMSIS.
 - Moved on GitHub

 ******************************************************************************
 */

// /////////////////////////////////////////////////////////////////////////////
// Includes.
// /////////////////////////////////////////////////////////////////////////////

#ifndef _ARCADEIT_STATUS_LEDS_H_
#define _ARCADEIT_STATUS_LEDS_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "../ArcadeIT_Common.h"
#include "../ArcadeIT_Utilities.h"
#include "../ArcadeIT_Firmware.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////

void ArcadeIT_Status_LEDS_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Status_LEDS_Set
(
  void *pParameters   // The pointer to the array of parameters.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Status_LED1_Set
(
  void *pParameters   // The pointer to the array of parameters.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Status_LED2_Set
(
  void *pParameters   // The pointer to the array of parameters.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Status_LED1_Toggle(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Status_LED2_Toggle(void);

// /////////////////////////////////////////////////////////////////////////////

#endif // _ARCADEIT_STATUS_LEDS_H_
