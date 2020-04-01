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
 * @file    ArcadeIT_VGA_LL.c
 * @version V0.13
 * @date    31-19-2017
 * @last    31-03-2020
 * @brief   This driver can generate a VGA standard video signal. It supports
 *          up to 640x480pixels resolution, and 11/12 bits of color depth using
 *          the ArcadeIT! VGA interface.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

 1.8 - VGA/RGB port

 ArcadeIT! Motherboard CN12 - PortA - VGA/RGB

 Pin  GPIO Function                     VGA/RGB interface
 ---------------------------------------------------------------------
 GND  VSS  GND                          CN12-15
 3.3V VDD  3.3V                         CN12-16
 040  PA0  GPIO VGA HSYNC/RGB COMP SYNC CN12-1
 041  PA1  GPIO VGA VSYNC               CN12-2
 162  PB4  GPIO RED_0                   CN12-10
 163  PB5  GPIO RED_1                   CN12-9
 164  PB6  GPIO RED_2                   CN12-12
 165  PB7  GPIO RED_3                   CN12-11
 167  PB8  GPIO GREEN_0                 CN12-14
 168  PB9  GPIO GREEN_1                 CN12-13
 079  PB10 GPIO GREEN_2                 CN12-4
 080  PB11 GPIO GREEN_3                 CN12-3
 085  PB12 GPIO BLUE_0                  CN12-6
 086  PB13 GPIO BLUE_1                  CN12-5
 087  PB14 GPIO BLUE_2                  CN12-8
 088  PB15 GPIO BLUE_3                  CN12-7

 VGA/RGB port  / CN12
             .--------.
 2 VSYNC     |  o  o  |   1 HSYNC
 4 GREEN_2   |  o  o  |   3 GREEN_3
 6 BLUE_0    |  o  o  |   5 BLUE_1
 8 BLUE_2    |  o  o |    7 BLUE_3
 10 RED_0    |  o  o |    9 RED_1
 12 RED_2    |  o  o  |  11 RED_3
 14 GREEN_0  |  o  o  |  13 GREEN_1
 16 3.3V     |  o  o  |  15 GND
             '--------'

 ******************************************************************************
 HISTORY
 31-19-2017:
 - Creation date of the library structure.

 31-03-2020:
 - Port to CMSIS and GitHUB.

 // /////////////////////////////////////////////////////////////////////////////
 */

#ifndef _ARCADEIT_VGA_LL_H
#define _ARCADEIT_VGA_LL_H

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
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

#define VGA_VSYNC_PORT     GPIOA
#define VGA_VSYNC_PIN      GPIO_Pin_1

// Shortcuts
#define TIM2_SR                           *((unsigned int *)((uint32_t)0x40000010))
#define TIM2_CCR2                         *((unsigned int *)((uint32_t)0x40000038))
#define TIM2_CNT                          *((unsigned int *)((uint32_t)0x40000024))

#define DMA2_Stream1_CR                   *((unsigned int *)((uint32_t)0x40026428))
#define DMA2_Stream1_NDTR                 *((unsigned int *)((uint32_t)0x4002642C))
#define DMA2_Stream1_M0AR                 ((unsigned int *)((uint32_t)0x40026434))
#define DMA2_Stream1_M1AR                 ((unsigned int *)((uint32_t)0x40026438))

#define DMA_Channel_7                     ((uint32_t)0x0E000000)
#define DMA_DIR_MemoryToPeripheral        ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable         ((uint32_t)0x00000000)
#define DMA_MemoryInc_Enable              ((uint32_t)0x00000400)
#define DMA_PeripheralDataSize_HalfWord   ((uint32_t)0x00000800)
#define DMA_Mode_Circular                 ((uint32_t)0x00000100)
#define DMA_Priority_VeryHigh             ((uint32_t)0x00030000)
#define DMA_MemoryBurst_Single            ((uint32_t)0x00000000)
#define DMA_PeripheralBurst_Single        ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord       ((uint32_t)0x00002000)
#define DMA_FIFOMode_Disable              ((uint32_t)0x00000000)
#define DMA_FIFOThreshold_Full            ((uint32_t)0x00000003)
#define DMA_IT_TCIF1                      ((uint32_t)0x10008800)

#define LINE_RENDERER_DMA_STREAM_A        DMA2_Stream0

#define DMA2_LIFCR         *((unsigned int *)((uint32_t)0x40026408))

