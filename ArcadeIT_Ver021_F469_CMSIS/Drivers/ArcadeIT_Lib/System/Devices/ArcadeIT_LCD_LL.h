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
 * @file    ArcadeIT_LCD_LL.h
 * @version V0.13
 * @date    17-06-2017
 * @last    03-02-2020
 * @brief   This library is used to drive the two LCD ports of the ArcadeIT.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

  1.10 - LCD ports

  ArcadeIT! Motherboard CN4 - PortC - Parallel LCD1

  Pin  GPIO Function                     LCD 1 Interface
  ---------------------------------------------------------------------
  GND  VSS  GND                          CN4-14
  3.3V VDD  3.3V                         CN4-16
  092  PD11 FMC_A16, LCD_RS              CN4-11
  -    -    LCD_1_CS                     CN4-10
  146  PD4  FMC_NOE, LCD_RD              CN4-15
  147  PD5  FMC_NWE, LCD_WR              CN4-13
  084  PH7  GPIO, LCD_RESET              CN4-1
  076  PE13 GPIO, PWM, LCD_LEDK          CN4-12(*)
  097  PD14 FMC_D0 BUS_D0                CN4-2
  098  PD15 FMC_D1 BUS_D1                CN4-4
  142  PD0  FMC_D2 BUS_D2                CN4-6
  143  PD1  FMC_D3 BUS_D3                CN4-8
  68   PE7  FMC_D4 BUS_D4                CN4-3
  69   PE8  FMC_D5 BUS_D5                CN4-5
  70   PE9  FMC_D6 BUS_D6                CN4-7
  73   PE10 FMC_D7 BUS_D7                CN4-9

  (*) In order to drive the brightness of the display connected to this port
  the TIMER 1 must be configured to drive the Channel 3, that is connected to
  this pin.

  LCD1 / CN4
              .--------.
   2 BUS_D0   |  o  o  |   1 LCD_RESET
   4 BUS_D1   |  o  o  |   3 BUS_D4
   6 BUS_D2   |  o  o  |   5 BUS_D5
   8 BUS_D3   |  o  o |    7 BUS_D6
  10 LCD_1_CS |  o  o |    9 BUS_D7
  12 LCD_LEDK |  o  o  |  11 LCD_RS
  14 NC       |  o  o  |  13 LCD_WR
  16 3.3V     |  o  o  |  15 LCD_RD
              '--------'


  ArcadeIT! Motherboard CN3 - PortD - Parallel LCD2

  Pin  GPIO Function                     LCD 2 Interface
  ---------------------------------------------------------------------
  GND  VSS  GND                          CN3-14
  3.3V VDD  3.3V                         CN3-16
  092  PD11 FMC_A16, LCD_RS              CN3-11
  MUX BUS   LCD_2_CS                     CN3-10
  146  PD4  FMC_NOE, LCD_RD              CN3-15
  147  PD5  FMC_NWE, LCD_WR              CN3-13
  084  PH7  GPIO, LCD_RESET              CN3-1
  077  PE14 GPIO, PWM, LCD_LEDK          CN4-12(*)
  097  PD14 FMC_D0 BUS_D0                CN3-2
  098  PD15 FMC_D1 BUS_D1                CN3-4
  142  PD0  FMC_D2 BUS_D2                CN3-6
  143  PD1  FMC_D3 BUS_D3                CN3-8
  68   PE7  FMC_D4 BUS_D4                CN3-3
  69   PE8  FMC_D5 BUS_D5                CN3-5
  70   PE9  FMC_D6 BUS_D6                CN3-7
  73   PE10 FMC_D7 BUS_D7                CN3-9
  
  (*) In order to drive the brightness of the display connected to this port
  the TIMER 1 must be configured to drive the Channel 4, that is connected to
  this pin.
  
  LCD2 / CN3
              .--------.
   2 BUS_D0   |  o  o  |   1 LCD_RESET
   4 BUS_D1   |  o  o  |   3 BUS_D4
   6 BUS_D2   |  o  o  |   5 BUS_D5
   8 BUS_D3   |  o  o |    7 BUS_D6
  10 LCD_2_CS |  o  o |    9 BUS_D7
  12 LCD_LEDK |  o  o  |  11 LCD_RS
  14 NC       |  o  o  |  13 LCD_WR
  16 3.3V     |  o  o  |  15 LCD_RD
              '--------'

 ******************************************************************************
 HISTORY
 31-19-2017:
 - Creation date of the library structure.

 03-02-2020:
 - Port to CMSIS and GitHub.

 ******************************************************************************
*/

#ifndef _ARCADEIT_LCD_LL_H
#define _ARCADEIT_LCD_LL_H

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

#define ILI9341                           0
#define ST3577S                           1
#define S6D0154                           2
#define HX8340B                           3
#define LCD_CONTROLLER                    ILI9341

