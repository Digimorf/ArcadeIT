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
 * @file    ArcadeIT_LCD_LL.c
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

  1.10 - LCD ports


  ArcadeIT! Motherboard CN3 - PortC - Parallel LCD1

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
  14 GND      |  o  o  |  13 LCD_WR
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
  14 GND      |  o  o  |  13 LCD_WR
  16 3.3V     |  o  o  |  15 LCD_RD
              '--------'

 ******************************************************************************
 HISTORY
 16-06-2017:
 - Creation date of the library.

 21-06-2017:
 - Ported the project on Atollic TrueStudio.

 03-02-2020:
 - Port to CMSIS and GitHub.

 ******************************************************************************
*/

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
//#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! Storage libraries.
#include <System/Disk/ArcadeIT_Storage.h>
#include <System/Disk/ArcadeIT_SD_Card.h>
#include <System/Disk/ArcadeIT_RAMDisk.h>

// ArcadeIT! System clocks and counters.
#include "System/Units/ArcadeIT_Scheduler.h"

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_BUS_Port.h>

// ArcadeIT! Audio and video.
#include <System/Devices/ArcadeIT_LCD_LL.h>
#include <System/Devices/ArcadeIT_LCD_Definitions.h>
#include <System/Devices/ArcadeIT_VGA_LL.h>
/*
#include "System/Video/ArcadeIT_Gfx_Utilities.h"
*/

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>
#include <System/Devices/ArcadeIT_BUS_Port.h>


// /////////////////////////////////////////////////////////////////////////////
// External
// /////////////////////////////////////////////////////////////////////////////
extern uint8_t *gDisplayAddress[2];

LCD_Driver_t gLCDDriver[2];
uint8_t gLCDLineBuffer[320];

/*
extern const uint8_t EmulatorScreen_Pacman[77586];
extern const uint8_t bootImage320x240[77586];
extern __IO Video_Driver_t gVideoDriver;
extern Color_Palette_t *gVGAPaletteShow, *gVGAPaletteRender;


*/
// /////////////////////////////////////////////////////////////////////////////
// Globals
// /////////////////////////////////////////////////////////////////////////////
// Sequence of startup steps.
enum StartupSteps
{
    startup_step_port,
    startup_step_lcd,
    startup_step_framerefresh,
    startup_step_layer
};

