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
 * @file    ArcadeIT_Serial_Port.h
 * @version V0.13
 * @date    16-06-2017
 * @last    11-02-2020
 * @brief   This library is used to drive the Serial port at low level. This
 *          port can be used for the debug of the ArcadeIT! system or for data
 *          transfer.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.3 - Serial port

  ArcadeIT! Motherboard CN7 - Serial Port

  Pin  GPIO Function    Serial interface
  ---------------------------------------
  3.3V VDD  3.3V        CN7-1
  042  PA2  USART2 TX   CN7-2
  047  PA3  USART2 RX   CN7-3
  GND  VSS  GND         CN7-4

 ArcadeIT! mainboard:

  Serial port  / CN7
  .----.
  | o  |  1 3.3V
  | o |   2 TX
  | o |   3 RX
  | o  |  4 GND
  '----'

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

 11-02-2020:
 - Ported to STM32CubeIDE and stored into GitHUB.

 12-02-2020:
 - Cleaned the code that initializes the USART and added more info to the in-code
   documentation.

 *******************************************************************************
 */

// /////////////////////////////////////////////////////////////////////////////
// Includes.
// /////////////////////////////////////////////////////////////////////////////

#ifndef _ARCADEIT_SERIAL_PORT_H_
#define _ARCADEIT_SERIAL_PORT_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions.
// /////////////////////////////////////////////////////////////////////////////
  #define SYS_SERIAL_TX_PER                   GPIOA
  #define SYS_SERIAL_TX_PIN                   GPIO_Pin_2
  #define SYS_SERIAL_TX_PIN_NO                2
  #define SYS_SERIAL_TX_AF                    ((uint8_t)0x07)

  #define SYS_SERIAL_RX_PER                   GPIOA
  #define SYS_SERIAL_RX_PIN                   GPIO_Pin_3
  #define SYS_SERIAL_RX_PIN_NO                3
  #define SYS_SERIAL_RX_AF                    ((uint8_t)0x07)

  #define SYS_SERIAL_IRQ                      USART2_IRQn
  #define SYS_SERIAL_IRQ_HANDLER              USART2_IRQHandler

  #define SYS_SERIAL_PORT                     USART2

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

// VT100 ESCAPE SEQUENCES //////////////////////////////////////////////////////
// Device Status
// -------------
// The following codes are used for reporting terminal/display settings, and vary depending on the implementation:
// Requests a Report Device Code response from the device.
#define QUERY_DEVICE_CODE                "\x1b[c"
// Generated by the device in response to Query Device Code request.
#define REPORT_DEVICE_CODE               "\x1b[{code}0c"
// Requests a Report Device Status response from the device.
#define QUERY_DEVICE_STATUS              "\x1b[5n"
// Generated by the device in response to a Query Device Status request; indicates that device is functioning correctly.
#define REPORT_DEVICE_OK                 "\x1b[0n"
// Generated by the device in response to a Query Device Status request; indicates that device is functioning improperly.
#define REPORT_DEVICE_FAILURE            "\x1b[3n"
// Requests a Report Cursor Position response from the device.
#define QUERY_CURSOR_POSITION            "\x1b[6n"
// Generated by the device in response to a Query Cursor Position request; reports current cursor position.
#define REPORT_CURSOR_POSITION           "\x1b[%d;%dR"

// Terminal Setup
// --------------
// The h and l codes are used for setting terminal/display mode, and vary depending on the implementation. Line Wrap is one of the few setup codes that tend to be used consistently:
// Reset all terminal settings to default.
#define RESET_DEVICE                     "\x1b\x63"
// Text wraps to next line if longer than the length of the display area.
#define ENABLE_LINE_WRAP                 "\x1b[7h"
// Disables line wrapping.
#define DISABLE_LINE_WRAP                "\x1b[7l"

// Fonts
// -----
// Some terminals support multiple fonts: normal/bold, swiss/italic, etc. There are a variety of special codes for certain terminals; the following are fairly standard:
// Set default font.
#define FONT_SET_G0                      "\x1b("
// Set alternate font.
#define FONT_SET_G1                      "\x1b)"
// Gfx mode on.
#define FONT_GFX_ON                      "\x1b(0" // DEC special graphics table as G0
// Gfx mode off.
#define FONT_GFX_OFF                     "\x1b(B" // ASCII table as G0

