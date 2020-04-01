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

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! System clocks and counters.
//#include <System/Peripherals/ArcadeIT_DMM.h>

// ArcadeIT! Audio and video.
#include <System/Devices/ArcadeIT_VGA_LL.h>
//#include <System/Devices/Peripherals/ArcadeIT_AUDIO_LL.h>
//#include <System/Video/ArcadeIT_Gfx_Utilities.h>

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Externals
// /////////////////////////////////////////////////////////////////////////////

extern uint32_t gMemoryHeap;
extern uint32_t gMemoryChip;
extern uint8_t gTerminal;

extern unsigned char *gTestPattern[4];
extern unsigned char *gBootImage[4];
extern __IO uint8_t *gDisplayAddress[2];
extern __IO Video_Driver_t gVideoDriver, gVideoDriverBackup;
//extern ArcadeIT_Terminal_t gArcadeITTerminal[ARCADEIT_TERMINALS];

// /////////////////////////////////////////////////////////////////////////////
// Globals
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Variable used outside the rendering function to avoid the continuous creation
// in the stack.

// Video frame buffer related.
__IO uint8_t *gVGAFrameBufferTemp = NULL;
uint8_t *gFBPtr8 = NULL;
__IO uint16_t *gFBPtr16 = NULL;
__IO uint8_t gSwapBuffer = FALSE;

uint8_t *gVGAGraphicLayers0, *gVGAGraphicLayers1;
uint8_t gDummyBuffer[320];
uint8_t gVGALineHClkSteps = 0;

__IO uint16_t *gVGALineBufferShow = NULL;
__IO uint16_t *gVGALineBufferRender = NULL;
__IO uint16_t *gVGALineBufferBlank = NULL;
__IO uint16_t *gVGALineBufferTmp = NULL;
__IO void *gVGALineBufferScanline = NULL;
__IO uint16_t *gVGALineBufferRenderTmp = NULL;

// FIXME
// Add the support of multiple palettes that can be exchanged at run time.
uint16_t *gPalette = NULL;
uint16_t *gPaletteColorsList = NULL;
Color_Palette_t *gVGAPaletteRender = NULL;
Color_Palette_t *gVGAPaletteShow = NULL;
Color_Palette_t *gVGAPaletteTemp = NULL;
Color_Palette_t gCurrentPalette, gLoadedPalette1, gLoadedPalette2;
Color_Palette_t gDefaultPalette =
  { 1,      // The id of the palette.
  256,    // Max number of colors depending the layer mode.
    {
    // B    G    R
    RGB_TO_12_1_BIT(0, 0, 0), //  ARCADEIT_COLOR_TRANSPARENT  0
    RGB_TO_12_1_BIT(0, 0, 0), //  ARCADEIT_COLOR_BLACK        1
    RGB_TO_12_1_BIT(128, 128, 128), //  ARCADEIT_COLOR_GREY1        2
    RGB_TO_12_1_BIT(255, 255, 255), //  ARCADEIT_COLOR_WHITE        3
    RGB_TO_12_1_BIT(255, 0, 0), //  ARCADEIT_COLOR_BLUE         4
    RGB_TO_12_1_BIT(128, 0, 0), //  ARCADEIT_COLOR_BLUE1        5
    RGB_TO_12_1_BIT(32, 0, 0), //  ARCADEIT_COLOR_BLUE2        6
    RGB_TO_12_1_BIT(0, 255, 0), //  ARCADEIT_COLOR_GREEN        7
    RGB_TO_12_1_BIT(0, 128, 0), //  ARCADEIT_COLOR_GREEN1       8
    RGB_TO_12_1_BIT(0, 32, 0), //  ARCADEIT_COLOR_GREEN2       9
    RGB_TO_12_1_BIT(0, 0, 255), //  ARCADEIT_COLOR_RED         10
    RGB_TO_12_1_BIT(0, 0, 128), //  ARCADEIT_COLOR_RED1        11
    RGB_TO_12_1_BIT(0, 0, 32), //  ARCADEIT_COLOR_RED2        12
    RGB_TO_12_1_BIT(255, 0, 255), //  ARCADEIT_COLOR_MAGENTA     13
    RGB_TO_12_1_BIT(0, 255, 255), //  ARCADEIT_COLOR_YELLOW      14
    RGB_TO_12_1_BIT(255, 255, 0)  //  ARCADEIT_COLOR_CYAN        15
    } };

__IO uint8_t gSwapPalette = FALSE;
__IO uint8_t gNewFrame = FALSE;
__IO uint16_t gBorderColor = RGB_TO_12_1_BIT(0, 0, 0); //  ARCADEIT_COLOR_TRANSPARENT  0

// Video output related.
uint32_t gPixelCounter = 0, gScanlinePixel = 0;
uint8_t gColorIndex = 0;

__IO int16_t gVGAHWScanline = 0;
__IO uint8_t *gVGAHWScanlineFrameBuffer = NULL;
__IO int16_t gVGASubScanline = 0;

uint16_t gPixelValue = 0;
uint16_t gBlankColor = RGB_TO_12_1_BIT(0, 0, 0);
uint32_t gPixelClock = 0;
uint8_t gVGAResolutionDivision = 0;
uint8_t gLineRender = TRUE;

__IO uint32_t gVGAFrame = 0;

uint16_t lcycles = 0;

uint8_t *bootImagePtr = NULL;
uint32_t gImageStart = 0;

__IO uint16_t gLineBuffer0[820], gLineBuffer1[820], gLineBuffer2[820], gLineBuffer3[820];

uint8_t gscln[320];

// /////////////////////////////////////////////////////////////////////////////
// Pixel clock values for the different sections of a VGA scan line.
/*
 uint8_t hclocks;
 uint16_t full;
 uint16_t active;
 uint16_t front_porch;
 uint16_t sync;
 uint16_t back_porch;
 */