// /////////////////////////////////////////////////////////////////////////////
// Sequence to send to the LCD for orientation configuration.
const uint16_t gLCDSequence[4][2][256] =
{
  {
    {
      LCD_CMD_REGISTER, SOFTWARE_RESET, //software reset
      LCD_CMD_DELAY, 20,
      LCD_CMD_REGISTER, SLEEP_OUT,
      LCD_CMD_DELAY, 120,

      // Factory commands
      LCD_CMD_REGISTER, POWER_CONTROL_B,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x81,
      //LCD_CMD_DATA, 0x83,
      LCD_CMD_DATA, 0x30,
      LCD_CMD_REGISTER, POWER_ON_SEQUENCE_CONTROL,
      LCD_CMD_DATA, 0x55,
      LCD_CMD_DATA, 0x01,
      LCD_CMD_DATA, 0x23,
      LCD_CMD_DATA, 0x01,
      /*LCD_CMD_DATA, 0x64,
      LCD_CMD_DATA, 0x03,
      LCD_CMD_DATA, 0x12,
      LCD_CMD_DATA, 0x81,*/
      LCD_CMD_REGISTER, DRIVER_TIMING_CONTROL_A,
      LCD_CMD_DATA, 0x84,
      LCD_CMD_DATA, 0x11,
      LCD_CMD_DATA, 0x7A,
      /*LCD_CMD_DATA, 0x85,
      LCD_CMD_DATA, 0x01,
      LCD_CMD_DATA, 0x79,*/
      LCD_CMD_REGISTER, POWER_CONTROL_A,
      LCD_CMD_DATA, 0x39,
      LCD_CMD_DATA, 0x2C,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x34,
      LCD_CMD_DATA, 0x02,
      LCD_CMD_REGISTER, PUMP_RATIO_CONTROL,
      LCD_CMD_DATA, 0x10,
      //LCD_CMD_DATA, 0x20,
      LCD_CMD_REGISTER, DRIVER_TIMING_CONTROL_B,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x00,

      // CONTRAST
      LCD_CMD_REGISTER, POWER_CONTROL_1,              // Power control
      LCD_CMD_DATA, 0x1F,                             // VRH [5:0], GVDD 3.3V.

      LCD_CMD_REGISTER, POWER_CONTROL_2,              // Power control
      LCD_CMD_DATA, 0x02,                             // BT[3:0]

      // LEVELS
      LCD_CMD_REGISTER, VCOM_CONTROL_1,               // VCM control 1
      LCD_CMD_DATA, 0x1F,                             // VMH [6:0]
      LCD_CMD_DATA, 0x5F,                             // VML [6:0]

      LCD_CMD_REGISTER, VCOM_CONTROL_2,               // VCM control 2
      LCD_CMD_DATA, 0xC0,

      LCD_CMD_REGISTER, MEMORY_ACCESS_CONTROL,        // Memory Access Control
      LCD_CMD_DATA, 0x68,
      LCD_CMD_REGISTER, PIXEL_FORMAT_SET,             // Pixel format
      LCD_CMD_DATA, PIXEL_FORMAT_RGB565BIT,           // 16bit
      LCD_CMD_REGISTER, FRAME_CONTROL_IN_NORMAL_MODE, // Frame rate
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x1D,                             // 65Hz
      LCD_CMD_REGISTER, DISPLAY_FUNCTION_CONTROL,     // Display Function Control
      LCD_CMD_DATA, 0x0A,
      LCD_CMD_DATA, 0xA2,
      LCD_CMD_DATA, 0x27,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, ENTRY_MODE_SET,               // Entry mode
      LCD_CMD_DATA, 0x07,

      // Factory command
      LCD_CMD_REGISTER, ENABLE_3G,                    // 3Gamma Function
      LCD_CMD_DATA, 0x08,
      LCD_CMD_REGISTER, GAMMA_SET,                    // Gamma curve selected
      LCD_CMD_DATA, 0x01,

      // Gamma correction curve
      LCD_CMD_REGISTER, POSITIVE_GAMMA_CORRECTION,    // Positive gamma correction
      LCD_CMD_DATA, 0x1f,
      LCD_CMD_DATA, 0x1a,
      LCD_CMD_DATA, 0x18,
      LCD_CMD_DATA, 0x0a,
      LCD_CMD_DATA, 0x0f,
      LCD_CMD_DATA, 0x06,
      LCD_CMD_DATA, 0x45,
      LCD_CMD_DATA, 0x87,
      LCD_CMD_DATA, 0x32,
      LCD_CMD_DATA, 0x0a,
      LCD_CMD_DATA, 0x07,
      LCD_CMD_DATA, 0x02,
      LCD_CMD_DATA, 0x07,
      LCD_CMD_DATA, 0x05,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, NEGATIVE_GAMMA_CORRECTION,    // Negative gamma correction
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x25,
      LCD_CMD_DATA, 0x27,
      LCD_CMD_DATA, 0x05,
      LCD_CMD_DATA, 0x10,
      LCD_CMD_DATA, 0x09,
      LCD_CMD_DATA, 0x3a,
      LCD_CMD_DATA, 0x78,
      LCD_CMD_DATA, 0x4d,
      LCD_CMD_DATA, 0x05,
      LCD_CMD_DATA, 0x18,
      LCD_CMD_DATA, 0x0d,
      LCD_CMD_DATA, 0x38,
      LCD_CMD_DATA, 0x3a,
      LCD_CMD_DATA, 0x1f,

      LCD_CMD_REGISTER, WRITE_CTRL_DISPLAY,
      LCD_CMD_DATA, 0x28,

      LCD_CMD_REGISTER, WRITE_CONTENT_ADAPTIVE_BRIGHTNESS_CONTROL,
      LCD_CMD_DATA, 0x02,

      LCD_CMD_REGISTER, WRITE_DISPLAY_BRIGHTNESS,
      LCD_CMD_DATA, 0x00,

      LCD_CMD_REGISTER, NORMAL_DISPLAY_MODE_ON,

      LCD_CMD_REGISTER, SLEEP_OUT,                    //Exit Sleep
      LCD_CMD_DELAY, 120,
      LCD_CMD_REGISTER, DISPLAY_ON,                   //Display on
      LCD_CMD_DELAY, 50,
      0
    },
    { // Vertical
      LCD_CMD_REGISTER, SOFTWARE_RESET, //software reset
      LCD_CMD_DELAY, 5,
      LCD_CMD_REGISTER, SLEEP_OUT,
      LCD_CMD_DELAY, 120,
/*
      // Factory commands
      LCD_CMD_REGISTER, 0xCF,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x83,
      LCD_CMD_DATA, 0X30,
      LCD_CMD_REGISTER, 0xED,
      LCD_CMD_DATA, 0x64,
      LCD_CMD_DATA, 0x03,
      LCD_CMD_DATA, 0X12,
      LCD_CMD_DATA, 0X81,
      LCD_CMD_REGISTER, 0xE8,
      LCD_CMD_DATA, 0x85,
      LCD_CMD_DATA, 0x01,
      LCD_CMD_DATA, 0x79,
      LCD_CMD_REGISTER, 0xCB,
      LCD_CMD_DATA, 0x39,
      LCD_CMD_DATA, 0x2C,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x34,
      LCD_CMD_DATA, 0x02,
      LCD_CMD_REGISTER, 0xF7,
      LCD_CMD_DATA, 0x20,
      LCD_CMD_REGISTER, 0xEA,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x00,

      LCD_CMD_REGISTER, POWER_CONTROL_2,              //Power control
      LCD_CMD_DATA, 0x11,                             // SAP[2:0];BT[3:0]
      LCD_CMD_REGISTER, VCOM_CONTROL_1,               // VCM control 1
      LCD_CMD_DATA, 0x34,
      LCD_CMD_DATA, 0x3D,
      LCD_CMD_REGISTER, VCOM_CONTROL_2,               // VCM control 2
      LCD_CMD_DATA, 0xC0,
      LCD_CMD_REGISTER, MEMORY_ACCESS_CONTROL,        // Memory Access Control
      LCD_CMD_DATA, 0x08,
      LCD_CMD_REGISTER, PIXEL_FORMAT_SET,             // Pixel format
      LCD_CMD_DATA, 0x66,                             // 18bit
      LCD_CMD_REGISTER, FRAME_CONTROL_IN_NORMAL_MODE, // Frame rate
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x13,                             // 100Hz
      LCD_CMD_REGISTER, DISPLAY_FUNCTION_CONTROL,     // Display Function Control
      LCD_CMD_DATA, 0x0A,
      LCD_CMD_DATA, 0xA2,
      LCD_CMD_DATA, 0x27,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, ENTRY_MODE_SET,               // Entry mode
      LCD_CMD_DATA, 0x07,
      // Factory command
      LCD_CMD_REGISTER, 0xF2,                         // 3Gamma Function Disable
      LCD_CMD_DATA, 0x08,
      LCD_CMD_REGISTER, GAMMA_SET,                    // Gamma curve selected
      LCD_CMD_DATA, 0x01,
      LCD_CMD_REGISTER, POSITIVE_GAMMA_CORRECTION,    // Positive gamma correction
      LCD_CMD_DATA, 0x1f,
      LCD_CMD_DATA, 0x1a,
      LCD_CMD_DATA, 0x18,
      LCD_CMD_DATA, 0x0a,
      LCD_CMD_DATA, 0x0f,
      LCD_CMD_DATA, 0x06,
      LCD_CMD_DATA, 0x45,
      LCD_CMD_DATA, 0x87,
      LCD_CMD_DATA, 0x32,
      LCD_CMD_DATA, 0x0a,
      LCD_CMD_DATA, 0x07,
      LCD_CMD_DATA, 0x02,
      LCD_CMD_DATA, 0x07,
      LCD_CMD_DATA, 0x05,
      LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, NEGATIVE_GAMMA_CORRECTION,    // Negative gamma correction
      LCD_CMD_DATA, 0x00,
      LCD_CMD_DATA, 0x25,
      LCD_CMD_DATA, 0x27,
      LCD_CMD_DATA, 0x05,
      LCD_CMD_DATA, 0x10,
      LCD_CMD_DATA, 0x09,
      LCD_CMD_DATA, 0x3a,
      LCD_CMD_DATA, 0x78,
      LCD_CMD_DATA, 0x4d,
      LCD_CMD_DATA, 0x05,
      LCD_CMD_DATA, 0x18,
      LCD_CMD_DATA, 0x0d,
      LCD_CMD_DATA, 0x38,
      LCD_CMD_DATA, 0x3a,
      LCD_CMD_DATA, 0x1f,
*/
      LCD_CMD_REGISTER, SLEEP_OUT,                    //Exit Sleep
      LCD_CMD_DELAY, 120,
      LCD_CMD_REGISTER, DISPLAY_ON,                   //Display on
      LCD_CMD_DELAY, 50,
      0
    }
  }
  ,
  { // ST7735S
    { // Horizontal
      LCD_CMD_REGISTER, 0x11,
      LCD_CMD_DELAY,    18,
      // ST7735S Frame Rate-
      LCD_CMD_REGISTER, 0xB1, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D,
      LCD_CMD_REGISTER, 0xB2, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D,
      LCD_CMD_REGISTER, 0xB3, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D,
      // End ST7735S Frame Rate
      LCD_CMD_REGISTER, 0xB4, LCD_CMD_DATA, 0x03,
      LCD_CMD_REGISTER, 0xC0, LCD_CMD_DATA, 0x28, LCD_CMD_DATA, 0x08, LCD_CMD_DATA, 0x04,
      LCD_CMD_REGISTER, 0xC1, LCD_CMD_DATA, 0XC0,
      LCD_CMD_REGISTER, 0xC2, LCD_CMD_DATA, 0x0D, LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, 0xC3, LCD_CMD_DATA, 0x8D, LCD_CMD_DATA, 0x2A,
      LCD_CMD_REGISTER, 0xC4, LCD_CMD_DATA, 0x8D, LCD_CMD_DATA, 0xEE,
      // End ST7735S Power Sequence
      LCD_CMD_REGISTER, 0xC5, LCD_CMD_DATA, 0x1A,
      //MX, MY, RGB mode
      LCD_CMD_REGISTER, 0x36, LCD_CMD_DATA, (0x0A << 4) | 0x0,
      // ST7735S Gamma Sequence
      LCD_CMD_REGISTER, 0xE0,
      LCD_CMD_DATA,     0x04, LCD_CMD_DATA, 0x22, LCD_CMD_DATA, 0x07, LCD_CMD_DATA, 0x0A, LCD_CMD_DATA, 0x2E, LCD_CMD_DATA, 0x30, LCD_CMD_DATA, 0x25, LCD_CMD_DATA, 0x2A,
      LCD_CMD_DATA,     0x28, LCD_CMD_DATA, 0x26, LCD_CMD_DATA, 0x2E, LCD_CMD_DATA, 0x3A, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x03, LCD_CMD_DATA, 0x13,
      LCD_CMD_REGISTER, 0xE1,
      LCD_CMD_DATA,     0x04, LCD_CMD_DATA, 0x16, LCD_CMD_DATA, 0x06, LCD_CMD_DATA, 0x0D, LCD_CMD_DATA, 0x2D, LCD_CMD_DATA, 0x26, LCD_CMD_DATA, 0x23, LCD_CMD_DATA, 0x27,
      LCD_CMD_DATA,     0x27, LCD_CMD_DATA, 0x25, LCD_CMD_DATA, 0x2D, LCD_CMD_DATA, 0x3B, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x04, LCD_CMD_DATA, 0x13,
      // End ST7735S Gamma Sequence
      // 262K mode
      LCD_CMD_REGISTER, 0x3A, LCD_CMD_DATA, 0x06,
      // Display on
      LCD_CMD_REGISTER, 0x29,
      LCD_CMD_DELAY,    0x1,
      0,
    },
    { // Vertical
      LCD_CMD_REGISTER, 0x11,
      LCD_CMD_DELAY,    18,
      // ST7735S Frame Rate-
      LCD_CMD_REGISTER, 0xB1, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D,
      LCD_CMD_REGISTER, 0xB2, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D,
      LCD_CMD_REGISTER, 0xB3, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x2C, LCD_CMD_DATA, 0x2D,
      // End ST7735S Frame Rate
      LCD_CMD_REGISTER, 0xB4, LCD_CMD_DATA, 0x03,
      LCD_CMD_REGISTER, 0xC0, LCD_CMD_DATA, 0x28, LCD_CMD_DATA, 0x08, LCD_CMD_DATA, 0x04,
      LCD_CMD_REGISTER, 0xC1, LCD_CMD_DATA, 0XC0,
      LCD_CMD_REGISTER, 0xC2, LCD_CMD_DATA, 0x0D, LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, 0xC3, LCD_CMD_DATA, 0x8D, LCD_CMD_DATA, 0x2A,
      LCD_CMD_REGISTER, 0xC4, LCD_CMD_DATA, 0x8D, LCD_CMD_DATA, 0xEE,
      // End ST7735S Power Sequence
      LCD_CMD_REGISTER, 0xC5, LCD_CMD_DATA, 0x1A,
      //MX, MY, RGB mode
      LCD_CMD_REGISTER, 0x36, LCD_CMD_DATA, 0/*(0x0A << 4)*/ | 0x0,
      // ST7735S Gamma Sequence
      LCD_CMD_REGISTER, 0xE0,
      LCD_CMD_DATA,     0x04, LCD_CMD_DATA, 0x22, LCD_CMD_DATA, 0x07, LCD_CMD_DATA, 0x0A, LCD_CMD_DATA, 0x2E, LCD_CMD_DATA, 0x30, LCD_CMD_DATA, 0x25, LCD_CMD_DATA, 0x2A,
      LCD_CMD_DATA,     0x28, LCD_CMD_DATA, 0x26, LCD_CMD_DATA, 0x2E, LCD_CMD_DATA, 0x3A, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x03, LCD_CMD_DATA, 0x13,
      LCD_CMD_REGISTER, 0xE1,
      LCD_CMD_DATA,     0x04, LCD_CMD_DATA, 0x16, LCD_CMD_DATA, 0x06, LCD_CMD_DATA, 0x0D, LCD_CMD_DATA, 0x2D, LCD_CMD_DATA, 0x26, LCD_CMD_DATA, 0x23, LCD_CMD_DATA, 0x27,
      LCD_CMD_DATA,     0x27, LCD_CMD_DATA, 0x25, LCD_CMD_DATA, 0x2D, LCD_CMD_DATA, 0x3B, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x04, LCD_CMD_DATA, 0x13,
      // End ST7735S Gamma Sequence
      // 262K mode
      LCD_CMD_REGISTER, 0x3A, LCD_CMD_DATA, 0x06,
      // Display on
      LCD_CMD_REGISTER, 0x29,
      LCD_CMD_DELAY,    0x1,
      0,
    }
  },
  { // S6D0154 Parallel.
    { // Horizontal
      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x1A,
      LCD_CMD_REGISTER, 0x12, LCD_CMD_DATA, 0x31, LCD_CMD_DATA, 0x21,
      LCD_CMD_REGISTER, 0x13, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x0E,
      LCD_CMD_REGISTER, 0x14, LCD_CMD_DATA, 0x42, LCD_CMD_DATA, 0x49,

      LCD_CMD_REGISTER, 0x10, LCD_CMD_DATA, 0x08, LCD_CMD_DATA, 0x00,
      LCD_CMD_DELAY,    0x1A,

      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x1A,

      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x03, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x1A,

      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x07, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x1A,

      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x0F, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x1A,

      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x0F, LCD_CMD_DATA, 0x3A,
      LCD_CMD_DELAY,    0x2E,

      LCD_CMD_REGISTER, 0x01, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x28,
      LCD_CMD_REGISTER, 0x02, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x00,
      /*
      R/W RS IB15 IB14 IB13 IB12 IB11 IB10 IB9  IB8  IB7 IB6 IB5 IB4 IB3 IB2 IB1 IB0
      W   1  0    0    0    BGR  0    0    MDT1 MDT0 0   0   ID1 ID0 AM  0   0   0
      */                                   //12
      LCD_CMD_REGISTER, 0x03, LCD_CMD_DATA, 0x12, LCD_CMD_DATA, 0x28, // BGR format,
      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x10, LCD_CMD_DATA, 0x12,
      LCD_CMD_REGISTER, 0x08, LCD_CMD_DATA, 0x03, LCD_CMD_DATA, 0x03,
      LCD_CMD_REGISTER, 0x0B, LCD_CMD_DATA, 0x11, LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, 0x0C, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, 0x0F, LCD_CMD_DATA, 0x18, LCD_CMD_DATA, 0x01,
      LCD_CMD_REGISTER, 0x15, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x20,

      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x12,
      LCD_CMD_DELAY,    0x38,

      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x13,
      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x17,

      LCD_CMD_REGISTER, 0x20, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,     // Xo
      LCD_CMD_REGISTER, 0x21, LCD_CMD_DATA, 319 >> 8, LCD_CMD_DATA, 319,  // Yo
      LCD_CMD_REGISTER, 0x37, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,     // X1
      LCD_CMD_REGISTER, 0x39, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,     // Y1
      LCD_CMD_REGISTER, 0x36, LCD_CMD_DATA, 239 >> 8, LCD_CMD_DATA, 239,  // X2
      LCD_CMD_REGISTER, 0x38, LCD_CMD_DATA, 319 >> 8, LCD_CMD_DATA, 319,  // Y2

      LCD_CMD_REGISTER, 0x22,
      LCD_CMD_DELAY,    0x11,
      0,
    },
    { // Vertical
      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x1A,
      LCD_CMD_REGISTER, 0x12, LCD_CMD_DATA, 0x31, LCD_CMD_DATA, 0x21,
      LCD_CMD_REGISTER, 0x13, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x0E,
      LCD_CMD_REGISTER, 0x14, LCD_CMD_DATA, 0x42, LCD_CMD_DATA, 0x49,

      LCD_CMD_REGISTER, 0x10, LCD_CMD_DATA, 0x08, LCD_CMD_DATA, 0x00,
      LCD_CMD_DELAY,    0x10,
      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x10,
      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x03, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x10,

      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x07, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x10,
      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x0F, LCD_CMD_DATA, 0x1A,
      LCD_CMD_DELAY,    0x10,
      LCD_CMD_REGISTER, 0x11, LCD_CMD_DATA, 0x0F, LCD_CMD_DATA, 0x3A,
      LCD_CMD_DELAY,    0x30,

      LCD_CMD_REGISTER, 0x01, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x28,
      LCD_CMD_REGISTER, 0x02, LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x00,
      /*
      R/W RS IB15 IB14 IB13 IB12 IB11 IB10 IB9  IB8  IB7 IB6 IB5 IB4 IB3 IB2 IB1 IB0
      W   1  0    0    0    BGR  0    0    MDT1 MDT0 0   0   ID1 ID0 AM  0   0   0
      */
      LCD_CMD_REGISTER, 0x03, LCD_CMD_DATA, 0x12, LCD_CMD_DATA, 0x00,    // BGR format,
      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x10, LCD_CMD_DATA, 0x12,
      LCD_CMD_REGISTER, 0x08, LCD_CMD_DATA, 0x03, LCD_CMD_DATA, 0x03,
      LCD_CMD_REGISTER, 0x0B, LCD_CMD_DATA, 0x11, LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, 0x0C, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,
      LCD_CMD_REGISTER, 0x0F, LCD_CMD_DATA, 0x18, LCD_CMD_DATA, 0x01,
      LCD_CMD_REGISTER, 0x15, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x20,

      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x12,
      LCD_CMD_DELAY,    0x40,

      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x13,     /* GRAM Address Set */
      LCD_CMD_REGISTER, 0x07, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x17,     /* Display Control DISPLAY ON */

      LCD_CMD_REGISTER, 0x20, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,     // Xo
      LCD_CMD_REGISTER, 0x21, LCD_CMD_DATA, 319 >> 8, LCD_CMD_DATA, 319,  // Yo
      LCD_CMD_REGISTER, 0x37, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,     // X1
      LCD_CMD_REGISTER, 0x39, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00,     // Y1
      LCD_CMD_REGISTER, 0x36, LCD_CMD_DATA, 239 >> 8, LCD_CMD_DATA, 239,  // X2
      LCD_CMD_REGISTER, 0x38, LCD_CMD_DATA, 319 >> 8, LCD_CMD_DATA, 319,  // Y2

      LCD_CMD_REGISTER, 0x22,
      LCD_CMD_DELAY,    0x1,
      0,
    }
  },
  { // Parallel HX8340B
    { // Horizontal
      LCD_CMD_REGISTER, 0xC1,                                                             //  1: ???, 3 args, no delay
      LCD_CMD_DATA, 0xFF, LCD_CMD_DATA, 0x83, LCD_CMD_DATA, 0x40,
      LCD_CMD_REGISTER, 0x11,                                                             //  2: No args, delay follows
      LCD_CMD_DELAY, 150,                                                                 //     150 ms delay
      LCD_CMD_REGISTER, 0xCA,                                                             //  3: Undoc'd register?  3 args, no delay
      LCD_CMD_DATA, 0x70, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0xD9,
      LCD_CMD_REGISTER, 0xB0,                                                             //  4: Undoc'd register?  2 args, no delay
      LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x11,
      LCD_CMD_REGISTER, 0xC9,                                                             //  5: Drive ability, 8 args + delay
      LCD_CMD_DATA, 0x90, LCD_CMD_DATA, 0x49, LCD_CMD_DATA, 0x10, LCD_CMD_DATA, 0x28,
      LCD_CMD_DATA, 0x28, LCD_CMD_DATA, 0x10, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x06,
      LCD_CMD_DELAY, 20,                                                                  //     20 ms delay
      LCD_CMD_REGISTER, 0xC2,                                                             //  6: Positive gamma control, 9 args
      LCD_CMD_DATA, 0x60, LCD_CMD_DATA, 0x71, LCD_CMD_DATA, 0x01,                         //     2.2
      LCD_CMD_DATA, 0x0E, LCD_CMD_DATA, 0x05, LCD_CMD_DATA, 0x02,
      LCD_CMD_DATA, 0x09, LCD_CMD_DATA, 0x31, LCD_CMD_DATA, 0x0A,
      LCD_CMD_REGISTER, 0xC3,                                                             //  7: Negative gamma, 8 args + delay
      LCD_CMD_DATA, 0x67, LCD_CMD_DATA, 0x30, LCD_CMD_DATA, 0x61, LCD_CMD_DATA, 0x17,     //     2.2
      LCD_CMD_DATA, 0x48, LCD_CMD_DATA, 0x07, LCD_CMD_DATA, 0x05, LCD_CMD_DATA, 0x33,
      LCD_CMD_DELAY, 10,                                                                  //     10 ms delay
      LCD_CMD_REGISTER, 0xB5,                                                             //  8: Power Control 5, 3 args
      LCD_CMD_DATA, 0x35, LCD_CMD_DATA, 0x20, LCD_CMD_DATA, 0x45,
      LCD_CMD_REGISTER, 0xB4,                                                             //  9: Power control 4, 3 args + delay
      LCD_CMD_DATA, 0x33, LCD_CMD_DATA, 0x25, LCD_CMD_DATA, 0x4c,
      LCD_CMD_DELAY, 10,                                                                  //     10 ms delay
      LCD_CMD_REGISTER, 0x3A,                                                             // 10: Color Mode, 1 arg
      LCD_CMD_DATA, 0x05,                                                                 //     0x05 = 16bpp, 0x06 = 18bpp*/
      LCD_CMD_REGISTER, 0x29,                                                             // 11: Display on, no args, w/delay
      LCD_CMD_DELAY, 10,                                                                  //     10 ms delay
      LCD_CMD_REGISTER, 0x2A,                                                             // 12: Physical column pointer, 4 args
      LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0xaf,     //     175 (max X)
      LCD_CMD_REGISTER, 0x2B,                                                             // 13: Physical page pointer, 4 args
      LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0xdb,     //     219 (max Y)
      LCD_CMD_REGISTER, 0x2C,                                                             // 14: Start GRAM write
      LCD_CMD_DELAY,    0x1,
      0,
    },
    { // Vertical
      LCD_CMD_REGISTER, 0xC1,                                                             //  1: ???, 3 args, no delay
      LCD_CMD_DATA, 0xFF, LCD_CMD_DATA, 0x83, LCD_CMD_DATA, 0x40,
      LCD_CMD_REGISTER, 0x11,                                                             //  2: No args, delay follows
      LCD_CMD_DELAY, 150,                                                                 //     150 ms delay
      LCD_CMD_REGISTER, 0xCA,                                                             //  3: Undoc'd register?  3 args, no delay
      LCD_CMD_DATA, 0x70, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0xD9,
      LCD_CMD_REGISTER, 0xB0,                                                             //  4: Undoc'd register?  2 args, no delay
      LCD_CMD_DATA, 0x01, LCD_CMD_DATA, 0x11,
      LCD_CMD_REGISTER, 0xC9,                                                             //  5: Drive ability, 8 args + delay
      LCD_CMD_DATA, 0x90, LCD_CMD_DATA, 0x49, LCD_CMD_DATA, 0x10, LCD_CMD_DATA, 0x28,
      LCD_CMD_DATA, 0x28, LCD_CMD_DATA, 0x10, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x06,
      LCD_CMD_DELAY, 20,                                                                  //     20 ms delay
      LCD_CMD_REGISTER, 0xC2,                                                             //  6: Positive gamma control, 9 args
      LCD_CMD_DATA, 0x60, LCD_CMD_DATA, 0x71, LCD_CMD_DATA, 0x01,                         //     2.2
      LCD_CMD_DATA, 0x0E, LCD_CMD_DATA, 0x05, LCD_CMD_DATA, 0x02,
      LCD_CMD_DATA, 0x09, LCD_CMD_DATA, 0x31, LCD_CMD_DATA, 0x0A,
      LCD_CMD_REGISTER, 0xC3,                                                             //  7: Negative gamma, 8 args + delay
      LCD_CMD_DATA, 0x67, LCD_CMD_DATA, 0x30, LCD_CMD_DATA, 0x61, LCD_CMD_DATA, 0x17,     //     2.2
      LCD_CMD_DATA, 0x48, LCD_CMD_DATA, 0x07, LCD_CMD_DATA, 0x05, LCD_CMD_DATA, 0x33,
      LCD_CMD_DELAY, 10,                                                                  //     10 ms delay
      LCD_CMD_REGISTER, 0xB5,                                                             //  8: Power Control 5, 3 args
      LCD_CMD_DATA, 0x35, LCD_CMD_DATA, 0x20, LCD_CMD_DATA, 0x45,
      LCD_CMD_REGISTER, 0xB4,                                                             //  9: Power control 4, 3 args + delay
      LCD_CMD_DATA, 0x33, LCD_CMD_DATA, 0x25, LCD_CMD_DATA, 0x4c,
      LCD_CMD_DELAY, 10,                                                                  //     10 ms delay
      LCD_CMD_REGISTER, 0x3A,                                                             // 10: Color Mode, 1 arg
      LCD_CMD_DATA, 0x05,                                                                 //     0x05 = 16bpp, 0x06 = 18bpp
      LCD_CMD_REGISTER, 0x29,                                                             // 11: Display on, no args, w/delay
      LCD_CMD_DELAY, 10,                                                                  //     10 ms delay
      LCD_CMD_REGISTER, 0x2A,                                                             // 12: Physical column pointer, 4 args
      LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0xaf,     //     175 (max X)
      LCD_CMD_REGISTER, 0x2B,                                                             // 13: Physical page pointer, 4 args
      LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0x00, LCD_CMD_DATA, 0xdb,     //     219 (max Y)
      LCD_CMD_REGISTER, 0x2C,                                                             // 14: Start GRAM write
      LCD_CMD_DELAY,    0x1,
      0
    }
  }
};

