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

#include <stm32f469xx.h>
#include <stm32f4xx.h>

// C standard libraries.
#include <stdio.h>
#include <stdint.h>

// STM32 definitions.
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
  #define ARCADEIT_SYS_CLOCK                  180000000

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
  #define RCC_AHB1Periph_BKPSRAM              ((uint32_t)0x00040000)

  #define RCC_AHB2Periph_OTG_FS               ((uint32_t)0x00000080)

  #define RCC_AHB3Periph_FMC                  ((uint32_t)0x00000001)

  #define RCC_APB1Periph_TIM2                 ((uint32_t)0x00000001)
  #define RCC_APB1Periph_TIM6                 ((uint32_t)0x00000010)
  #define RCC_APB1Periph_USART2               ((uint32_t)0x00020000)
  #define RCC_APB1Periph_I2C2                 ((uint32_t)0x00400000)
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

  static __IO uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

  #define NVIC_PriorityGroup_0   ((uint32_t)0x700) /*!< 0 bits for pre-emption priority 4 bits for subpriority */
  #define NVIC_PriorityGroup_1   ((uint32_t)0x600) /*!< 1 bits for pre-emption priority 3 bits for subpriority */
  #define NVIC_PriorityGroup_2   ((uint32_t)0x500) /*!< 2 bits for pre-emption priority 2 bits for subpriority */
  #define NVIC_PriorityGroup_3   ((uint32_t)0x400) /*!< 3 bits for pre-emption priority 1 bits for subpriority */
  #define NVIC_PriorityGroup_4   ((uint32_t)0x300) /*!< 4 bits for pre-emption priority 0 bits for subpriority */

  #define TIM_CKD_DIV1                       ((uint16_t)0x0000)
  #define TIM_CKD_DIV2                       ((uint16_t)0x0100)
  #define TIM_CKD_DIV4                       ((uint16_t)0x0200)
  #define TIM_PSCReloadMode_Update           ((uint16_t)0x0000)
  #define TIM_PSCReloadMode_Immediate        ((uint16_t)0x0001)

  #define TIM_OCPreload_Enable               ((uint16_t)0x0008)
  #define TIM_OCPreload_Disable              ((uint16_t)0x0000)
  #define TIM_OCMode_Timing                  ((uint16_t)0x0000)
  #define TIM_OCMode_Active                  ((uint16_t)0x0010)
  #define TIM_OCMode_Inactive                ((uint16_t)0x0020)
  #define TIM_OCMode_Toggle                  ((uint16_t)0x0030)
  #define TIM_OC3Mode_PWM                    ((uint16_t)0x0060)
  #define TIM_OC4Mode_PWM                    ((uint16_t)0x6000)
  #define TIM_OCPolarity_High                ((uint16_t)0x0000)
  #define TIM_OCPolarity_Low                 ((uint16_t)0x0002)
  #define TIM_OutputState_Disable            ((uint16_t)0x0000)
  #define TIM_OutputState_Enable             ((uint16_t)0x0001)
  #define TIM_OCNPolarity_High               ((uint16_t)0x0000)
  #define TIM_DMA_Update                     ((uint16_t)0x0100)
  #define TIM_DMA_CC1                        ((uint16_t)0x0200)
  #define TIM_DMABase_CR1                    ((uint16_t)0x0000)
  #define TIM_DMABase_CR2                    ((uint16_t)0x0001)
  #define TIM_DMABase_SMCR                   ((uint16_t)0x0002)
  #define TIM_DMABase_DIER                   ((uint16_t)0x0003)
  #define TIM_DMABase_SR                     ((uint16_t)0x0004)
  #define TIM_DMABase_EGR                    ((uint16_t)0x0005)
  #define TIM_DMABase_CCMR1                  ((uint16_t)0x0006)
  #define TIM_DMABase_CCMR2                  ((uint16_t)0x0007)
  #define TIM_DMABase_CCER                   ((uint16_t)0x0008)
  #define TIM_DMABase_CNT                    ((uint16_t)0x0009)
  #define TIM_DMABase_PSC                    ((uint16_t)0x000A)
  #define TIM_DMABase_ARR                    ((uint16_t)0x000B)
  #define TIM_DMABase_RCR                    ((uint16_t)0x000C)
  #define TIM_DMABase_CCR1                   ((uint16_t)0x000D)
  #define TIM_DMABase_CCR2                   ((uint16_t)0x000E)
  #define TIM_DMABase_CCR3                   ((uint16_t)0x000F)
  #define TIM_DMABase_CCR4                   ((uint16_t)0x0010)
  #define TIM_DMABase_BDTR                   ((uint16_t)0x0011)
  #define TIM_DMABase_DCR                    ((uint16_t)0x0012)
  #define TIM_DMABase_OR                     ((uint16_t)0x0013)
  #define TIM_DMABurstLength_1Transfer       ((uint16_t)0x0000)
  #define TIM_DMABurstLength_2Transfers      ((uint16_t)0x0100)
  #define TIM_DMABurstLength_3Transfers      ((uint16_t)0x0200)
  #define TIM_DMABurstLength_4Transfers      ((uint16_t)0x0300)
  #define TIM_DMABurstLength_5Transfers      ((uint16_t)0x0400)
  #define TIM_DMABurstLength_6Transfers      ((uint16_t)0x0500)
  #define TIM_DMABurstLength_7Transfers      ((uint16_t)0x0600)
  #define TIM_DMABurstLength_8Transfers      ((uint16_t)0x0700)
  #define TIM_DMABurstLength_9Transfers      ((uint16_t)0x0800)
  #define TIM_DMABurstLength_10Transfers     ((uint16_t)0x0900)
  #define TIM_DMABurstLength_11Transfers     ((uint16_t)0x0A00)
  #define TIM_DMABurstLength_12Transfers     ((uint16_t)0x0B00)
  #define TIM_DMABurstLength_13Transfers     ((uint16_t)0x0C00)
  #define TIM_DMABurstLength_14Transfers     ((uint16_t)0x0D00)
  #define TIM_DMABurstLength_15Transfers     ((uint16_t)0x0E00)
  #define TIM_DMABurstLength_16Transfers     ((uint16_t)0x0F00)
  #define TIM_DMABurstLength_17Transfers     ((uint16_t)0x1000)
  #define TIM_DMABurstLength_18Transfers     ((uint16_t)0x1100)

  #define DMA_Stream0_IT_MASK                (uint32_t)(DMA_LISR_FEIF0 | DMA_LISR_DMEIF0 | \
                                                        DMA_LISR_TEIF0 | DMA_LISR_HTIF0 | \
                                                        DMA_LISR_TCIF0)

  #define DMA_Stream1_IT_MASK                (uint32_t)(DMA_Stream0_IT_MASK << 6)
  #define DMA_Stream2_IT_MASK                (uint32_t)(DMA_Stream0_IT_MASK << 16)
  #define DMA_Stream3_IT_MASK                (uint32_t)(DMA_Stream0_IT_MASK << 22)
  #define DMA_Stream4_IT_MASK                (uint32_t)(DMA_Stream0_IT_MASK | (uint32_t)0x20000000)
  #define DMA_Stream5_IT_MASK                (uint32_t)(DMA_Stream1_IT_MASK | (uint32_t)0x20000000)
  #define DMA_Stream6_IT_MASK                (uint32_t)(DMA_Stream2_IT_MASK | (uint32_t)0x20000000)
  #define DMA_Stream7_IT_MASK                (uint32_t)(DMA_Stream3_IT_MASK | (uint32_t)0x20000000)
  #define TRANSFER_IT_MASK                   (uint32_t)0x0F3C0F3C
