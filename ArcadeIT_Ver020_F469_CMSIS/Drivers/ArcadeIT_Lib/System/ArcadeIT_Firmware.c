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
 * @file    ArcadeIT_Firmware.c
 * @version V0.13
 * @date    28-06-2017
 * @last    01-07-2020
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

 20-09-2018:
 - Ported the project on STM32CubeIDE.

 06-02-2020:
 - Converted to pure CMSIS.

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

#if 0
// ArcadeIT! Storage libraries.
#include "System/Disk/ArcadeIT_Storage.h"
#include "System/Peripherals/ArcadeIT_RAMDisk_LL.h"
#include "System/Peripherals/ArcadeIT_SD_Card_LL.h"

// ArcadeIT! System clocks and counters.
#include "System/Peripherals/ArcadeIT_RTC.h"
#endif
#include "System/Units/ArcadeIT_Scheduler.h"
#if 0
#include "System/Peripherals/ArcadeIT_DMM.h"
#endif

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_TestPads.h>
#include <System/Devices/ArcadeIT_Status_LEDs.h>
#include <System/Devices/ArcadeIT_Serial_Port.h>

#if 0
#include "System/Peripherals/ArcadeIT_USB_Port.h"
#include "System/Peripherals/ArcadeIT_SPI_Port.h"
#include "System/Peripherals/ArcadeIT_BUS_Port.h"
#include "System/Peripherals/ArcadeIT_Parallel_Port.h"

// ArcadeIT! Audio and video.
#include "System/Peripherals/ArcadeIT_VGA_LL.h"
#include "System/Peripherals/ArcadeIT_AUDIO_LL.h"
#include "System/Peripherals/ArcadeIT_LCD_LL.h"
#include "System/Video/ArcadeIT_Gfx_Utilities.h"
#include "System/Video/ArcadeIT_Graphics.h"

// ArcadeIT! Applications.
#include "System/Applications/ArcadeIT_Terminal.h"
#include "System/Applications/ArcadeIT_VGA_Terminal.h"
#include "System/Shell/ArcadeIT_DOS.h"
#include "Emulation/Machine/ColecoVision.h"

#include "Audio/Sam/reciter.h"
#include "Audio/Sam/sam.h"
#include "Audio/Sam/debug.h"
#endif
#define ARCADEIT_DEMOMODE

// /////////////////////////////////////////////////////////////////////////////
// External elements.
// /////////////////////////////////////////////////////////////////////////////
/*
// Select the system board.
extern uint8_t *gVGAFrameBufferTemp;
extern __IO uint8_t gSwapBuffer;
extern const uint8_t gVGACharacterSet6x8[13074];

extern __IO uint8_t *gDisplayAddress[2];
extern LCD_Driver_t gLCDDriver[2];
extern uint16_t gLCDLineBuffer[320];
extern Color_Palette_t gDefaultPalette, *gVGAPaletteShow, *gVGAPaletteRender;

extern const uint8_t gVGATerminalBackground[77586];
extern ArcadeIT_Terminal_t gArcadeITTerminal[ARCADEIT_TERMINALS];

extern __IO uint16_t *gVGAScanlineFrameBuffer;

extern Shell_batch_t gBatchRun;
extern unsigned char *gBootImage8BPP[4];
extern unsigned char *gBootImage24BPP[4];
extern Shell_Command_t gCurrentCommand;
*/

// external files used for demos
extern const unsigned char gANSIDemo1[7352];

// //////////////////////////////////////////////////////////////////////////////
// Globals
// //////////////////////////////////////////////////////////////////////////////

uint32_t gDevices = 0, gUnits = 0, gStorage = 0; // Flags for the activation of the systems

__IO uint32_t gSystemTimer = 0, gSecondaryTimer = 0, gSystemTick = 0;

