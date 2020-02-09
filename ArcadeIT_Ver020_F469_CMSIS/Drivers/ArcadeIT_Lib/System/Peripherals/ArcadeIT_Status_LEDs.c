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

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// ArcadeIT! Peripherals and buses.
#include "System/Peripherals/ArcadeIT_Status_LEDs.h"
//#include "System/Peripherals/ArcadeIT_Serial_Port.h"

// //////////////////////////////////////////////////////////////////////////////
// Functions.
// //////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Status_LEDS_Init(void)
{
  /*
  * DESCRIPTION: This function is used to initialize the hardware that drives
  *              the GPIOs connected to the two status LEDs.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  *
  * From: RM0386, Reference manual, page 206-207
  *
  * STATUS LEDs PH2, PH3
  *
  * These registers are 32 bit wide, values for each pin go from 0 to 3
  *  GPIO port mode register (GPIOx_MODER) (x = A to K)
  *  GPIO port speed register (GPIOx_OSPEEDR) (x = A to K)
  *  GPIO port pull-up register (GPIOx_PUPDR) (x = A to K)
  *
  * |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
  * +---------+---------+---------+---------+---------+---------+---------+---------+
  * | PIN 15  | PIN 14  | PIN 13  | PIN 12  | PIN 11  | PIN 10  | PIN 09  | PIN 08  |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  * | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  * | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   |
  * | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR |
  * | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    |
  * |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  * | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  *
  * |  0    0    0    0 .  0    0    0    0 |  1    1    1    1 .  0    0    0    0 |
  * +---------+---------+---------+---------#####################---------+---------+
  * | PIN 07  | PIN 06  | PIN 05  | PIN 04  # PIN 03  # PIN 02  # PIN 01  | PIN 00  |
  * +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
  * | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 # 07 | 06 # 05 | 04 # 03 | 02 | 01 | 00 |
  * +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
  * | MODER   | MODER   | MODER   | MODER   # MODER   # MODER   # MODER   | MODER   |
  * | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR # OSPEEDR # OSPEEDR # OSPEEDR | OSPEEDR |
  * | PUDR    | PUDR    | PUDR    | PUDR    # PUDR    # PUDR    # PUDR    | PUDR    |
  * |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  #  [1:0]  #  [1:0]  #  [1:0]  |  [1:0]  |
  * +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
  * | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw # rw | rw # rw | rw | rw | rw |
  * +----+----+----+----+----+----+----+----#####################----+----+----+----+
  *
  * This register is 32 bit wide but only 16 are used since values for each pin
  * require only one bit.
  *  GPIO port type register (GPIOx_OTYPER) (x = A to K)
  *
  * |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
  * +---------+---------+---------+---------+---------+---------+---------+---------+
  * |         |         |         |         |         |         |         |         |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  * | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  * | OTYPER  | OTYPER  | OTYPER  | OTYPER  | OTYPER  | OTYPER  | OTYPER  | OTYPER  |
  * |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  * |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
  * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  *
  * |  0    0    0    0 .  0    0    0    0 |  1    1    1    1 .  0    0    0    0 |
  * +----+----+----+----+----+----+----+----#####################----+----+----+----+
  * |P15 |P14 |P13 |P12 |P11 |P10 |P09 |P08 #P07 |P06 #P05 |P04 #P03 |P02 |P01 |P00 |
  * +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
  * | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 # 07 | 06 # 05 | 04 # 03 | 02 | 01 | 00 |
  * +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
  * | OTYPER  | OTYPER  | OTYPER  | OTYPER  # OTYPER  # OTYPER  # OTYPER  | OTYPER  |
  * +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
  * | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw # rw | rw # rw | rw | rw | rw |
  * +----+----+----+----+----+----+----+----#####################----+----+----+----+
  *
 */

  uint32_t lPinPosition = 0;

  // Turn on the MCU peripheral by enabling its Clock.
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOH) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOH;

  // Configure the output lines connected to the Status LEDs as output, LOW
  // level as default.
  // -----------------------
  lPinPosition = (SYS_STATUS_LED1_PIN_NO * 2); // the position is a multiple of 2

  // GPIO pin(PH2) in output
  SYS_STATUS_LED1_PER->MODER   &= ~GPIO_MODER_MODER2;
  SYS_STATUS_LED1_PER->MODER   |= (((uint32_t)GPIO_Mode_OUT) << lPinPosition);

  // Maximum frequency allowed is 100MHz, so keep it in mind when you want
  // to test 180MHz, you have to set the divider at least 2
  SYS_STATUS_LED1_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;
  SYS_STATUS_LED1_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);

  // type output
  SYS_STATUS_LED1_PER->OTYPER  &= ~GPIO_OTYPER_OT_2;
  SYS_STATUS_LED1_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_STATUS_LED1_PIN_NO);

  // pull up configuration
  SYS_STATUS_LED1_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR2;
  SYS_STATUS_LED1_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_DOWN) << lPinPosition);

  // -----------------------
  lPinPosition = (SYS_STATUS_LED2_PIN_NO * 2); // the position is a multiple of 2

  // GPIO pin(PH3) in output
  SYS_STATUS_LED2_PER->MODER   &= ~GPIO_MODER_MODER3;
  SYS_STATUS_LED2_PER->MODER   |= (((uint32_t)GPIO_Mode_OUT) << lPinPosition);

  // Maximum frequency allowed is 100MHz, so keep it in mind when you want
  // to test 180MHz, you have to set the divider at least 2
  SYS_STATUS_LED2_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;
  SYS_STATUS_LED2_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);

  // type output
  SYS_STATUS_LED2_PER->OTYPER  &= ~GPIO_OTYPER_OT_3;
  SYS_STATUS_LED2_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_STATUS_LED2_PIN_NO);

  // pull up configuration
  SYS_STATUS_LED2_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR3;
  SYS_STATUS_LED2_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_DOWN) << lPinPosition);

  // -----------------------
  // Put LEDs line LOW.
  SYS_STATUS_LED1_PER->ODR &= ~(SYS_STATUS_LED1_PIN);
  SYS_STATUS_LED2_PER->ODR &= ~(SYS_STATUS_LED2_PIN);

  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    //ArcadeIT_Serial_Port_String_Send(TEXT_STATUS_LEDS_INITED);

  } // End if.

} // End ArcadeIT_Status_LEDS_Init.