// /////////////////////////////////////////////////////////////////////////////
void LCD_Parallel_Port_Setup
(
  uint8_t pPort   // port number (LCD 0 or 1)
)
{
  /*
  DESCRIPTION:  Sets the FSMC port that the LCD is connected to, plus the driver lines.
  PARMS:        See above .
  RETURNS:      Nothing.
  */


  uint32_t lPinPosition;

  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOH) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOH;

  // Configure the Reset pin that is common to both LCDs
  lPinPosition = (SYS_LCD_RESET_PIN_NO * 2);
  SYS_LCD_RESET_PER->MODER   &= ~GPIO_MODER_MODER7;
  SYS_LCD_RESET_PER->MODER   |= (((uint32_t)GPIO_Mode_OUT) << lPinPosition);
  SYS_LCD_RESET_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7;
  SYS_LCD_RESET_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_100MHz) << lPinPosition);
  SYS_LCD_RESET_PER->OTYPER  &= ~GPIO_OTYPER_OT_7;
  SYS_LCD_RESET_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_LCD_RESET_PIN_NO);
  SYS_LCD_RESET_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR7;
  SYS_LCD_RESET_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);

  /*
   * The back light of each LCDs is driven by the PWM channel that runs
   * at 60Hz. So this is the way used to calculate the TIM parameters
   * 1 tick = 5,55ns (180 Mhz)
   *
   */

  if ((RCC->APB2ENR & RCC_APB2Periph_TIM1) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_TIM1;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOE) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOE;

  if (pPort & LCD_1_ID)
  {
    lPinPosition = (SYS_LCD_1_BL_PWM_PIN_NO * 2);
    SYS_LCD_1_BL_PWM_PER->MODER   &= ~GPIO_MODER_MODER13;
    SYS_LCD_1_BL_PWM_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
    SYS_LCD_1_BL_PWM_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13;
    SYS_LCD_1_BL_PWM_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_50MHz) << lPinPosition);
    SYS_LCD_1_BL_PWM_PER->OTYPER  &= ~GPIO_OTYPER_OT_13;
    SYS_LCD_1_BL_PWM_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_LCD_1_BL_PWM_PIN_NO);
    SYS_LCD_1_BL_PWM_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR13;
    SYS_LCD_1_BL_PWM_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);
    SYS_LCD_1_BL_PWM_PER->AFR[1] &= ~((uint32_t)(0xF << ((SYS_LCD_1_BL_PWM_PIN_NO - 8) * 4)));
    SYS_LCD_1_BL_PWM_PER->AFR[1] |=  (uint32_t)(SYS_LCD_BL_PWM_TIMER_AF << ((SYS_LCD_1_BL_PWM_PIN_NO - 8) * 4));

  } // end if

  if (pPort & LCD_2_ID)
  {
    lPinPosition = (SYS_LCD_2_BL_PWM_PIN_NO * 2);
    SYS_LCD_2_BL_PWM_PER->MODER   &= ~GPIO_MODER_MODER14;
    SYS_LCD_2_BL_PWM_PER->MODER   |= (((uint32_t)GPIO_Mode_AF) << lPinPosition);
    SYS_LCD_2_BL_PWM_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR14;
    SYS_LCD_2_BL_PWM_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_50MHz) << lPinPosition);
    SYS_LCD_2_BL_PWM_PER->OTYPER  &= ~GPIO_OTYPER_OT_14;
    SYS_LCD_2_BL_PWM_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_LCD_2_BL_PWM_PIN_NO);
    SYS_LCD_2_BL_PWM_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR14;
    SYS_LCD_2_BL_PWM_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);
    SYS_LCD_2_BL_PWM_PER->AFR[1] &= ~((uint32_t)(0xF << ((SYS_LCD_2_BL_PWM_PIN_NO - 8) * 4)));
    SYS_LCD_2_BL_PWM_PER->AFR[1] |=  (uint32_t)(SYS_LCD_BL_PWM_TIMER_AF << ((SYS_LCD_2_BL_PWM_PIN_NO - 8) * 4));

  } // end if

  /*
   * 60Hz frequency
   * 1/60 = 16ms
   * TIM1 45Mhz MAX. 22us x cycle
   * cycles x complete PWM sample: 16000/22 = 727, this is the period to use.
   *
   */

  uint32_t TIMER_Frequency = ARCADEIT_SYS_CLOCK;
  uint32_t COUNTER_Frequency = LCD_BACKLIGHT_PWM_STEPS * LCD_BACKLIGHT_PWM_FREQUENCY;
  uint32_t PSC_Value = (TIMER_Frequency / COUNTER_Frequency) - 1;
  uint16_t ARR_Value = LCD_BACKLIGHT_PWM_STEPS - 1;

  TIM1->CR1 &= (uint16_t)(~(TIM_CR1_DIR | TIM_CR1_CMS));
  TIM1->CR1 &=  (uint16_t)(~TIM_CR1_CKD);
  TIM1->CR1 |= (uint32_t)TIM_CKD_DIV4;

  TIM1->ARR = ARR_Value;
  TIM1->PSC = PSC_Value;
  TIM1->RCR = 0;

  TIM1->EGR = TIM_PSCReloadMode_Immediate;

  TIM1->CCER |=   (((pPort & LCD_1_ID) ? TIM_CCER_CC3E   : 0) | ((pPort & LCD_2_ID) ? TIM_CCER_CC4E   : 0));
  TIM1->CCER &=  ~(((pPort & LCD_1_ID) ? TIM_CCER_CC3P   : 0) | ((pPort & LCD_2_ID) ? TIM_CCER_CC4P   : 0));
  TIM1->CCER |=   (((pPort & LCD_1_ID) ? TIM_CCER_CC3P   : 0) | ((pPort & LCD_2_ID) ? TIM_CCER_CC4P   : 0));

  TIM1->CCMR2 &= ~(((pPort & LCD_1_ID) ? TIM_CCMR2_OC3CE : 0) | ((pPort & LCD_2_ID) ? TIM_CCMR2_OC4CE : 0));
  TIM1->CCMR2 &= ~(((pPort & LCD_1_ID) ? TIM_CCMR2_OC3M  : 0) | ((pPort & LCD_2_ID) ? TIM_CCMR2_OC4M  : 0));
  TIM1->CCMR2 |=  (((pPort & LCD_1_ID) ? TIM_OC3Mode_PWM : 0) | ((pPort & LCD_2_ID) ? TIM_OC4Mode_PWM : 0));
  TIM1->CCMR2 |=  (((pPort & LCD_1_ID) ? TIM_CCMR2_OC3PE : 0) | ((pPort & LCD_2_ID) ? TIM_CCMR2_OC4PE : 0));
  TIM1->CCMR2 &= ~(((pPort & LCD_1_ID) ? TIM_CCMR2_OC3FE : 0) | ((pPort & LCD_2_ID) ? TIM_CCMR2_OC4FE : 0));
  TIM1->CCMR2 &= ~(((pPort & LCD_1_ID) ? TIM_CCMR2_CC3S  : 0) | ((pPort & LCD_2_ID) ? TIM_CCMR2_CC4S  : 0));

  TIM1->CR2 |=    (((pPort & LCD_1_ID) ? TIM_CR2_OIS3    : 0) | ((pPort & LCD_2_ID) ? TIM_CR2_OIS4    : 0));

  TIM1->CCR3 = 0;
  TIM1->CCR4 = 0;

  TIM1->BDTR = 0;
  TIM1->BDTR |= TIM_BDTR_MOE;

  TIM1->CR2 |= TIM_CR2_CCPC;

  TIM1->CR1 |= TIM_CR1_CEN;

  // Shows a message to serial port as debug
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    // Starts and configure the serial port.
    ArcadeIT_Serial_Port_String_Send("LCD hardware setup." CURSOR_NEWLINE);

  } // End if.

} // End LCD_Parallel_Port_Setup.

