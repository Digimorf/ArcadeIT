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
 *               F I R M W A R E   I N T E R F A C E
 * -----------------------------------------------------------------
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_Firmware.h
 * @version V0.13
 * @date    28-06-2017
 * @last    02-07-2020
 * @brief   This library is used to interface the firmware to the user application.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY

 28-06-2017:
 - Creation date of the library structure.

 06-02-2020:
 - Converted to pure CMSIS.

 ******************************************************************************
 */

#ifndef __ARCADEIT_FIRMWARE_H
#define __ARCADEIT_FIRMWARE_H

// /////////////////////////////////////////////////////////////////////////////
// Include
// /////////////////////////////////////////////////////////////////////////////

// C standard libraries.
#include <stdio.h>
#include <stdint.h>

// STM32 definitions.
#include "stm32f4xx.h"

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

  #define TEST

  #define ARCADEIT_SYSTEM

  #define VECT_TAB_OFFSET                     0x00 // Vector Table base offset field.

  #define HSE_STARTUP_TIMEOUT                 360000000
  #define HSE_VALUE                           8000000
  #define PLL_M                               4   // PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
  #define PLL_Q                               7   // USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ
  #define PLL_N                               180
  #define PLL_P                               2   // SYSCLK = PLL_VCO / PLL_P

  #define GPIO_Pin_0                          ((uint16_t)0x0001)
  #define GPIO_Pin_1                          ((uint16_t)0x0002)
  #define GPIO_Pin_2                          ((uint16_t)0x0004)
  #define GPIO_Pin_3                          ((uint16_t)0x0008)
  #define GPIO_Pin_4                          ((uint16_t)0x0010)
  #define GPIO_Pin_5                          ((uint16_t)0x0020)
  #define GPIO_Pin_6                          ((uint16_t)0x0040)
  #define GPIO_Pin_7                          ((uint16_t)0x0080)
  #define GPIO_Pin_8                          ((uint16_t)0x0100)
  #define GPIO_Pin_9                          ((uint16_t)0x0200)
  #define GPIO_Pin_10                         ((uint16_t)0x0400)
  #define GPIO_Pin_11                         ((uint16_t)0x0800)
  #define GPIO_Pin_12                         ((uint16_t)0x1000)
  #define GPIO_Pin_13                         ((uint16_t)0x2000)
  #define GPIO_Pin_14                         ((uint16_t)0x4000)
  #define GPIO_Pin_15                         ((uint16_t)0x8000)

  #define GPIO_Speed_2MHz                     0x00 // Low speed
  #define GPIO_Speed_25MHz                    0x01 // Medium speed
  #define GPIO_Speed_50MHz                    0x02 // Fast speed
  #define GPIO_Speed_100MHz                   0x03 // High speed

  #define GPIO_OType_PP                       0x00
  #define GPIO_OType_OD                       0x01

  #define GPIO_PuPd_NOPULL                    0x00
  #define GPIO_PuPd_UP                        0x01
  #define GPIO_PuPd_DOWN                      0x02

  #define GPIO_Mode_IN                        0x00 // GPIO Input Mode
  #define GPIO_Mode_OUT                       0x01 // GPIO Output Mode
  #define GPIO_Mode_AF                        0x02 // GPIO Alternate function Mode
  #define GPIO_Mode_AN                        0x03 // GPIO Analog Mode

  #define RCC_AHB1Periph_GPIOA                ((uint32_t)0x00000001)
  #define RCC_AHB1Periph_GPIOB                ((uint32_t)0x00000002)
  #define RCC_AHB1Periph_GPIOC                ((uint32_t)0x00000004)
  #define RCC_AHB1Periph_GPIOD                ((uint32_t)0x00000008)
  #define RCC_AHB1Periph_GPIOE                ((uint32_t)0x00000010)
  #define RCC_AHB1Periph_GPIOF                ((uint32_t)0x00000020)
  #define RCC_AHB1Periph_GPIOG                ((uint32_t)0x00000040)
  #define RCC_AHB1Periph_GPIOH                ((uint32_t)0x00000080)
  #define RCC_AHB1Periph_GPIOI                ((uint32_t)0x00000100)
  #define RCC_AHB1Periph_DMA1                 ((uint32_t)0x00200000)
  #define RCC_AHB1Periph_DMA2                 ((uint32_t)0x00400000)
  #define RCC_AHB1Periph_DMA2D                ((uint32_t)0x00800000)
  #define RCC_AHB2Periph_OTG_FS               ((uint32_t)0x00000080)
  #define RCC_AHB3Periph_FMC                  ((uint32_t)0x00000001)

  #define RCC_APB1Periph_TIM2                 ((uint32_t)0x00000001)
  #define RCC_APB1Periph_TIM6                 ((uint32_t)0x00000010)
  #define RCC_APB1Periph_USART2               ((uint32_t)0x00020000)
  #define RCC_APB1Periph_PWR                  ((uint32_t)0x10000000)
  #define RCC_APB1Periph_DAC                  ((uint32_t)0x20000000)
  #define RCC_APB2Periph_TIM1                 ((uint32_t)0x00000001)
  #define RCC_APB2Periph_TIM8                 ((uint32_t)0x00000002)
  #define RCC_APB2Periph_SPI1                 ((uint32_t)0x00001000)
  #define RCC_APB2Periph_SYSCFG               ((uint32_t)0x00004000)

  // Hardware RCC ports
  #define RCC_AHB1ENR                         RCC->AHB1ENR
  #define RCC_AHB2ENR                         RCC->AHB2ENR
  #define RCC_AHB3ENR                         RCC->AHB3ENR
  #define RCC_APB1ENR                         RCC->APB1ENR
  #define RCC_APB2ENR                         RCC->APB2ENR

  static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

