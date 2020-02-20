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
 * @file    ArcadeIT_SPI_Port.h
 * @version V0.13
 * @date    26-07-2017
 * @last    20-02-2020
 * @brief   This library is used to drive the SPI port at low level.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

  1.2 - SD Card

  ArcadeIT! Motherboard CN10 - PortB - SPI

  Pin  GPIO Function                     SD Card interface
  ---------------------------------------------------------------------
  3.3V VDD  SD 3.3V                      CN10-1
  GND  VSS  SD GND                       CN10-2
  051  PA5  SD CLK    (SPI1_SCK)         CN10-3
  -    -    -                            CN10-4
  053  PA7  SD MOSI   (SPI1_MOSI)        CN10-5
  052  PA6  SD MISO   (SPI1_MISO)        CN10-6
  055  PC5  SD DETECT (GPIO SD Inserted) CN10-7
  054  PC4  SD CS     (GPIO Chip Select) CN10-8


  SPI port, SD Card / CN10
               .--------.
   2 GND       |  o  o  |   1 3.3V
   4 NC        |  o  o |    3 SPI1_SCK
   6 SPI1_MISO |  o  o |    5 SPI1_MOSI
   8 CS        |  o  o  |   7 SD DETECT
               '--------'

 *******************************************************************************
 HISTORY
 26-07-2017:
 - Creation date of the library structure.

 21-09-2017:
 - Cleaned the code from the specific memory location positioning of the
   functions.

 16-10-2018:
 - Cleaned code and optimized it for portability.

 20-02-2020:
 - converted to CMSIS and ported to GitHUB

 ******************************************************************************
*/

// /////////////////////////////////////////////////////////////////////////////
// Includes.
// /////////////////////////////////////////////////////////////////////////////

#ifndef _ARCADEIT_SPI_PORT_H_
#define _ARCADEIT_SPI_PORT_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////
#define SPI_BaudRatePrescaler_2        ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4        ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8        ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16       ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32       ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64       ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128      ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256      ((uint16_t)0x0038)

#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
#define SPI_Mode_Master                 ((uint16_t)0x0104)
#define SPI_DataSize_8b                 ((uint16_t)0x0000)
#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_NSS_Soft                    ((uint16_t)0x0200)
#define SPI_FirstBit_MSB                ((uint16_t)0x0000)
#define SPI_I2S_FLAG_RXNE              ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE               ((uint16_t)0x0002)

#define DMA_Channel_0                  ((uint32_t)0x00000000)
#define DMA_Channel_1                  ((uint32_t)0x02000000)
#define DMA_Channel_2                  ((uint32_t)0x04000000)
#define DMA_Channel_3                  ((uint32_t)0x06000000)
#define DMA_Channel_4                  ((uint32_t)0x08000000)
#define DMA_Channel_5                  ((uint32_t)0x0A000000)
#define DMA_Channel_6                  ((uint32_t)0x0C000000)
#define DMA_Channel_7                  ((uint32_t)0x0E000000)

#define SYS_SD_SPI_TIMEOUT             250
#define NORMAL 0
#define DMA    1
#define SYS_SD_SPI_METHOD              NORMAL//DMA

// SPI Clock = APB2 / pre scaler = 90MHz / prescaler
#define SYS_SD_SPI_HIGH                SPI_BaudRatePrescaler_2
#define SYS_SD_SPI_LOW                 SPI_BaudRatePrescaler_256
#define SYS_SD_SPI_SPEED               SYS_SD_SPI_HIGH

#define SYS_SD_SPI_DMA_PORT            DMA2

#define SYS_SD_SPI_DMA_MOSI_STREAM     DMA2_Stream3 // tx
#define SYS_SD_SPI_DMA_MOSI_STREAM_CH  DMA_Channel_3

#define SYS_SD_SPI_DMA_MISO_STREAM     DMA2_Stream2 // rx
#define SYS_SD_SPI_DMA_MISO_STREAM_CH  DMA_Channel_3

#define SYS_SD_SPI_PORT                SPI1
#define SYS_SD_SPI_AF                  ((uint8_t)0x05)

#define SYS_SD_SPI_SCK_PER             GPIOA
#define SYS_SD_SPI_SCK_PIN             GPIO_Pin_5
#define SYS_SD_SPI_SCK_PIN_NO          5

#define SYS_SD_SPI_MOSI_PER            GPIOA
#define SYS_SD_SPI_MOSI_PIN            GPIO_Pin_7
#define SYS_SD_SPI_MOSI_PIN_NO         7

#define SYS_SD_SPI_MISO_PER            GPIOA
#define SYS_SD_SPI_MISO_PIN            GPIO_Pin_6
#define SYS_SD_SPI_MISO_PIN_NO         6

#define SYS_SD_SPI_CS_PER              GPIOC
#define SYS_SD_SPI_CS_PIN              GPIO_Pin_4
#define SYS_SD_SPI_CS_PIN_NO           4

#define USE_DETECT_PIN                 FALSE

#define SYS_SD_DETECT_EXT_PER          EXTI_PortSourceGPIOC
#define SYS_SD_DETECT_PER              GPIOC
#define SYS_SD_DETECT_PIN              GPIO_Pin_5
#define SYS_SD_DETECT_PIN_NO           5
#define SYS_SD_SPI_PORT_IRQ            EXTI9_5_IRQn
#define SYS_SD_SPI_PORT_IRQHANDLER     EXTI9_5_IRQHandler

// MACROS
#define SYS_SPI_SLOW()                 { SYS_SD_SPI_PORT->CR1 = (SYS_SD_SPI_PORT->CR1 & ~0x38) | SYS_SD_SPI_LOW;   }   /* Set SCLK = PCLK / 256 */
#define SYS_SPI_FAST()                 { SYS_SD_SPI_PORT->CR1 = (SYS_SD_SPI_PORT->CR1 & ~0x38) | SYS_SD_SPI_HIGH;  }   /* Set SCLK = PCLK / 2 */

#define SYS_SPI_CS_LOW()               { SYS_SD_SPI_CS_PER->ODR &= ~SYS_SD_SPI_CS_PIN; }
#define SYS_SPI_CS_HIGH()              { SYS_SD_SPI_CS_PER->ODR |= SYS_SD_SPI_CS_PIN; }

#define TEXT_SPI_PORT_INITED           "SPI port enabled.\n\r"

// /////////////////////////////////////////////////////////////////////////////
// Macros.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////

uint8_t ArcadeIT_SPI_Port_RW_Byte
(
  uint8_t pData // The data byte to send
);
// -----------------------------------------------------------------------------
void ArcadeIT_SPI_Port_Read_Buffer
(
  uint8_t *pBuffer,       // Pointer to data buffer.
  uint32_t pNumberOfBytes // Number of bytes to receive.
);
// -----------------------------------------------------------------------------
void ArcadeIT_SPI_Port_Write_Buffer
(
  uint8_t *pBuffer,       // Pointer to data buffer.
  uint32_t pNumberOfBytes // Number of bytes to send.
);
// -----------------------------------------------------------------------------
int ArcadeIT_SPI_Port_Init (void);

// /////////////////////////////////////////////////////////////////////////////

#endif // _ARCADEIT_SPI_PORT_H_