// /////////////////////////////////////////////////////////////////////////////
void LCD_Wr_Data
(
  uint8_t pLCDId,  // The id of the display.
  uint8_t pData    // The data byte to write.
)
{
  /*
  DESCRIPTION:  Write data to the LCD.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  __IO uint8_t* lLCDPtr = gLCDDriver[pLCDId].address;

  lLCDPtr[LCD_DATA] = pData;

} // End LCD_Wr_Data.

// /////////////////////////////////////////////////////////////////////////////
void LCD_Wr_Register
(
  uint8_t pLCDId,    // The id of the display.
  uint8_t pRegister  // The register byte select.
)
{
  /*
  DESCRIPTION:  Select a register to the LCD.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  __IO uint8_t* lLCDPtr = gLCDDriver[pLCDId].address;

  lLCDPtr[LCD_REGISTER] = pRegister;

} // End LCD_Wr_Register. 

// /////////////////////////////////////////////////////////////////////////////
void LCD_Area_Set
(
  uint8_t pLCDId, // The id of the display.
  uint16_t pX1,
  uint16_t pY1,
  uint16_t pX2,
  uint16_t pY2
)
{  
  /*
  DESCRIPTION:  Sets the active area of the LCD.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  #if (LCD_CONTROLLER == ILI9341)
  LCD_Wr_Register(pLCDId, COLUMN_ADDRESS_SET);
  LCD_Wr_Data(pLCDId, pX1 >> 8);
  LCD_Wr_Data(pLCDId, pX1);
  LCD_Wr_Data(pLCDId, pX2 >> 8);
  LCD_Wr_Data(pLCDId, pX2);

  LCD_Wr_Register(pLCDId, PAGE_ADDRESS_SET);
  LCD_Wr_Data(pLCDId, pY1 >> 8);
  LCD_Wr_Data(pLCDId, pY1);
  LCD_Wr_Data(pLCDId, pY2 >> 8);
  LCD_Wr_Data(pLCDId, pY2);

  LCD_Wr_Register(pLCDId, MEMORY_WRITE);
  #endif

  #if (LCD_CONTROLLER == ST3577S)
  LCD_Wr_Register(pLCDId, 0x2a);
  LCD_Wr_Data(pLCDId, pX1 >> 8);
  LCD_Wr_Data(pLCDId, pX1);
  LCD_Wr_Data(pLCDId, pX2 >> 8);
  LCD_Wr_Data(pLCDId, pX2);

  LCD_Wr_Register(pLCDId, 0x2b);
  LCD_Wr_Data(pLCDId, pY1 >> 8);
  LCD_Wr_Data(pLCDId, pY1);
  LCD_Wr_Data(pLCDId, pY2 >> 8);
  LCD_Wr_Data(pLCDId, pY2);

  LCD_Wr_Register(pLCDId, 0x2c);
  #endif

  #if (LCD_CONTROLLER == S6D0154)
  LCD_Wr_Register(pLCDId, 0x37);
  LCD_Wr_Data(pLCDId, pX1 >> 8);
  LCD_Wr_Data(pLCDId, pX1);

  LCD_Wr_Register(pLCDId, 0x36);
  LCD_Wr_Data(pLCDId, pX2 >> 8);
  LCD_Wr_Data(pLCDId, pX2);

  LCD_Wr_Register(pLCDId, 0x39);
  LCD_Wr_Data(pLCDId, pY1 >> 8);
  LCD_Wr_Data(pLCDId, pY1);

  LCD_Wr_Register(pLCDId, 0x38);
  LCD_Wr_Data(pLCDId, pY2 >> 8);
  LCD_Wr_Data(pLCDId, pY2);

  LCD_Wr_Register(pLCDId, 0x22);
  #endif

} // End LCD_Area_Set.

// /////////////////////////////////////////////////////////////////////////////
void LCD_Clear
(
  uint8_t pLCDId, // The id of the display.
  uint32_t pColor // The Color in hex format 0x00RRGGBB.
)
{
  /*
  DESCRIPTION:  Clear a display filling with black.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  uint8_t lRed, lGreen, lBlue;

  lRed = (( pColor >> 16 ) & 0xFF );
  lGreen = (( pColor >> 8 ) & 0xFF );
  lBlue = (( pColor ) & 0xFF );

  __IO uint8_t* lLCDPtr = gDisplayAddress[pLCDId];

  uint16_t y, x, c = RGB_TO_16BIT565(lRed, lGreen, lBlue);
  for(y = 0; y < gLCDDriver[pLCDId].height; y++)
  {
    for (x = 0; x < gLCDDriver[pLCDId].width; x++)
    {
      	lLCDPtr[LCD_DATA] = c >> 8;
      	lLCDPtr[LCD_DATA] = c & 0xFF;

    } // End for.

  } // End for.

} // End LCD_Clear.

// /////////////////////////////////////////////////////////////////////////////
void LCD_Reset(void)
{
  /*
  DESCRIPTION:  Resets the LCDs by asserting and deasserting the Reset line.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  ARCADEIT_LCD_RST_HIGH();
  ArcadeIT_System_Delay(1);   // Delay 1ms

  ARCADEIT_LCD_RST_LOW();
  ArcadeIT_System_Delay(20);  // Delay 20ms

  ARCADEIT_LCD_RST_HIGH();
  ArcadeIT_System_Delay(120); // Delay 120ms

} // End LCD_Reset.

// /////////////////////////////////////////////////////////////////////////////
void LCD_Configure
(
  uint8_t pLCDNumber,     // Id of the LCD to set orientation.
  uint16_t *pData
)
{
  /*
  DESCRIPTION:  Initializes the LCD sending the commands sequence to set orientation.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  uint16_t lIndex = 0;

  // Select the LCD.
  while ( pData[lIndex] > 0 )
  {
      switch (pData[lIndex])
      {
          case LCD_CMD_REGISTER:
              lIndex++;
              LCD_Wr_Register(pLCDNumber, pData[lIndex]);
          break;

          case LCD_CMD_DATA:
              lIndex++;
              LCD_Wr_Data(pLCDNumber, pData[lIndex]);
          break;

          case LCD_CMD_DELAY:
              lIndex++;
              ArcadeIT_System_Delay(pData[lIndex]);
          break;

      } // End switch.

      lIndex++;

  } // End while;

} // End LCD_Configure.

// /////////////////////////////////////////////////////////////////////////////
void LCD_Parallel_Init
(
  uint8_t pLCDId,              // the id of the LCD.
  uint8_t pLCDType,            // the type of the LCD.
  uint16_t pLCDWidth,          // width of the LCD.
  uint16_t pLCDHeight,         // height of the LCD.
  uint8_t pOrientation,        // Orientation of the LCD: 0 Horizontal, 1: Vertical.
  uint32_t pBackgroundColor    // The Color in hex format 0x00RRGGBB.
)
{
  /*
  DESCRIPTION:  Initializes 23636the LCD.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  pLCDId--;

  gLCDDriver[pLCDId].id = pLCDId;
  gLCDDriver[pLCDId].type = pLCDType;
  gLCDDriver[pLCDId].width = pLCDWidth;
  gLCDDriver[pLCDId].height = pLCDHeight;
  gLCDDriver[pLCDId].orientation = pOrientation;
  gLCDDriver[pLCDId].video_buffer_memory = gLCDLineBuffer;
  gLCDDriver[pLCDId].scanline = 0;
  gLCDDriver[pLCDId].address = gDisplayAddress[pLCDId];

  gLCDDriver[pLCDId].video_buffer_memory = (uint8_t*)malloc(gLCDDriver[pLCDId].width * gLCDDriver[pLCDId].height);

  LCD_Configure(pLCDId, (uint16_t*)&gLCDSequence[pLCDType][pOrientation][0]);

  LCD_Area_Set(pLCDId, 0, 0, 320, 240);

  // Clear the display with the background color.
  LCD_Clear(pLCDId, pBackgroundColor);

  // ---------------------------------------------------------------------------
  // Configure the DMA Stream for fast transfer to LCD using DMA
  // if ((RCC_AHB1ENR & RCC_AHB1Periph_DMA2) == FALSE) RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_DMA2, ENABLE);

  if ((RCC->AHB1ENR & RCC_AHB1Periph_DMA2D) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_DMA2D;

  // Configure the DMA Stream
/*
  DMA_InitTypeDef lDMA_InitStructure;
  NVIC_InitTypeDef lNVIC_InitStructure;

  DMA_Cmd (DMA2_Stream0, DISABLE);
  DMA_DeInit (DMA2_Stream0);

  // Set the parameters to be configured
  lDMA_InitStructure.DMA_Channel = DMA_Channel_0;
  lDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  lDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  lDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
  lDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  lDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  lDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  lDMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  lDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  lDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  lDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  lDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init (DMA2_Stream0, &lDMA_InitStructure);

  // ---------------------------------------------------------------------------
  lNVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  lNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  lNVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  lNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init (&lNVIC_InitStructure);

  //gLCDDriver[0].mirror =  TRUE;
*/



  /*
  NVIC_InitTypeDef lNVIC_InitStructure;
  lNVIC_InitStructure.NVIC_IRQChannel = DMA2D_IRQn;
  lNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  lNVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  lNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init (&lNVIC_InitStructure);
  DMA2D_ITConfig(DMA2D_IT_TC, ENABLE);
  */

}; // End LCD_Parallel_Init.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_LCD_1_Refresh(void)
{
    /*
    DESCRIPTION:  send the buffer to the LCD
    PARMS:        See above.
    RETURNS:      Nothing.
    */

/*
  uint8_t *lPtr1;
  uint16_t *lPtr2;
  uint32_t c = 0;

  lPtr1 = gVideoDriver.video_buffer_show + (gLCDDriver[0].scanline * gLCDDriver[0].width);
  lPtr2 = gLCDDriver[0].video_buffer_memory;

  for (c = 0; c < gLCDDriver[0].width; c++) *lPtr2++ = gLCDDriver[0].palette.colors[*lPtr1++];

  LCD_DMA_Transfer(0, gLCDDriver[0].video_buffer_memory, gLCDDriver[0].width);

  gLCDDriver[0].scanline = 0;
  gLCDDriver[0].refresh = 1;

  LCD_NewFrame(0);


  while (gLCDDriver[0].scanline < gLCDDriver[0].height)
    {
      lPtr1 = gVideoDriver.video_buffer_show + (gLCDDriver[0].scanline * gLCDDriver[0].width);
      lPtr2 = gLCDDriver[0].video_buffer_memory;

      for (c = 0; c < gLCDDriver[0].width; c++)
        *lPtr2++ = gLCDDriver[0].palette.colors[*lPtr1++];

      //while (DMA_GetCmdStatus(DMA2_Stream0));
      LCD_DMA_Transfer(0, gLCDDriver[0].video_buffer_memory, gLCDDriver[0].width);

      gLCDDriver[0].scanline++;
    }
    */
}
// /////////////////////////////////////////////////////////////////////////////
void LCD_DMA_Transfer
(
 uint8_t pLCDId,       // The id of the LCD to set active flag
 uint16_t *pBuffer,
 uint32_t pSize
)
{
    /*
    DESCRIPTION:  send the buffer to the LCD
    PARMS:        See above.
    RETURNS:      Nothing.
    */

  /* Start DMA2D transfer by setting START bit */
  DMA2D->CR |= (uint32_t)DMA2D_CR_START;

}
// /////////////////////////////////////////////////////////////////////////////
void
DMA2_Stream0_IRQHandler (void)
{
  /*
   DESCRIPTION:  Interrupt service routine for the scanline renderer.
   PARMS:        See above.
   RETURNS:      Nothing.
   */


/*
  __IO uint8_t *lPtr1;
  uint16_t *lPtr2;
  uint32_t c = 0, l = 0;

  //DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);

  DMA2_Stream0->CR &= ~DMA_SxCR_EN;

  lPtr1 = gVideoDriver.video_buffer_show + (gLCDDriver[0].scanline * gLCDDriver[0].width);
  lPtr2 = gLCDDriver[0].video_buffer_memory;

  for (c = 0; c < gLCDDriver[0].width; c++)
    *lPtr2++ = gLCDDriver[0].palette.colors[*lPtr1++];

  gLCDDriver[0].scanline++;
  if (gLCDDriver[0].scanline == gLCDDriver[0].height)
    {
      gLCDDriver[0].scanline = 0;
      LCD_NewFrame(0);
    }
  else
    {
      DMA2_Stream0->M0AR = (uint32_t)gDisplayAddress[0]+LCD_DATA;
      DMA2_Stream0->PAR = (uint32_t)gLCDDriver[0].video_buffer_memory;
      DMA2_Stream0->NDTR = gLCDDriver[0].width;

      DMA2_Stream0->CR |= DMA_SxCR_TCIE;        // enable transfer complete interrupt
      DMA2_Stream0->CR |= DMA_SxCR_EN;
    }
    */
}