#define PARALLEL                          0
#define SPI                               1
#define PERIPHERAL                        PARALLEL

#define SYS_LCD_RESET_PER                 GPIOH
#define SYS_LCD_RESET_PIN                 GPIO_Pin_7
#define SYS_LCD_RESET_PIN_NO              7

#define SYS_LCD_1_BL_PWM_PER              GPIOE
#define SYS_LCD_1_BL_PWM_PIN              GPIO_Pin_13
#define SYS_LCD_1_BL_PWM_PIN_NO           13

#define SYS_LCD_2_BL_PWM_PER              GPIOE
#define SYS_LCD_2_BL_PWM_PIN              GPIO_Pin_14
#define SYS_LCD_2_BL_PWM_PIN_NO           14

#define SYS_LCD_BL_PWM_TIMER              TIM1
#define SYS_LCD_BL_PWM_TIMER_AF           ((uint32_t)0x1)

#define LCD_1_ID                          0x1
#define LCD_1_WIDTH                       320
#define LCD_1_HEIGHT                      240
#define LCD_1_ORIENTATION                 HORIZONTAL
#define LCD_1_BL_PWM_DUTY                 0
#define LCD_1_BL_PWM_CHANNEL              3
#define LCD_1_CS_ADDRESS                  0x61400000//0x65800000

#define LCD_2_ID                          0x2
#define LCD_2_WIDTH                       320
#define LCD_2_HEIGHT                      240
#define LCD_2_ORIENTATION                 HORIZONTAL
#define LCD_2_BL_PWM_DUTY                 0
#define LCD_2_BL_PWM_CHANNEL              4
#define LCD_2_CS_ADDRESS                  0x60C00000//0x65C00000

#define LCD_RS_REGISTER_OFFSET            0x00010000
#define LCD_CMD_REGISTER                  1
#define LCD_CMD_DATA                      2
#define LCD_CMD_DELAY                     3

#define LCD_BACKLIGHT_PWM_STEPS           100   // 100 steps for each PWM frame.
#define LCD_BACKLIGHT_PWM_FREQUENCY       240

#define NUMBER_LCDS                       2

// Predefined colors for test purpose
#define COLOR_NUM_BYTES_LCD               3

#define HORIZONTAL 0
#define VERTICAL   1

#define CLUT_CM_ARGB8888      ((uint32_t)0x00000000)
#define CLUT_CM_RGB888        ((uint32_t)0x00000001)
#define NO_MODIF_ALPHA_VALUE  ((uint32_t)0x00000000)
#define REPLACE_ALPHA_VALUE   ((uint32_t)0x00000001)
#define COMBINE_ALPHA_VALUE   ((uint32_t)0x00000002)
#define CR_MASK               ((uint32_t)0xFFFCE0FC)  /* DMA2D CR Mask */
#define PFCCR_MASK            ((uint32_t)0x00FC00C0)  /* DMA2D FGPFCCR Mask */
#define DEAD_MASK             ((uint32_t)0xFFFF00FE)  /* DMA2D DEAD Mask */
#define DMA2D_M2M             ((uint32_t)0x00000000)
#define DMA2D_M2M_PFC         ((uint32_t)0x00010000)
#define DMA2D_M2M_BLEND       ((uint32_t)0x00020000)
#define DMA2D_R2M             ((uint32_t)0x00030000)
#define DMA2D_ARGB8888        ((uint32_t)0x00000000)
#define DMA2D_RGB888          ((uint32_t)0x00000001)
#define DMA2D_RGB565          ((uint32_t)0x00000002)
#define DMA2D_ARGB1555        ((uint32_t)0x00000003)
#define DMA2D_ARGB4444        ((uint32_t)0x00000004)
#define CM_ARGB8888           ((uint32_t)0x00000000)
#define CM_RGB888             ((uint32_t)0x00000001)
#define CM_RGB565             ((uint32_t)0x00000002)
#define CM_ARGB1555           ((uint32_t)0x00000003)
#define CM_ARGB4444           ((uint32_t)0x00000004)
#define CM_L8                 ((uint32_t)0x00000005)
#define CM_AL44               ((uint32_t)0x00000006)
#define CM_AL88               ((uint32_t)0x00000007)
#define CM_L4                 ((uint32_t)0x00000008)
#define CM_A8                 ((uint32_t)0x00000009)
#define CM_A4                 ((uint32_t)0x0000000A)

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// Parallel
#define LCD_REGISTER                      0x00000
#define LCD_DATA                          0x10000