// //////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Status_LEDS_Set
(
  void *pParameters   // The pointer to the array of parameters.
)
{
  /*
  * DESCRIPTION: This function is used to set the value of the status LEDs.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Put LEDs line LOW.
  GPIOH->ODR &= ~(SYS_STATUS_LED1_PIN);
  SYS_STATUS_LED2_PER->ODR &= ~(SYS_STATUS_LED2_PIN);

  if (((uint32_t*)pParameters)[PAR_1])
  {
    // Compute the output value according to the parameter provided.
    GPIOH->ODR |= ((uint16_t)(((uint32_t*)pParameters)[PAR_2]) & (uint16_t)1) ? SYS_STATUS_LED1_PIN : (uint16_t)0;
    SYS_STATUS_LED2_PER->ODR |= ((uint16_t)(((uint32_t*)pParameters)[PAR_2]) & (uint16_t)2) ? SYS_STATUS_LED2_PIN : (uint16_t)0;

  } // End if.

} // End ArcadeIT_Status_LEDS_Set.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Status_LED1_Set
(
  void *pParameters   // The pointer to the array of parameters.
)
{
  /*
  * DESCRIPTION: This function is used to set the value of the status LEDs.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  if (((uint32_t*)pParameters)[PAR_1])
  {
    // Set LEDs line LOW.
    GPIOH->ODR &= ~(SYS_STATUS_LED1_PIN);

    // Compute the output value according to the parameter provided.
    GPIOH->ODR |= (uint16_t)(((uint32_t*)pParameters)[PAR_2]) ? SYS_STATUS_LED1_PIN : (uint16_t)0;

  } // End if.

} // End ArcadeIT_Status_LED1_Set.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Status_LED2_Set
(
  void *pParameters   // The pointer to the array of parameters.
)
{
  /*
  * DESCRIPTION: This function is used to set the value of the status LEDs.
  * PARAMETERS:  pValue is the value that the status LEDs should assume.
  * RETURNS:     Nothing.
  */

  if (((uint32_t*)pParameters)[PAR_1])
  {
    // Set LEDs line LOW.
    SYS_STATUS_LED2_PER->ODR &= ~(SYS_STATUS_LED2_PIN);

    // Compute the output value according to the parameter provided.
    SYS_STATUS_LED2_PER->ODR |= (uint16_t)(((uint32_t*)pParameters)[PAR_2]) ? SYS_STATUS_LED2_PIN : (uint16_t)0;

  } // End if..

} // End ArcadeIT_Status_LED2_Set.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Status_LED1_Toggle(void)
{
  /*
  * DESCRIPTION: This function is used to set the value of the status LEDs.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  // Set LEDs line LOW or HIGH depending the current status of the LED.
  if (GPIOH->ODR & SYS_STATUS_LED1_PIN)
  {
    GPIOH->ODR &= ~(SYS_STATUS_LED1_PIN);
  }
  else
  {
    GPIOH->ODR |= SYS_STATUS_LED1_PIN;

  } // End if.

} // End ArcadeIT_Status_LED1_Toggle.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Status_LED2_Toggle(void)
{
  /*
  * DESCRIPTION: This function is used to set the value of the status LEDs.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  // Set LEDs line LOW or HIGH depending the current status of the LED.
  if (SYS_STATUS_LED2_PER->ODR & SYS_STATUS_LED2_PIN)
  {
    SYS_STATUS_LED2_PER->ODR &= ~(SYS_STATUS_LED2_PIN);
  }
  else
  {
    SYS_STATUS_LED2_PER->ODR |= SYS_STATUS_LED2_PIN;

  } // End if.

} // End ArcadeIT_Status_LED2_Toggle.

// /////////////////////////////////////////////////////////////////////////////