// Cursor Control
// --------------
// Sets the cursor position where subsequent text will begin.
// If no row/column parameters are provided (ie. "\x1b[H), the cursor will move
// to the home position, at the upper left of the screen.
#define CURSOR_HOME                      "\x1b[\x48"
#define CURSOR_SET                       "\x1b[%d;%d\x48"
// Moves the cursor up by COUNT rows; the default count is 1.
#define CURSOR_UP                        "\x1b[%dA"
// Moves the cursor down by COUNT rows; the default count is 1.
#define CURSOR_DOWN                      "\x1b[%dB"
//Moves the cursor forward by COUNT columns; the default count is 1.
#define CURSOR_FORWARD                   "\x1b[%dC"
// Moves the cursor backward by COUNT columns; the default count is 1.
#define CURSOR_BACKWARD                  "\x1b[%luD"
// Identical to Cursor Home.
#define FORCE_CURSOR_POSITION            "\x1b[%d;%d\x66"
// Save current cursor position.
#define SAVE_CURSOR                      "\x1b[s"
// Restores cursor position after a Save Cursor.
#define UNSAVE_CURSOR                    "\x1b[u"
// Save current cursor position.
#define SAVE_CURSOR_AND_ATTRS            "\x1b\x37"
// Restores cursor position after a Save Cursor.
#define RESTORE_CURSOR_AND_ATTRS         "\x1b\x38"
// Hide the cursor.
#define CURSOR_OFF                       "\x1b[?25l"
// Show the cursor.
#define CURSOR_ON                        "\x1b[?25h"

// Scrolling
// ---------
// Enable scrolling for entire display.
#define SCROLL_SCREEN                    "\x1b[r"
// Enable scrolling from row {start} to row {end}.
#define SCROLL_SCREEN_AREA               "\x1b[%d;%d\x72"
// Scroll display down one line.
#define SCROLL_DOWN                      "\x1bD"
// Scroll display up one line.
#define SCROLL_UP                        "\x1bM"

// Tab Control
// -----------
// Sets a tab at the current position.
#define SET_TAB                          "\x1b\x48"
//Clears tab at the current position.
#define CLEAR_TAB                        "\x1b[g"
// Clears all tabs.
#define CLEAR_ALL_TABS                   "\x1b[3g"

// Erasing Text
// ------------
// Erases from the current cursor position to the end of the current line.
#define ERASE_END_OF_LINE                "\x1b[K"
// Erases from the current cursor position to the start of the current line.
#define ERASE_START_OF_LINE              "\x1b[1K"
// Erases the entire current line.
#define ERASE_LINE                       "\x1b[2K"
// Erases the screen from the current line down to the bottom of the screen.
#define ERASE_DOWN                       "\x1b[J"
// Erases the screen from the current line up to the top of the screen.
#define ERASE_UP                         "\x1b[1J"
// Erases the screen with the background colour and moves the cursor to home.
#define ERASE_SCREEN                     "\x1b[2J"

// Printing
// --------
// Some terminals support local printing:
// Print the current screen.
#define PRINT_SCREEN                     "\x1b[i"
// Print the current line.
#define PRINT_LINE                       "\x1b[1i"
// Disable log.
#define STOP_PRINT_LOG                   "\x1b[4i"
// Start log; all received text is echoed to a printer.
#define START_PRINT_LOG                  "\x1b[5i"

//Define Key
//----------
// Associates a string of text to a keyboard key. {key} indicates the key by its ASCII value in decimal.
#define SET_KEY_DEFINITION               "\x1b[%d;\"%s\"p"

// Set Display Attributes
// ----------------------
// Set Attribute Mode   "\x1b[{attr1};...;{attrn}m
// Sets multiple display attribute settings. The following lists standard attributes:
#define ATTR_RESET_ALL_ATTRIBUTES          0
#define ATTR_BRIGHT                        1
#define ATTR_DIM                           2
#define ATTR_UNDERSCORE                    4
#define ATTR_BLINK                         5
#define ATTR_HIDDEN                        8
#define ATTR_REVERSE                       7
// Foreground Colors.
#define ATTR_COLOR_FG_Red                 31
#define ATTR_COLOR_FG_Green               32
#define ATTR_COLOR_FG_Black               30
#define ATTR_COLOR_FG_Yellow              33
#define ATTR_COLOR_FG_Blue                34
#define ATTR_COLOR_FG_Magenta             35
#define ATTR_COLOR_FG_Cyan                36
#define ATTR_COLOR_FG_White               37
// Background Colors.
#define ATTR_COLOR_BG_Black               40
#define ATTR_COLOR_BG_Red                 41
#define ATTR_COLOR_BG_Green               42
#define ATTR_COLOR_BG_Yellow
#define ATTR_COLOR_BG_Blue                44
#define ATTR_COLOR_BG_Magenta             45
#define ATTR_COLOR_BG_Cyan                46
#define ATTR_COLOR_BG_White               47

