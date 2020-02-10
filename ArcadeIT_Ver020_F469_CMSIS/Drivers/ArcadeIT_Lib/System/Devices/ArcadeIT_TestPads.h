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

 These registers are 32 bit wide, values for each pin go from 0 to 3
  GPIO port mode register (GPIOx_MODER) (x = A to K)
  GPIO port speed register (GPIOx_OSPEEDR) (x = A to K)
  GPIO port pull-up register (GPIOx_PUPDR) (x = A to K)

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------###########---------+
 | PIN 15  | PIN 14  | PIN 13  | PIN 12  | PIN 11  | PIN 10  # PIN 09  # PIN 08  |
 +----+----+----+----+----+----+----+----+----+----+----+----#----+----#----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 # 19 | 18 # 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----#----+----#----+----+
 | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   # MODER   # MODER   |
 | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR # OSPEEDR # OSPEEDR |
 | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    # PUDR    # PUDR    |
 |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  #  [1:0]  #  [1:0]  |
 +----+----+----+----+----+----+----+----+----+----+----+----#----+----#----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw # rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----###########----+----+

 |  0    0    0    0 .  0    0    0    0 |  1    1    1    1 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 | PIN 07  | PIN 06  | PIN 05  | PIN 04  | PIN 03  | PIN 02  | PIN 01  | PIN 00  |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   |
 | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR |
 | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    |
 |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 This register is 32 bit wide but only 16 are used since values for each pin
 require only one bit.
  GPIO port type register (GPIOx_OTYPER) (x = A to K)

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 |         |         |         |         |         |         |         |         |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 |  0    0    0    0 .  0    0    1    0 |  0    0    0    0 .  0    0    0    0 |
 +----+----+----+----+----+----######----+----+----+----+----+----+----+----+----+
 |P15 |P14 |P13 |P12 |P11 |P10 #P09 #P08 |P07 |P06 |P05 |P04 |P03 |P02 |P01 |P00 |
 +----+----+----+----+----+----#----#----+----+----+----+----+----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 # 09 # 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----#----#----+----+----+----+----+----+----+----+----+
 | OT | OT | OT | OT | OT | OT # OT # OT | OT | OT | OT | OT | OT | OT | OT | OT |
 +----+----+----+----+----+----#----#----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw # rw # rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----######----+----+----+----+----+----+----+----+----+

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
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// /////////////////////////////////////////////////////////////////////////////
// Defines.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_TestPad_Init
(
    uint32_t pFrequencySystem,  // What frequency source to test.
    uint32_t pFrequencyDivider  // What divider to test.
);
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
