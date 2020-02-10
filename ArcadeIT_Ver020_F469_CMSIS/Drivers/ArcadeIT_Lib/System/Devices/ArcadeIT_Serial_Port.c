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
 * @file    ArcadeIT_Serial_Port.c
 * @version V0.13
 * @date    16-06-2017
 * @last    10-02-2020
 * @brief   This library is used to drive the Serial port at low level. This
 *          port can be used for the debug of the ArcadeIT! system or for data
 *          transfer in the future.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY
 16-06-2017:
 - Creation date of the library structure.

 21-06-2017:
 - Ported the project on Atollic TrueStudio.

 21-09-2017:
 - Cleaned the code from the specific memory location positioning of the functions.
 - Moved functions to different files for better reference.

 15-10-2018:
 - Cleaned code and optimized it for portability.

 ******************************************************************************
 HARDWARE USED

 1.3 - Serial port

 ArcadeIT! Motherboard CN7 - Serial Port

 Pin  GPIO Function    Serial interface
 ---------------------------------------
 3.3V VDD  3.3V        CN7-1
 042  PA2  SYS_SERIAL_PORT TX   CN7-2
 047  PA3  SYS_SERIAL_PORT RX   CN7-3
 GND  VSS  GND         CN7-4

 Serial port  / CN7
 .----.
 | o  |  1 3.3V
 | o |   2 TX
 | o |   3 RX
 | o  |  4 GND
 '----'

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

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////

uint16_t gUSART_TX_Block_Index = 0;
char gUSART_TX_Block[SYS_SERIAL_TX_Block_Size];

uint16_t gUSART_RX_Block_Index = 0;
char gUSART_RX_Block[SYS_SERIAL_RX_Block_Size];

__IO uint8_t gUSART_TX_Transfer = FALSE, gUSART_RX_Transfer = FALSE;

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Serial_Port_Char_Send
(
    char pCharacter   // The character to send to the serial port.
)
{
  /*
   * DESCRIPTION: Sends a character to the serial port.
   * PARAMETERS:  See Above.
   * RETURNS:     Nothing.
   */

  // wait until data register is empty.
  while ((SYS_SERIAL_PORT->SR & SYS_SERIAL_FLAG_TXE) == RESET);

  // send the character.
  SYS_SERIAL_PORT->DR = (pCharacter & (uint16_t)0x01FF);

} // End ArcadeIT_Serial_Port_Char_Send.

// -----------------------------------------------------------------------------
uint8_t ArcadeIT_Serial_Port_Char_Get(void)
{
  /*
   * DESCRIPTION: Retrieve a character from the serial port.
   * PARAMETERS:  See Above.
   * RETURNS:     Nothing.
   */

  char lCharacter = 0;

  // else test for character ready?
  if (SYS_SERIAL_PORT->SR & SYS_SERIAL_FLAG_RXNE)
  {
    // retrieve a character...
    lCharacter = (char)(SYS_SERIAL_PORT->DR & 0x01FF);

    // return character.
    return lCharacter;

  } // End if.
  else
    // no character ready.
    return -1;

 return 0;

} // End ArcadeIT_Serial_Port_Char_Get.

// -----------------------------------------------------------------------------
void ArcadeIT_Serial_Port_String_Send
(
    char *pString   // The string to send character by character to the serial port.
)
{
  /*
   * DESCRIPTION: Prepare data and sends it to the serial port.
   * PARAMETERS:  See Above.
   * RETURNS:     Nothing.
   */

  // while not NULL transmit next character.
  char lCharacter;

  while ((lCharacter = *((char*)pString++)) != 0)
  {
    ArcadeIT_Serial_Port_Char_Send (lCharacter);

  } // End while.

} // End ArcadeIT_Serial_Port_String_Send.