// 256 indexed colors.
#define ATTR_COLOR_256_BG                 "\x1B[48;5;%d\x6d"
#define ATTR_COLOR_256_FG                 "\x1B[38;5;%d\x6d"
#define ANSI_COLOR_256_FGBG               "\x1B[38;5;%d\x6d\x1B[48;5;%d\x6d"
#define ANSI_PIXEL_256_FGBG               ANSI_COLOR_256_FGBG

// RGB colors.
#define ATTR_COLOR_RGB_BG                 "\x1B[48;2;%d;%d;%x6d"
#define ATTR_COLOR_RGB_FG                 "\x1B[38;2;%d;%d;%x6d"

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Serial_Port_Init
(
  uint32_t pBaud   // Baud rate to set the serial port to.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Serial_Port_Char_Send
(
  char pCharacter   // The character to send to the serial port.
);
// -----------------------------------------------------------------------------
uint8_t ArcadeIT_Serial_Port_Char_Get(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Serial_Port_String_Send
(
  char *pString   // The string to send character by character to the serial port.
);
// /////////////////////////////////////////////////////////////////////////////

#endif // _ARCADEIT_SERIAL_PORT_H_

/*
 From: RM0386, Reference manual, page 1040

 Figure 327. USART block diagram

 Considering the common use of a serial port in embedded systems, we need only
 two signals to configure, RX and TX. But we need also to configure the clock
 of the USART so that it reaches the desired BAUD rate.

 First, the BAUD is a common measure of symbol rate, one of the components that
 determine the speed of communication over a data channel. That said, the basic
 symbol that can be transmitted is a Bit (1 or 0), so we can consider the BAUD
 rate to correspond to bps, or Bits per second.

 Usually we use to transfer characters or bytes over the serial port. That means
 8 bits of data for each. But actually there are more bits transferred, because
 the system that receives data needs to understand when a byte starts and finishes.

 THat's why start/stop bits are used.

 Take a look at Figure 328, page 1041 of the reference manual.

 The ArcadeIT will use a simple and common configuration, that we could represent
 with: 115000/8-N-1, that means:

 BAUD rate/word length in bits/Parity bit/Stop bit.

 The Parity bit is used as a test to see if a byte has been transferred correctly
 when set to E or O, Even or Odd. N means None, so no test. This is a simple way
 to verify the transfer. I won't explain here this theory, but you can find enough
 information on internet.

 Going back to the BAUD rate, considering 8 bits + 1 stop bit, we need to transfer
 9 bits for each character we send. Typical BAUD rates used in embedded projects
 are: 9600, 57600, and 115200.
 9600 BAUD    -> ~1066 bytes per second
 57600 BAUD   -> ~6400 bytes per second
 115200 BAUD  -> ~12800 bytes per second

 Why we need to understand all this? Because we need to set the clock of the USART
 peripheral, and we need to do some math to calculate the values to store into
 the registers.

 For now consider that in one second the STM32F469 runs 180000000 cycles, but the
 BUS used by the USART2 is the APB1, which is clocked at 45MHz

 -

 From: RM0386, Reference manual, page 1050

 30.4.4 Fractional baud rate generation

 In order to calculate the correct value to store into the register BRR that drives
 the transmission clock we use the formula

 BAUD = freq / (8*(2-OVER8) * USARTDIV)

 we need to read this equation finding USARTDIV which is the value to put into BRR

 BAUD * 8 * (2-OVER8) * USARTDIV = freq
 USARTDIV = freq / (BAUD * 8 * (2 - OVER8))

 We don't use the oversample, so OVER8 = 0

 USASRTDIV = 45Mhz / (115200 * 8 * 2)
 USASRTDIV = 45000000 / ‭1843200
 ‬
 USASRTDIV = 24.4140625
 Mantissa = 24
 Fraction = 0.4140625

 So we need to calcuate the two values using fixed point math using 4 bits for
 fraction and the rest for mantissa and then store them into the BRR register:

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |                      MANTISSA                             |      FRACTION     |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

 -

 From: RM0386, Reference manual, page 165

 6.3.10 RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)

 We first need to enable the clock of the GPIO port A by enabling the corresponding
 flag in register RCC_AHB1ENR: Bit 0 is called GPIOAEN (GPIOA clock enable).
 Then we can configure the GPIOA pins 2 and 3.

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

 |  0    0    0    0 .  0    0    0    0 |  1    1    1    1 .  0    0    0    0 |
 +---------+---------+---------+---------#####################---------+---------+
 | PIN 07  | PIN 06  | PIN 05  | PIN 04  # PIN 03  # PIN 02  # PIN 01  | PIN 00  |
 +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 # 07 | 06 # 05 | 04 # 03 | 02 | 01 | 00 |
 +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
 | MODER   | MODER   | MODER   | MODER   # MODER   # MODER   # MODER   | MODER   |
 | OSPEEDR | OSPEEDR | OSPEEDR | OSPEEDR # OSPEEDR # OSPEEDR # OSPEEDR | OSPEEDR |
 | PUDR    | PUDR    | PUDR    | PUDR    # PUDR    # PUDR    # PUDR    | PUDR    |
 |  [1:0]  |  [1:0]  |  [1:0]  |  [1:0]  #  [1:0]  #  [1:0]  #  [1:0]  |  [1:0]  |
 +----+----+----+----+----+----+----+----#----+----#----+----#----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw # rw | rw # rw | rw # rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----#####################----+----+----+----+

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

 |  0    0    0    0 .  0    0    1    0 |  0    0    0    0 .  1    1    0    0 |
 +----+----+----+----+----+----+----+----+----+----+----+----###########----+----+
 |P15 |P14 |P13 |P12 |P11 |P10 |P09 |P08 |P07 |P06 |P05 |P04 #P03 #P02 #P01 |P00 |
 +----+----+----+----+----+----+----+----+----+----+----+----#----#----#----+----+
 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 # 03 # 02 # 01 | 00 |
 +----+----+----+----+----+----+----+----+----+----+----+----#----#----#----+----+
 | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT | OT # OT # OT # OT | OT |
 +----+----+----+----+----+----+----+----+----+----+----+----#----#----#----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw # rw # rw # rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----###########----+----+

 -

 From: RM0386, Datasheet, page 74, Table 12. Alternate function

 USART2 function is applied to pins PA2, PA3 using alternate function 7

 -

 From: RM0386, Reference manual, page 210

 7.4.9 GPIO alternate function low register (GPIOx_AFRL)

 Since we have sixteen different alternate functions, 4 bits are needed for each
 pin to be configured. So two registers are used for pins 7..0 and pins 15..8,
 respectively registers GPIOx_AFRL and GPIOx_AFRH.

 For USART2 we use pins 2 and 3, so the low register GPIOx_AFRL is used.

 |  0    0    0    0 .  0    0    0    0 |  0    0    0    0 .  0    0    0    0 |
 +---------+---------+---------+---------+---------+---------+---------+---------+
 |      PIN 07       |      PIN 06       |      PIN 05       |      PIN 04       |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 |    AFRH07[3:0]    |    AFRH06[3:0]    |    AFRH05[3:0]    |    AFRH04[3:0]    |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw | rw |
 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

     alternate fn 7      alternate fn 7
 |  0    1    1    1 .  0    1    1    1 |  0    0    0    0 .  0    0    0    0 |
 #########################################---------+---------+---------+---------+
 #      PIN 03       #      PIN 02       #      PIN 01       |      PIN 00       |
 #----+----+----+----#----+----+----+----#----+----+----+----+----+----+----+----+
 # 15 | 14 | 13 | 12 # 11 | 10 | 09 | 08 # 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
 #----+----+----+----#----+----+----+----#----+----+----+----+----+----+----+----+
 #    AFRH03[3:0]    #    AFRH02[3:0]    #    AFRH01[3:0]    |    AFRH00[3:0]    |
 #----+----+----+----#----+----+----+----#----+----+----+----+----+----+----+----+
 # rw | rw | rw | rw # rw | rw | rw | rw # rw | rw | rw | rw | rw | rw | rw | rw |
 #########################################----+----+----+----+----+----+----+----+

 -

 From: RM0386, Reference manual, page 168

 6.3.13 RCC APB1 peripheral clock enable register (RCC_APB1ENR)

 We first need to enable the clock of the USART2 by enabling the corresponding
 flag in register RCC_APB1ENR: Bit 17 is called USART2EN (USART2 clock enable).
 Then we can configure the USART2...

 -

 When everything is set we can enable the USART by setting the
 Bit 13 UE: USART enable of the CR1 register.

 */