#define TIM8_CR1           *((unsigned int *)((uint32_t)0x40010400))
#define TIM8_DIER          *((unsigned int *)((uint32_t)0x4001040C))
#define TIM8_ARR           *((unsigned int *)((uint32_t)0x4001042C))
#define TIM8_CR1_EN        TIM8_CR1 |= TIM_CR1_CEN;

#define SYS_VGA_VSINC_PIN_NO 1
#define SYS_VGA_VSINC_PER    GPIOA

#define DMA2_Stream1_CR_TCIE_EN DMA2_Stream1_CR |= (DMA_SxCR_EN | DMA_SxCR_TCIE)

#define ArcadeIT_VGA_BPORCH_PERIOD  515

/*
 const VGAResolutionData_t ArcadeIT_VGA_Resolution[4] =
{
  {  FACTOR_VGA, "640x480 60Hz", {  7, 816, 640, 16, 98,  62 }, 480 },
  { FACTOR_HVGA, "320x240 60Hz", { 14, 408, 320,  8, 49,  31 }, 240 },
  { FACTOR_HVGA, "268x240 60Hz", { 17, 336, 268,  7, 40,  21 }, 240 },
  { FACTOR_QVGA, "160x120 60Hz", { 28, 204, 160,  4, 25,  15 }, 120 }
};
 */
#define RESOLUTION_VGA     0
#define RESOLUTION_HVGA    1
#define RESOLUTION_H286    2
#define RESOLUTION_QVGA    3

#define FACTOR_VGA         0
#define FACTOR_HVGA        1
#define FACTOR_QVGA        2
#define ArcadeIT_VGA_RES   RESOLUTION_QVGA

#define SHOW   0
#define RENDER 1

#define PALETTED_RENDERER  0
#define RGB_RENDERER       1

#define VGA_LAYER_BACK     0
#define VGA_LAYER_FORE     1
#define VGA_LAYERS_MAX     2

#define VGA640x480x60Hz    0
#define HVGA320x240x60Hz   1
#define CVGA268x240x60Hz   2
#define QVGA160x120x60Hz   3
/*
#define VGA_LINE_FIRST          0
#define VGA_LINE_BORDER_BOTTOM  8
#define VGA_LINE_FRONT_PORCH    2
#define VGA_LINE_VSYNC_TIME     2
#define VGA_LINE_BACK_PORCH     25
#define VGA_LINE_BORDER_TOP     8
#define VGA_LINE_ACTIVE_TIME    480
#define VGA_LINE_LAST           524
#define VGA_LINE_ACTIVE_ON      VGA_LINE_FIRST
#define VGA_LINE_ACTIVE_OFF     VGA_LINE_ACTIVE_ON + VGA_LINE_ACTIVE_TIME
#define VGA_LINE_VSYNC_ON       VGA_LINE_ACTIVE_OFF + VGA_LINE_BORDER_BOTTOM + VGA_LINE_FRONT_PORCH
#define VGA_LINE_VSYNC_OFF      VGA_LINE_VSYNC_ON + VGA_LINE_VSYNC_TIME
*/
#define VGA_LINE_FIRST          0
#define VGA_LINE_FRONT_PORCH    10
#define VGA_LINE_VSYNC_TIME     2
#define VGA_LINE_BACK_PORCH     33
#define VGA_LINE_ACTIVE_TIME    480
#define VGA_LINE_LAST           524
#define VGA_LINE_ACTIVE_ON      VGA_LINE_FIRST
#define VGA_LINE_ACTIVE_OFF     VGA_LINE_ACTIVE_ON + VGA_LINE_ACTIVE_TIME
#define VGA_LINE_VSYNC_ON       VGA_LINE_ACTIVE_OFF + VGA_LINE_FRONT_PORCH
#define VGA_LINE_VSYNC_OFF      VGA_LINE_VSYNC_ON + VGA_LINE_VSYNC_TIME

#define VGA_COLOR_8BPP       1
#define VGA_COLOR_8BPP_MASK  0xFF00
#define VGA_COLOR_12BPP      2
#define VGA_COLOR_12BPP_MASK 0xFFF8

#define VGA_DEFAULT_PALETTE_COLORS 16
                                      // B    G    R