// ---------------------------------------------------------------------------------------
  #define TEXT_ARCADEIT_INIT "Initializing ArcadeIT! System...\n\r"
// ---------------------------------------------------------------------------------------
// Peripherals: Test pads
// ---------------------------------------------------------------------------------------
  #define CFGR_MCO2_RESET_MASK                ((uint32_t)0x07FFFFFF)

  #define RCC_MCO2Source_SYSCLK               ((uint32_t)0x00000000)
  #define RCC_MCO2Source_PLLI2SCLK            ((uint32_t)0x40000000)
  #define RCC_MCO2Source_HSE                  ((uint32_t)0x80000000)
  #define RCC_MCO2Source_PLLCLK               ((uint32_t)0xC0000000)

  #define RCC_MCO2Div_1                       ((uint32_t)0x00000000)
  #define RCC_MCO2Div_2                       ((uint32_t)0x20000000)
  #define RCC_MCO2Div_3                       ((uint32_t)0x28000000)
  #define RCC_MCO2Div_4                       ((uint32_t)0x30000000)
  #define RCC_MCO2Div_5                       ((uint32_t)0x38000000)

  #define SYS_TESTPADS_RCC_REG                RCC_AHB1ENR
  #define SYS_TESTPADS_RCC_CMD                RCC_AHB1PeriphClockCmd
  #define SYS_TESTPADS_RCC_PER                RCC_AHB1Periph_GPIOC
  #define SYS_TESTPADS_PER                    GPIOC
  #define SYS_TESTPADS_PIN                    GPIO_Pin_9
  #define SYS_TESTPADS_PIN_NO                 9
  #define SYS_TESTPADS_DIV                    RCC_MCO2Div_4
  #define SYS_TESTPADS_SRCCLK                 RCC_MCO2Source_SYSCLK
  #define TEXT_TEST_PADS_INITED               "Clock test pad enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: AUDIO