// -----------------------------------------------------------------------------
void ArcadeIT_Serial_Port_Init
(
    uint32_t pBaud   // Baud rate to set the serial port to.
)
{
  /*
   * DESCRIPTION: Enables and configures the Serial port USART of the ArcadeIT!
   *              system.
   * PARAMETERS:  See Above.
   * RETURNS:     Nothing.
   */

  uint32_t lPinPosition = (SYS_SERIAL_TX_PIN_NO * 2);

  // ArcadeIT_Serial_Port_Init
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
  if ((RCC->APB1ENR & RCC_APB1Periph_USART2) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_USART2;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_DMA1) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_DMA1;

  // Configure the pin PA2 as alternate function 7 (USART Tx).
  SYS_SERIAL_TX_PER->MODER   &= ~GPIO_MODER_MODER2;
  SYS_SERIAL_TX_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);

  SYS_SERIAL_TX_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;
  SYS_SERIAL_TX_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_50MHz) << lPinPosition);

  // type output
  SYS_SERIAL_TX_PER->OTYPER  &= ~GPIO_OTYPER_OT_2;
  SYS_SERIAL_TX_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_SERIAL_TX_PIN_NO);

  // pull up configuration
  SYS_SERIAL_TX_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR2;
  SYS_SERIAL_TX_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << lPinPosition);

  // Connect PXx to USARTx_Tx and Rx.
  SYS_SERIAL_TX_PER->AFR[SYS_SERIAL_TX_PIN_NO >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_SERIAL_TX_PIN_NO & (uint32_t)0x07) * 4));
  SYS_SERIAL_TX_PER->AFR[SYS_SERIAL_TX_PIN_NO >> 0x03] =
      SYS_SERIAL_TX_PER->AFR[SYS_SERIAL_TX_PIN_NO >> 0x03]
      | ((uint32_t)(SYS_SERIAL_TX_AF) << ((uint32_t)((uint32_t)SYS_SERIAL_TX_PIN_NO & (uint32_t)0x07) * 4));

  // Configure the pin PA3 as alternate function 7 (USART Rx).
  SYS_SERIAL_RX_PER->MODER   &= ~GPIO_MODER_MODER3;
  SYS_SERIAL_RX_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);

  SYS_SERIAL_RX_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;
  SYS_SERIAL_RX_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_50MHz) << lPinPosition);

  // type output
  SYS_SERIAL_RX_PER->OTYPER  &= ~GPIO_OTYPER_OT_3;
  SYS_SERIAL_RX_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_SERIAL_RX_PIN_NO);

  // pull up configuration
  SYS_SERIAL_RX_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR3;
  SYS_SERIAL_RX_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << lPinPosition);

  // Connect PXx to USARTx_Tx and Rx.
  SYS_SERIAL_RX_PER->AFR[SYS_SERIAL_RX_PIN_NO >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)SYS_SERIAL_RX_PIN_NO & (uint32_t)0x07) * 4)) ;
  SYS_SERIAL_RX_PER->AFR[SYS_SERIAL_RX_PIN_NO >> 0x03] =
      SYS_SERIAL_RX_PER->AFR[SYS_SERIAL_RX_PIN_NO >> 0x03]
      | ((uint32_t)(SYS_SERIAL_RX_AF) << ((uint32_t)((uint32_t)SYS_SERIAL_RX_PIN_NO & (uint32_t)0x07) * 4));

  // When the two pins TX and RX are configured we setup the USART2
  uint32_t tmpreg = 0x00, apbclock = 0x00;
  uint32_t integerdivider = 0x00;
  uint32_t fractionaldivider = 0x00;

  // USART CR2 Configuration
  tmpreg = SYS_SERIAL_PORT->CR2;

  // Clear STOP[13:12] bits
  tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);

  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit :
      Set STOP[13:12] bits according to USART_StopBits value */
  tmpreg |= (uint32_t)SYS_SERIAL_StopBits_1;

  /* Write to USART CR2 */
  SYS_SERIAL_PORT->CR2 = (uint16_t)tmpreg;

/*---------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = SYS_SERIAL_PORT->CR1;

  /* Clear M, PCE, PS, TE and RE bits */
  tmpreg &= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);

  /* Configure the USART Word Length, Parity and mode:
     Set the M bits according to USART_WordLength value
     Set PCE and PS bits according to USART_Parity value
     Set TE and RE bits according to USART_Mode value */
  tmpreg = (uint32_t)(SYS_SERIAL_WordLength_8b
                    | SYS_SERIAL_Parity_No
                    |(SYS_SERIAL_Mode_Rx | SYS_SERIAL_Mode_Tx));

  /* Write to USART CR1 */
  SYS_SERIAL_PORT->CR1 = (uint16_t)tmpreg;

/*---------------------------- USART CR3 Configuration -----------------------*/
  tmpreg = SYS_SERIAL_PORT->CR3;

  /* Clear CTSE and RTSE bits */
  tmpreg &= (uint32_t)~((uint32_t)CR3_CLEAR_MASK);

  /* Configure the USART HFC :
      Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
  tmpreg |= SYS_SERIAL_HardwareFlowControl_None;

  /* Write to USART CR3 */
  SYS_SERIAL_PORT->CR3 = (uint16_t)tmpreg;

/*---------------------------- USART BRR Configuration -----------------------*/
  /* Configure the USART Baud Rate */
  apbclock = ((((HSE_VALUE / (RCC->PLLCFGR & RCC_PLLCFGR_PLLM)) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6))/((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2)) >> (APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> 4])) >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> 10];

  /* Determine the integer part */
  if ((SYS_SERIAL_PORT->CR1 & USART_CR1_OVER8) != 0)
  {
    /* Integer part computing in case Oversampling mode is 8 Samples */
    integerdivider = ((25 * apbclock) / (2 * pBaud));
  }
  else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
  {
    /* Integer part computing in case Oversampling mode is 16 Samples */
    integerdivider = ((25 * apbclock) / (4 * pBaud));
  }
  tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
  fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
  if ((SYS_SERIAL_PORT->CR1 & USART_CR1_OVER8) != 0)
  {
    tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  }
  else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
  {
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  }

  /* Write to USART BRR register */
  SYS_SERIAL_PORT->BRR = (uint16_t)tmpreg;

  SYS_SERIAL_PORT->CR1 |= USART_CR1_UE;

  // Welcome
  ArcadeIT_Serial_Port_String_Send(ERASE_SCREEN);
  ArcadeIT_Serial_Port_String_Send(CURSOR_HOME);
  ArcadeIT_Serial_Port_String_Send(TEXT_ARCADEIT_INIT);
  ArcadeIT_Serial_Port_String_Send(TEXT_SERIAL_PORT_INITED);

} // End ArcadeIT_Serial_Port_Init.

// /////////////////////////////////////////////////////////////////////////////
