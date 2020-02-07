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
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_TestPads.h
 * @version V0.13
 * @date    17-07-2018
 * @last    26-07-2020
 * @brief   This library is used to output the MCU clock frequency out of the
 *          test pads for debug purpose.
 *
 ******************************************************************************
 * @attention
 * <h2><center>&copy; COPYRIGHT 2020 Digimorf</center></h2>
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.14 - Test pads

 Pin  GPIO      Component     Function
 ---------------------------------------------
 122  PC9 GPIO, MCO2          clock source to output pin 2

 <TESTPAD1]----------O

 ******************************************************************************
 HISTORY
 17-07-2018:
 - Creation date of the library structure.

 06-02-2020:
 - Converted to pure CMSIS.

 ******************************************************************************
*/

#ifndef _ARCADEIT_TESTPADS_H_
#define _ARCADEIT_TESTPADS_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "../ArcadeIT_Common.h"
#include "../ArcadeIT_Utilities.h"
#include "../ArcadeIT_Firmware.h"

// /////////////////////////////////////////////////////////////////////////////
// Defines.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_TestPads_Init(void);
//------------------------------------------------------------------------------
void ArcadeIT_TestPad_DeInit(void);
//------------------------------------------------------------------------------
void ArcadeIT_TestPad_Frequency
(
  uint32_t pFrequencySystem,  // What frequency source to test.
  uint32_t pFrequencyDivider  // What divider to test.
);
// /////////////////////////////////////////////////////////////////////////////

#endif // __ARCADEIT_TESTPADS_H_