#define ARCADEIT_COLOR_TRANSPARENT  0
#define ARCADEIT_COLOR_BLACK        1 // 0,   0,   0,
#define ARCADEIT_COLOR_GREY1        2 // 128, 128, 128,
#define ARCADEIT_COLOR_WHITE        3 // 255, 255, 255,
#define ARCADEIT_COLOR_BLUE         4 // 255, 0,   0,
#define ARCADEIT_COLOR_BLUE1        5 // 128, 0,   0,
#define ARCADEIT_COLOR_BLUE2        6 // 32,  0,   0,
#define ARCADEIT_COLOR_GREEN        7 // 0,   255, 0,
#define ARCADEIT_COLOR_GREEN1       8 // 0,   128, 0,
#define ARCADEIT_COLOR_GREEN2       9 // 0,   32,  0,
#define ARCADEIT_COLOR_RED         10 // 0,   0,   255,
#define ARCADEIT_COLOR_RED1        11 // 0,   0,   128,
#define ARCADEIT_COLOR_RED2        12 // 0,   0,   32,
#define ARCADEIT_COLOR_MAGENTA     13 // 255, 0,   255,
#define ARCADEIT_COLOR_YELLOW      14 // 0,   255, 255,
#define ARCADEIT_COLOR_CYAN        15 // 255, 255, 0,

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// VGA
#define VGA_VSYNC_LOW()    GPIO_ResetBits(VGA_VSYNC_PORT, ArcadeIT_VGA_VSYNC_PIN)
#define VGA_VSYNC_HIGH()   GPIO_SetBits(VGA_VSYNC_PORT, ArcadeIT_VGA_VSYNC_PIN)

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////
typedef struct VGAResolutionPixelClocks
{
  uint8_t hclocks;
  uint16_t full;
  uint16_t active;
  uint16_t front_porch;
  uint16_t sync;
  uint16_t back_porch;

} VGAResolutionPixelClocks_t, *VGAResolutionPixelClocks_ptr;

typedef struct VGAResolutionData
{
  uint8_t factor;
  char name[32];
  VGAResolutionPixelClocks_t pixel_clock_definition;
  uint16_t lines_total;

} VGAResolutionData_t, *VGAResolutionData_ptr;

typedef struct Video_Driver
{
 uint8_t device;
 uint8_t mode;
 VGAResolutionData_t *resolution;

 uint8_t vga_color_bpp;
 uint8_t color_size;

 uint8_t output;
 uint8_t v_blank;

 uint16_t width;
 uint16_t height;
 uint8_t padding_h;
 uint8_t padding_v;
 uint32_t scereen_size;

 uint8_t screen_rotation;
 uint8_t layers;
 uint16_t rgb_transparent;

 uint8_t video_buffer_memory_src;
 __IO uint8_t *video_buffer_show;
 __IO uint8_t *video_buffer_render;
 uint8_t double_buffer;

 void *audio_callback;

 uint8_t renderer;
 uint8_t *boot_image;

 uint16_t show_scanlines;
 uint16_t color_foreground;
 uint16_t color_background;

} Video_Driver_t, *Video_Driver_ptr;

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Exported globals
// /////////////////////////////////////////////////////////////////////////////
extern uint8_t gLineRender;
// /////////////////////////////////////////////////////////////////////////////
// Exported functions
// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_GFX_Colors_Range_Get (
    uint8_t *pSourcePtr, // The pointer to memory where colors are stored into a list of bytes.
    uint8_t pFrom,
    uint8_t pTo,
    uint8_t pBytesPerColor,    // The number of bytes per color.
    Color_Palette_t *pPalette // The pointer to the palette where to store colors into.
    );
// -----------------------------------------------------------------------------
void ArcadeIT_GFX_Palette_Get
(
  uint16_t *pSourcePtr,       // The pointer to memory where colors are stored into a list of bytes.
  uint16_t pEntries,         // The number of colors.
  uint8_t pBytesPerColor,    // The number of bytes per color.
  Color_Palette_t *pPalette  // The pointer to the palette where to store colors into.
);
// -----------------------------------------------------------------------------
void ArcadeIT_GFX_Colors_Get
(
  uint8_t *pSourcePtr,       // The pointer to memory where colors are stored into a list of bytes.
  uint16_t pEntries,         // The number of colors.
  uint8_t pBytesPerColor,    // The number of bytes per color.
  Color_Palette_t *pPalette  // The pointer to the palette where to store colors into.
);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Deinit(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Mode_Set(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Info(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Start(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Stop(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Switch_Screen(uint8_t pSwitch);
// -----------------------------------------------------------------------------
uint8_t ArcadeIT_VGA_VBlank(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Swap_Buffer(void);
// -----------------------------------------------------------------------------
void ArcadeIT_VGA_Swap_Palette(void);

// /////////////////////////////////////////////////////////////////////////////
#endif // _ARCADEIT_VGA_LL_H