// ---------------------------------------------------------------------------------------
  #define SYS_AUDIO_DMA_RCC_REG               RCC_AHB1ENR
  #define SYS_AUDIO_DMA_RCC_CMD               RCC_AHB1PeriphClockCmd
  #define SYS_AUDIO_DMA_RCC_PER               RCC_AHB1Periph_DMA1
  #define SYS_AUDIO_DMA_PER                   DMA1
  #define SYS_AUDIO_DMA_STREAM                DMA1_Stream5
  #define SYS_AUDIO_DMA_STREAM_CH             DMA_Channel_7
  #define SYS_AUDIO_DMA_IRQ                   DMA1_Stream5_IRQn
  #define SYS_AUDIO_DMA_IRQHANDLER            DMA1_Stream5_IRQHandler

  #define SYS_AUDIO_OUT_RCC_REG               RCC_AHB1ENR
  #define SYS_AUDIO_OUT_RCC_CMD               RCC_AHB1PeriphClockCmd
  #define SYS_AUDIO_OUT_RCC_PER               RCC_AHB1Periph_GPIOA
  #define SYS_AUDIO_OUT_PER                   GPIOA
  #define SYS_AUDIO_OUT_PIN                   GPIO_Pin_4

  #define SYS_AUDIO_DAC_RCC_REG               RCC_APB1ENR
  #define SYS_AUDIO_DAC_RCC_CMD               RCC_APB1PeriphClockCmd
  #define SYS_AUDIO_DAC_RCC_PER               RCC_APB1Periph_DAC
  #define SYS_AUDIO_DAC_PER                   DAC
  #define SYS_AUDIO_DAC_12_PER                DAC->DHR12R1
  #define SYS_AUDIO_DAC_8_PER                 DAC->DHR8R1
  #define SYS_AUDIO_DAC_CH                    DAC_Channel_1

  #define SYS_AUDIO_TIM_RCC_REG               RCC_APB1ENR
  #define SYS_AUDIO_TIM_RCC_CMD               RCC_APB1PeriphClockCmd
  #define SYS_AUDIO_TIM_RCC_PER               RCC_APB1Periph_TIM6
  #define SYS_AUDIO_TIM_PER                   TIM6

  #define TEXT_AUDIO_INITED                   "Audio enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: BUS
// ---------------------------------------------------------------------------------------
// TODO
  #define TEXT_BUS_INITED                      "System BUS enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: SRAM
// ---------------------------------------------------------------------------------------
// TODO
  #define TEXT_SRAM_PORT_INITED                "SRAM port enabled.\n\r"
  #define TEXT_SRAM_PORT_SET                   "SRAM port set.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: EXPANSION
// ---------------------------------------------------------------------------------------
// TODO
  #define TEXT_EXPANSION_PORT_INITED           "Expansion port enabled.\n\r"
  #define TEXT_EXPANSION_PORT_SET              "Expansion port set.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: LCD
// ---------------------------------------------------------------------------------------
  #define ARCADEIT_LCD_RESET_RCC                RCC_AHB1PeriphClockCmd
  #define ARCADEIT_LCD_RESET_PERIPHERAL         RCC_AHB1Periph_GPIOH
  #define ARCADEIT_LCD_RESET_PORT               GPIOH
  #define ARCADEIT_LCD_RESET_PIN                GPIO_Pin_7

  #define ARCADEIT_LCD_BL_PWM_TIMER_RCC         RCC_APB2PeriphClockCmd
  #define ARCADEIT_LCD_BL_PWM_TIMER_PERIPHERAL  RCC_APB2Periph_TIM1
  #define ARCADEIT_LCD_BL_PWM_TIMER             TIM1
  #define ARCADEIT_LCD_BL_PWM_TIMER_AF          GPIO_AF_TIM1

  #define ARCADEIT_LCD_BL_PWM_PORT_RCC          RCC_AHB1PeriphClockCmd
  #define ARCADEIT_LCD_BL_PWM_PORT_PERIPHERAL   RCC_AHB1Periph_GPIOE
  #define ARCADEIT_LCD_1_BL_PWM_PORT            GPIOE
  #define ARCADEIT_LCD_1_BL_PWM_PIN             GPIO_Pin_13
  #define ARCADEIT_LCD_1_BL_PWM_PIN_S           GPIO_PinSource13
  #define ARCADEIT_LCD_2_BL_PWM_PORT            GPIOE
  #define ARCADEIT_LCD_2_BL_PWM_PIN             GPIO_Pin_14
  #define ARCADEIT_LCD_2_BL_PWM_PIN_S           GPIO_PinSource14

  #define TEXT_LCD_INITED                       "LCD %d enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: Parallel
// ---------------------------------------------------------------------------------------
  #define TEXT_PARALLEL_PORT_INITED     "Parallel port enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: RTC (Real Time Clock)
// ---------------------------------------------------------------------------------------
  #define TEXT_RTC_INITED               "Real time clock enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: Serial port