// Common
#define ARCADEIT_LCD_RST_LOW()            (SYS_LCD_RESET_PER->ODR &= ~SYS_LCD_RESET_PIN)
#define ARCADEIT_LCD_RST_HIGH()           (SYS_LCD_RESET_PER->ODR |= SYS_LCD_RESET_PIN)

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////
// Definition of the hardware LCD.
typedef struct LCD_Driver
{
  uint8_t id;                  // Holds the id of the LCD.
  uint8_t type;                // Holds the type of the LCD.
  uint16_t width;              // Holds the width of the hardware display.
  uint16_t height;             // Holds the height of the hardware display.
  uint8_t orientation;         // this is the orientation flag for the hardware LCD. This is set when init the LCD.
  uint8_t active;              //
  uint8_t *address;
  uint32_t brightness;
  uint32_t scereen_size;
  uint8_t screen_rotation;
  uint8_t layers;
  uint8_t mirror;
  uint16_t rgb_transparent;
  uint8_t *video_buffer_memory;
  uint16_t scanline;
  uint8_t refresh;
  uint8_t double_buffer;
  uint8_t renderer;
  uint8_t *boot_image;
  uint8_t padding_h;
  uint8_t padding_v;
  uint16_t show_scanlines;
  uint8_t bpp_size;
  uint16_t palette[256];

} LCD_Driver_t, *LCD_Driver_ptr;

// /////////////////////////////////////////////////////////////////////////////
// Exported globals
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Exported functions
// /////////////////////////////////////////////////////////////////////////////
void LCD_Parallel_Port_Setup(
    uint8_t pPort   // port number (LCD 0 or 1)
  );
// -----------------------------------------------------------------------------
void LCD_Wr_Data(
    uint8_t pLCDId,  // The id of the display.
    uint8_t pData    // The data byte to write.
  );
// -----------------------------------------------------------------------------
void LCD_Wr_Register(
    uint8_t pLCDId,    // The id of the display.
    uint8_t pRegister  // The register byte select.
  );
// -----------------------------------------------------------------------------
void LCD_Area_Set(
    uint8_t pLCDId, // The id of the display.
    uint16_t pX1,
    uint16_t pY1,
    uint16_t pX2,
    uint16_t pY2
  );
// -----------------------------------------------------------------------------
void LCD_Clear(
    uint8_t pLCDId, // The id of the display.
    uint32_t pColor // The Color in hex format 0x00RRGGBB.
  );
// -----------------------------------------------------------------------------
void LCD_Reset(void);
// -----------------------------------------------------------------------------
void LCD_Configure(
    uint8_t pLCDNumber,     // Id of the LCD to set orientation.
    uint16_t *pData
  );
// -----------------------------------------------------------------------------
void LCD_Parallel_Init(
    uint8_t pLCDId,              // the id of the LCD.
    uint8_t pLCDType,            // the id of the LCD.
    uint16_t pLCDWidth,          // width of the LCD.
    uint16_t pLCDHeight,         // height of the LCD.
    uint8_t pOrientation,        // Orientation of the LCD: 0 Horizontal, 1: Vertical.
    uint32_t pBackgroundColor    // The Color in hex format 0x00RRGGBB.
  );
// -----------------------------------------------------------------------------
void LCD_Active(
    uint8_t pLCDId,        // The id of the LCD to set active flag
    uint8_t pStatus        // 0:OFF, 1:ON
  );
// -----------------------------------------------------------------------------
void LCD_NewFrame(
    uint8_t pLCDId        // The id of the LCD to set active flag
  );
// -----------------------------------------------------------------------------
void ArcadeIT_LCD_1_Refresh(void);
// -----------------------------------------------------------------------------
void LCD_DMA_Transfer(
    uint8_t pLCDId,       // The id of the LCD to set active flag
    uint16_t *pBuffer,
    uint32_t pSize
  );
// -----------------------------------------------------------------------------
void LCD_Palette_Get(
    uint8_t pLCDId,        // The id of the LCD to store the colors
    uint16_t *pColors      // The array of the colors
  );
// -----------------------------------------------------------------------------
void LCD_0_Backlight_Inc(void);
// -----------------------------------------------------------------------------
void LCD_0_Backlight_Dec(void);
// -----------------------------------------------------------------------------
void LCD_0_Backlight_Set(
    uint32_t pValue // The brightness to set the LCD 0 from 0 to 100
    );
// -----------------------------------------------------------------------------
void LCD_1_Backlight_Inc(void);
// -----------------------------------------------------------------------------
void LCD_1_Backlight_Dec(void);
// -----------------------------------------------------------------------------
void LCD_1_Backlight_Set(
    uint32_t pValue // The brightness to set the LCD 1 from 0 to 100
    );
// -----------------------------------------------------------------------------
void LCD_Backlight_Off(
  uint8_t pLCD   // The number of the LCD.
);
// -----------------------------------------------------------------------------
void LCD_Backlight_On(
  uint8_t pLCD   // The number of the LCD.
);
// -----------------------------------------------------------------------------
void LCD_Driver_Start(void);
// -----------------------------------------------------------------------------