// /////////////////////////////////////////////////////////////////////////////
void LCD_Palette_Get
(
 uint8_t pLCDId,        // The id of the LCD to set active flag
 uint16_t *pColors
)
{
  //DMA2D_Suspend(ENABLE);
  // Copy the VDP palette to the screen palette.
  uint16_t lColorId = 0;

  for (lColorId = 0; lColorId < 256; lColorId++)
    {
      //DMA2D->BGCLUT[lColorId] = P12BIT_TO_RGB_DMA2D_LCD_FG(pColors[lColorId]);
      // BACKGROUND IS LCD
      //DMA2D->FGCLUT[lColorId] = P12BIT_TO_RGB_DMA2D_VGA2(0xFFF8);
      // foreground is VGA
      gLCDDriver->palette[lColorId] = P12BIT_TO_16BIT565(pColors[lColorId]);

    } // End for.
/*
  for (lColorId = 0; lColorId < 16; lColorId++)
    {
      //DMA2D->FGCLUT[lColorId]    = ((lColorId<<12) & 0xFFFFFF00) | 0xFF;
      //DMA2D->FGCLUT[lColorId+16] = ((lColorId<<28) & 0xFFFFFF00) | 0xFF;
      DMA2D->FGCLUT[lColorId] = ((lColorId<<12) & 0xFFFFFFFF) | 0xFF;
      // foreground is VGA
    } // End for.
*/
  //DMA2D_Suspend(DISABLE);

/*
  DMA2D->BGCLUT[0] = 0;//P12BIT_TO_RGB_DMA2D_LCD_BG(pColors[lColorId]);
  DMA2D->FGCLUT[0] = 0;//P12BIT_TO_RGB_DMA2D_LCD_FG(pColors[lColorId]);
*/
} // end LCD_Palette_Get