// ---------------------------------------------------------------------------------------
  #define ARCADEIT_SERIAL_METHOD         NORMAL

  #define SYS_SERIAL_TX_RCC_REG          RCC_AHB1ENR
  #define SYS_SERIAL_TX_RCC_CMD          RCC_AHB1PeriphClockCmd
  #define SYS_SERIAL_TX_RCC_PER          RCC_AHB1Periph_GPIOA
  #define SYS_SERIAL_TX_PER              GPIOA
  #define SYS_SERIAL_TX_PIN              GPIO_Pin_2
  #define SYS_SERIAL_TX_PIN_NO           2
  #define SYS_SERIAL_TX_AF               ((uint8_t)0x07)  /* USART2 Alternate Function mapping  */

  #define SYS_SERIAL_RX_RCC_REG          RCC_AHB1ENR
  #define SYS_SERIAL_RX_RCC_CMD          RCC_AHB1PeriphClockCmd
  #define SYS_SERIAL_RX_RCC_PER          RCC_AHB1Periph_GPIOA
  #define SYS_SERIAL_RX_PER              GPIOA
  #define SYS_SERIAL_RX_PIN              GPIO_Pin_3
  #define SYS_SERIAL_RX_PIN_NO           3
  #define SYS_SERIAL_RX_AF               ((uint8_t)0x07)  /* USART2 Alternate Function mapping  */

  #define SYS_SERIAL_IRQ                 USART2_IRQn
  #define SYS_SERIAL_IRQ_HANDLER         USART2_IRQHandler

  #define SYS_SERIAL_RCC_REG             RCC_APB1ENR
  #define SYS_SERIAL_RCC_CMD             RCC_APB1PeriphClockCmd
  #define SYS_SERIAL_RCC_PER             RCC_APB1Periph_USART2
  #define SYS_SERIAL_PORT                USART2

  #define SYS_SERIAL_StopBits_1               ((uint16_t)0x0000)
  #define SYS_SERIAL_WordLength_8b            ((uint16_t)0x0000)
  #define SYS_SERIAL_Parity_No                ((uint16_t)0x0000)
  #define SYS_SERIAL_Mode_Rx                  ((uint16_t)0x0004)
  #define SYS_SERIAL_Mode_Tx                  ((uint16_t)0x0008)
  #define SYS_SERIAL_HardwareFlowControl_None ((uint16_t)0x0000)

  // USART CR1 register clear Mask ((~(uint16_t)0xE9F3))
  #define CR1_CLEAR_MASK            ((uint16_t)(USART_CR1_M | USART_CR1_PCE | \
                                                USART_CR1_PS | USART_CR1_TE | \
                                                USART_CR1_RE))

  // USART CR2 register clock bits clear Mask ((~(uint16_t)0xF0FF))
  #define CR2_CLOCK_CLEAR_MASK      ((uint16_t)(USART_CR2_CLKEN | USART_CR2_CPOL | \
                                                USART_CR2_CPHA | USART_CR2_LBCL))

  // USART CR3 register clear Mask ((~(uint16_t)0xFCFF))
  #define CR3_CLEAR_MASK            ((uint16_t)(USART_CR3_RTSE | USART_CR3_CTSE))

  #define SYS_SERIAL_FLAG_TXE            ((uint16_t)0x0080)
  #define SYS_SERIAL_FLAG_RXNE           ((uint16_t)0x0020)
  #define SYS_SERIAL_FLAG_IDLE           ((uint16_t)0x0010)

  #define SYS_SERIAL_RX_Block_Size       64
  #define SYS_SERIAL_TX_Block_Size       256

  #define SYS_SERIAL_SPEED               115200

  #define TEXT_SERIAL_PORT_INITED        "Serial port enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: I2C