/*
  The ArcadeIT! mainboard has two ports for parallel interfaced LCDs. That means
  that to drive a LCD we have an 8-bit data bus, and several control lines:
  Rd (read), Wr (write), RS (register/data), CS (chip select) and Reset.
  Then we can control the brightness of the display setting the back light to
  values in the range 0..100 (Off - full bright).

  LCDs are connected to the FMC bus, so to send data to the display controller is
  easy as to write a byte into a memory location, and the FMC system will to the rest.

  The brightness of the display is set by a specific function that can be called
  also by the task scheduler to obtain an automatic fade effect.

  The brightness control is achieved by the generation of two PWM channels controlled
  by the Timer1 of the STM3F469. You can learn more about the PWM signal and how it
  affects the brightness of a LED here:

  https://learn.sparkfun.com/tutorials/pulse-width-modulation/all#

  The STM32F469 can generate PWM signals using Timers set accordingly. For this
  task we use the advanced timer TIM1, that basically counts up and toggle the
  signal when it reach a specific value. The tricky part is to set the correct
  frequency for the PWM frame so that the blinking won't be visible.

  A PWM frame can be considered as the time elapsed two pulses.

    <------->
    .-.      .-.
    | |      | |
    | |      | |
    ----------------

  Considering that we need to drive the back light of the display, we need to
  have thsae pulses fast enough to not notice it, so a frequency at least of 60 Hz
  should be fine.


     <- At least 60 times per second --------------->

     <-------><-------><------->            <------->
     .-.      .-.      .-.      .-.         .-.      .-.
     | |      | |      | |      | |         | |      | |
     | |      | |      | |      | |         | |      | |
   ----------------------------------     ----------------


  We want to set the value of brightness from 0 to 99, so our PWM frame will be
  divided in 100 steps.


    1       99             50    50             99       1

      <------>                  <-->                     -
    ..        ..          .----.    .--         .-------. .
    ||        ||          |    |    |           |       | |
    ||        ||          |    |    |           |       | |
  ----------------      ----------------      ----------------


  We should have 6000 steps per second, now, the Timer 1 clock frequency for
  the ArcadeIT! is set at 180MHz so if the timer is not prescaled down the
  increment of the timer counter happens every 5.5nS, so we need to configure a
  prescaler so that each cycle of the timer will be 1S/6000 = 166 uS
  The prescaler can divide the counter clock frequency by any factor between
  1 and 65536

  From the reference manual RM0386, page 760

  We know that the prescaler should count n times before clocking the timer. If we
  set the prescaler to 1, we basically divide the master clock by 2 since the
  prescaler counts 0 -> 1 -> Timer Clock++. Otherwise if we set the prescaler to
  65536 the timer clock will pulse every 65536 prescaler clocks, so the master
  clock is dvided by 65536.

  That said we need to divide 180000000 by 6000 to get 30000, that is the number
  of master clock cycles every one timer clock cycle.

  Our prescaler should be set to 30000 and stored into the PSC register.


               TIMx      60Hz timer

                     |
                 ARR |---------------------------------
               ^   . |             -             -
               |   . |            -             -
               |   . |           -             -
               |   . |          -             -
               |   9 |         -             -
               |   8 |        -             -
               |   7 |       -             -
               |   6 |      -             -
               |   5 |     -             -
               |   4 |    -             -
               |   3 |   -             -             -
               |   2 |  -             -             -
               |   1 | -             -             -
              CNT  0 |-             -             -
                     ----------------------------------
                      Timer clock cycles


  To generate a PWM signal we need to configure a compare value for thje counter
  that when it is reached the timer system toggles the associated pin.


                   .------.      .------.      .----
                   |      |      |      |      |
                   |      |      |      |      |
            TIMx   |      |      |      |      |
                   '      '------'      '------'

              ARR |------------------------------------
            ^   . |########     -########     -#####
            |   . |########    - ########    - #####
            |   . |########   -  ########   -  #####
            |   . |########  -   ########  -   #####
            |   9 |######## -    ######## -    #####
            |   8 |--------------------------------- Counter Compare Value
            |   7 |#######-      #######-      #####
            |   6 |######-#      ######-#      #####
            |   5 |#####-##      #####-##      #####
            |   4 |####-###      ####-###      ####-
            |   3 |###-####      ###-####      ###-#
            |   2 |##-#####      ##-#####      ##-##
            |   1 |#-######      #-######      #-###
           CNT  0 |-#######      -#######      -####
                  --------------------------------------
                   Timer clock cycles

 */


// /////////////////////////////////////////////////////////////////////////////
#endif // _ARCADEIT_LCD_LL_H