// ---------------------------------------------------------------------------------------
  #define TEXT_ARCADEIT_INIT "Initializing ArcadeIT! System...\n\r"
// ---------------------------------------------------------------------------------------
// Peripherals: Test pads
// ---------------------------------------------------------------------------------------

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
  #define ARCADEIT_A100_LCD                     ILI9341
  #define ARCADEIT_A100_LCD_WIDTH               320
  #define ARCADEIT_A100_LCD_HEIGHT              240
  #define TEXT_LCD_INITED                       "LCD ports enabled.\n\r"
  #define TEXT_LCD_PORT_SET                     "LCD port set.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: Parallel
// ---------------------------------------------------------------------------------------
  #define TEXT_PARALLEL_PORT_INITED             "Parallel port enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: RTC (Real Time Clock)
// ---------------------------------------------------------------------------------------
  #define TEXT_RTC_INITED                       "Real time clock enabled.\n\r"

// ---------------------------------------------------------------------------------------
// Peripherals: Serial port
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
// Peripherals: I2C
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
// Peripherals: SPI1
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
// Peripherals: Status LEDs
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
// Peripherals: USB
// ---------------------------------------------------------------------------------------
// TODO

  #define ARCADEIT_USB_KEYBOARD_BUFFER_SIZE 64 // bytes
  #define ARCADEIT_USB_TIMER_REFRESH        50 // ms

  #define TEXT_USB_PORT_INITED   "USB port enabled.\n\r"