// /////////////////////////////////////////////////////////////////////////////
void LCD_NewFrame
(
  uint8_t pLCDId        // The id of the LCD to set active flag
)
{
  LCD_Area_Set(0,0,0,gLCDDriver->width-1,gLCDDriver->height-1);
  LCD_Wr_Register(pLCDId, MEMORY_WRITE);
}
// /////////////////////////////////////////////////////////////////////////////
void LCD_Active
(
 uint8_t pLCDId,        // The id of the LCD to set active flag
 uint8_t pStatus        // 0:OFF, 1:ON
)
{
    /*
    DESCRIPTION:  Set the active property of the LCD.
    PARMS:        See above.
    RETURNS:      Nothing.
    */

    gLCDDriver[pLCDId].active = pStatus;

} // LCD_Active.

// /////////////////////////////////////////////////////////////////////////////
void LCD_0_Backlight_Set(uint32_t pValue)
{
  gLCDDriver[0].brightness = (pValue <= LCD_BACKLIGHT_PWM_STEPS) ? pValue : LCD_BACKLIGHT_PWM_STEPS;

  TIM1->CCR3 = gLCDDriver[0].brightness;

} // End

// /////////////////////////////////////////////////////////////////////////////
void LCD_0_Backlight_Inc(void)
{
  gLCDDriver[0].brightness++;
  if (gLCDDriver[0].brightness > LCD_BACKLIGHT_PWM_STEPS) gLCDDriver[0].brightness = LCD_BACKLIGHT_PWM_STEPS;

  TIM1->CCR3 = gLCDDriver[0].brightness;

} // End