// ---------------------------------------------------------------------------------------
  #define SYS_I2C_TIMEOUT             250
  #define SYS_I2C_METHOD              NORMAL

  #define SYS_I2C_DMA_RCC_REG         RCC_AHB1ENR
  #define SYS_I2C_DMA_RCC_CMD         RCC_AHB1PeriphClockCmd
  #define SYS_I2C_DMA_RCC_PER         RCC_AHB1Periph_DMA2
  #define SYS_I2C_DMA_PORT            DMA2

  #define SYS_I2C_DMA_MOSI_STREAM     DMA2_Stream3 // tx
  #define SYS_I2C_DMA_MOSI_STREAM_CH  DMA_Channel_3

  #define SYS_I2C_DMA_MISO_STREAM     DMA2_Stream2 // rx
  #define SYS_I2C_DMA_MISO_STREAM_CH  DMA_Channel_3

  #define SYS_I2C_RCC_REG             RCC_APB2ENR
  #define SYS_I2C_RCC_CMD             RCC_APB2PeriphClockCmd
  #define SYS_I2C_RCC_PER             RCC_APB2Periph_SPI1
  #define SYS_I2C_PORT                SPI1
  #define SYS_I2C_AF                  GPIO_AF_SPI1

  #define SYS_I2C_SCK_RCC_REG         RCC_AHB1ENR
  #define SYS_I2C_SCK_RCC_CMD         RCC_AHB1PeriphClockCmd
  #define SYS_I2C_SCK_RCC_PER         RCC_AHB1Periph_GPIOH
  #define SYS_I2C_SCK_PER             GPIOH
  #define SYS_I2C_SCK_PIN             GPIO_Pin_4
  #define SYS_I2C_SCK_SRC             GPIO_PinSource4

  #define SYS_I2C_SDA_RCC_REG         RCC_AHB1ENR
  #define SYS_I2C_SDA_RCC_CMD         RCC_AHB1PeriphClockCmd
  #define SYS_I2C_SDA_RCC_PER         RCC_AHB1Periph_GPIOH
  #define SYS_I2C_SDA_PER             GPIOH
  #define SYS_I2C_SDA_PIN             GPIO_Pin_5
  #define SYS_I2C_SDA_SRC             GPIO_PinSource5

  #define TEXT_I2C_PORT_INITED        "I2C port enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: SPI1