const VGAResolutionData_t ArcadeIT_VGA_Resolution[4] =
  {
    { FACTOR_VGA, "VGA 640x480x60Hz",
      { 7, 817, 640, 24, 98, 55 }, 480 },
    { FACTOR_HVGA, "HVGA 320x240x60Hz",
      { 14, 408, 320, 12, 49, 27 }, 240 },
    { FACTOR_HVGA, "268x240x60Hz (TMS9928)",
      { 17, 337, 268, 7, 40, 22 }, 240 },
    { FACTOR_QVGA, "QVGA 160x120x60Hz",
      { 28, 204, 160, 6, 24, 14 }, 120 }, };
// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Info (void)
{
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    sprintf (gString, "VGA resolution: %s\n\rBorders V: %d\n\rBorders H: %d\n\r\n\r", gVideoDriver.resolution->name, gVideoDriver.padding_v, gVideoDriver.padding_h);
    //ArcadeIT_Terminal_Debug(TERMINAL_DEBUG, lString);
    ArcadeIT_Serial_Port_String_Send(gString);

  } // end if

} // End VGA_Info.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_GFX_Palette_Get (uint16_t *pSourcePtr, // The pointer to memory where colors are stored into a list of bytes.
uint16_t pEntries,         // The number of colors.
uint8_t pBytesPerColor,    // The number of bytes per color.
Color_Palette_t *pPalette // The pointer to the palette where to store colors into.
)
{
  /*
   DESCRIPTION:  Load a list of RGB colors in hex format 0x00RRGGBB from a list of bytes.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  uint16_t lColorIndex = 0;
  for (lColorIndex = 0; lColorIndex < pEntries; lColorIndex++)
    {
      // Predefined colors for test purpose
      pPalette->colors[lColorIndex] = pSourcePtr[lColorIndex];

    } // End for.

} // End ArcadeIT_GFX_Palette_Get.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_GFX_Colors_Range_Get (uint8_t *pSourcePtr, // The pointer to memory where colors are stored into a list of bytes.
uint8_t pFrom, uint8_t pTo, uint8_t pBytesPerColor,    // The number of bytes per color.
Color_Palette_t *pPalette // The pointer to the palette where to store colors into.
)
{
  /*
   DESCRIPTION:  Load a list of RGB colors in hex format 0x00RRGGBB from a list of bytes.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  uint8_t lRed, lGreen, lBlue;

  uint16_t lColorIndex = 0;
  for (lColorIndex = pFrom; lColorIndex < pTo; lColorIndex++)
    {
      lRed = pSourcePtr[(lColorIndex * pBytesPerColor) + 2];
      lGreen = pSourcePtr[(lColorIndex * pBytesPerColor) + 1];
      lBlue = pSourcePtr[(lColorIndex * pBytesPerColor) + 0];

      // Predefined colors for test purpose
      pPalette->colors[lColorIndex] = RGB_TO_12_1_BIT(lRed, lGreen, lBlue);

    } // End for.

} // End ArcadeIT_GFX_Colors_Range_Get.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_GFX_Colors_Get (uint8_t *pSourcePtr, // The pointer to memory where colors are stored into a list of bytes.
uint16_t pEntries,         // The number of colors.
uint8_t pBytesPerColor,    // The number of bytes per color.
Color_Palette_t *pPalette // The pointer to the palette where to store colors into.
)
{
  /*
   DESCRIPTION:  Load a list of RGB colors in hex format 0x00RRGGBB from a list of bytes.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  uint8_t lRed, lGreen, lBlue;

  uint16_t lColorIndex = 0;
  for (lColorIndex = 0; lColorIndex < pEntries; lColorIndex++)
    {
      lRed = pSourcePtr[(lColorIndex * pBytesPerColor) + 2];
      lGreen = pSourcePtr[(lColorIndex * pBytesPerColor) + 1];
      lBlue = pSourcePtr[(lColorIndex * pBytesPerColor) + 0];

      // Predefined colors for test purpose
      pPalette->colors[lColorIndex] = RGB_TO_12_1_BIT(lRed, lGreen, lBlue);

    } // End for.

} // End ArcadeIT_GFX_Palette_Get.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_HClock_Set (void)
{
  /*
   "VGA industry standard" 640x480 pixel mode
   General characteristics

   Clock frequency 25.175 MHz
   Line  frequency 31469 Hz
   Field frequency 59.94 Hz

   1 PIXEL = 39.72ns * 800 = 31777ns
   31777ns VGA scanline.

   One line
   8 pixels front porch
   96 pixels horizontal sync
   40 pixels back porch
   8 pixels left border
   640 pixels video
   8 pixels right border
   ---
   800 pixels total per line

   One field
   2 lines front porch
   2 lines vertical sync
   25 lines back porch
   8 lines top border
   480 lines video
   8 lines bottom border
   ---
   525 lines total per field

   This value has been derived by the study of a typical VGA scanline and the clock
   of the ArcadeIt board to obtain a good pixel clock value. Basically:

   31777ns Scanline duration / 5.55ns  HClok step      = 5725.58 HClk steps
   5725.58 HClk steps / 800 Scanline clock pixels      = 7.156 HClk steps for each VGA pixel

   The period of the timer that trigs the DMA must be set to (7 - 1) = 6

   now we have a problem. 7.156 - 7 = 0.156, this reminder * 800 = 124.8 HClk steps lost during
   a scanline, this cause a little bit faster line frequency that may cause the monitor not to hook
   the signal.

   7 * 5.55 * 800 = 31080ns

   We can go as close as possible by adding Pixel clocks to the buffer in the active area, maintaining
   unchanged the HSync and the back porch that are important to the signal specifics.

   We can add 125 / 7 = 17.85 pixel clocks (18) to the buffer to stream to the VGA DAC. So the total
   pixel clocks become

   818 * 7 HClk steps * 5.55ns = 31779ns very close to 31770.
   */

  gVGALineHClkSteps = gVideoDriver.resolution->pixel_clock_definition.hclocks;

  RCC->APB2RSTR |= RCC_APB2Periph_TIM8;
  RCC->APB2RSTR &= ~RCC_APB2Periph_TIM8;

  if ((RCC->APB2ENR & RCC_APB2Periph_TIM8) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_TIM8;

  uint16_t tmpcr1 = 0;
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;

  tmpcr1 = TIM8->CR1;

  // Select the Counter Mode
  tmpcr1 &= (uint16_t)(~(TIM_CR1_DIR | TIM_CR1_CMS));
  TIM8->CR1 = tmpcr1;
  TIM8->ARR = gVGALineHClkSteps - 1;
  TIM8->PSC = 0;
  TIM8->RCR = 0;

  // Generate an update event to reload the Prescaler
  // and the repetition counter(only for TIM1 and TIM8) value immediately
  TIM8->EGR = TIM_PSCReloadMode_Immediate;
  // Disable the Channel 1: Reset the CC1E Bit
  TIM8->CCER &= (uint16_t)~TIM_CCER_CC1E;
  // Get the TIMx CCER register value
  tmpccer = TIM8->CCER;
  // Get the TIMx CR2 register value
  tmpcr2 =  TIM8->CR2;
  // Get the TIMx CCMR1 register value
  tmpccmrx = TIM8->CCMR1;
  // Reset the Output Compare Mode Bits
  tmpccmrx &= (uint16_t)~TIM_CCMR1_OC1M;
  tmpccmrx &= (uint16_t)~TIM_CCMR1_CC1S;
  // Select the Output Compare Mode
  tmpccmrx |= TIM_OCMode_Toggle;
  // Reset the Output Polarity level
  tmpccer &= (uint16_t)~TIM_CCER_CC1P;
  // Set the Output Compare Polarity
  tmpccer |= TIM_OCPolarity_High;
  // Set the Output State
  tmpccer |= TIM_OutputState_Enable;
  // Reset the Output N Polarity level
  tmpccer &= (uint16_t)~TIM_CCER_CC1NP;
  // Set the Output N Polarity
  tmpccer |= TIM_OCNPolarity_High;
  // Reset the Output N State
  tmpccer &= (uint16_t)~TIM_CCER_CC1NE;
  // Reset the Output Compare and Output Compare N IDLE State
  tmpcr2 &= (uint16_t)~TIM_CR2_OIS1;
  tmpcr2 &= (uint16_t)~TIM_CR2_OIS1N;
  // Write to TIMx CR2
  TIM8->CR2 = tmpcr2;
  // Write to TIMx CCMR1
  TIM8->CCMR1 = tmpccmrx;
  // Set the Capture Compare Register value
  TIM8->CCR1 = gVGALineHClkSteps / 2;
  // Write to TIMx CCER
  TIM8->CCER = tmpccer;
  // Set the ARR Preload Bit
  TIM8->CR1 |= TIM_CR1_ARPE;

  TIM8_DIER |= TIM_DMA_Update;

  /* Set the DMA Base and the DMA Burst Length */
  TIM8->DCR = TIM_DMABase_RCR | TIM_DMABurstLength_1Transfer;

} // End ArcadeIT_VGA_HClock_Set.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Init (void)
{
  // Clock enable.
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;

  // Configure v sync pin in output push/pull mode.
  uint32_t lPinPosition;
  lPinPosition = (SYS_VGA_VSINC_PIN_NO * 2);
  SYS_VGA_VSINC_PER->MODER   &= ~GPIO_MODER_MODER1;
  SYS_VGA_VSINC_PER->MODER   |= (((uint32_t)GPIO_Mode_OUT) << lPinPosition);
  SYS_VGA_VSINC_PER->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR1;
  SYS_VGA_VSINC_PER->OSPEEDR |= ((uint32_t)(GPIO_Speed_25MHz) << lPinPosition);
  SYS_VGA_VSINC_PER->OTYPER  &= ~GPIO_OTYPER_OT_1;
  SYS_VGA_VSINC_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_VGA_VSINC_PIN_NO);
  SYS_VGA_VSINC_PER->PUPDR   &= ~GPIO_PUPDR_PUPDR1;
  SYS_VGA_VSINC_PER->PUPDR   |= (((uint32_t)GPIO_PuPd_UP) << lPinPosition);

  // Clock enable.
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOB) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOB;

  // Configure v sync pin in output push/pull mode.
  SYS_VGA_VSINC_PER->MODER   &= 0xFFFFFFC0;
  SYS_VGA_VSINC_PER->MODER   |= ((uint32_t)0x55555540);
  SYS_VGA_VSINC_PER->OSPEEDR &= ~0xFFFFFFC0;
  SYS_VGA_VSINC_PER->OSPEEDR |= ((uint32_t)0x55555540);
  SYS_VGA_VSINC_PER->OTYPER  &= ~0xFFFFFFF8;
  SYS_VGA_VSINC_PER->OTYPER  |= (uint16_t)(GPIO_OType_PP << SYS_VGA_VSINC_PIN_NO);
  SYS_VGA_VSINC_PER->PUPDR   &= 0xFFFFFFC0;
  SYS_VGA_VSINC_PER->PUPDR   |= ((uint32_t)0xAAAAAA80);

  // ---------------------------------------------------------------------------
  ArcadeIT_VGA_HClock_Set ();
  // ---------------------------------------------------------------------------
  // Configure the DMA Stream

  // Reset DMAy Streamx control register
  DMA2_Stream1->CR  = 0;

  // Reset DMAy Streamx Number of Data to Transfer register
  DMA2_Stream1->NDTR = DMA2_Stream1->PAR = DMA2_Stream1->M0AR = DMA2_Stream1->M1AR = 0;

  // Reset DMAy Streamx FIFO control register
  DMA2_Stream1->FCR = (uint32_t)0x00000021;
  DMA2->LIFCR = DMA_Stream1_IT_MASK;

  if ((RCC->AHB1ENR & RCC_AHB1Periph_DMA2) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_DMA2;

  // Set the parameters to be configured
  DMA2_Stream1->CR = DMA_Channel_7
                   | DMA_DIR_MemoryToPeripheral
                   | DMA_PeripheralInc_Disable
                   | DMA_MemoryInc_Enable
                   | DMA_PeripheralDataSize_HalfWord
                   | DMA_MemoryDataSize_HalfWord
                   | DMA_Mode_Circular
                   | DMA_Priority_VeryHigh
                   | DMA_MemoryBurst_Single
                   | DMA_PeripheralBurst_Single;

  DMA2_Stream1->CR = DMA_FIFOMode_Disable | DMA_FIFOThreshold_Full;
  DMA2_Stream1->NDTR = gVideoDriver.resolution->pixel_clock_definition.full;
  DMA2_Stream1->PAR = ((uint32_t) &GPIOB->ODR);

  // Compute the Corresponding IRQ Priority ----------------------------------
  uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

  tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
  tmppre = (0x4 - tmppriority);
  tmpsub = tmpsub >> tmppriority;
  tmppriority = NVIC_VIDEO_PRIORITY << tmppre;
  tmppriority = tmppriority << 0x04;
  NVIC->IP[DMA2_Stream1_IRQn] = tmppriority;
  NVIC->ISER[DMA2_Stream1_IRQn >> 0x05] = (uint32_t)0x01 << (DMA2_Stream1_IRQn & (uint8_t)0x1F);

  // ---------------------------------------------------------------------------
  // Manage the buffers for the color depth of the video mode.
  if (gVGALineBufferBlank)
    {
      //free(gVGALineBufferBlank);
      gVGALineBufferBlank = NULL;
      gMemoryHeap -= (gVideoDriver.resolution->pixel_clock_definition.full * sizeof(uint16_t));
    }
  //gVGALineBufferBlank = (uint16_t*)malloc((gVideoDriver.resolution->pixel_clock_definition.full) * sizeof(uint16_t));
  gVGALineBufferBlank = gLineBuffer0;

  if (gVGALineBufferShow)
    {
      //free(gVGALineBufferShow);
      gVGALineBufferShow = NULL;
      gMemoryHeap -= (gVideoDriver.resolution->pixel_clock_definition.full * sizeof(uint16_t));
    }
  //gVGALineBufferShow = (uint16_t*)malloc((gVideoDriver.resolution->pixel_clock_definition.full) * sizeof(uint16_t) * 2);
  gVGALineBufferShow = gLineBuffer1;

  if (gVGALineBufferRender)
    {
      //free(gVGALineBufferShow);
      gVGALineBufferRender = NULL;
      gMemoryHeap -= (gVideoDriver.resolution->pixel_clock_definition.full * sizeof(uint16_t));
    }
  //gVGALineBufferShow = (uint16_t*)malloc((gVideoDriver.resolution->pixel_clock_definition.full) * sizeof(uint16_t) * 2);
  gVGALineBufferRender = gLineBuffer2;

  // We keep track of the amount of memory allocated.
  //gMemoryHeap += 2 * (gVideoDriver.resolution->pixel_clock_definition.full * sizeof(uint16_t));

  uint16_t lPixel = 0;
  for (lPixel = 0; lPixel < gVideoDriver.resolution->pixel_clock_definition.full; lPixel++)
    {
      /* HORIZONTAL
       *                                         |
       *  active                                 |fp syn bp
       *  _______________________________________|__     _____
       *                                         |  \___/
       */

      if (lPixel < gVideoDriver.resolution->pixel_clock_definition.active)
        {
          ((uint16_t*) gVGALineBufferShow)[lPixel] = 0x0008;
          ((uint16_t*) gVGALineBufferRender)[lPixel] = 0x0008;
          ((uint16_t*) gVGALineBufferBlank)[lPixel] = 0x0008;

        }
      else if (lPixel < (gVideoDriver.resolution->pixel_clock_definition.active + gVideoDriver.resolution->pixel_clock_definition.front_porch))
        {
          ((uint16_t*) gVGALineBufferShow)[lPixel] = 0x0008;
          ((uint16_t*) gVGALineBufferRender)[lPixel] = 0x0008;
          ((uint16_t*) gVGALineBufferBlank)[lPixel] = 0x0008;

        }
      else if (lPixel < (gVideoDriver.resolution->pixel_clock_definition.active + gVideoDriver.resolution->pixel_clock_definition.front_porch + gVideoDriver.resolution->pixel_clock_definition.sync))
        {
          // This range of pixel clocks have 0 value to generate the HSync.
          ((uint16_t*) gVGALineBufferShow)[lPixel] = 0;
          ((uint16_t*) gVGALineBufferRender)[lPixel] = 0;
          ((uint16_t*) gVGALineBufferBlank)[lPixel] = 0;
        }
      else
        {
          ((uint16_t*) gVGALineBufferShow)[lPixel] = 0x0008;
          ((uint16_t*) gVGALineBufferRender)[lPixel] = 0x0008;
          ((uint16_t*) gVGALineBufferBlank)[lPixel] = 0x0008;

        } // End if

    } // End for

  // ---------------------------------------------------------------------------
  // Set the initial line buffers for the double buffering system
  DMA2_Stream1_CR |= (uint32_t) DMA_SxCR_DBM;
  *DMA2_Stream1_M0AR = (uint32_t) gVGALineBufferShow;
  *DMA2_Stream1_M1AR = (uint32_t) gVGALineBufferRender;

  // ---------------------------------------------------------------------------
  gVGASubScanline = 0;
  gVGAHWScanline = 0;

  // ---------------------------------------------------------------------------
  //ArcadeIT_Utility_DMA_Init (); // Used for fast memory copy between buffers

  memset(gDummyBuffer, 0, 320);

} // End ArcadeIT_VGA_Init.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Mode_Set (void)
{
  /*
   DESCRIPTION:  Change the video mode.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  uint32_t lPixel = 0;
  uint8_t *lBuffer = NULL;
  uint16_t lBlankColor = 0;

  gVideoDriver.resolution = &ArcadeIT_VGA_Resolution[gVideoDriver.mode];
  //gVideoDriver.width = gVideoDriver.resolution->pixel_clock_definition.active;
  //gVideoDriver.height = gVideoDriver.resolution->lines_total;

  switch (gVideoDriver.renderer)
    {
    case PALETTED_RENDERER:
      gVideoDriver.color_size = sizeof(uint8_t);
      break;

    case RGB_RENDERER:
      gVideoDriver.color_size = sizeof(uint16_t);
      break;

    } // End switch.
  gVideoDriver.scereen_size = (gVideoDriver.width * gVideoDriver.color_size * gVideoDriver.height);

  // Manage the position of the frame buffer into on board memory or...
  switch (gVideoDriver.video_buffer_memory_src)
    {
    //----------------------------------------------------------------------------------------
    // MCU embedded RAM memory
    case MEMORY_MCU:
      if (gVideoDriver.video_buffer_show)
        {
          if ((uint32_t) gVideoDriver.video_buffer_show >= (uint32_t) gRAMAddress)
            gVideoDriver.video_buffer_show = (uint8_t*) DMM_Free (gRAMAddress, &gVideoDriver.video_buffer_show);
          else
            free (gVideoDriver.video_buffer_show);

        } // End if.

      gVideoDriver.video_buffer_show = (uint8_t*) malloc (gVideoDriver.scereen_size);
      if (gVideoDriver.renderer == RGB_RENDERER)
        {
          lBuffer = gVideoDriver.video_buffer_show;
          lPixel = gVideoDriver.scereen_size/2;
          lBlankColor = 0x0008;

          while (lPixel)
            {
              *(uint16_t*)lBuffer = lBlankColor;
              lBuffer += 2;
              lPixel--;
            }
        }
      else
        memset (gVideoDriver.video_buffer_show, 0, (gVideoDriver.scereen_size));

      // We keep track of the amount of memory allocated.
      gMemoryHeap += gVideoDriver.scereen_size;

      // Manage the double buffer
      if (gVideoDriver.double_buffer)
        {
          if (gVideoDriver.video_buffer_render)
            {
              if ((uint32_t) gVideoDriver.video_buffer_render >= (uint32_t) gRAMAddress)
                gVideoDriver.video_buffer_render = (uint8_t*) DMM_Free (gRAMAddress, &gVideoDriver.video_buffer_render);
              else
                free (gVideoDriver.video_buffer_render);

            } // End if.

          gVideoDriver.video_buffer_render = (uint8_t*) malloc (gVideoDriver.scereen_size);
          if (gVideoDriver.renderer == RGB_RENDERER)
            {
              lBuffer = gVideoDriver.video_buffer_render;
              lPixel = gVideoDriver.scereen_size/2;
              lBlankColor = 0x0008;

              while (lPixel)
                {
                  *(uint16_t*)lBuffer = lBlankColor;
                  lBuffer += 2;
                  lPixel--;
                }
            }
          else
            memset (gVideoDriver.video_buffer_render, 0, (gVideoDriver.scereen_size));

          // We keep track of the amount of memory allocated.
          gMemoryHeap += gVideoDriver.scereen_size;
        }
      else
        {
          gVideoDriver.video_buffer_render = gVideoDriver.video_buffer_show;

        } // End if
      break;

    //----------------------------------------------------------------------------------------
    // SRAM memory expansion
    case MEMORY_BOARD:
      if (gVideoDriver.video_buffer_show)
        {
          if ((uint32_t) gVideoDriver.video_buffer_show >= (uint32_t) gRAMAddress)
            gVideoDriver.video_buffer_show = (uint8_t*) DMM_Free (gRAMAddress, &gVideoDriver.video_buffer_show);
          else
            free (gVideoDriver.video_buffer_show);

          gMemoryChip -= gVideoDriver.scereen_size;

        } // End if.

      gVideoDriver.video_buffer_show = (uint8_t*) DMM_Malloc (gRAMAddress, &gVideoDriver.video_buffer_show, (gVideoDriver.scereen_size));
      if (gVideoDriver.renderer == RGB_RENDERER)
        {
          lBuffer = gVideoDriver.video_buffer_show;
          lPixel = gVideoDriver.scereen_size/2;
          lBlankColor = 0x0008;

          while (lPixel)
            {
              *(uint16_t*)lBuffer = lBlankColor;
              lBuffer += 2;
              lPixel--;
            }
        }
      else
        memset (gVideoDriver.video_buffer_show, 0, (gVideoDriver.scereen_size));

      // We keep track of the amount of memory allocated.
      gMemoryChip += gVideoDriver.scereen_size;

      // Manage the double buffer
      if (gVideoDriver.double_buffer)
        {
          if (gVideoDriver.video_buffer_render)
            {
              if ((uint32_t) gVideoDriver.video_buffer_render >= (uint32_t) gRAMAddress)
                gVideoDriver.video_buffer_render = (uint8_t*) DMM_Free (gRAMAddress, &gVideoDriver.video_buffer_render);
              else
                free (gVideoDriver.video_buffer_render);

              gMemoryChip -= gVideoDriver.scereen_size;

            } // End if.

          gVideoDriver.video_buffer_render = (uint8_t*) DMM_Malloc (gRAMAddress, &gVideoDriver.video_buffer_render, (gVideoDriver.scereen_size));
          if (gVideoDriver.renderer == RGB_RENDERER)
            {
              lBuffer = gVideoDriver.video_buffer_render;
              lPixel = gVideoDriver.scereen_size/2;
              lBlankColor = 0x0008;

              while (lPixel)
                {
                  *(uint16_t*)lBuffer = lBlankColor;
                  lBuffer += 2;
                  lPixel--;
                }
            }
          else
            memset (gVideoDriver.video_buffer_render, 0, (gVideoDriver.scereen_size));

          // We keep track of the amount of memory allocated.
          gMemoryChip += gVideoDriver.scereen_size;
        }
      else
        {
          gVideoDriver.video_buffer_render = gVideoDriver.video_buffer_show;

        } // End if
#if 0
      // FIXME
      // We prepare here the buffers for the terminal background to prevent the corruption of pointers while swapping
      if (gTerminal && gArcadeITTerminal[TERMINAL_ACTIVE].background_image_active)
        {
          // Prepare two frame buffers for the terminal background.
          if (gVGAGraphicLayers0)
            DMM_Free (gRAMAddress, &gVGAGraphicLayers0);
          gVGAGraphicLayers0 = (uint8_t*) DMM_Malloc (gRAMAddress, &gVGAGraphicLayers0, gVideoDriver.scereen_size / 2);
          memset (gVGAGraphicLayers0, 0, gVideoDriver.scereen_size / 2);

          // We keep track of the amount of memory allocated.
          gMemoryChip += gVideoDriver.scereen_size / 2;

          if (gVGAGraphicLayers1)
            DMM_Free (gRAMAddress, &gVGAGraphicLayers1);
          gVGAGraphicLayers1 = (uint8_t*) DMM_Malloc (gRAMAddress, &gVGAGraphicLayers1, gVideoDriver.scereen_size / 2);
          memset (gVGAGraphicLayers1, 0, gVideoDriver.scereen_size / 2);

          // We keep track of the amount of memory allocated.
          gMemoryChip += gVideoDriver.scereen_size / 2;

        } // End if.
#endif
      break;

    } // End switch.

  gVideoDriver.padding_h = (gVideoDriver.resolution->pixel_clock_definition.active - gVideoDriver.width) / 2;
  gVideoDriver.padding_v = (gVideoDriver.resolution->lines_total - gVideoDriver.height) / 2;

  gVGAHWScanlineFrameBuffer = NULL;

  // Palette
  gVGAPaletteShow = &gLoadedPalette1;
  gVGAPaletteRender = &gLoadedPalette2;

} // End ArcadeIT_VGA_Mode_Set.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Deinit (void)
{
#if 0
  // Clean the allocated memory.
  if (gVGALineBufferBlank != NULL)
  {
    free(gVGALineBufferBlank);
    if (gVideoDriver.vga_color_bpp == VGA_COLOR_8BPP)
        gMemoryHeap -= 409 * sizeof(uint8_t);
    else
        gMemoryHeap -= 409 * sizeof(uint16_t);

  } // End if.

  // Clean the allocated memory.
  if (gVGALineBufferShow != NULL)
  {
    free(gVGALineBufferShow);
    if (gVideoDriver.vga_color_bpp == VGA_COLOR_8BPP)
        gMemoryHeap -= 409 * sizeof(uint8_t);
    else
        gMemoryHeap -= 409 * sizeof(uint16_t);

  } // End if.

  // Clean the allocated memory.
  if (gVideoDriver.video_buffer_render != NULL)
  {
    free(gVideoDriver.video_buffer_render);
    gMemoryHeap -= (gVideoDriver.width * (gVideoDriver.height+1)) * sizeof(uint8_t);

  } // End if.

  // Clean the allocated memory.
  if (gVideoDriver.video_buffer_show != NULL)
  {
    free(gVideoDriver.video_buffer_show);
    gMemoryHeap -= (gVideoDriver.width * (gVideoDriver.height+1)) * sizeof(uint8_t);

  } // End if.
#endif
} // End ArcadeIT_VGA_Deinit

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Switch_Screen (uint8_t pSwitch // On/Off
)
{
  /*
   DESCRIPTION:  Turns on or off the rendering of the screen.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  gVideoDriver.output = pSwitch ? TRUE : FALSE;

} // End ArcadeIT_VGA_Switch_Screen.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Swap_Buffer (void)
{
  /*
   DESCRIPTION:  Swaps the frame buffer when the VGA is in double buffer mode.
   PARMS:        See above.
   RETURNS:      Nothing.
   */
  if (gVideoDriver.double_buffer)
    {
      gSwapBuffer = TRUE;

    } // End if.

} // End ArcadeIT_VGA_Swap_Buffer.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Swap_Palette (void)
{
  /*
   DESCRIPTION:  Swaps the Palette of the VGA mode.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  gSwapPalette = TRUE;

} // End ArcadeIT_VGA_Swap_Palette.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Start (void)
{
  /*
   DESCRIPTION:  Initializes the LCD Driver by configuring the SPI ports and the LCD hardware.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  // restart from the upper left corner of the frame buffer. This counter is incremented
  // every pixel of the active area.
  gVGAHWScanlineFrameBuffer = gVideoDriver.video_buffer_show;

#if 0
      if (gTestPattern[gVideoDriver.mode])
        gVGAHWScanlineFrameBuffer = gTestPattern[gVideoDriver.mode];
      else
        gVGAHWScanlineFrameBuffer = gVideoDriver.video_buffer_show;
#endif

  gVGASubScanline = 0;
  gVGAHWScanline = 1;

  *DMA2_Stream1_M0AR = (uint32_t) gVGALineBufferShow;
  *DMA2_Stream1_M1AR = (uint32_t) gVGALineBufferRender;

  uint16_t lPixel = 0;
  for (lPixel = 0; lPixel < gVideoDriver.resolution->pixel_clock_definition.active / 2; lPixel++)
    {
      ((uint32_t*) gVGALineBufferShow)[lPixel] = (uint32_t*) gVGAHWScanlineFrameBuffer[lPixel];
      if (gVideoDriver.height > 240)
        {
          ((uint32_t*) gVGALineBufferRender)[lPixel] = (uint32_t*) gVGAHWScanlineFrameBuffer[gVideoDriver.resolution->pixel_clock_definition.active / 2 + lPixel];
        }
      else
        {
          ((uint32_t*) gVGALineBufferRender)[lPixel] = (uint32_t*) gVGAHWScanlineFrameBuffer[lPixel];
        }

    } // End for

  DMA2_Stream1_CR |= DMA_SxCR_TCIE;        // enable transfer complete interrupt
  DMA2_Stream1_CR |= DMA_SxCR_EN;
  TIM8_CR1 |= TIM_CR1_CEN;

  gVideoDriver.v_blank = FALSE;

} // End ArcadeIT_VGA_Driver_Start.

// /////////////////////////////////////////////////////////////////////////////
void
ArcadeIT_VGA_Stop (void)
{
  /*
   DESCRIPTION:  Initializes the LCD Driver by configuring the SPI ports and the LCD hardware.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  DMA2_Stream1_CR &= ~DMA_SxCR_TCIE;
  DMA2_Stream1_CR &= ~DMA_SxCR_EN;
  TIM8_CR1 &= ~TIM_CR1_CEN;

} // End ArcadeIT_VGA_Driver_Stop.

// /////////////////////////////////////////////////////////////////////////////
uint8_t
ArcadeIT_VGA_VBlank (void)
{
  /*
   DESCRIPTION:  return if the raster is in the VBlank area.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  return gVideoDriver.v_blank;

} // End ArcadeIT_VGA_Wait_VBlank.

// /////////////////////////////////////////////////////////////////////////////
void
DMA2_Stream1_IRQHandler (void)
{
  /*
   DESCRIPTION:  Interrupt service routine for the scanline renderer.
   PARMS:        See above.
   RETURNS:      Nothing.
   */

  DMA2_LIFCR = (uint32_t) (DMA_IT_TCIF1 & (uint32_t) 0x0F7D0F7D);

#if 0
  if (gAudioDriver.mode == AUDIO_RAW)
    {
      if (gAudioDriver.bits == AUDIO_BIT_8)
        {
          // Render one sample of the audio chip.
          DAC_DHR8R1 = gAudioPlaybackBuffer8Ptr[gVGAHWScanline];
        }
      else if (gAudioDriver.bits == AUDIO_BIT_16)
        {
          // Render one sample of the audio chip.
          DAC_DHR12R1 = gAudioPlaybackBuffer16Ptr[gVGAHWScanline];

        } // End if.

    } // End if.
#endif
  /* VERTICAL
   *
   *  active                                 |fp syn bp
   *  _______________________________________|__     _____
   *                                         |  \___/
   *  0                                      |
   */

  if (gVGAHWScanline == VGA_LINE_VSYNC_ON)
    {
      GPIOA->ODR &= ~VGA_VSYNC_PIN;
    }
  else if (gVGAHWScanline == VGA_LINE_VSYNC_OFF)
    {
      GPIOA->ODR |= VGA_VSYNC_PIN;
    }
  else if (gVGAHWScanline < VGA_LINE_ACTIVE_OFF)
    {
      if ((gVGAHWScanline == VGA_LINE_ACTIVE_ON) || (gVGAHWScanline == VGA_LINE_ACTIVE_ON + 1))
        {
          if (DMA2_Stream1_CR & ((uint32_t) (DMA_SxCR_CT)))
            *DMA2_Stream1_M0AR = (uint32_t) gVGALineBufferShow;
          else
            *DMA2_Stream1_M1AR = (uint32_t) gVGALineBufferShow;

        } // End if.

      // use Temporary variables to store the buffer to play and Render
      // so that we can check What buffer is Being played and set the render buffer Too Bad Boys
      // Check the current scan line
      // use the formula in Excel file to check if it's time To render a line or not
      // If it's time then check if the render is needed
      // other wise set the render buffer equal to the play buffer.
      // Check the buffer that is currently being read
      // set the buffer not played to the buffer to render into
      // render the line by converting the pixels from the frame buffer
      // At that Point the line is ready for the next cycle of the renderer
      if (gVGASubScanline == (1 << gVideoDriver.resolution->factor))
        {
          gLineRender = TRUE;
          gVGASubScanline = 0;

        } // End if

      uint8_t *lTmpPtr = gVGAHWScanlineFrameBuffer;
      uint8_t *lTmpPtr2 = gVGAHWScanlineFrameBuffer;
      uint8_t *lTmpPtr3 = gVGAHWScanlineFrameBuffer;
      uint16_t lColorTmp1, lColorTmp2;

      if (gLineRender)
        {
          if (DMA2_Stream1_CR & ((uint32_t) (DMA_SxCR_CT)))
            {
              gVGALineBufferRender = *DMA2_Stream1_M0AR;
              gVGALineBufferShow = *DMA2_Stream1_M1AR;   //DMA2_Stream1_M1AR, being played
            }
          else
            {
              gVGALineBufferRender = *DMA2_Stream1_M1AR;
              gVGALineBufferShow = *DMA2_Stream1_M0AR;   //DMA2_Stream1_M1AR, being played
            }

          if ((gVGAHWScanline < (gVideoDriver.padding_v << gVideoDriver.resolution->factor)) || (gVGAHWScanline >= ((gVideoDriver.padding_v + gVideoDriver.height) << gVideoDriver.resolution->factor)))
            {
              gScanlinePixel = gVideoDriver.resolution->pixel_clock_definition.active;
              gVGALineBufferRenderTmp = gVGALineBufferRender;
              while (gScanlinePixel--)
                *gVGALineBufferRenderTmp++ = gBorderColor;
            }
          else
            {
              // Active area.
              // We point the renderer to the buffer that is not currently being played
              /*
               CT: current target (only in double-buffer mode)
               This bit is set and cleared by hardware. It can also be written by software.
               0: current target memory is Memory 0 (addressed by the DMA_SxM0AR pointer)
               1: current target memory is Memory 1 (addressed by the DMA_SxM1AR pointer)

               This bit can be written only if EN is ‘0’ to indicate the target memory area of the first transfer.
               Once the stream is enabled, this bit operates as a status flag indicating which memory area
               is the current target.
               */

              //------------------------------------------------------------------------------
              gVGALineBufferRenderTmp = gVGALineBufferRender;

              // Render the active line
              if (gVideoDriver.output)
                {
                  // Highlight the data streaming to the VGA with SCK pin of the I2C port
                  //ArcadeIT_I2C_Port_SetSCK (ENABLE);

                  gScanlinePixel = gVideoDriver.padding_h;
                  while (gScanlinePixel--)
                    *gVGALineBufferRenderTmp++ = gBorderColor;

                  if (gVideoDriver.renderer == RGB_RENDERER)
                    {
                      gScanlinePixel = gVideoDriver.width / 2;
                      while (gScanlinePixel--)
                        {
                          *(uint32_t*) gVGALineBufferRenderTmp = *(uint32_t*) gVGAHWScanlineFrameBuffer; // active area
                          gVGALineBufferRenderTmp += 2;
                          gVGAHWScanlineFrameBuffer += 2;
                        }
                    }
                  else
                    {
#if 0
                      DMA2D->FGMAR = lTmpPtr;
                      DMA2D->BGMAR = gDummyBuffer;
                      DMA2D->OMAR = (uint32_t)gVGALineBufferRenderTmp;
                      DMA2D->CR &= ((uint32_t)0xFFFCE0FC);
                      DMA2D->CR |= DMA2D_M2M_PFC;
                      DMA2D->OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
                      DMA2D->OPFCCR |= DMA2D_ARGB4444;
                      DMA2D_StartTransfer();

                      gVGAHWScanlineFrameBuffer += 320;

#else
                      gScanlinePixel = gVideoDriver.width / 8;
                      while (gScanlinePixel--)
                        {
                          lColorTmp1 = gVGAPaletteShow->colors[lTmpPtr[0]];
                          lColorTmp2 = gVGAPaletteShow->colors[lTmpPtr[1]];
                          ((uint32_t*) gVGALineBufferRenderTmp)[0] = lColorTmp1 | lColorTmp2 << 16; // active area
                          lColorTmp1 = gVGAPaletteShow->colors[lTmpPtr[2]];
                          lColorTmp2 = gVGAPaletteShow->colors[lTmpPtr[3]];
                          ((uint32_t*) gVGALineBufferRenderTmp)[1] = lColorTmp1 | lColorTmp2 << 16; // active area

                          lTmpPtr += 4;
                          gVGALineBufferRenderTmp += 4;
                          gVGAHWScanlineFrameBuffer += 4;

                          lColorTmp1 = gVGAPaletteShow->colors[lTmpPtr[0]];
                          lColorTmp2 = gVGAPaletteShow->colors[lTmpPtr[1]];
                          ((uint32_t*) gVGALineBufferRenderTmp)[0] = lColorTmp1 | lColorTmp2 << 16; // active area
                          lColorTmp1 = gVGAPaletteShow->colors[lTmpPtr[2]];
                          lColorTmp2 = gVGAPaletteShow->colors[lTmpPtr[3]];
                          ((uint32_t*) gVGALineBufferRenderTmp)[1] = lColorTmp1 | lColorTmp2 << 16; // active area

                          lTmpPtr += 4;
                          gVGALineBufferRenderTmp += 4;
                          gVGAHWScanlineFrameBuffer += 4;
                        }
#endif
                    }

                  gScanlinePixel = gVideoDriver.padding_h;
                  while (gScanlinePixel--)
                    *gVGALineBufferRenderTmp++ = gBorderColor;

                }
              else
                {
                  gScanlinePixel = gVideoDriver.width + (gVideoDriver.padding_h * 2);
                  gVGALineBufferRenderTmp = gVGALineBufferRender;
                  while (gScanlinePixel--)
                    *gVGALineBufferRenderTmp++ = gBorderColor;

                } // End if.

            } // End if chain.

          gLineRender = FALSE;

        } // End if line has to be rendered.
      else
        {
          // ------------------------------------------------------------------------
          // If mirroring to the LCD
#if 0
          if (gLCDDriver[0].mirror)
            {
              DMA2D_AbortTransfer();
    #if 0
              DMA2D->FGMAR = gLCDDriver[0].video_buffer_memory;
              DMA2D->BGMAR = gDummyBuffer;
              DMA2D->OMAR = (uint32_t)(gDisplayAddress[0]+LCD_DATA);
              DMA2D->CR &= ((uint32_t)0xFFFCE0FC);
              DMA2D->CR |= DMA2D_M2M_PFC;//BLEND;
              DMA2D->OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
              DMA2D->OPFCCR |= DMA2D_RGB565;
    #else
              DMA2D->FGMAR = gDummyBuffer;
              DMA2D->BGMAR = lTmpPtr3;
              DMA2D->OMAR = (uint32_t)(gDisplayAddress[0]+LCD_DATA);
              DMA2D->CR &= ((uint32_t)0xFFFCE0FC);
              DMA2D->CR |= DMA2D_M2M_BLEND;
              DMA2D->OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
              DMA2D->OPFCCR |= DMA2D_RGB565;
    #endif
              DMA2D_StartTransfer();
            }
#endif
          // ------------------------------------------------------------------------

        }
    }
  else if ((gVGAHWScanline == 480) || (gVGAHWScanline == 481))
    {
      if (DMA2_Stream1_CR & ((uint32_t) (DMA_SxCR_CT)))
        *DMA2_Stream1_M0AR = (uint32_t) gVGALineBufferBlank;
      else
        *DMA2_Stream1_M1AR = (uint32_t) gVGALineBufferBlank;

      if (gVGAHWScanline == 480)
        gVideoDriver.v_blank = TRUE;
    }
  else if (gVGAHWScanline == 524)
    {
      // At the end of the screen we can manage the swap of the frame buffer is supported by the resolution.
      // set the buffer and Render the first line for the next Frame.
      if (gSwapBuffer && gVideoDriver.double_buffer)
        {
          gVGAFrameBufferTemp = gVideoDriver.video_buffer_render;
          gVideoDriver.video_buffer_render = gVideoDriver.video_buffer_show;
          gVideoDriver.video_buffer_show = gVGAFrameBufferTemp;

          gSwapBuffer = FALSE;

        } // End if.

      if (gSwapPalette)
        {
          gVGAPaletteTemp = gVGAPaletteRender;
          gVGAPaletteRender = gVGAPaletteShow;
          gVGAPaletteShow = gVGAPaletteTemp;

          gSwapPalette = FALSE;

        } // End if.
#if 0
      // This is used to syncronizing the audio buffer swapping every video frame.
      if (gNewFrame)
        {
          if (gAudioDriver.mode == AUDIO_RAW)
            {
              gAudioTempBuffer16Ptr = gAudioLoadingBuffer16Ptr;
              gAudioLoadingBuffer16Ptr = gAudioPlaybackBuffer16Ptr;
              gAudioPlaybackBuffer16Ptr = gAudioTempBuffer16Ptr;

            } // End if.

          gNewFrame = FALSE;

        } // End if.
#endif
      // Reset line counters.
      gVGAHWScanline = -1;
      gVGASubScanline = 0;
      gVGAFrame++;
      gVGAHWScanlineFrameBuffer = gVideoDriver.video_buffer_show;
      gVideoDriver.v_blank = FALSE;
      gBorderColor = gVGAPaletteShow->colors[0];

    } // End if chain.

  gVGAHWScanline++;
  gVGASubScanline++;

} // End DMA2_Stream1_IRQHandler.
