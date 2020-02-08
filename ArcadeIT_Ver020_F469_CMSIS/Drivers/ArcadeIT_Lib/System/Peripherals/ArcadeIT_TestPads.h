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
 * @last    08-02-2020
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


 ArcadeIT! mainboard:

 <TESTPAD1]----------O

 is used to test the STM32F469 microcontroller configuration by measuring the
 different frequencies, HCLK, SYS etc.

 From: RM0386, Reference manual, page 144

 6.2.10 Clock-out capability

  Two microcontroller clock output (MCO) pins are available:
  • MCO1
  • MCO2
    You can output four different clock sources onto the MCO2 pin (PC9) using the
    configurable prescaler (from 1 to 5):
    – HSE clock
    – PLL clock
    – System clock (SYSCLK)
    – PLLI2S clock
    The desired clock source is selected using the MCO2PRE[2:0] and MCO2 bits in the
    RCC clock configuration register (RCC_CFGR).
    For the different MCO pins, the corresponding GPIO port has to be programmed in alternate
    function mode.
    The selected clock to output onto MCO must not exceed 100 MHz (the maximum I/O
    speed).

 From: RM0386, Datasheet, page 76, Table 12. Alternate function

 MCO2 function is applied to pin PC9 using alternate function 0

 ******************************************************************************
 HISTORY

 17-07-2018:
 - Creation date of the library structure.

 06-02-2020:
 - Converted to pure CMSIS.
 - Moved on GitHub

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
    uint32_t pFrequencySystem,  // clock source output to testpad
    uint32_t pFrequencyDivider  // the frequency divider
);
// /////////////////////////////////////////////////////////////////////////////

#endif // __ARCADEIT_TESTPADS_H_