/*
uint8_t gTerminal = FALSE;

__IO Video_Driver_t gVideoDriver, gVideoDriverBackup;

uint32_t gMemoryHeap = 0, gMemoryChip = 0;

__IO uint32_t gSecondaryTimer = 0, gUSBRefreshTimer = 0, gSystemTimer = 0, gSystemTick = 0;

uint32_t gSerialPortBAUD = 0;

uint32_t gSnapNo = 0;

uint8_t gInputMode = actionNone;
uint16_t gLastAction = 0xFF;
uint32_t gActionNumber = 0, gPadAction = 0, gActionsRead = 0;
uint32_t gTimeAction = 0, gMachineFrame = 0, gMachineLastFrame = 0;
uint8_t gArcadseIT_Tasks_Enable = TRUE;

char gString[ARCADEIT_STRING_LENGTH_MAX];
uint16_t lColorId = 0;
uint8_t lbuffer[256];
uint16_t lLine[256];
uint8_t gDummyBuffer[256];
*/
// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_SysTick_Deinit (void)
{
  /*
   * DESCRIPTION: This function stops the SysTick Timer.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // Disable SysTick IRQ and SysTick Timer.

} // end ArcadeIT_SysTick_Deinit

// -----------------------------------------------------------------------------
int ArcadeIT_SysTick_Init (void)
{
  /*
   * DESCRIPTION: This function sets the SysTick Timer, it counts every 1ms.
   * PARAMETERS:  See above.
   * RETURNS:     lRetStatus: this is the status of the success of the function (1:ERROR, 0:OK).
   */

  // Configure the return status to OK.
  int lRetStatus = FALSE;

  // Setup SysTick Timer interrupts.
  lRetStatus = SysTick_Config(ARCADEIT_HCLKS_PER_MS);

  // Sets the priority of the interrupt
  NVIC_SetPriority (SysTick_IRQn, NVIC_SYSTICK_PRIORITY << 2);

  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    ArcadeIT_Serial_Port_String_Send (TEXT_SYSTICK_INITED);

  } // End if.

  // And return the status.
  return lRetStatus;

} // end ArcadeIT_SysTick_Init.

// -----------------------------------------------------------------------------
void ArcadeIT_System_Delay (
    uint32_t pDelay // The amount of ms to wait for. The duration of a tick.
    )
{
  /*
   * DESCRIPTION: Performs a cycle of empty number of cycles to simulate a delay.
   * PARAMETERS:  See above.
   * RETURNS:     Nothing
   */

  // Get the parameter from the parameters pointer.
  gSystemTimer = pDelay;
  while (gSystemTimer);

} // end ArcadeIT_System_Delay.

// -----------------------------------------------------------------------------
void ArcadeIT_System_Init (void)
{
  /*
   * DESCRIPTION: This function initializes the core of the system
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  __IO uint32_t lStartUpCounter = 0;

  SCB->AIRCR = ((uint32_t)0x05FA0000) | NVIC_PriorityGroup_2; // Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup 2 value
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  // FPU settings: set CP10 and CP11 Full Access
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;     // Vector Table Relocation in Internal FLASH

  // In ArcadeIT PLL (clocked by HSE) is used as System clock source
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  // Wait till HSE is ready and if Time out is reached exit
  while(((RCC->CR & RCC_CR_HSERDY) == 0) && (++lStartUpCounter != HSE_STARTUP_TIMEOUT));

  if (RCC->CR & RCC_CR_HSERDY)
  {
    // Select regulator voltage output Scale 1 mode
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
    // HCLK = SYSCLK / 1
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    // PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    // PCLK1 = HCLK / 4
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // 0x00001400U
    // Configure the main PLL
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    // Enable the main PLL
    RCC->CR |= RCC_CR_PLLON;
    // Wait till the main PLL is ready
    while((RCC->CR & RCC_CR_PLLRDY) == 0) {}
    // Enable the Over-drive to extend the clock frequency to 180 Mhz
    PWR->CR |= PWR_CR_ODEN;
    while((PWR->CSR & PWR_CSR_ODRDY) == 0) {}
    PWR->CR |= PWR_CR_ODSWEN;
    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0) {}
    // Configure Flash prefetch, Instruction cache, Data cache and wait state
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
    // Select the main PLL as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    // Wait till the main PLL is used as system clock source
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL) {}
  }
  else
  {
    // If HSE fails to start-up, the application will have wrong clock
    // configuration. User can add here some code to deal with this error

  } // end if

  // HCLK counter, DWT struct is defined inside the core_cm4.h file
  DWT->CTRL |= 1;  // enable the counter
  DWT->CYCCNT = 0; // reset the counter

} // end ArcadeIT_System_Init.

// -----------------------------------------------------------------------------
void ArcadeIT_System_Reset (void)
{
  /*
   * DESCRIPTION: This function resets the ArcadeIT system.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  // Performs a software reset.
  NVIC_SystemReset ();

} // end ArcadeIT_System_Reset.

// -----------------------------------------------------------------------------
void ArcadeIT_Wait_VBlank (void)
{
  /*
   * DESCRIPTION: This function waits until a vertical blank signal from the
   *              video driver happens.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  // Synchronize every task in the VBlank video time.
  while (!ArcadeIT_If_VBlank ());

} // end ArcadeIT_Wait_VBlank.

// -----------------------------------------------------------------------------
uint8_t ArcadeIT_If_VBlank (void)
{
  /*
   * DESCRIPTION: This function returns the status of the vertical blank signal.
   *              video driver happens.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  // Synchronize every task in the VBlank video time.
  return 0;//ArcadeIT_VGA_VBlank ();

} // end ArcadeIT_If_VBlank.

// -----------------------------------------------------------------------------
void ArcadeIT_SysTick_Handler (void)
{
  /*
   * DESCRIPTION: Performs a cycle of empty number of cycles to simulate a delay.
   * PARAMETERS:  TimeDivider: this is the number to divide the system clock by (i.e. 1000: MSec, 1000000: MicroSec... ).
   * RETURNS:     Nothing
   */

  // Decrement timers synchronized to the MCU in ms.
  gSystemTick++;

  if (gSecondaryTimer > 0)
    gSecondaryTimer--;

  if (gSystemTimer > 0)
    gSystemTimer--;

  // Run the tasks scheduler.
  if (gUnits & ARCADEIT_UNIT_SCHEDULER)
  {
      ArcadeIT_Scheduler_Update ();

  } // End if.

} // end ArcadeIT_SysTick_Handler.

