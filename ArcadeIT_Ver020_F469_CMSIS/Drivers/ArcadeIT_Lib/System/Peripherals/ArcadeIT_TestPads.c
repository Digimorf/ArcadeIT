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
 * @file    ArcadeIT_TestPads.c
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

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// ArcadeIT! Peripherals and buses.
#include "System/Peripherals/ArcadeIT_TestPads.h"
//#include "System/Peripherals/ArcadeIT_Serial_Port.h"

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_TestPad_Set
(
    uint32_t pFrequencySystem,  // clock source output to testpad
    uint32_t pFrequencyDivider  // the frequency divider
)
{
  /*
  * DESCRIPTION: This function is used to set the frequency of the test pad.
  * PARAMETERS:  pFrequencySystem    clock source output to testpad
  *              pFrequencyDivider   the frequency divider
  * RETURNS:     Nothing.
  *
  * In ArcadeIT_Firmware.h
  *
  * Source
  *  RCC_MCO2Source_SYSCLK
  *  RCC_MCO2Source_PLLI2SCLK
  *  RCC_MCO2Source_HSE
  *  RCC_MCO2Source_PLLCLK
  *
  * Divider
  *  RCC_MCO2Div_1
  *  RCC_MCO2Div_2
  *  RCC_MCO2Div_3
  *  RCC_MCO2Div_4
  *  RCC_MCO2Div_5
  *
  */

  uint32_t lTmpReg = RCC->CFGR;
  lTmpReg  &= CFGR_MCO2_RESET_MASK;                 // Clear MCO2 and MCO2PRE[2:0] bits
  lTmpReg  |= pFrequencySystem | pFrequencyDivider; // Select MCO2 clock source and prescaler
  RCC->CFGR = lTmpReg;

} // End ArcadeIT_TestPad_Set.

// -----------------------------------------------------------------------------
void ArcadeIT_TestPad_Init
(
    uint32_t pFrequencySystem,  // What frequency source to test.
    uint32_t pFrequencyDivider  // What divider to test.
)
{
  /*
  * DESCRIPTION: This function is used to initialize the hardware that drives
  *              the test pads. Output SYSCLK/4 clock on MCO2 pin(PC9)
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  uint32_t lPinPosition = (SYS_TESTPADS_PIN_NO * 2);

  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;

  // Configure MCO2 pin(PC9) in alternate function 0
  SYS_TESTPADS_PER->MODER   &= ~(GPIO_MODER_MODER0 << lPinPosition);
  SYS_TESTPADS_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);

  // Maximum frequency allowed is 100MHz, so keep it in mind when you want
  // to test 180MHz, you have to set the divider at least 2
  SYS_TESTPADS_PER->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << lPinPosition);
  SYS_TESTPADS_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);

  // type output
  SYS_TESTPADS_PER->OTYPER  &= ~(GPIO_OTYPER_OT_0 << SYS_TESTPADS_PIN_NO);
  SYS_TESTPADS_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_TESTPADS_PIN_NO);

  // pull up configuration
  SYS_TESTPADS_PER->PUPDR   &= ~(GPIO_PUPDR_PUPDR0 << lPinPosition);
  SYS_TESTPADS_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);

  ArcadeIT_TestPad_Set(pFrequencySystem, pFrequencyDivider);

  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    // Starts and configure the serial port.
    //ArcadeIT_Serial_Port_String_Send(TEXT_TEST_PADS_INITED);

  } // End if.

} // End ArcadeIT_TestPads_Init.

// /////////////////////////////////////////////////////////////////////////////
