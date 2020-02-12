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
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! devices.
#include <Devices/ArcadeIT_TestPads.h>
#include <Devices/ArcadeIT_Serial_Port.h>

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
  *              the test pads. Output SYSCLK/4 clock on pin PC9
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  uint32_t lPinPosition = (SYS_TESTPADS_PIN_NO * 2);

  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;

  // Configure pin PC9 in alternate function 0 (MCO2)
  SYS_TESTPADS_PER->MODER   &= ~GPIO_MODER_MODER9;
  SYS_TESTPADS_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);

  // Maximum frequency allowed is 100MHz, so keep it in mind when you want
  // to test 180MHz, you have to set the divider at least 2
  SYS_TESTPADS_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR9;
  SYS_TESTPADS_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);

  // type output
  SYS_TESTPADS_PER->OTYPER  &= ~GPIO_OTYPER_OT_9;
  SYS_TESTPADS_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_TESTPADS_PIN_NO);

  // pull up configuration
  SYS_TESTPADS_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR9;
  SYS_TESTPADS_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);

  // Shows a message to serial port as debug
  ArcadeIT_TestPad_Set(pFrequencySystem, pFrequencyDivider);

  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    // Starts and configure the serial port.
    ArcadeIT_Serial_Port_String_Send(TEXT_TEST_PADS_INITED);

  } // End if.

} // End ArcadeIT_TestPads_Init.

// /////////////////////////////////////////////////////////////////////////////

/*

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

 From: RM0386, Reference manual, page 150

 6.3.3 RCC clock configuration register (RCC_CFGR)

 We need to configure the MCO2 function to use a particular clock source and the
 prescaler that we want to divide the frequency by. In our case we set these two
 parameters with a specific function.

 source is configured by setting bits 31..30 of the register, whie the prescaler
 is set by setting bits 29..27.

 -

 From: RM0386, Reference manual, page 165

 6.3.10 RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)

 We first need to enable the clock of the GPIO port C by enabling the corresponding
 flag in register RCC_AHB1ENR: Bit 2 is called GPIOCEN (GPIOC clock enable).
 Then we can configure the GPIOC pin 9.

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

 -

 From: RM0386, Datasheet, page 76, Table 12. Alternate function

 MCO2 function is applied to pin PC9 using alternate function 0

 -

 From: RM0386, Reference manual, page 211

 7.4.10 GPIO alternate function high register (GPIOx_AFRH)

 Since we have sixteen different alternate functions, 4 bits are needed for each
 pin to be configured. So two registers are used for pins 7..0 and pins 15..8,
 respectvely registers GPIOx_AFRL and GPIOx_AFRH.

  For MCO2 we use pin 9, so the high register GPIOx_AFRH is used.

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 |      PIN 15       |      PIN 14       |      PIN 13       |      PIN 12       |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |    AFRH15[3:0]    |    AFRH14[3:0]    |    AFRH13[3:0]    |    AFRH12[3:0]    |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

                                             alternate fn 0
 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------#####################---------+---------+
 |      PIN 11       |      PIN 10       #      PIN 09       #      PIN 08       |
 +----+----+----+----+----+----+----+----#----+----+----+----#----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 # 07 | 06 | 05 | 04 # 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----#----+----+----+----#----+----+----+----+
 |    AFRH11[3:0]    |    AFRH10[3:0]    #    AFRH09[3:0]    #    AFRH08[3:0]    |
 +----+----+----+----+----+----+----+----#----+----+----+----#----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw | rw | rw # rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----#####################----+----+----+----+

 */
