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
 - Moved on GitHub

 ******************************************************************************
*/

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "../ArcadeIT_Common.h"
#include "../ArcadeIT_Utilities.h"
#include "../ArcadeIT_Firmware.h"

// ArcadeIT! Peripherals and buses.
#include "ArcadeIT_TestPads.h"
//#include "ArcadeIT_Serial_Port.h"

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_TestPad_Set
(
    uint32_t pFrequencySystem,  // What frequency source to test.
    uint32_t pFrequencyDivider  // What divider to test.
)
{
  /*
  * DESCRIPTION: This function is used to set the frequency of the test pad.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
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

  // Configure MCO2 pin(PC9) in alternate function
  SYS_TESTPADS_PER->MODER   &= ~(GPIO_MODER_MODER0 << lPinPosition);
  SYS_TESTPADS_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);

  SYS_TESTPADS_PER->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << lPinPosition);
  SYS_TESTPADS_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);

  SYS_TESTPADS_PER->OTYPER  &= ~(GPIO_OTYPER_OT_0 << SYS_TESTPADS_PIN_NO);
  SYS_TESTPADS_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_TESTPADS_PIN_NO);

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