// ---------------------------------------------------------------------------------------
  #define SYS_SD_SPI_TIMEOUT             250
  #define SYS_SD_SPI_METHOD              DMA

  // SPI Clock = APB2 / pre scaler = 90MHz / prescaler
  #define SYS_SD_SPI_HIGH                SPI_BaudRatePrescaler_2
  #define SYS_SD_SPI_LOW                 SPI_BaudRatePrescaler_256
  #define SYS_SD_SPI_SPEED               SYS_SD_SPI_HIGH

  #define SYS_SD_SPI_DMA_RCC_REG         RCC_AHB1ENR
  #define SYS_SD_SPI_DMA_RCC_CMD         RCC_AHB1PeriphClockCmd
  #define SYS_SD_SPI_DMA_RCC_PER         RCC_AHB1Periph_DMA2
  #define SYS_SD_SPI_DMA_PORT            DMA2

  #define SYS_SD_SPI_DMA_MOSI_STREAM     DMA2_Stream3 // tx
  #define SYS_SD_SPI_DMA_MOSI_STREAM_CH  DMA_Channel_3

  #define SYS_SD_SPI_DMA_MISO_STREAM     DMA2_Stream2 // rx
  #define SYS_SD_SPI_DMA_MISO_STREAM_CH  DMA_Channel_3

  #define SYS_SD_SPI_RCC_REG             RCC_APB2ENR
  #define SYS_SD_SPI_RCC_CMD             RCC_APB2PeriphClockCmd
  #define SYS_SD_SPI_RCC_PER             RCC_APB2Periph_SPI1
  #define SYS_SD_SPI_PORT                SPI1
  #define SYS_SD_SPI_AF                  GPIO_AF_SPI1

  #define SYS_SD_SPI_SCK_RCC_REG         RCC_AHB1ENR
  #define SYS_SD_SPI_SCK_RCC_CMD         RCC_AHB1PeriphClockCmd
  #define SYS_SD_SPI_SCK_RCC_PER         RCC_AHB1Periph_GPIOA
  #define SYS_SD_SPI_SCK_PER             GPIOA
  #define SYS_SD_SPI_SCK_PIN             GPIO_Pin_5
  #define SYS_SD_SPI_SCK_SRC             GPIO_PinSource5

  #define SYS_SD_SPI_MOSI_RCC_REG        RCC_AHB1ENR
  #define SYS_SD_SPI_MOSI_RCC_CMD        RCC_AHB1PeriphClockCmd
  #define SYS_SD_SPI_MOSI_RCC_PER        RCC_AHB1Periph_GPIOA
  #define SYS_SD_SPI_MOSI_PER            GPIOA
  #define SYS_SD_SPI_MOSI_PIN            GPIO_Pin_7
  #define SYS_SD_SPI_MOSI_SRC            GPIO_PinSource7

  #define SYS_SD_SPI_MISO_RCC_REG        RCC_AHB1ENR
  #define SYS_SD_SPI_MISO_RCC_CMD        RCC_AHB1PeriphClockCmd
  #define SYS_SD_SPI_MISO_RCC_PER        RCC_AHB1Periph_GPIOA
  #define SYS_SD_SPI_MISO_PER            GPIOA
  #define SYS_SD_SPI_MISO_PIN            GPIO_Pin_6
  #define SYS_SD_SPI_MISO_SRC            GPIO_PinSource6

  #define SYS_SD_SPI_CS_RCC_REG          RCC_AHB1ENR
  #define SYS_SD_SPI_CS_RCC_CMD          RCC_AHB1PeriphClockCmd
  #define SYS_SD_SPI_CS_RCC_PER          RCC_AHB1Periph_GPIOC
  #define SYS_SD_SPI_CS_PER              GPIOC
  #define SYS_SD_SPI_CS_PIN              GPIO_Pin_4

  #define USE_DETECT_PIN                 FALSE

  #define SYS_SD_DETECT_RCC_REG          RCC_AHB1ENR
  #define SYS_SD_DETECT_RCC_CMD          RCC_AHB1PeriphClockCmd
  #define SYS_SD_DETECT_RCC_PER          RCC_AHB1Periph_GPIOC
  #define SYS_SD_DETECT_EXT_PER          EXTI_PortSourceGPIOC
  #define SYS_SD_DETECT_PER              GPIOC
  #define SYS_SD_DETECT_PIN              GPIO_Pin_5
  #define SYS_SD_DETECT_SRC              GPIO_PinSource5
  #define SYS_SD_SPI_PORT_IRQ            EXTI9_5_IRQn
  #define SYS_SD_SPI_PORT_IRQHANDLER     EXTI9_5_IRQHandler

  // < MACROS >
  #define SYS_SPI_SLOW()                 { SYS_SD_SPI_PORT->CR1 = (SYS_SD_SPI_PORT->CR1 & ~0x38) | SYS_SD_SPI_LOW;   }   /* Set SCLK = PCLK / 256 */
  #define SYS_SPI_FAST()                 { SYS_SD_SPI_PORT->CR1 = (SYS_SD_SPI_PORT->CR1 & ~0x38) | SYS_SD_SPI_HIGH;  }   /* Set SCLK = PCLK / 2 */

  #define SYS_SPI_CS_LOW()               GPIO_ResetBits(SYS_SD_SPI_CS_PER, SYS_SD_SPI_CS_PIN)
  #define SYS_SPI_CS_HIGH()              GPIO_SetBits(SYS_SD_SPI_CS_PER, SYS_SD_SPI_CS_PIN)

  #define TEXT_SPI_PORT_INITED           "SPI port enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: Status LEDs
// ---------------------------------------------------------------------------------------
  #define SYS_STATUS_LED1_RCC_REG        RCC_AHB1ENR
  #define SYS_STATUS_LED1_RCC_CMD        RCC_AHB1PeriphClockCmd
  #define SYS_STATUS_LED1_RCC_PER        RCC_AHB1Periph_GPIOH
  #define SYS_STATUS_LED1_PER            GPIOH
  #define SYS_STATUS_LED1_PIN            GPIO_Pin_2
  #define SYS_STATUS_LED1_PIN_NO         2

  #define SYS_STATUS_LED2_RCC_REG        RCC_AHB1ENR
  #define SYS_STATUS_LED2_RCC_CMD        RCC_AHB1PeriphClockCmd
  #define SYS_STATUS_LED2_RCC_PER        RCC_AHB1Periph_GPIOH
  #define SYS_STATUS_LED2_PER            GPIOH
  #define SYS_STATUS_LED2_PIN            GPIO_Pin_3
  #define SYS_STATUS_LED2_PIN_NO         3

  #define TEXT_STATUS_LEDS_INITED        "Status LEDs enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: USB
// ---------------------------------------------------------------------------------------
// TODO

  #define ARCADEIT_USB_KEYBOARD_BUFFER_SIZE 64 // bytes
  #define ARCADEIT_USB_TIMER_REFRESH        50 // ms

  #define TEXT_USB_PORT_INITED              "USB port enabled.\n\r"