// /////////////////////////////////////////////////////////////////////////////
void LCD_0_Backlight_Dec(void)
{
  if (gLCDDriver[0].brightness) gLCDDriver[0].brightness--;

  TIM1->CCR3 = gLCDDriver[0].brightness;

} // End

// /////////////////////////////////////////////////////////////////////////////
void LCD_1_Backlight_Set(uint32_t pValue)
{
  gLCDDriver[1].brightness = (pValue <= LCD_BACKLIGHT_PWM_STEPS) ? pValue : LCD_BACKLIGHT_PWM_STEPS;

  TIM1->CCR4 = gLCDDriver[1].brightness;

} // End

// /////////////////////////////////////////////////////////////////////////////
void LCD_1_Backlight_Inc(void)
{
  gLCDDriver[1].brightness++;
  if (gLCDDriver[1].brightness > LCD_BACKLIGHT_PWM_STEPS) gLCDDriver[1].brightness = LCD_BACKLIGHT_PWM_STEPS;

  TIM1->CCR4 = gLCDDriver[1].brightness;

} // End

// /////////////////////////////////////////////////////////////////////////////
void LCD_1_Backlight_Dec(void)
{
  if (gLCDDriver[1].brightness) gLCDDriver[1].brightness--;

  TIM1->CCR4 = gLCDDriver[1].brightness;

} // End

