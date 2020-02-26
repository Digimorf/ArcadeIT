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
 * @last    21-02-2020
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
#define SPI_FREQ_45_00_MHz              ((uint16_t)0x0000) //SPI_BaudRatePrescaler_2
#define SPI_FREQ_22_50_MHz              ((uint16_t)0x0008) //SPI_BaudRatePrescaler_4
#define SPI_FREQ_11_25_MHz              ((uint16_t)0x0010) //SPI_BaudRatePrescaler_8
#define SPI_FREQ_5_62_MHz               ((uint16_t)0x0018) //SPI_BaudRatePrescaler_16
#define SPI_FREQ_2_81_MHz               ((uint16_t)0x0020) //SPI_BaudRatePrescaler_32
#define SPI_FREQ_1_40_MHz               ((uint16_t)0x0028) //SPI_BaudRatePrescaler_64
#define SPI_FREQ_0_70_MHz               ((uint16_t)0x0030) //SPI_BaudRatePrescaler_128
#define SPI_FREQ_0_35_MHz               ((uint16_t)0x0038) //SPI_BaudRatePrescaler_256

#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
#define SPI_Mode_Master                 ((uint16_t)0x0104)
#define SPI_DataSize_8b                 ((uint16_t)0x0000)
#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_NSS_Soft                    ((uint16_t)0x0200)
#define SPI_FirstBit_MSB                ((uint16_t)0x0000)
#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

#define DMA_Channel_0                   ((uint32_t)0x00000000)
#define DMA_Channel_1                   ((uint32_t)0x02000000)
#define DMA_Channel_2                   ((uint32_t)0x04000000)
#define DMA_Channel_3                   ((uint32_t)0x06000000)
#define DMA_Channel_4                   ((uint32_t)0x08000000)
#define DMA_Channel_5                   ((uint32_t)0x0A000000)
#define DMA_Channel_6                   ((uint32_t)0x0C000000)
#define DMA_Channel_7                   ((uint32_t)0x0E000000)

#define SYS_SD_SPI_TIMEOUT              250
#define NORMAL 0
#define DMA    1
#define SYS_SD_SPI_METHOD               NORMAL//DMA

// SPI Clock = APB2 / pre scaler = 90MHz / prescaler
#define SYS_SD_SPI_SPEED                SPI_FREQ_3_12_MHz

#define SYS_SD_SPI_DMA_PORT             DMA2

#define SYS_SD_SPI_DMA_MOSI_STREAM      DMA2_Stream3 // tx
#define SYS_SD_SPI_DMA_MOSI_STREAM_CH   DMA_Channel_3

#define SYS_SD_SPI_DMA_MISO_STREAM      DMA2_Stream2 // rx
#define SYS_SD_SPI_DMA_MISO_STREAM_CH   DMA_Channel_3

#define SYS_SD_SPI_PORT                 SPI1
#define SYS_SD_SPI_AF                   ((uint8_t)0x05)

#define SYS_SD_SPI_SCK_PER              GPIOA
#define SYS_SD_SPI_SCK_PIN              GPIO_Pin_5
#define SYS_SD_SPI_SCK_PIN_NO           5

#define SYS_SD_SPI_MOSI_PER             GPIOA
#define SYS_SD_SPI_MOSI_PIN             GPIO_Pin_7
#define SYS_SD_SPI_MOSI_PIN_NO          7

#define SYS_SD_SPI_MISO_PER             GPIOA
#define SYS_SD_SPI_MISO_PIN             GPIO_Pin_6
#define SYS_SD_SPI_MISO_PIN_NO          6

#define SYS_SD_SPI_CS_PER               GPIOC
#define SYS_SD_SPI_CS_PIN               GPIO_Pin_4
#define SYS_SD_SPI_CS_PIN_NO            4

#define USE_DETECT_PIN                  FALSE

#define SYS_SD_DETECT_EXT_PER           EXTI_PortSourceGPIOC
#define SYS_SD_DETECT_PER               GPIOC
#define SYS_SD_DETECT_PIN               GPIO_Pin_5
#define SYS_SD_DETECT_PIN_NO            5
#define SYS_SD_SPI_PORT_IRQ             EXTI9_5_IRQn
#define SYS_SD_SPI_PORT_IRQHANDLER      EXTI9_5_IRQHandler

