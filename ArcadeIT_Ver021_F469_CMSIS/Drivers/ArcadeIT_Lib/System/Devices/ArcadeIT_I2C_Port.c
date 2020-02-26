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
 * @file    ArcadeIT_I2C_Port.h
 * @version V0.13
 * @date    16-02-2020
 * @last    20-02-2020
 * @brief   This library is used to drive the I2C port at low level.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.3 - I2C port

  ArcadeIT! Motherboard CN15 - I2C Port

  Pin  GPIO Function    I2C interface
  ---------------------------------------
  3.3V VDD  3.3V        CN15-1
  046  PH5  SDA         CN15-2
  045  PH4  SCL         CN15-3
  GND  VSS  GND         CN15-4

 ArcadeIT! mainboard:

  I2C port / CN15
  .---.
  | o |  1 3.3V
  | o |  2 SDA
  | o |  3 SCL
  | o |  4 GND
  '---'

 ******************************************************************************
 HISTORY
 16-02-2020:
 - Creation date of the library structure.

 *******************************************************************************
 */

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_I2C_Port.h>
#include <System/Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
int ArcadeIT_I2C_CheckEvent(
    uint32_t pEvent // the event flags
    )
{
  /*
   * DESCRIPTION: This function reads the two status registers of the I2C peripheral
   *              and checks the event.
   * PARAMETERS:  See above.
   * RETURNS:     Nothing.
   */

  uint32_t lastevent = 0;

  // Get the last event value from I2C status register
  lastevent = (I2C2->SR1 | (I2C2->SR2 << 16)) & FLAG_MASK;

  // Return status
  return (lastevent & pEvent) == pEvent ? 1 : 0;

} // end ArcadeIT_I2C_CheckEvent

// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_WaitEvent(
    uint32_t pEvent // the event flags
    )
{
  /*
   * DESCRIPTION: This function waits until an event happens
   * PARAMETERS:  See above.
   * RETURNS:     Nothing.
   */

  int8_t lResult = 1;

  int32_t lTimeoutTimer = I2C_TIMEOUT_TIME;

  // waits until the event happens on the status registers
  while(ArcadeIT_I2C_CheckEvent(pEvent)==0)
  {
    if (lTimeoutTimer-- < 0)
    {
      lResult = 0;
      break;

    } // end if

  } // end while

  return lResult;

} // end ArcadeIT_I2C_WaitEvent

// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_StartCommunication(void)
{
  /*
   * DESCRIPTION: This function sends a start condition over the I2C bus.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  int8_t lResult = 0;

  while(I2C2->SR2 & I2C_FLAG_BUSY); // wait for bus free

  // Generate a START condition
  I2C2->CR1 |= I2C_CR1_START;

  // Wait until the Master gets the control of the line
  lResult = ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);

  return lResult;

} // end ArcadeIT_I2C_StartCommunication

// -----------------------------------------------------------------------------
void ArcadeIT_I2C_EndCommunication(void)
{
  /*
   * DESCRIPTION: This function sends a stop condition over the I2C bus.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  // Generate a STOP condition
  I2C2->CR1 |= I2C_CR1_STOP;

} // end ArcadeIT_I2C_EndCommunication

// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_Send7bitAddress(
    uint8_t pAddress,   // the address of the I2C device to control
    uint8_t pDirection  // the direction to set for the communication
    )
{
  /*
   * DESCRIPTION: This function waits until an event happens
   * PARAMETERS:  See above.
   * RETURNS:     Nothing.
   */

  int8_t lResult = 1;

  // Test on the direction to set/reset the read/write bit
  if (pDirection != I2C_Direction_Transmitter)
  {
    // Set the address bit0 for read
    pAddress |= I2C_OAR1_ADD0;
  }
  else
  {
    // Reset the address bit0 for write
    pAddress &= (uint8_t)~((uint8_t)I2C_OAR1_ADD0);
  }

  // Send the address
  I2C2->DR = pAddress;

  return lResult;

} // end ArcadeIT_I2C_Send7bitAddress

// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_Send(
    uint8_t pData       // byte data to send
    )
{
  /*
   * DESCRIPTION: This function waits until an event happens
   * PARAMETERS:  See above.
   * RETURNS:     Nothing.
   */

  int8_t lResult = 0;

  I2C2->DR = pData;

  // wait for I2Cx EV8_2, byte has been transmitted
  lResult = ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  return lResult;

} // end ArcadeIT_I2C_Send

// -----------------------------------------------------------------------------
int8_t ArcadeIT_I2C_Receive(
    uint8_t *pData,       // ponter to byte where to store the received data
    uint8_t pAcknowledge  // flag to enable or disable the acknowledge event
    )
{
  /*
   * DESCRIPTION: This function waits until an event happens
   * PARAMETERS:  See above.
   * RETURNS:     Nothing.
   */

  int8_t lResult = 0;

  if(pAcknowledge)
    I2C2->CR1 |= I2C_CR1_ACK;
  else
    I2C2->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK);

  lResult = ArcadeIT_I2C_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED);

  if (lResult)
    *pData = (uint8_t)I2C2->DR;
  else
    *pData = 0;

  return lResult;

} // end ArcadeIT_I2C_Receive