void SysTick_Handler(void)
{
  ArcadeIT_SysTick_Handler();

} // End SysTick_Handler.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_PowerUP_Systems (void)
{
  /*
   * DESCRIPTION: According to the configuration of the system this function turns
   *              ON the clock of the peripherals that the ArcadeIT! uses.
   * PARAMETERS:  None.
   * RETURNS:     Nothing
   */

  // ArcadeIT_TestPad_Init GPIOC
  if (gDevices & ARCADEIT_DEVICE_TESTPADS)
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;

  // ArcadeIT_Status_LEDS_Init GPIOH
  if (gDevices & ARCADEIT_DEVICE_STATUSLED)
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOH) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOH;

  // ArcadeIT_BUS_Port_Init GPIOD, GPIOE, GPIOF, GPIOG, GPIOI, FMC
  if (gDevices & ARCADEIT_DEVICE_BUS)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOD) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOE) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOE;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOF) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOF;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOG) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOI) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOI;
    if ((RCC->AHB3ENR & RCC_AHB3Periph_FMC) == FALSE) RCC->AHB3ENR |= RCC_AHB3Periph_FMC;

  } // End if.

  // LCD_Parallel_Port_Setup GPIOH, GPIOE, TIM1
  if (gDevices & ARCADEIT_DEVICE_LCDS)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOE) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOE;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOH) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOH;
    if ((RCC->APB2ENR & RCC_APB2Periph_TIM1) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_TIM1;

  } // End if.

  // ArcadeIT_Parallel_Port_Init GPIOD, GPIOC, GPIOI
  if (gDevices & ARCADEIT_DEVICE_PARALLEL)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOD) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOI) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOI;

  } // End if.

  // ArcadeIT_RTC_Init PWR
  if (gUnits & ARCADEIT_UNIT_RTC)
    if ((RCC->APB1ENR & RCC_APB1Periph_PWR) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_PWR;

  // ArcadeIT_Serial_Port_Init GPIOA, USART2, DMA1
  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
    if ((RCC->APB1ENR & RCC_APB1Periph_USART2) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_USART2;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_DMA1) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_DMA1;

  } // End if.

  // ArcadeIT_SPI_Port_Init GPIOA, GPIOD, GPIOC, SPI1
  if (gDevices & ARCADEIT_DEVICE_SPI1)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOD) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
    if ((RCC->APB2ENR & RCC_APB2Periph_SPI1) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_SPI1;

  } // End if.

  // ArcadeIT_VGA_Init GPIOA, GPIOB, DMA2, TIM8
  if (gDevices & ARCADEIT_DEVICE_VGA)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOB) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOB;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_DMA2) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_DMA2;
    if ((RCC->APB2ENR & RCC_APB2Periph_TIM8) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_TIM8;

  } // End if.

  // USB GPIOC, GPIOA, GPIOE, SYSCFG, OTG_FS, TIM2
  if (gDevices & ARCADEIT_DEVICE_USB)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOC) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOE) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOE;
    if ((RCC->APB2ENR & RCC_APB2Periph_SYSCFG) == FALSE) RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
    if ((RCC->AHB2ENR & RCC_AHB2Periph_OTG_FS) == FALSE) RCC->AHB2ENR |= RCC_AHB2Periph_OTG_FS;
    if ((RCC->APB1ENR & RCC_APB1Periph_TIM2) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_TIM2;

  } // End if.

  // Audio DMA1, GPIOA, GPIOF, DAC, TIM6
  if (gDevices & ARCADEIT_DEVICE_AUDIO)
  {
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOA) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOF) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOF;
    if ((RCC->AHB1ENR & RCC_AHB1Periph_DMA1) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_DMA1;
    if ((RCC->APB1ENR & RCC_APB1Periph_DAC) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_DAC;
    if ((RCC->APB1ENR & RCC_APB1Periph_TIM6) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_TIM6;

  } // End if.

} // end ArcadeIT_PowerUP_Systems

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_ArcadeIT_Start (void)
{
  /*S
   * DESCRIPTION: Starts all the systems according to the activation flags stored
   *              into global variables: gSystems, gStorage, gDevices.
   * PARAMETERS:  None.
   * RETURNS:     Nothing
   */

  // ---------------------------------------------------------------------------
  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    // Starts and configure the serial port.
    ArcadeIT_Serial_Port_Init (SYS_SERIAL_SPEED);

  } // End if.
  // ---------------------------------------------------------------------------
  // Clock and timer system. Thjis must be called before the SD card initialization.
  // 1ms System Tick timer.
  ArcadeIT_SysTick_Init ();

  // ---------------------------------------------------------------------------
  if (gDevices & ARCADEIT_DEVICE_TESTPADS)
  {
    /*
    *
    * Source
    *  RCC_MCO2Source_SYSCLK
    *  RCC_MCO2Source_PLLI2SCLK
    *  RCC_MCO2Source_HSE
    *  RCC_MCO2Source_PLLCLK
    *
    * Divider
    *  RCC_MCO2Div_1
    *  RCC_MCO2Div_2
    *  RCC_MCO2Div_3
    *  RCC_MCO2Div_4
    *  RCC_MCO2Div_5
    *
    *  You can test the correct frequency of the MCU by using an oscilloscope
    *  or a logic analyzer on the testpad of the motherboard. The tests below
    *  should give you the values shown.
    *
    */

    ArcadeIT_TestPad_Init (RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4);  //  45 MHz

    //ArcadeIT_TestPad_Init (RCC_MCO2Source_PLLCLK, RCC_MCO2Div_4); // 45 MHz
    //ArcadeIT_TestPad_Init (RCC_MCO2Source_HSE, RCC_MCO2Div_1);    //  8 MHz

  } // End if.
  // ---------------------------------------------------------------------------
  if (gDevices & ARCADEIT_DEVICE_STATUSLED)
  {
    // Start LED Status system.
    ArcadeIT_Status_LEDS_Init ();

  } // End if.
  // ---------------------------------------------------------------------------
  // Systems scheduler task. This must be set AFTER the Systick has been initialized
  // because uses the ISR function that is handled by the Systick.
  if (gUnits & ARCADEIT_UNIT_SCHEDULER)
  {
    // Start the scheduler system.
    ArcadeIT_Scheduler_Task_Init ();

  } // End if.
  // ---------------------------------------------------------------------------



  // ---------------------------------------------------------------------------
  ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);

} // End ArcadeIT_Start