// MACROS
#define SYS_SPI_SLOW()                  { SYS_SD_SPI_PORT->CR1 = (SYS_SD_SPI_PORT->CR1 & ~0x38) | SPI_FREQ_0_35_MHz;   }   /* Set SCLK = PCLK / 256 */
#define SYS_SPI_FAST()                  { SYS_SD_SPI_PORT->CR1 = (SYS_SD_SPI_PORT->CR1 & ~0x38) | SPI_FREQ_22_50_MHz;  }   /* Set SCLK = PCLK / 2 */

#define SYS_SPI_CS_LOW()                { SYS_SD_SPI_CS_PER->ODR &= ~SYS_SD_SPI_CS_PIN; }
#define SYS_SPI_CS_HIGH()               { SYS_SD_SPI_CS_PER->ODR |= SYS_SD_SPI_CS_PIN; }

#define TEXT_SPI_PORT_INITED            "SPI port enabled.\n\r"

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
int ArcadeIT_SPI_Port_Init (
  uint16_t pClock // the clock frequency to set the SPI port
  );
// -----------------------------------------------------------------------------

// /////////////////////////////////////////////////////////////////////////////

#endif // _ARCADEIT_SPI_PORT_H_

/*
 Serial peripheral interface/ inter-IC sound (SPI/I2S)

 The SPI interface can be used to communicate with external devices using the SPI
 protocol like SRAM, EEPROM, LCDs and SD-Cards which is the main use here.

 The SPI port used from the ones available from the STM32F469 in the ArcadeIT!
 is the SPI1.

 From: RM0386, Reference manual, page 1088

 This section describes the SPI port functions, but this driver has been written
 to provide a common and standard driver. We will explain what and how registers
 are set to start the SPI port, and how transmit or receive data over it.

 -

 From: RM0386, Reference manual, page 1090

 Figure 352. SPI block diagram

 This diagram shows the architecture of the SPI peripheral inside the STM32F469
 microcontroller.

 -

 We have to enable the clock of the peripheral that drives the signals SCK, MISO,
 MOSI, and the CS signal. Then we need to configure them.

 PA5  SD CLK
 PA7  SD MOSI
 PA6  SD MISO
 PC4  SD CS

 From: RM0386, Reference manual, page 206-207

 These registers are 32 bit wide, values for each pin go from 0 to 3
  GPIO port mode register (GPIOx_MODER) (x = A to K)
  GPIO port speed register (GPIOx_OSPEEDR) (x = A to K)
  GPIO port pull-up register (GPIOx_PUPDR) (x = A to K)

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 | PIN 15  | PIN 14  | PIN 13  | PIN 12  | PIN 11  | PIN 10  | PIN 09  | PIN 08  |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   | MODER   |
 | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR |
 | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    | PUDR    |
 |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 !            GPIOA            !  GPIOC  !
 |  1    1    1    1 .  1    1 !  1    1 |  0    0    0    0 .  0    0    0    0 |
 ###############################**********----+----+---------+---------+---------+
 # PIN 07  # PIN 06  # PIN 05  * PIN 04  * PIN 03  | PIN 02  | PIN 01  | PIN 00  |
 #----+----#----+----#----+----*----+----*----+----+----+----|----+----+----+----+
 # 15 | 14 # 13 | 12 # 11 | 10 * 09 | 08 * 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 #----+----#----+----#----+----*----+----*----+----+----+----|----+----+----+----+
 # MODER   # MODER   # MODER   * MODER   * MODER   | MODER   | MODER   | MODER   |
 # OSPEEDR # OSPEEDR # OSPEEDR * OSPEEDR * OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR |
 # PUDR    # PUDR    # PUDR    * PUDR    * PUDR    | PUDR    | PUDR    | PUDR    |
 #  [1:0]  #  [1:0]  #  [1:0]  *  [1:0]  *  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  |
 #----+----#----+----#----+----*----+----*----+----+----+----|----+----+----+----+
 # rw | rw # rw | rw # rw | rw * rw | rw * rw | rw | rw | rw | rw | rw | rw | rw |
 ###############################**********----+----+---------+----+----+----+----+

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


                                         !    GPIOA     !GPIOC
 |  0    0    0    0 .  0    0    0    0 |  1    1    1 !  1 .  0    0    0    0 |
 +----+----+----+----+----+----+----+----###############******----+----+----+----+
 |P15 |P14 |P13 |P12 |P11 |P10 |P09 |P08 #P07 #P06 #P05 #P04 *P03 |P02 |P01 |P00 |
 +----+----+----+----+----+----+----+----#----#----#----#----*----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 # 07 # 06 # 05 # 04 * 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----#----#----#----#----*----+----+----+----+
 | OT | OT | OT | OT | OT | OT | OT | OT # OT # OT # OT # OT * OT | OT | OT | OT |
 +----+----+----+----+----+----+----+----#----#----#----#----*----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw # rw # rw # rw # rw * rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----###############******----+----+----+----+

 -

 From: Datasheet, page 81

 Table 12. Alternate functions

 To activate the SPI1 function we need to apply to pins PA5, PA6, PA7 the alternate
 function 5.

 Since we have sixteen different alternate functions, 4 bits are needed for each
 pin to be configured. So two registers are used for pins 7..0 and pins 15..8,
 respectively registers GPIOx_AFRL and GPIOx_AFRH.

 For SPI1 we use pins 5, 6 and 7, so the low register GPIOx_AFRL is used.

     alternate fn 5      alternate fn 5      alternate fn 5

 |  0    1    0    1 .  0    1    0    1 |  0    1    0    1 .  0    0    0    0 |
 #############################################################---------+---------+
 #      PIN 07       #      PIN 06       #      PIN 05       #      PIN 04       |
 #----+----+----+----#----+----+----+----#----+----+----+----#----+----+----+----+
 # 31 | 30 | 29 | 28 # 27 | 26 | 25 | 24 # 23 | 22 | 21 | 20 # 19 | 18 | 17 | 16 |
 #----+----+----+----#----+----+----+----#----+----+----+----#----+----+----+----+
 #    AFRH07[3:0]    #    AFRH06[3:0]    #    AFRH05[3:0]    #    AFRH04[3:0]    |
 #----+----+----+----#----+----+----+----#----+----+----+----#----+----+----+----+
 # rw | rw | rw | rw # rw | rw | rw | rw # rw | rw | rw | rw # rw | rw | rw | rw |
 #############################################################----+----+----+----+


 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 |      PIN 03       |      PIN 02       |      PIN 01       |      PIN 00       |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |    AFRH03[3:0]    |    AFRH02[3:0]    |    AFRH01[3:0]    |    AFRH00[3:0]    |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 -

 From: RM0386, Reference manual, page 171

 6.3.14 RCC APB2 peripheral clock enable register (RCC_APB2ENR)

 We first need to enable the clock of the SPI1 by enabling the corresponding
 flag in register RCC_APB2ENR: Bit 12 is called SPI1EN (SPI1 clock enable).
 Then we can configure the SPI1 peripheral (frequency, data etc.)

 -
 The APB2 bus is clocked at half of the main clock, so it can reach 90MHz. The SPI
 port maximum frequency has to divided by a prescaler, which can assume values
 between 0 and 7, thus the SPI frequency can range from:

   90MHz / 2^(0+1) = 90MHz / 2 =  45.00 MHz
   90MHz / 2^(1+1) = 90MHz / 4 =  22.50 MHz
   90MHz / 2^(2+1) = 90MHz / 8 =  11.25 MHz
   90MHz / 2^(3+1) = 90MHz / 16 =  5.62 MHz
   90MHz / 2^(4+1) = 90MHz / 32 =  2.81 MHz
   90MHz / 2^(5+1) = 90MHz / 64 =  ‭1.40 ‬MHz
   90MHz / 2^(6+1) = 90MHz / 128 = ‭0.70 ‬MHz
   90MHz / 2^(7+1) = 90MHz / 256 = 0.35 MHz

 -

 Typical transmission

               +-----+-----+-----+-----+-----+-----+-----+-----+---+
               |                      byte                     |Ack|
               +-----+-----+-----+-----+-----+-----+-----+-----+---+
  Start        |     |     |     |     |     |     |     |     |   |   End
  condition    |0    |1    |2    |3    |4    |5    |6    |7    |   |   condition
  |------.     .--.        .--------.        .-----------------#####    .------|
  |SDA   |     |1 |   0    |1     1 |   0    |1     1     1    #####    |   SDA|
         '--- -'  '--------'        '--------'                 ##### ---' .----|
  |----.       .--.  .--.  .--.  .--.  .--.  .--.  .--.  .--.  .--.
  |SCL |       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |       | SCL|
  |    '----- -'  '--'  '--'  '--'  '--'  '--'  '--'  '--'  '--'  '- -----'    |

 */