// -----------------------------------------------------------------------------
void ArcadeIT_I2C_Port_Init(
    uint32_t pClock     // The frequency clock of the I2C device
    )
{
  /*
   * DESCRIPTION: Enables and configures the Serial port USART of the ArcadeIT!
   *              system.
   * PARAMETERS:  See Above.
   * RETURNS:     Nothing.
   */

  uint32_t lPinPosition;

  // configure the pins of the I2C
  lPinPosition = (SYS_I2C_SCL_PIN_NO * 2);

  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOH) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOH;
  if ((RCC->APB1ENR & RCC_APB1Periph_I2C2) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_I2C2;

  // Configure the pin PH4 as alternate function 4 (I2C SCL).
  SYS_I2C_SCL_PER->MODER   &= ~GPIO_MODER_MODER4;
  SYS_I2C_SCL_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
  SYS_I2C_SCL_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4;
  SYS_I2C_SCL_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_2MHz) << lPinPosition);
  SYS_I2C_SCL_PER->OTYPER  &= ~GPIO_OTYPER_OT_4;
  SYS_I2C_SCL_PER->OTYPER  |= (uint16_t)(GPIO_OType_OD << SYS_I2C_SCL_PIN_NO);
  SYS_I2C_SCL_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR4;
  SYS_I2C_SCL_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << lPinPosition);
  SYS_I2C_SCL_PER->AFR[0]  &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_I2C_SCL_PIN_NO & (uint32_t)0x07) * 4));
  SYS_I2C_SCL_PER->AFR[0]  |= ((uint32_t)(SYS_I2C_AF) << ((uint32_t)((uint32_t)SYS_I2C_SCL_PIN_NO & (uint32_t)0x07) * 4));

  lPinPosition = (SYS_I2C_SDA_PIN_NO * 2);

  // Configure the pin PH5 as alternate function 4 (I2C SDA).
  SYS_I2C_SDA_PER->MODER   &= ~GPIO_MODER_MODER5;
  SYS_I2C_SDA_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
  SYS_I2C_SDA_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5;
  SYS_I2C_SDA_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_2MHz) << lPinPosition);
  SYS_I2C_SDA_PER->OTYPER  &= ~GPIO_OTYPER_OT_5;
  SYS_I2C_SDA_PER->OTYPER  |= (uint16_t)(GPIO_OType_OD << SYS_I2C_SDA_PIN_NO);
  SYS_I2C_SDA_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR5;
  SYS_I2C_SDA_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << lPinPosition);
  SYS_I2C_SDA_PER->AFR[0] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_I2C_SDA_PIN_NO & (uint32_t)0x07) * 4)) ;
  SYS_I2C_SDA_PER->AFR[0] |= ((uint32_t)(SYS_I2C_AF) << ((uint32_t)((uint32_t)SYS_I2C_SDA_PIN_NO & (uint32_t)0x07) * 4));

  // reset the I2C peripheral
  I2C2->CR1 |= I2C_CR1_SWRST;
  ArcadeIT_System_Delay(1000);
  I2C2->CR1 = 0x0;

  // configure the I2C port
  I2C2->CR1 &= ~I2C_CR1_PE;

  uint32_t apbclock = ((((HSE_VALUE / PLL_M) * PLL_N) / PLL_P) >> 2); // for the ArcadeIT! should be 45MHz
  uint16_t tmpreg = 0, freqrange = 0;
  uint16_t result = 0x04;
  uint8_t lDuty = I2C_DUTY_2;

  freqrange = (apbclock / 1000000);

  I2C2->CR2 &= ~I2C_CR2_FREQ;
  I2C2->CR2 |= freqrange;

  // Configure speed in standard mode
  if (pClock <= 100000)
  {
    // Standard mode speed calculate
    result = (uint16_t)(apbclock / (pClock << 1));

    // Test if CCR value is under 0x4
    if (result < 0x04)
    {
      // Set minimum allowed value
      result = 0x04;

    } // End if.

    // Set speed value for standard mode
    tmpreg |= result;

    // Set Maximum Rise Time for standard mode
    I2C2->TRISE = freqrange + 1;
  }
  else
  {
    if (lDuty == I2C_DUTY_2)
    {
      // Fast mode speed calculate: Tlow/Thigh = 2
      result = (uint16_t)(apbclock / (pClock * 3));
    }
    else
    {
      // Fast mode speed calculate: Tlow/Thigh = 16/9
      result = (uint16_t)(apbclock / (pClock * 25));

      // Set DUTY bit
      result |= I2C_DutyCycle_16_9;

    } // End if.

    // Test if CCR value is under 0x1
    if ((result & I2C_CCR_CCR) == 0)
    {
      // Set minimum allowed value
      result |= (uint16_t)0x0001;

    } // End if.

    // Set speed value and set F/S bit for fast mode
    tmpreg |= (uint16_t)(result | I2C_CCR_FS);

    // Set Maximum Rise Time for fast mode
    I2C2->TRISE = (uint16_t)(((freqrange * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);

  } // End if.

  I2C2->CCR = tmpreg;
  I2C2->OAR1 = I2C_AcknowledgedAddress_7bit;
  I2C2->OAR2 = 0;
  I2C2->CR1 |= I2C_CR1_ACK;
  I2C2->CR1 |= I2C_CR1_PE;

  // Shows a message to serial port as debug
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_I2C_INITED);

  } // End if.

} // end ArcadeIT_I2C_Port_Init

// /////////////////////////////////////////////////////////////////////////////