// /////////////////////////////////////////////////////////////////////////////
void LCD_Backlight_Off
(
  uint8_t pLCD   // The number of the LCD.
)
{
  /*
  DESCRIPTION:  Set the brightness on of the LCD.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  if (gSystems & ARCADEIT_UNIT_SCHEDULER)
  {
    switch (pLCD)
    {
      case 0:
        ArcadeIT_Scheduler_Task_Set(3, (void *)LCD_0_Backlight_Dec, NULL, LCD_BACKLIGHT_PWM_STEPS, 8);
      break;

      case 1:
        ArcadeIT_Scheduler_Task_Set(3, (void *)LCD_1_Backlight_Dec, NULL, LCD_BACKLIGHT_PWM_STEPS, 8);
      break;

    } // End switch.

  } // End if.

} // End LCD_Backlight_Off

// /////////////////////////////////////////////////////////////////////////////
void LCD_Backlight_On
(
  uint8_t pLCD   // The number of the LCD.
)
{
  /*
  DESCRIPTION:  Set the brightness on of the LCD.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  if (gSystems & ARCADEIT_UNIT_SCHEDULER)
  {
    switch (pLCD)
    {
      case 0:
        ArcadeIT_Scheduler_Task_Set(3, (void *)LCD_0_Backlight_Inc, NULL, 90, 2);
      break;

      case 1:
        ArcadeIT_Scheduler_Task_Set(4, (void *)LCD_1_Backlight_Inc, NULL, 90, 2);
      break;

    } // End switch.

  } // End if.

} // End LCD_Backlight_On

// /////////////////////////////////////////////////////////////////////////////
void LCD_Driver_Start(void)
{
  /*
  DESCRIPTION:  Initializes the LCD Driver by configuring the SPI ports and the LCD hardware.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  char lString[256];

  LCD_Parallel_Port_Setup(LCD_1_ID | LCD_2_ID);

  // Hardware Reset of the LCDs.
  LCD_Reset();

  // Initialize the LCDs.
  LCD_Parallel_Init(LCD_1_ID, ARCADEIT_A100_LCD, ARCADEIT_A100_LCD_WIDTH, ARCADEIT_A100_LCD_HEIGHT, HORIZONTAL, 0x000000);
  LCD_Parallel_Init(LCD_2_ID, ARCADEIT_A100_LCD, ARCADEIT_A100_LCD_WIDTH, ARCADEIT_A100_LCD_HEIGHT, HORIZONTAL, 0x000000);

  // Load the boot screen.
  if (gLCDDriver[0].boot_image != NULL)
  {
    // Show a boot image at startup on LCD.
    //ArcadeIT_TGA_Load_LCD(0, 0, gLCDDriver[0].boot_image, -1);
    //ArcadeIT_TGA_Load(0, gDisplayAddress[0], gLCDDriver[0].boot_image, -1, FALSE, TO_LCD);
    //ArcadeIT_TGA_Load(0, gDisplayAddress[0], &EmulatorScreen_Pacman[0], -1, TRUE, TO_LCD);
    //ArcadeIT_TGA_Load_LCD(0, 0, EmulatorScreen_Pacman, -1);
  } // End if.

  // Load the boot screen.
  if (gLCDDriver[1].boot_image != NULL)
  {
    // Show a boot image at startup on LCD.
    //ArcadeIT_TGA_Load_LCD(1, 0, gLCDDriver[1].boot_image, -1);
    //ArcadeIT_TGA_Load(1, gDisplayAddress[1], gLCDDriver[1].boot_image, -1, FALSE, TO_LCD);
    //ArcadeIT_TGA_Load(1, gDisplayAddress[1], &EmulatorScreen_Pacman[1], -1, TRUE, TO_LCD);
    //ArcadeIT_TGA_Load_LCD(1, 0, EmulatorScreen_Pacman, -1);
  } // End if.

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    sprintf(lString, TEXT_LCD_INITED);
    ArcadeIT_Serial_Port_String_Send(lString);

  } // End if.

} // End LCD_Driver_Init.

// /////////////////////////////////////////////////////////////////////////////