// -----------------------------------------------------------------------------
// Peripherals: VGA
// -----------------------------------------------------------------------------
// TODO
  #define TEXT_VGA_PORT_INITED              "VGA port enabled.\n\r"

// -----------------------------------------------------------------------------
// Peripherals: Systick
// -----------------------------------------------------------------------------

  #define TEXT_SYSTICK_INITED               "Systick counter enabled.\n\r"
  #define SYS_ACTION_TIMEOUT                4000
  #define ARCADEIT_HCLKS_PER_MS             180000

  #define NVIC_PriorityGroup_0              ((uint32_t)0x700) /*!< 0 bits for pre-emption priority 4 bits for subpriority */
  #define NVIC_PriorityGroup_1              ((uint32_t)0x600) /*!< 1 bits for pre-emption priority 3 bits for subpriority */
  #define NVIC_PriorityGroup_2              ((uint32_t)0x500) /*!< 2 bits for pre-emption priority 2 bits for subpriority */
  #define NVIC_PriorityGroup_3              ((uint32_t)0x400) /*!< 3 bits for pre-emption priority 1 bits for subpriority */
  #define NVIC_PriorityGroup_4              ((uint32_t)0x300) /*!< 4 bits for pre-emption priority 0 bits for subpriority */

// -----------------------------------------------------------------------------
// Peripherals: Dynamic Memory Manager
// -----------------------------------------------------------------------------

  #define TEXT_DMM_ENABLED                  "DMM (Dynamic Memory Manager) enabled.\n\r"

// -----------------------------------------------------------------------------
// System settings.
  #define ARCADEIT_RELEASE_YEAR            18
  #define ARCADEIT_RELEASE_MONTH           10
  #define ARCADEIT_RELEASE_DAY             5
  #define ARCADEIT_RELEASE_HOURS           8
  #define ARCADEIT_RELEASE_MINUTES         25
  #define ARCADEIT_RELEASE_SECONDS         10

// -----------------------------------------------------------------------------
// This is the list of devices/systems and drivers that can be connected to the system.
// -----------------------------------------------------------------------------
  #define ARCADEIT_DEVICE_BUS              0x00001
  #define ARCADEIT_DEVICE_RAM_MODULE       0x00002
  #define ARCADEIT_DEVICE_EXPANSION        0x00004
  // ---------
  #define ARCADEIT_DEVICE_SPI1             0x00010
  #define ARCADEIT_DEVICE_I2C              0x00020
  // ---------
  #define ARCADEIT_DEVICE_LCDS             0x00100
  #define ARCADEIT_DEVICE_VGA              0x00200
  #define ARCADEIT_DEVICE_AUDIO            0x00400
  // ---------
  #define ARCADEIT_DEVICE_PARALLEL         0x01000
  #define ARCADEIT_DEVICE_USB              0x02000
  #define ARCADEIT_DEVICE_SERIAL_PORT      0x04000
  // ---------
  #define ARCADEIT_DEVICE_STATUSLED        0x10000
  #define ARCADEIT_DEVICE_TESTPADS         0x20000
  // ---------
  #define ARCADEIT_UNIT_RTC                0x00001
  #define ARCADEIT_UNIT_DMM                0x00010
  #define ARCADEIT_UNIT_SCHEDULER          0x00100
  // ---------
  #define ARCADEIT_STORAGE_RAM_DISK        0x00001
  #define ARCADEIT_STORAGE_SD_CARD_SPI1    0x00002

  #define ARCADEIT_TERMINAL_OUTPUT_SERIAL  0x00001

  #define ARCADEIT_TERMINAL_OUTPUT_VGA     0x00010
  #define ARCADEIT_TERMINAL_INPUT_SERIAL   0x00002
  #define ARCADEIT_TERMINAL_INPUT_USB      0x00020

  #define TERMINAL_SERIAL                  0
  #define TERMINAL_VGA                     1
  #define TERMINAL_LCD                     2

  #define TERMINAL_DEBUG                   TERMINAL_SERIAL
  #define TERMINAL_ACTIVE                  TERMINAL_VGA

  #define SYSTEM_TIMESTAMP_LEN             18

  #define MEMORY_MCU                       0
  #define MEMORY_BOARD                     1
  #define MEMORY_EXPANSION                 2