// ////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Test_Bench (void)
{
  // The following variables hold the flags that start the features/peripherals
  // of the ArcadeIT System. It could be possible to configure the ArcadeIT System from a
  // *.ini text file on the SD Card, or other sources.

  // System features.
  gUnits = NONE
      //| ARCADEIT_UNIT_RTC           // Real time clock
      //| ARCADEIT_UNIT_DMM           // The custom ArcadeIt Dynamic Memory Manager
        | ARCADEIT_UNIT_SCHEDULER     // The task scheduler system.
      ;

  gStorage = NONE
      //| ARCADEIT_STORAGE_RAM_DISK     // Ram disk with a FAT File system on the SRAM expansion
      //| ARCADEIT_STORAGE_SD_CARD_SPI1 // File system FAT on SD Card over the SPI1 port
      ;

  gDevices = NONE
      //| ARCADEIT_SYSTEM_BUS           // The main BUS of the system.
      //| ARCADEIT_DEVICE_RAM_MODULE    // SRAM expansion
      //| ARCADEIT_DEVICE_EXPANSION     // SLOTS expansion
      //| ARCADEIT_DEVICE_SPI1          // SPI 1 port
      //| ARCADEIT_DEVICE_I2C           // I2C port
      //| ARCADEIT_DEVICE_LCDS          // LCDs port
      //| ARCADEIT_DEVICE_VGA           // VGA port
      //| ARCADEIT_DEVICE_AUDIO         // Audio DAC port
      //| ARCADEIT_DEVICE_PARALLEL      // Parallel port
      //| ARCADEIT_DEVICE_USB           // USB port
        | ARCADEIT_DEVICE_SERIAL_PORT   // Serial port USART 2
        | ARCADEIT_DEVICE_STATUSLED     // Two Status LEDs
        | ARCADEIT_DEVICE_TESTPADS      // System clock test pads
       ;

  ArcadeIT_ArcadeIT_Start();

#ifdef TEST // Test suite
  // ===========================================================================
  // Status LEDs
  for (uint8_t lCycles = 0; lCycles < 4; lCycles++)
  {
    ArcadeIT_Status_LED1_Toggle();
    ArcadeIT_System_Delay(250);
    ArcadeIT_Status_LED2_Toggle();
    ArcadeIT_System_Delay(250);

  } // End if.
  // --------------------------------------------------------------------------
  // We setup the scheduler to blink the Status LED 1 at 1Hz (every seconds).

  if (gUnits & ARCADEIT_UNIT_SCHEDULER)
  {
    if (gDevices & ARCADEIT_DEVICE_STATUSLED)
    {
          ArcadeIT_Scheduler_Task_Set (
              0,
              (void*) ArcadeIT_Status_LED1_Toggle,
              NULL,
              ARCADEIT_SCHEDULER_REPEAT_INFINITELY,
              ARCADEIT_PAUSE_HALF_SECOND
              );

     } // End if.

  } // End if.

  // --------------------------------------------------------------------------
  // Shows 256 colors over the serial terminal
  char lString[256];
  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    ArcadeIT_System_Delay(2000);

    ArcadeIT_Status_LED2_Toggle();

    ArcadeIT_Serial_Port_String_Send("ANSI colors table on serial terminal:\n\r");

    for (uint8_t lRow = 0; lRow < 16; lRow++)
    {
      for (uint8_t lCol = 0; lCol < 16; lCol++)
      {
        sprintf(lString, ATTR_COLOR_256_BG, lRow * 16 + lCol);
        ArcadeIT_Serial_Port_String_Send(lString);
        sprintf(lString, " %03d ", lRow * 16 + lCol);
        ArcadeIT_Serial_Port_String_Send(lString);

      } // end for

      ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);

    } // end for

    sprintf(lString, ATTR_COLOR_256_BG, 20);
    ArcadeIT_Serial_Port_String_Send(lString);

    ArcadeIT_Status_LED2_Toggle();

  } // End if.
  // --------------------------------------------------------------------------
  // Shows ANSI artwork over the serial terminal
  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    ArcadeIT_System_Delay(2000);

    ArcadeIT_Status_LED2_Toggle();

    ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);
    ArcadeIT_Serial_Port_String_Send("ANSI art on serial terminal:\n\r");
    ArcadeIT_System_Delay(1000);

    unsigned char *lANSIArtPtr = (unsigned char *)&gANSIDemo1[0];
    for (uint16_t lChar = 0; lChar < 7352; lChar++)
    {
      ArcadeIT_Serial_Port_Char_Send(*lANSIArtPtr++);

    } // end for

    ArcadeIT_Serial_Port_String_Send(CURSOR_NEWLINE);

    ArcadeIT_Status_LED2_Toggle();

  } // End if.
  // ===========================================================================
#endif

  while (1)
  {

    // Periodic tasks
    if (gUnits & ARCADEIT_UNIT_SCHEDULER)
    {
      ArcadeIT_Scheduler_Run ();

    } // End if.

  } // End while.

} // End ArcadeIT_Test_Bench.