// -----------------------------------------------------------------------------
// Peripherals: VGA
// -----------------------------------------------------------------------------
// TODO
  #define TEXT_VGA_PORT_INITED   "VGA port enabled.\n\r"

// -----------------------------------------------------------------------------
// Peripherals: Systick
// -----------------------------------------------------------------------------

  #define TEXT_SYSTICK_INITED    "Systick counter enabled.\n\r"
  #define SYS_ACTION_TIMEOUT     4000
  #define ARCADEIT_HCLKS_PER_MS  180000

// -----------------------------------------------------------------------------
// Peripherals: Dynamic Memory Manager
// -----------------------------------------------------------------------------

  #define TEXT_DMM_ENABLED          "DMM (Dynamic Memory Manager) enabled.\n\r"

// -----------------------------------------------------------------------------
// System settings.
  #define ARCADEIT_RELEASE_YEAR     20
  #define ARCADEIT_RELEASE_MONTH    10
  #define ARCADEIT_RELEASE_DAY      5
  #define ARCADEIT_RELEASE_HOURS    8
  #define ARCADEIT_RELEASE_MINUTES  25
  #define ARCADEIT_RELEASE_SECONDS  10

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
  #define ARCADEIT_DEVICE_SERIAL           0x04000
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

  #define ARCADEIT_SECOND                  1
  #define ARCADEIT_MILLISECOND             1000
  #define ARCADEIT_MICROSECOND             1000000
  #define ARCADEIT_NANOSECOND              1000000000

  #define TEXT_SCHEDULER_INITED            "Tasks scheduler enabled.\n\r"

  #define EDITOR_TEXT_STATS_NAME           "Code Editor V1.0"
  #define SHELL_TEXT_STATS_NAME            "Shell V1.0"

  #define TAB_TO_SPACES                    4
  #define TAB_CHARACTER                    ASCII_SPACE

  #define ARCADEIT_PAUSE_HALF_SECOND       500
  #define ARCADEIT_PAUSE_SECOND            1000
  #define ARCADEIT_PAUSE_5_SECONDS         5000
  #define ARCADEIT_PAUSE_10_SECONDS        10000

  #define DUMMY_BYTE                       0xFF
  #define ARCADEIT_DUMMY_BYTE              0xFF

  #define ARCADEIT_SAM_RAM_SIZE            128*1024

// -----------------------------------------------------------------------------
  #define MAXIMUM_PARAMETERS_PER_FN        8
  #define PAR_1                            0
  #define PAR_2                            1
  #define PAR_3                            2
  #define PAR_4                            3
  #define PAR_5                            4
  #define PAR_6                            5
  #define PAR_7                            6
  #define PAR_8                            7

// -----------------------------------------------------------------------------
//#define ABUF_INIT {NULL, 0}

  #define NVIC_VIDEO_PRIORITY              0
  #define NVIC_AUDIO_PRIORITY              1
  #define NVIC_USART_PRIORITY              2
  #define NVIC_SYSTICK_PRIORITY            3
  #define NVIC_USB_PRIORITY                4
  #define NVIC_LCD_PRIORITY                5

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

#define SYS_MCU_CYCLES()     ((uint32_t)DWT->CYCCNT)
#define SYS_MCU_CYCLES_RESET DWT->CYCCNT = 0

#define SYS_MCU_TO_NS(hclk)  ((uint32_t)(((hclk) * (5.55)) / 1))
#define SYS_MCU_TO_US(hclk)  ((uint32_t)(((hclk) * (5.55)) / 1000))
#define SYS_MCU_TO_MS(hclk)  ((uint32_t)(((hclk) * (5.55)) / 1000000))
#define SYS_MCU_TO_S(hclk)   ((uint32_t)(((hclk) * (5.55)) / 1000000000))

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

extern __IO uint8_t gCubeMonitorTrigger1;

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