// Timer counters used by the Systick system. These counters are
// decremented every interrupt generated by the Systick interrupt routine.
// The user can monitor one of these counters in order to perform delays and other
// Time based tasks.
#define ARCADEIT_SECOND                    1
#define ARCADEIT_MILLISECOND               1000
#define ARCADEIT_MICROSECOND               1000000
#define ARCADEIT_NANOSECOND                1000000000

#define TEXT_SCHEDULER_INITED              "Tasks scheduler enabled.\n\r"

#define EDITOR_TEXT_STATS_NAME             "Code Editor V1.0"
#define SHELL_TEXT_STATS_NAME              "Shell V1.0"

#define TAB_TO_SPACES                      4
#define TAB_CHARACTER                      ASCII_SPACE

#define ARCADEIT_PAUSE_HALF_SECOND         500
#define ARCADEIT_PAUSE_SECOND              1000
#define ARCADEIT_PAUSE_5_SECONDS           5000
#define ARCADEIT_PAUSE_10_SECONDS          10000

#define DUMMY_BYTE                         0xFF
#define ARCADEIT_DUMMY_BYTE                0xFF

#define ARCADEIT_SAM_RAM_SIZE              128*1024

// -----------------------------------------------------------------------------
#define MAXIMUM_PARAMETERS_PER_FN          8
#define PAR_1                              0
#define PAR_2                              1
#define PAR_3                              2
#define PAR_4                              3
#define PAR_5                              4
#define PAR_6                              5
#define PAR_7                              6
#define PAR_8                              7

// -----------------------------------------------------------------------------
//#define ABUF_INIT {NULL, 0}

#define NVIC_VIDEO_PRIORITY                0
#define NVIC_AUDIO_PRIORITY                1
#define NVIC_USART_PRIORITY                2
#define NVIC_SYSTICK_PRIORITY              3
#define NVIC_USB_PRIORITY                  4
#define NVIC_LCD_PRIORITY                  5

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

#define SYS_MCU_CYCLES	DWT->CYCCNT

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

// Prototype of a function used by the ArcadeIT shell and system.
typedef int (*func)(uint32_t *pParameters);

typedef struct Color_Palette
{
    uint8_t  id;            // The id of the palette.
    uint16_t max_colors;    // Max number of colors depending the layer mode.
    uint16_t colors[256];   // The pointer to the entries of the list.

} Color_Palette_t, *Color_Palette_ptr;

enum inputmode
{
  actionNone,
  actionPlay,
  actionRecord
};

// /////////////////////////////////////////////////////////////////////////////
// Common externals (Globals for the system)
// /////////////////////////////////////////////////////////////////////////////
extern char gString[ARCADEIT_STRING_LENGTH_MAX];
extern char gTimeStamp[SYSTEM_TIMESTAMP_LEN];

extern __IO uint32_t gSecondaryTimer;
extern __IO uint32_t gSystemTimer, gSystemTick;

extern uint32_t gDevices, gSystems, gStorage;

// Globals for the SRAM expansion module and memory messages
extern uint32_t gMemoryHeap, gMemoryChip;

extern uint8_t *gRAMAddress, *gRAMSpeechAddress;
extern uint32_t gRAMSize;

// Others
extern uint8_t gTerminalEnable, gArcadseIT_Tasks_Enable;

// /////////////////////////////////////////////////////////////////////////////
// Function.
// /////////////////////////////////////////////////////////////////////////////
int ArcadeIT_SysTick_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_SysTick_Deinit(void);
// -----------------------------------------------------------------------------
void SysTick_Handler(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Start(void);
// -----------------------------------------------------------------------------
void ArcadeIT_System_Reset(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Refresh(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Tasks(void);
// -----------------------------------------------------------------------------
void ArcadeIT_System_Delay
(
  uint32_t pDelay   // The amount of ticks to wait for. The duration of a tick.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Wait_VBlank(void);
// -----------------------------------------------------------------------------
uint8_t ArcadeIT_If_VBlank(void);

// -----------------------------------------------------------------------------
void ArcadeIT_Test_Bench(void);
// -----------------------------------------------------------------------------
void ArcadeIT_SandBox(void);

// /////////////////////////////////////////////////////////////////////////////
#endif // End __ARCADEIT_FIRMWARE_H
