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
 * @file    ArcadeIT_BUS_Port.h
 * @version V0.13
 * @date    26-07-2017
 * @last    27-02-2020
 * @brief   This library is used to drive the FMC bus at low level for the
 *          SRAM expansion, the IO bus expansion and the two parallel LCDs.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HARDWARE USED

  The FMC speed is HCLK, in the case of STM32F469 is 180MHz
  - clock period: 5.5ns

  1.0 - Memory mapper

  Pin  GPIO Function
  ---------------------------------------------------------------------
  005  PE6  FMC_A22             PERIPHERAL MUX 0  BUS_A22
  001  PE4  FMC_A23             PERIPHERAL MUX 1  BUS_A23
  156  PG13 FMC_A24             PERIPHERAL MUX 2  BUS_A24

  These pins are driven by the Flexible Memory Controller when
  accessing Memory locations. This lets address data to specific
  peripherals.

  The memory map locations are associated to expansion slots,
  SRAM and Parallel LCDs.

  BITS [21 20 19] Control select the bank of 512KB of the SRAM expansion
  002  PE3  FMC_A19             BUS_A19
  003  PE4  FMC_A20             BUS_A20
  004  PE5  FMC_A21             BUS_A21

           ----        16        8         0
  0000_000|0_00|00.0|000_0000.0000_0000.0000
           ----          512KB

  --- SRAM/Peripheral address        31   27    23    19    | 15   11   7    3            MEM SIZE   Modules
                                     ---      MUX           |                                0
                                     0000_bb0|0_00|X0.0|000 | 0000.0000_0000.0000  512KB  1          (1)
                                     0000_bb0|0_00|X0.1|000 | 0000.0000_0000.0000 1024KB  2          (2)
                                     0000_bb0|0_00|X1.0|000 | 0000.0000_0000.0000
                                     0000_bb0|0_00|X1.1|000 | 0000.0000_0000.0000 2048KB  3          (4)
                                     ---------------------- | -----------------------------------------------
                                     X NOT USED FOR NOW     |
                                     0000_bb0|0_00|10.0|000 | 0000.0000_0000.0000
                                     0000_bb0|0_00|10.1|000 | 0000.0000_0000.0000
                                     0000_bb0|0_00|11.0|000 | 0000.0000_0000.0000
                                     0000_bb0|0_00|11.1|000 | 0000.0000_0000.0000 4096KB  4          (8)

  512KB chip 0 @ 0x6000_0000
  512KB chip 1 @ 0x6008_0000
  512KB chip 2 @ 0x6010_0000
  512KB chip 3 @ 0x6018_0000
                 0x6020_0000

  SRAM IDT 71V424S10YG, SRAM 512Kx8 ASYNCHRONOUS 3.3V CMOS SRAM
  https://www.mouser.it/datasheet/2/464/IDT_71V424_DST_20141030-881857.pdf
  Memory transaction: Min 10ns

  Timings for the FMC with access mode A.
      Read:
          Address Setup:      Min 4ns, Max 10ns -> 1HCLK @ 5.55ns = 5.55ns
          Data Setup:                           -> 2HCLK @ 5.55ns = 11.1ns
          Turn Around:                          -> 1HCLK @ 5.55ns = 5.55ns    22.2ns read cycle

      Write:
          Address Setup:      Min 4ns, Max 10ns -> 1HCLK @ 5.55ns = 5.55ns
          Data Setup:                           -> 3HCLK @ 5.55ns = 16.65ns
          Turn Around:                          -> 1HCLK @ 5.55ns = 5.55ns    38.85ns write cycle


  --- Peripheral selection
  MUX 0: (0x0000_0000 - 0x003F_FFFF) 0000_bb0|0_00|00.0|000_0000.0000_0000.0000 2MB SRAM
  MUX 1: (0x0040_0000 - 0x004F_FFFF) 0000_bb0|0_01|00.0|000_0000.0000_0000.0000 4MB expansion slot 0, 8-bit bus
  MUX 2: (0x0080_0000 - 0x008F_FFFF) 0000_bb0|0_10|00.0|000_0000.0000_0000.0000 4MB expansion slot 1, 8-bit bus
  MUX 3: (0x00C0_0000 - 0x00CF_FFFF) 0000_bb0|0_11|00.0|000_0000.0000_0000.0000 4MB expansion slot 2, 8-bit bus
  MUX 4: (0x0100_0000 - 0x013F_FFFF) 0000_bb0|1_00|00.0|000_0000.0000_0000.0000 4MB expansion slot 3, 8-bit bus
  MUX 5: (0x0140_0000 - 0x014F_FFFF) 0000_bb0|1_01|00.0|000_0000.0000_0000.0000 4MB expansion slot 4, 8-bit bus
  MUX 6: (0x0180_0000)               0000_bb0|1_10|00.0|000_0000.0000_0000.0000 Parallel LCD 1, 8-bit bus
  MUX 7: (0x01C0_0000)               0000_bb0|1_11|00.0|000_0000.0000_0000.0000 Parallel LCD 2, 8-bit bus

  bb: selects banks according to the datasheet. HADDR[27..26]
  BANK1
  0x6000_0000 NE1 Sub bank 0-64MB
  0x6400_0000 NE2 Sub bank 64-128MB
  0x6800_0000 NE3 Sub bank 128-192MB
  0x6C00_0000 NE4 Sub bank 192-256MB

  0x6000_0000 SRAM (Sub bank 1)

  0x6400_0000 BUS_EXPANSION (Sub bank 2)
      MUX 1: (0x6440_0000 - 0x004F_FFFF) 0000_bb0|0_01|00.0|000_0000.0000_0000.0000 4MB expansion slot 0, 8-bit bus
      MUX 2: (0x6480_0000 - 0x008F_FFFF) 0000_bb0|0_10|00.0|000_0000.0000_0000.0000 4MB expansion slot 1, 8-bit bus
      MUX 3: (0x64C0_0000 - 0x00CF_FFFF) 0000_bb0|0_11|00.0|000_0000.0000_0000.0000 4MB expansion slot 2, 8-bit bus
      MUX 4: (0x6500_0000 - 0x013F_FFFF) 0000_bb0|1_00|00.0|000_0000.0000_0000.0000 4MB expansion slot 3, 8-bit bus
      MUX 5: (0x6540_0000 - 0x014F_FFFF) 0000_bb0|1_01|00.0|000_0000.0000_0000.0000 4MB expansion slot 4, 8-bit bus

      MUX 6: (0x6580_0000)               0000_bb0|1_10|00.0|000_0000.0000_0000.0000 Parallel LCD 1, 8-bit bus
      MUX 7: (0x65C0_0000)               0000_bb0|1_11|00.0|000_0000.0000_0000.0000 Parallel LCD 2, 8-bit bus

  FMC_NE1: SRAM module
  FMC_NE2: BUS_EXPANSION

  Pin  GPIO Function
  --------------------------------------------------------
  119  PC6 GPIO             MEMORY SIZE 0
  120  PC7 GPIO             MEMORY SIZE 1
  121  PC8 GPIO             MEMORY SIZE 2

  These pins are driven by the memory board inserted into
  the memory slot and tell the Microcontroller the size of
  the available memory on the board. A maximum of 2 Mega bytes
  are allowed by the memory mapper.

  These pins are Pulled UP by default, so the size is calculated
  combining the signals as a 2 bits word.

  SIZE BIT   INSTALLED SIZE
  ---------------------------
   X 0 0      -
   X 0 1      512 Kb
   X 1 0      1 MB
   X 1 1      2 MB

   X NOT USED FOR NOW

  SRAM module can contain:
     1 512K SRAM chip for 512 KB
     2 512K SRAM chip for 1 MB
     4 512K SRAM chip for 2 MB


  1.11 - External SRAM Memory port

  ArcadeIT! Motherboard CN2 - PortG - Memory

  Pin  GPIO Function                     External SRAM Memory Interface
  ---------------------------------------------------------------------
  GND  VSS  GND                          CN2-40
  3.3V VDD  3.3V                         CN2-39
  119  PC6  GPIO, MEM_SIZE_0             CN2-26
  120  PC7  GPIO, MEM_SIZE_1             CN2-28
  121  PC8  GPIO, MEM_SIZE_2             CN2-30
  -    -    -, CS_SRAM                   CN2-32
  ?    ?    ?, OE_SRAM                   CN2-34
  146 +PD4  FMC_NOE, RD                  CN2-36
  147 +PD5  FMC_NWE, WR                  CN2-38
  016 +PF0  FMC_A0, BUS_A0               CN2-23
  017 +PF1  FMC_A1, BUS_A1               CN2-21
  018 +PF2  FMC_A2, BUS_A2               CN2-19
  019 +PF3  FMC_A3, BUS_A3               CN2-17
  020 +PF4  FMC_A4, BUS_A4               CN2-15
  021 +PF5  FMC_A5, BUS_A5               CN2-13
  060 +PF12 FMC_A6, BUS_A6               CN2-1
  063 +PF13 FMC_A7 , BUS_A7              CN2-3
  064 +PF14 FMC_A8 , BUS_A8              CN2-5
  065 +PF15 FMC_A9 , BUS_A9              CN2-7
  066 +PG0  FMC_A10, BUS_A10             CN2-9
  067 +PG1  FMC_A11, BUS_A11             CN2-11
  110 +PG2  FMC_A12, BUS_A12             CN2-25
  111 +PG3  FMC_A13, BUS_A13             CN2-27
  112 +PG4  FMC_A14, BUS_A14             CN2-29
  113 +PG5  FMC_A15, BUS_A15             CN2-31
  092 +PD11 FMC_A16, BUS_A16             CN2-33
  093 +PD12 FMC_A17, BUS_A17             CN2-35
  094 +PD13 FMC_A18, BUS_A18             CN2-37
  002 +PE3  FMC_A19, BUS_A19             CN2-22  *
  003 +PE4  FMC_A20, BUS_A20             CN2-20  * Select 8 512KB banks for a total of 4MB
  004 +PE5  FMC_A21, BUS_A21             CN2-18  *
  097 +PD14 FMC_D0 BUS_D0                CN2-2
  098 +PD15 FMC_D1 BUS_D1                CN2-4
  142 +PD0  FMC_D2 BUS_D2                CN2-6
  143 +PD1  FMC_D3 BUS_D3                CN2-8
  68  +PE7  FMC_D4 BUS_D4                CN2-10
  69  +PE8  FMC_D5 BUS_D5                CN2-12
  70  +PE9  FMC_D6 BUS_D6                CN2-14
  73  +PE10 FMC_D7 BUS_D7                CN2-16


  1.11 - External expansion port

  ArcadeIT! Motherboard CN1 - PortH - Expansion

  Pin  GPIO Function                     External Expansion Interface
  ---------------------------------------------------------------------
  GND  VSS  GND                          CN2-40
  3.3V VDD  3.3V                         CN2-39
  176  PI17 GPIO, IO access              CN2-34
  -    -    ENABLE_SLOT_0                CN2-32
  -    -    ENABLE_SLOT_1                CN2-30
  -    -    ENABLE_SLOT_2                CN2-28
  -    -    ENABLE_SLOT_3                CN2-26
  -    -    ENABLE_SLOT_4                CN2-24
  146  PD4  FMC_NOE, RD                  CN2-36
  147  PD5  FMC_NWE, WR                  CN2-38
  151  PD7  FMC_NE1, WR                  CN2-38
  016  PF0  FMC_A0, BUS_A0               CN2-23
  017  PF1  FMC_A1, BUS_A1               CN2-21
  018  PF2  FMC_A2, BUS_A2               CN2-19
  019  PF3  FMC_A3, BUS_A3               CN2-17
  020  PF4  FMC_A4, BUS_A4               CN2-15
  021  PF5  FMC_A5, BUS_A5               CN2-13
  060  PF12 FMC_A6, BUS_A6               CN2-1
  063  PF13 FMC_A7 , BUS_A7              CN2-3
  064  PF14 FMC_A8 , BUS_A8              CN2-5
  065  PF15 FMC_A9 , BUS_A9              CN2-7
  066  PG0  FMC_A10, BUS_A10             CN2-9
  067  PG1  FMC_A11, BUS_A11             CN2-11
  110  PG2  FMC_A12, BUS_A12             CN2-25
  111  PG3  FMC_A13, BUS_A13             CN2-27
  112  PG4  FMC_A14, BUS_A14             CN2-29
  113  PG5  FMC_A15, BUS_A15             CN2-31
  092  PD11 FMC_A16, BUS_A16             CN2-33
  093  PD12 FMC_A17, BUS_A17             CN2-35
  094  PD13 FMC_A18, BUS_A18             CN2-37
  002  PE3  FMC_A19, BUS_A19             CN2-22
  003  PE4  FMC_A20, BUS_A20             CN2-20
  004  PE5  FMC_A21, BUS_A21             CN2-18
  097  PD14 FMC_D0 BUS_D0                CN2-2
  098  PD15 FMC_D1 BUS_D1                CN2-4
  142  PD0  FMC_D2 BUS_D2                CN2-6
  143  PD1  FMC_D3 BUS_D3                CN2-8
  68   PE7  FMC_D4 BUS_D4                CN2-10
  69   PE8  FMC_D5 BUS_D5                CN2-12
  70   PE9  FMC_D6 BUS_D6                CN2-14
  73   PE10 FMC_D7 BUS_D7                CN2-16

 ******************************************************************************
 HISTORY

 26-07-2017:
 - Creation date of the library structure.

 27-02-2020:
 - Ported to STM32CubeIDE and stored into GitHUB.

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

// ArcadeIT! Peripherals and buses.
#include <System/Devices/ArcadeIT_Serial_Port.h>
#include <System/Devices/ArcadeIT_BUS_Port.h>

// /////////////////////////////////////////////////////////////////////////////
// Macros.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Global elements.
// /////////////////////////////////////////////////////////////////////////////
//      BB X MUX  512  ADDR
// 0110.00 0 1.11 00.0 000.0000.0000.0000.0000 SRAM
// 0110.00 0 1.01 00.0 000.0000.0000.0000.0000 LCD1
// 0110.00 0 0.11 00.0 000.0000.0000.0000.0000 LCD2

  uint8_t *gRAMAddress = (uint8_t *)((uint32_t)0x61C00000);
  uint8_t *gRAMSpeechAddress;

  uint32_t gRAMSize = 0;

  uint8_t *gExpansionSlotAddress[5] =
  {
    (uint8_t *)((uint32_t)0x64000000),
    (uint8_t *)((uint32_t)0x64400000),
    (uint8_t *)((uint32_t)0x64800000),
    (uint8_t *)((uint32_t)0x64C00000),
    (uint8_t *)((uint32_t)0x65000000)
  };

  uint8_t *gDisplayAddress[2] =
  {
    (uint8_t *)((uint32_t)0x61400000),
    (uint8_t *)((uint32_t)0x60C00000),
  };

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////

/**
  * Taken from STM32F4 Standard Peripheral Library 1.8 -----------------------
  *
  * @brief  Initializes the FMC NOR/SRAM Banks according to the specified
  *         parameters in the FMC_NORSRAMInitStruct.
  * @param  FMC_NORSRAMInitStruct : pointer to a FMC_NORSRAMInitTypeDef structure
  *         that contains the configuration information for the FMC NOR/SRAM
  *         specified Banks.
  * @retval None
  */

void FMC_NORSRAMInit(
    FMC_NORSRAMInitTypeDef* FMC_NORSRAMInitStruct // structure of definitions for the banks
    )
{
  uint32_t tmpr = 0, tmpbcr = 0, tmpbwr = 0;

  /* Get the BTCR register value */
  tmpbcr = FMC_Bank1->BTCR[FMC_NORSRAMInitStruct->FMC_Bank];

  /* Clear MBKEN, MUXEN, MTYP, MWID, FACCEN, BURSTEN, WAITPOL, WRAPMOD, WAITCFG, WREN,
           WAITEN, EXTMOD, ASYNCWAIT, CBURSTRW and CCLKEN bits */
  tmpbcr &= ((uint32_t)~(FMC_BCR1_MBKEN    | FMC_BCR1_MUXEN    | FMC_BCR1_MTYP     | \
                         FMC_BCR1_MWID     | FMC_BCR1_FACCEN   | FMC_BCR1_BURSTEN  | \
                         FMC_BCR1_WAITPOL  | FMC_BCR1_WAITCFG  | \
                         FMC_BCR1_WREN     | FMC_BCR1_WAITEN   | FMC_BCR1_EXTMOD   | \
                         FMC_BCR1_ASYNCWAIT| FMC_BCR1_CBURSTRW | FMC_BCR1_CCLKEN));

  /* NOR/SRAM Bank control register configuration */
  tmpbcr |=  (uint32_t)FMC_NORSRAMInitStruct->FMC_DataAddressMux     |
                       FMC_NORSRAMInitStruct->FMC_MemoryType         |
                       FMC_NORSRAMInitStruct->FMC_MemoryDataWidth    |
                       FMC_NORSRAMInitStruct->FMC_BurstAccessMode    |
                       FMC_NORSRAMInitStruct->FMC_WaitSignalPolarity |
                       FMC_NORSRAMInitStruct->FMC_WaitSignalActive   |
                       FMC_NORSRAMInitStruct->FMC_WriteOperation     |
                       FMC_NORSRAMInitStruct->FMC_WaitSignal         |
                       FMC_NORSRAMInitStruct->FMC_ExtendedMode       |
                       FMC_NORSRAMInitStruct->FMC_AsynchronousWait   |
                       FMC_NORSRAMInitStruct->FMC_WriteBurst         |
                       FMC_NORSRAMInitStruct->FMC_ContinousClock;

  FMC_Bank1->BTCR[FMC_NORSRAMInitStruct->FMC_Bank] = tmpbcr;

  if(FMC_NORSRAMInitStruct->FMC_MemoryType == FMC_MemoryType_NOR)
  {
    FMC_Bank1->BTCR[FMC_NORSRAMInitStruct->FMC_Bank] |= (uint32_t)FMC_BCR1_FACCEN;

  } // end if

  /* Configure Continuous clock feature when bank2..4 is used */
  if((FMC_NORSRAMInitStruct->FMC_ContinousClock == FMC_CClock_SyncAsync) && (FMC_NORSRAMInitStruct->FMC_Bank != FMC_Bank1_NORSRAM1))
  {
    tmpr = (uint32_t)((FMC_Bank1->BTCR[FMC_Bank1_NORSRAM1+1]) & ~(((uint32_t)0x0F) << 20));

    FMC_Bank1->BTCR[FMC_Bank1_NORSRAM1]  |= FMC_NORSRAMInitStruct->FMC_ContinousClock;
    FMC_Bank1->BTCR[FMC_Bank1_NORSRAM1]  |= FMC_BurstAccessMode_Enable;
    FMC_Bank1->BTCR[FMC_Bank1_NORSRAM1+1] = (uint32_t)(tmpr | (((FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_CLKDivision)-1) << 20));

  } // end if

  /* NOR/SRAM Bank timing register configuration */
  FMC_Bank1->BTCR[FMC_NORSRAMInitStruct->FMC_Bank+1] =
            (uint32_t)FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_AddressSetupTime              |
                      (FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_AddressHoldTime << 4)        |
                      (FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_DataSetupTime << 8)          |
                      (FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_BusTurnAroundDuration << 16) |
                      (FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_CLKDivision << 20)           |
                      (FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_DataLatency << 24)           |
                      FMC_NORSRAMInitStruct->FMC_ReadWriteTimingStruct->FMC_AccessMode;

  /* NOR/SRAM Bank timing register for write configuration, if extended mode is used */
  if(FMC_NORSRAMInitStruct->FMC_ExtendedMode == FMC_ExtendedMode_Enable)
  {
    /* Get the BWTR register value */
    tmpbwr = FMC_Bank1E->BWTR[FMC_NORSRAMInitStruct->FMC_Bank];

    /* Clear ADDSET, ADDHLD, DATAST, BUSTURN and ACCMOD bits */
    tmpbwr &= ((uint32_t)~(FMC_BWTR1_ADDSET  | FMC_BWTR1_ADDHLD | FMC_BWTR1_DATAST | \
                           FMC_BWTR1_BUSTURN | FMC_BWTR1_ACCMOD));

    tmpbwr |= (uint32_t)(FMC_NORSRAMInitStruct->FMC_WriteTimingStruct->FMC_AddressSetupTime             |
                        (FMC_NORSRAMInitStruct->FMC_WriteTimingStruct->FMC_AddressHoldTime << 4)        |
                        (FMC_NORSRAMInitStruct->FMC_WriteTimingStruct->FMC_DataSetupTime << 8)          |
                        (FMC_NORSRAMInitStruct->FMC_WriteTimingStruct->FMC_BusTurnAroundDuration << 16) |
                         FMC_NORSRAMInitStruct->FMC_WriteTimingStruct->FMC_AccessMode);

    FMC_Bank1E->BWTR[FMC_NORSRAMInitStruct->FMC_Bank] = tmpbwr;
  }
  else
  {
    FMC_Bank1E->BWTR[FMC_NORSRAMInitStruct->FMC_Bank] = 0x0FFFFFFF;

  } // end if

} // end FMC_NORSRAMInit

// -----------------------------------------------------------------------------
/**
  * Taken from STM32F4 Standard Peripheral Library 1.8 -------------------------
  *
  * @brief  Enables or disables the specified NOR/SRAM Memory Bank.
  * @param  FMC_Bank: specifies the FMC Bank to be used
  *          This parameter can be one of the following values:
  *            @arg FMC_Bank1_NORSRAM1: FMC Bank1 NOR/SRAM1
  *            @arg FMC_Bank1_NORSRAM2: FMC Bank1 NOR/SRAM2
  *            @arg FMC_Bank1_NORSRAM3: FMC Bank1 NOR/SRAM3
  *            @arg FMC_Bank1_NORSRAM4: FMC Bank1 NOR/SRAM4
  * @param  NewState: new state of the FMC_Bank. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void FMC_NORSRAMCmd(
    uint32_t FMC_Bank,
    FunctionalState NewState
    )
{
  if (NewState != DISABLE)
  {
    /* Enable the selected NOR/SRAM Bank by setting the PBKEN bit in the BCRx register */
    FMC_Bank1->BTCR[FMC_Bank] |= 1;
  }
  else
  {
    /* Disable the selected NOR/SRAM Bank by clearing the PBKEN bit in the BCRx register */
    FMC_Bank1->BTCR[FMC_Bank] &= ~1;
  }
}
// -----------------------------------------------------------------------------
int ArcadeIT_BUS_Port_Init (void)
{
  /*
   * DESCRIPTION: Enables and configures the FMC BUS of the ArcadeIT! system.
   * PARAMETERS:  None.
   * RETURNS:     Nothing.
   */

  // Enable GPIOs clock If not yet.
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOD) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOE) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOE;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOF) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOF;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOG) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG;
  if ((RCC->AHB1ENR & RCC_AHB1Periph_GPIOI) == FALSE) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOI;
  if ((RCC->AHB3ENR & RCC_AHB3Periph_FMC) == FALSE) RCC->AHB3ENR |= RCC_AHB3Periph_FMC;

  GPIOD->MODER   &= ~(GPIO_MODER_MODER0
                    | GPIO_MODER_MODER1
                    | GPIO_MODER_MODER4
                    | GPIO_MODER_MODER5
                    | GPIO_MODER_MODER11
                    | GPIO_MODER_MODER12
                    | GPIO_MODER_MODER13
                    | GPIO_MODER_MODER14
                    | GPIO_MODER_MODER15
                    | GPIO_MODER_MODER7);
  GPIOD->MODER   |= (((uint32_t)GPIO_Mode_AF) << (FMC_D2_BUS_D2   * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D3_BUS_D3   * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_NOE_BUS_RD  * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_NWE_BUS_WR  * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A16_BUS_A16 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A17_BUS_A17 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A18_BUS_A18 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D0_BUS_D0   * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D1_BUS_D1   * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_NE1_BUS_LCDEN * 2));

  GPIOD->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0
                    | GPIO_OSPEEDER_OSPEEDR1
                    | GPIO_OSPEEDER_OSPEEDR4
                    | GPIO_OSPEEDER_OSPEEDR5
                    | GPIO_OSPEEDER_OSPEEDR11
                    | GPIO_OSPEEDER_OSPEEDR12
                    | GPIO_OSPEEDER_OSPEEDR13
                    | GPIO_OSPEEDER_OSPEEDR14
                    | GPIO_OSPEEDER_OSPEEDR15
                    | GPIO_OSPEEDER_OSPEEDR7);
  GPIOD->OSPEEDR |= (((uint32_t)GPIO_Speed_100MHz) << (FMC_D2_BUS_D2   * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D3_BUS_D3   * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_NOE_BUS_RD  * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_NWE_BUS_WR  * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A16_BUS_A16 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A17_BUS_A17 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A18_BUS_A18 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D0_BUS_D0   * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D1_BUS_D1   * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_NE1_BUS_LCDEN * 2));

  GPIOD->OTYPER  &= ~(GPIO_OTYPER_OT_0
                    | GPIO_OTYPER_OT_1
                    | GPIO_OTYPER_OT_4
                    | GPIO_OTYPER_OT_5
                    | GPIO_OTYPER_OT_11
                    | GPIO_OTYPER_OT_12
                    | GPIO_OTYPER_OT_13
                    | GPIO_OTYPER_OT_14
                    | GPIO_OTYPER_OT_15
                    | GPIO_OTYPER_OT_7);
  GPIOD->OTYPER  |= ((uint32_t)(GPIO_OType_PP << FMC_D2_BUS_D2  )
                   | (uint32_t)(GPIO_OType_PP << FMC_D3_BUS_D3  )
                   | (uint32_t)(GPIO_OType_PP << FMC_NOE_BUS_RD )
                   | (uint32_t)(GPIO_OType_PP << FMC_NWE_BUS_WR )
                   | (uint32_t)(GPIO_OType_PP << FMC_A16_BUS_A16)
                   | (uint32_t)(GPIO_OType_PP << FMC_A17_BUS_A17)
                   | (uint32_t)(GPIO_OType_PP << FMC_A18_BUS_A18)
                   | (uint32_t)(GPIO_OType_PP << FMC_D0_BUS_D0  )
                   | (uint32_t)(GPIO_OType_PP << FMC_D1_BUS_D1  )
                   | (uint32_t)(GPIO_OType_PP << FMC_NE1_BUS_LCDEN ));

  GPIOD->PUPDR   &= ~(GPIO_PUPDR_PUPDR0
                    | GPIO_PUPDR_PUPDR1
                    | GPIO_PUPDR_PUPDR4
                    | GPIO_PUPDR_PUPDR5
                    | GPIO_PUPDR_PUPDR11
                    | GPIO_PUPDR_PUPDR12
                    | GPIO_PUPDR_PUPDR13
                    | GPIO_PUPDR_PUPDR14
                    | GPIO_PUPDR_PUPDR15
                    | GPIO_PUPDR_PUPDR7);
  GPIOD->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D2_BUS_D2   * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D3_BUS_D3   * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_NOE_BUS_RD  * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_NWE_BUS_WR  * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A16_BUS_A16 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A17_BUS_A17 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A18_BUS_A18 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D0_BUS_D0   * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D1_BUS_D1   * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_NE1_BUS_LCDEN * 2));

  GPIOD->AFR[0] &= ~((uint32_t)0xF << (FMC_D2_BUS_D2  * 4)
                   | (uint32_t)0xF << (FMC_D3_BUS_D3  * 4)
                   | (uint32_t)0xF << (FMC_NOE_BUS_RD * 4)
                   | (uint32_t)0xF << (FMC_NWE_BUS_WR * 4)
                   | (uint32_t)0xF << (FMC_NE1_BUS_LCDEN * 4));
  GPIOD->AFR[0] |=  ((uint32_t)(GPIO_AF_FMC << (FMC_D2_BUS_D2  * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_D3_BUS_D3  * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_NOE_BUS_RD * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_NWE_BUS_WR * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_NE1_BUS_LCDEN * 4)));

  GPIOD->AFR[1] &= ~((uint32_t)0xF << ((FMC_A16_BUS_A16 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_A17_BUS_A17 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_A18_BUS_A18 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_D0_BUS_D0   - 8) * 4)
                   | (uint32_t)0xF << ((FMC_D1_BUS_D1   - 8) * 4));
  GPIOD->AFR[1] |=  ((uint32_t)(GPIO_AF_FMC << ((FMC_A16_BUS_A16 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_A17_BUS_A17 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_A18_BUS_A18 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_D0_BUS_D0   - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_D1_BUS_D1   - 8) * 4)));
  // ---------------------------------------------------------------------------
  GPIOE->MODER   &= ~(GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4
                    | GPIO_MODER_MODER5
                    | GPIO_MODER_MODER7
                    | GPIO_MODER_MODER8
                    | GPIO_MODER_MODER9
                    | GPIO_MODER_MODER10
                    | GPIO_MODER_MODER2
                    | GPIO_MODER_MODER6 );
  GPIOE->MODER   |= (((uint32_t)GPIO_Mode_AF) << (FMC_A19_BUS_A19  * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A20_BUS_A20  * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A21_BUS_A21  * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D4_BUS_D4    * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D5_BUS_D5    * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D6_BUS_D6    * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_D7_BUS_D7    * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A23_BUS_MUX1 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A22_BUS_MUX0 * 2));

  GPIOE->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR3
                    | GPIO_OSPEEDER_OSPEEDR4
                    | GPIO_OSPEEDER_OSPEEDR5
                    | GPIO_OSPEEDER_OSPEEDR7
                    | GPIO_OSPEEDER_OSPEEDR8
                    | GPIO_OSPEEDER_OSPEEDR9
                    | GPIO_OSPEEDER_OSPEEDR10
                    | GPIO_OSPEEDER_OSPEEDR2
                    | GPIO_OSPEEDER_OSPEEDR6 );
  GPIOE->OSPEEDR |= (((uint32_t)GPIO_Speed_100MHz) << (FMC_A19_BUS_A19  * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A20_BUS_A20  * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A21_BUS_A21  * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D4_BUS_D4    * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D5_BUS_D5    * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D6_BUS_D6    * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_D7_BUS_D7    * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A23_BUS_MUX1 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A22_BUS_MUX0 * 2));

  GPIOE->OTYPER  &= ~(GPIO_OTYPER_OT_3
                    | GPIO_OTYPER_OT_4
                    | GPIO_OTYPER_OT_5
                    | GPIO_OTYPER_OT_7
                    | GPIO_OTYPER_OT_8
                    | GPIO_OTYPER_OT_9
                    | GPIO_OTYPER_OT_10
                    | GPIO_OTYPER_OT_2
                    | GPIO_OTYPER_OT_6 );
  GPIOE->OTYPER  |= ((uint32_t)(GPIO_OType_PP << FMC_A19_BUS_A19 )
                   | (uint32_t)(GPIO_OType_PP << FMC_A20_BUS_A20 )
                   | (uint32_t)(GPIO_OType_PP << FMC_A21_BUS_A21 )
                   | (uint32_t)(GPIO_OType_PP << FMC_D4_BUS_D4   )
                   | (uint32_t)(GPIO_OType_PP << FMC_D5_BUS_D5   )
                   | (uint32_t)(GPIO_OType_PP << FMC_D6_BUS_D6   )
                   | (uint32_t)(GPIO_OType_PP << FMC_D7_BUS_D7   )
                   | (uint32_t)(GPIO_OType_PP << FMC_A23_BUS_MUX1)
                   | (uint32_t)(GPIO_OType_PP << FMC_A22_BUS_MUX0));

  GPIOE->PUPDR   &= ~(GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4
                    | GPIO_PUPDR_PUPDR5
                    | GPIO_PUPDR_PUPDR7
                    | GPIO_PUPDR_PUPDR8
                    | GPIO_PUPDR_PUPDR9
                    | GPIO_PUPDR_PUPDR10
                    | GPIO_PUPDR_PUPDR2
                    | GPIO_PUPDR_PUPDR6 );
  GPIOE->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A19_BUS_A19  * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A20_BUS_A20  * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A21_BUS_A21  * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D4_BUS_D4    * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D5_BUS_D5    * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D6_BUS_D6    * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_D7_BUS_D7    * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A23_BUS_MUX1 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A22_BUS_MUX0 * 2));

  GPIOE->AFR[0] &= ~((uint32_t)0xF << (FMC_A19_BUS_A19  * 4)
                   | (uint32_t)0xF << (FMC_A20_BUS_A20  * 4)
                   | (uint32_t)0xF << (FMC_A21_BUS_A21  * 4)
                   | (uint32_t)0xF << (FMC_D4_BUS_D4    * 4)
                   | (uint32_t)0xF << (FMC_A23_BUS_MUX1 * 4)
                   | (uint32_t)0xF << (FMC_A22_BUS_MUX0 * 4));
  GPIOE->AFR[0] |=  ((uint32_t)(GPIO_AF_FMC << (FMC_A19_BUS_A19  * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A20_BUS_A20  * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A21_BUS_A21  * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_D4_BUS_D4    * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A23_BUS_MUX1 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A22_BUS_MUX0 * 4)));

  GPIOE->AFR[1] &= ~((uint32_t)0xF << ((FMC_D5_BUS_D5 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_D6_BUS_D6 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_D7_BUS_D7 - 8) * 4));
  GPIOE->AFR[1] |=  ((uint32_t)(GPIO_AF_FMC << ((FMC_D5_BUS_D5 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_D6_BUS_D6 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_D7_BUS_D7 - 8) * 4)));
  // ---------------------------------------------------------------------------
  GPIOF->MODER   &= ~(GPIO_MODER_MODER0
                    | GPIO_MODER_MODER1
                    | GPIO_MODER_MODER2
                    | GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4
                    | GPIO_MODER_MODER5
                    | GPIO_MODER_MODER12
                    | GPIO_MODER_MODER13
                    | GPIO_MODER_MODER14
                    | GPIO_MODER_MODER15);
  GPIOF->MODER   |= (((uint32_t)GPIO_Mode_AF) << (FMC_A00_BUS_A00* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A01_BUS_A01* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A02_BUS_A02* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A03_BUS_A03* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A04_BUS_A04* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A05_BUS_A05* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A06_BUS_A06* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A07_BUS_A07* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A08_BUS_A08* 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A09_BUS_A09* 2));

  GPIOF->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0
                    | GPIO_OSPEEDER_OSPEEDR1
                    | GPIO_OSPEEDER_OSPEEDR2
                    | GPIO_OSPEEDER_OSPEEDR3
                    | GPIO_OSPEEDER_OSPEEDR4
                    | GPIO_OSPEEDER_OSPEEDR5
                    | GPIO_OSPEEDER_OSPEEDR12
                    | GPIO_OSPEEDER_OSPEEDR13
                    | GPIO_OSPEEDER_OSPEEDR14
                    | GPIO_OSPEEDER_OSPEEDR15);
  GPIOF->OSPEEDR |= (((uint32_t)GPIO_Speed_100MHz) << (FMC_A00_BUS_A00 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A01_BUS_A01 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A02_BUS_A02 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A03_BUS_A03 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A04_BUS_A04 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A05_BUS_A05 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A06_BUS_A06 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A07_BUS_A07 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A08_BUS_A08 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A09_BUS_A09 * 2));

  GPIOF->OTYPER  &= ~(GPIO_OTYPER_OT_0
                    | GPIO_OTYPER_OT_1
                    | GPIO_OTYPER_OT_2
                    | GPIO_OTYPER_OT_3
                    | GPIO_OTYPER_OT_4
                    | GPIO_OTYPER_OT_5
                    | GPIO_OTYPER_OT_12
                    | GPIO_OTYPER_OT_13
                    | GPIO_OTYPER_OT_14
                    | GPIO_OTYPER_OT_15);
  GPIOF->OTYPER  |= ((uint32_t)(GPIO_OType_PP << FMC_A00_BUS_A00)
                   | (uint32_t)(GPIO_OType_PP << FMC_A01_BUS_A01)
                   | (uint32_t)(GPIO_OType_PP << FMC_A02_BUS_A02)
                   | (uint32_t)(GPIO_OType_PP << FMC_A03_BUS_A03)
                   | (uint32_t)(GPIO_OType_PP << FMC_A04_BUS_A04)
                   | (uint32_t)(GPIO_OType_PP << FMC_A05_BUS_A05)
                   | (uint32_t)(GPIO_OType_PP << FMC_A06_BUS_A06)
                   | (uint32_t)(GPIO_OType_PP << FMC_A07_BUS_A07)
                   | (uint32_t)(GPIO_OType_PP << FMC_A08_BUS_A08)
                   | (uint32_t)(GPIO_OType_PP << FMC_A09_BUS_A09));

  GPIOF->PUPDR   &= ~(GPIO_PUPDR_PUPDR0
                    | GPIO_PUPDR_PUPDR1
                    | GPIO_PUPDR_PUPDR2
                    | GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4
                    | GPIO_PUPDR_PUPDR5
                    | GPIO_PUPDR_PUPDR12
                    | GPIO_PUPDR_PUPDR13
                    | GPIO_PUPDR_PUPDR14
                    | GPIO_PUPDR_PUPDR15);
  GPIOF->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A00_BUS_A00 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A01_BUS_A01 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A02_BUS_A02 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A03_BUS_A03 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A04_BUS_A04 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A05_BUS_A05 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A06_BUS_A06 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A07_BUS_A07 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A08_BUS_A08 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A09_BUS_A09 * 2));

  GPIOF->AFR[0] &= ~((uint32_t)0xF << (FMC_A00_BUS_A00 * 4)
                   | (uint32_t)0xF << (FMC_A01_BUS_A01 * 4)
                   | (uint32_t)0xF << (FMC_A02_BUS_A02 * 4)
                   | (uint32_t)0xF << (FMC_A03_BUS_A03 * 4)
                   | (uint32_t)0xF << (FMC_A04_BUS_A04 * 4)
                   | (uint32_t)0xF << (FMC_A05_BUS_A05 * 4));
  GPIOF->AFR[0] |=  ((uint32_t)(GPIO_AF_FMC << (FMC_A00_BUS_A00 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A01_BUS_A01 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A02_BUS_A02 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A03_BUS_A03 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A04_BUS_A04 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A05_BUS_A05 * 4)));

  GPIOF->AFR[1] &= ~((uint32_t)0xF << ((FMC_A06_BUS_A06 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_A07_BUS_A07 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_A08_BUS_A08 - 8) * 4)
                   | (uint32_t)0xF << ((FMC_A09_BUS_A09 - 8) * 4));
  GPIOF->AFR[1] |=  ((uint32_t)(GPIO_AF_FMC << ((FMC_A06_BUS_A06 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_A07_BUS_A07 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_A08_BUS_A08 - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_A09_BUS_A09 - 8) * 4)));
  // ---------------------------------------------------------------------------
  GPIOG->MODER   &= ~(GPIO_MODER_MODER0
                    | GPIO_MODER_MODER1
                    | GPIO_MODER_MODER2
                    | GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4
                    | GPIO_MODER_MODER5
                    | GPIO_MODER_MODER9
                    | GPIO_MODER_MODER13);
  GPIOG->MODER   |= (((uint32_t)GPIO_Mode_AF) << (FMC_A10_BUS_A10 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A11_BUS_A11 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A12_BUS_A12 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A13_BUS_A13 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A14_BUS_A14 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A15_BUS_A15 * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_NE2_SRAMEN  * 2)
                   | ((uint32_t)GPIO_Mode_AF) << (FMC_A24_MUX2    * 2));

  GPIOG->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0
                    | GPIO_OSPEEDER_OSPEEDR1
                    | GPIO_OSPEEDER_OSPEEDR2
                    | GPIO_OSPEEDER_OSPEEDR3
                    | GPIO_OSPEEDER_OSPEEDR4
                    | GPIO_OSPEEDER_OSPEEDR5
                    | GPIO_OSPEEDER_OSPEEDR9
                    | GPIO_OSPEEDER_OSPEEDR13);
  GPIOG->OSPEEDR |= (((uint32_t)GPIO_Speed_100MHz) << (FMC_A10_BUS_A10 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A11_BUS_A11 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A12_BUS_A12 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A13_BUS_A13 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A14_BUS_A14 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A15_BUS_A15 * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_NE2_SRAMEN  * 2)
                   | ((uint32_t)GPIO_Speed_100MHz) << (FMC_A24_MUX2    * 2));

  GPIOG->OTYPER  &= ~(GPIO_OTYPER_OT_0
                    | GPIO_OTYPER_OT_1
                    | GPIO_OTYPER_OT_2
                    | GPIO_OTYPER_OT_3
                    | GPIO_OTYPER_OT_4
                    | GPIO_OTYPER_OT_5
                    | GPIO_OTYPER_OT_9
                    | GPIO_OTYPER_OT_13);
  GPIOG->OTYPER  |= ((uint32_t)(GPIO_OType_PP << FMC_A10_BUS_A10)
                   | (uint32_t)(GPIO_OType_PP << FMC_A11_BUS_A11)
                   | (uint32_t)(GPIO_OType_PP << FMC_A12_BUS_A12)
                   | (uint32_t)(GPIO_OType_PP << FMC_A13_BUS_A13)
                   | (uint32_t)(GPIO_OType_PP << FMC_A14_BUS_A14)
                   | (uint32_t)(GPIO_OType_PP << FMC_A15_BUS_A15)
                   | (uint32_t)(GPIO_OType_PP << FMC_NE2_SRAMEN )
                   | (uint32_t)(GPIO_OType_PP << FMC_A24_MUX2   ));

  GPIOG->PUPDR   &= ~(GPIO_PUPDR_PUPDR0
                    | GPIO_PUPDR_PUPDR1
                    | GPIO_PUPDR_PUPDR2
                    | GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4
                    | GPIO_PUPDR_PUPDR5
                    | GPIO_PUPDR_PUPDR9
                    | GPIO_PUPDR_PUPDR13);
  GPIOG->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A10_BUS_A10 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A11_BUS_A11 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A12_BUS_A12 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A13_BUS_A13 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A14_BUS_A14 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A15_BUS_A15 * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_NE2_SRAMEN  * 2)
                   | ((uint32_t)GPIO_PuPd_NOPULL) << (FMC_A24_MUX2    * 2));

  GPIOG->AFR[0] &= ~((uint32_t)0xF << (FMC_A10_BUS_A10 * 4)
                   | (uint32_t)0xF << (FMC_A11_BUS_A11 * 4)
                   | (uint32_t)0xF << (FMC_A12_BUS_A12 * 4)
                   | (uint32_t)0xF << (FMC_A13_BUS_A13 * 4)
                   | (uint32_t)0xF << (FMC_A14_BUS_A14 * 4)
                   | (uint32_t)0xF << (FMC_A15_BUS_A15 * 4));
  GPIOG->AFR[0] |=  ((uint32_t)(GPIO_AF_FMC << (FMC_A10_BUS_A10 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A11_BUS_A11 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A12_BUS_A12 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A13_BUS_A13 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A14_BUS_A14 * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << (FMC_A15_BUS_A15 * 4)));

  GPIOG->AFR[1] &= ~((uint32_t)0xF << ((FMC_NE2_SRAMEN - 8) * 4)
                   | (uint32_t)0xF << ((FMC_A24_MUX2   - 8) * 4));
  GPIOG->AFR[1] |=  ((uint32_t)(GPIO_AF_FMC << ((FMC_NE2_SRAMEN - 8) * 4)))
                  | ((uint32_t)(GPIO_AF_FMC << ((FMC_A24_MUX2   - 8) * 4)));
  // ---------------------------------------------------------------------------
  GPIOI->MODER   &= ~(GPIO_MODER_MODER7);
  GPIOI->MODER   |= (((uint32_t)GPIO_Mode_OUT) << (BUS_IO * 2));

  GPIOI->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR7);
  GPIOI->OSPEEDR |= (((uint32_t)GPIO_Speed_100MHz) << (BUS_IO * 2));

  GPIOI->OTYPER  &= ~(GPIO_OTYPER_OT_7);
  GPIOI->OTYPER  |= ((uint32_t)(GPIO_OType_PP << BUS_IO));

  GPIOI->PUPDR   &= ~(GPIO_PUPDR_PUPDR7);
  GPIOI->PUPDR   |= (((uint32_t)GPIO_PuPd_NOPULL) << (BUS_IO * 2));
  // ---------------------------------------------------------------------------
  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_BUS_INITED);

  } // End if.

  return TRUE;

} // End ArcadeIT_BUS_Port_Init.

// -----------------------------------------------------------------------------
void ArcadeIT_BUS_Port_SRAM(
    uint8_t pScaler // speed divider to slow down the access time to the memory bank
    )
{
  /*
  * DESCRIPTION: This function configures the access timings to the SRAM module.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  FMC_NORSRAMInitTypeDef        lFMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_R;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_W;

  // SRAM Timing configuration.
  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 2 * pScaler;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 1 * pScaler;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 2 * pScaler;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 1;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 0;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  lNORSRAMTimingStructure_W.FMC_AddressSetupTime        = 2 * pScaler;
  lNORSRAMTimingStructure_W.FMC_AddressHoldTime         = 1 * pScaler;
  lNORSRAMTimingStructure_W.FMC_DataSetupTime           = 2 * pScaler;
  lNORSRAMTimingStructure_W.FMC_BusTurnAroundDuration   = 1;
  lNORSRAMTimingStructure_W.FMC_CLKDivision             = 0;
  lNORSRAMTimingStructure_W.FMC_DataLatency             = 0;
  lNORSRAMTimingStructure_W.FMC_AccessMode              = FMC_AccessMode_A;

  // FMC SRAM control configuration.
  lFMC_NORSRAMInitStructure.FMC_Bank                    = FMC_Bank1_NORSRAM1;
  lFMC_NORSRAMInitStructure.FMC_DataAddressMux          = FMC_DataAddressMux_Disable;
  lFMC_NORSRAMInitStructure.FMC_MemoryType              = FMC_MemoryType_SRAM;
  lFMC_NORSRAMInitStructure.FMC_MemoryDataWidth         = FMC_NORSRAM_MemoryDataWidth_8b;
  lFMC_NORSRAMInitStructure.FMC_BurstAccessMode         = FMC_BurstAccessMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_AsynchronousWait        = FMC_AsynchronousWait_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalPolarity      = FMC_WaitSignalPolarity_Low;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalActive        = FMC_WaitSignalActive_BeforeWaitState;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignal              = FMC_WaitSignal_Disable;
  lFMC_NORSRAMInitStructure.FMC_ExtendedMode            = FMC_ExtendedMode_Enable;
  lFMC_NORSRAMInitStructure.FMC_WriteBurst              = FMC_WriteBurst_Disable;
  lFMC_NORSRAMInitStructure.FMC_ContinousClock          = FMC_CClock_SyncOnly;

  lFMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct   = &lNORSRAMTimingStructure_R;
  lFMC_NORSRAMInitStructure.FMC_WriteTimingStruct       = &lNORSRAMTimingStructure_W;

  /* SRAM configuration */
  FMC_NORSRAMInit(&lFMC_NORSRAMInitStructure);

  /* Enable FMC Bank1_SRAM1 Bank */
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_SRAM_PORT_SET);

  } // End if.

}; // End ArcadeIT_Bus_Port_SRAM.
#if 1
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Bus_Port_LCD(
    uint8_t pScaler // speed divider to slow down the access time to the memory bank
    )
{
  /*
  * DESCRIPTION: This function configures the access timings to the SRAM module.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  FMC_NORSRAMInitTypeDef        lFMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_R;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_W;

  // SRAM Timing configuration.
  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 2 * pScaler;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 6 * pScaler;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 4 * pScaler;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 1 * 1;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  lNORSRAMTimingStructure_W.FMC_AddressSetupTime        = 4 * pScaler;
  lNORSRAMTimingStructure_W.FMC_AddressHoldTime         = 8 * pScaler;
  lNORSRAMTimingStructure_W.FMC_DataSetupTime           = 4 * pScaler;
  lNORSRAMTimingStructure_W.FMC_BusTurnAroundDuration   = 1 * 1;
  lNORSRAMTimingStructure_W.FMC_CLKDivision             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_AccessMode              = FMC_AccessMode_A;

  // FMC SRAM control configuration.
  lFMC_NORSRAMInitStructure.FMC_Bank                    = FMC_Bank1_NORSRAM1;
  lFMC_NORSRAMInitStructure.FMC_DataAddressMux          = FMC_DataAddressMux_Disable;
  lFMC_NORSRAMInitStructure.FMC_MemoryType              = FMC_MemoryType_SRAM;
  lFMC_NORSRAMInitStructure.FMC_MemoryDataWidth         = FMC_NORSRAM_MemoryDataWidth_8b;
  lFMC_NORSRAMInitStructure.FMC_BurstAccessMode         = FMC_BurstAccessMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_AsynchronousWait        = FMC_AsynchronousWait_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalPolarity      = FMC_WaitSignalPolarity_Low;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalActive        = FMC_WaitSignalActive_BeforeWaitState;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignal              = FMC_WaitSignal_Disable;
  lFMC_NORSRAMInitStructure.FMC_ExtendedMode            = FMC_ExtendedMode_Enable;
  lFMC_NORSRAMInitStructure.FMC_WriteBurst              = FMC_WriteBurst_Disable;
  lFMC_NORSRAMInitStructure.FMC_ContinousClock          = FMC_CClock_SyncOnly;

  lFMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct   = &lNORSRAMTimingStructure_R;
  lFMC_NORSRAMInitStructure.FMC_WriteTimingStruct       = &lNORSRAMTimingStructure_W;

  /* SRAM configuration */
  FMC_NORSRAMInit(&lFMC_NORSRAMInitStructure);

  /* Enable FMC Bank1_SRAM1 Bank */
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_LCD_PORT_SET);

  } // End if.

}; // End void ArcadeIT_Bus_Port_LCD.
#endif
// /////////////////////////////////////////////////////////////////////////////
#if 0
void ArcadeIT_BUS_Port_SRAM(uint8_t pDivider)
{
  /*
  * DESCRIPTION: This function configures the access timings to the SRAM module.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  FMC_NORSRAMInitTypeDef        lFMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_R;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_W;

  // SRAM Timing configuration.
  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 2 * pDivider;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 6 * pDivider;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 4 * pDivider;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 1 * 1;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  lNORSRAMTimingStructure_W.FMC_AddressSetupTime        = 4 * pDivider;
  lNORSRAMTimingStructure_W.FMC_AddressHoldTime         = 8 * pDivider;
  lNORSRAMTimingStructure_W.FMC_DataSetupTime           = 4 * pDivider;
  lNORSRAMTimingStructure_W.FMC_BusTurnAroundDuration   = 1 * 1;
  lNORSRAMTimingStructure_W.FMC_CLKDivision             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_AccessMode              = FMC_AccessMode_A;

  // FMC SRAM control configuration.
  lFMC_NORSRAMInitStructure.FMC_Bank                    = FMC_Bank1_NORSRAM1;
  lFMC_NORSRAMInitStructure.FMC_DataAddressMux          = FMC_DataAddressMux_Disable;
  lFMC_NORSRAMInitStructure.FMC_MemoryType              = FMC_MemoryType_SRAM;
  lFMC_NORSRAMInitStructure.FMC_MemoryDataWidth         = FMC_NORSRAM_MemoryDataWidth_8b;
  lFMC_NORSRAMInitStructure.FMC_BurstAccessMode         = FMC_BurstAccessMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_AsynchronousWait        = FMC_AsynchronousWait_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalPolarity      = FMC_WaitSignalPolarity_Low;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalActive        = FMC_WaitSignalActive_BeforeWaitState;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignal              = FMC_WaitSignal_Disable;
  lFMC_NORSRAMInitStructure.FMC_ExtendedMode            = FMC_ExtendedMode_Enable;
  lFMC_NORSRAMInitStructure.FMC_WriteBurst              = FMC_WriteBurst_Disable;
  lFMC_NORSRAMInitStructure.FMC_ContinousClock          = FMC_CClock_SyncAsync;

  lFMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct   = &lNORSRAMTimingStructure_R;
  lFMC_NORSRAMInitStructure.FMC_WriteTimingStruct       = &lNORSRAMTimingStructure_W;

  /* SRAM configuration */
  FMC_NORSRAMInit(&lFMC_NORSRAMInitStructure);

  /* Enable FMC Bank1_SRAM1 Bank */
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_SRAM_PORT_SET);

  } // End if.

}; // End ArcadeIT_Bus_Port_SRAM.
#endif
// /////////////////////////////////////////////////////////////////////////////
#if 0
void ArcadeIT_Bus_Port_Expansion(uint8_t pDivider)
{
  /*
  * DESCRIPTION: This function configures the access timings to the Expansion bus.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  FMC_NORSRAMInitTypeDef        lFMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_R;
  FMC_NORSRAMTimingInitTypeDef  lNORSRAMTimingStructure_W;
#if 1
  // Timing configuration.
  /*
  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 2 * pDivider;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 5 * pDivider;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 4 * pDivider;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 1 * pDivider;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 1 * 1;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  lNORSRAMTimingStructure_W.FMC_AddressSetupTime        = 2 * pDivider;
  lNORSRAMTimingStructure_W.FMC_AddressHoldTime         = 5 * pDivider;
  lNORSRAMTimingStructure_W.FMC_DataSetupTime           = 4 * pDivider;
  lNORSRAMTimingStructure_W.FMC_BusTurnAroundDuration   = 1 * pDivider;
  lNORSRAMTimingStructure_W.FMC_CLKDivision             = 1 * 1;
  lNORSRAMTimingStructure_W.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_AccessMode              = FMC_AccessMode_A;

  // FMC SRAM control configuration.
  lFMC_NORSRAMInitStructure.FMC_Bank                    = FMC_Bank1_NORSRAM2;
  lFMC_NORSRAMInitStructure.FMC_DataAddressMux          = FMC_DataAddressMux_Disable;
  lFMC_NORSRAMInitStructure.FMC_MemoryType              = FMC_MemoryType_SRAM;
  lFMC_NORSRAMInitStructure.FMC_MemoryDataWidth         = FMC_NORSRAM_MemoryDataWidth_8b;
  lFMC_NORSRAMInitStructure.FMC_BurstAccessMode         = FMC_BurstAccessMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_AsynchronousWait        = FMC_AsynchronousWait_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalPolarity      = FMC_WaitSignalPolarity_Low;
  lFMC_NORSRAMInitStructure.FMC_WrapMode                = FMC_WrapMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalActive        = FMC_WaitSignalActive_BeforeWaitState;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignal              = FMC_WaitSignal_Disable;
  lFMC_NORSRAMInitStructure.FMC_ExtendedMode            = FMC_ExtendedMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_WriteBurst              = FMC_WriteBurst_Disable;
  lFMC_NORSRAMInitStructure.FMC_ContinousClock          = FMC_CClock_SyncAsync;
  lFMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct   = &lNORSRAMTimingStructure_R;
  lFMC_NORSRAMInitStructure.FMC_WriteTimingStruct       = &lNORSRAMTimingStructure_W;
  */

  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 15;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 40;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 25;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 1;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 0;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  // FMC SRAM control configuration.
  lFMC_NORSRAMInitStructure.FMC_Bank                    = FMC_Bank1_NORSRAM2;
  lFMC_NORSRAMInitStructure.FMC_DataAddressMux          = FMC_DataAddressMux_Disable;
  lFMC_NORSRAMInitStructure.FMC_MemoryType              = FMC_MemoryType_SRAM;
  lFMC_NORSRAMInitStructure.FMC_MemoryDataWidth         = FMC_NORSRAM_MemoryDataWidth_8b;
  lFMC_NORSRAMInitStructure.FMC_BurstAccessMode         = FMC_BurstAccessMode_Enable;
  lFMC_NORSRAMInitStructure.FMC_AsynchronousWait        = FMC_AsynchronousWait_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalPolarity      = FMC_WaitSignalPolarity_Low;
  lFMC_NORSRAMInitStructure.FMC_WrapMode                = FMC_WrapMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalActive        = FMC_WaitSignalActive_BeforeWaitState;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignal              = FMC_WaitSignal_Disable;
  lFMC_NORSRAMInitStructure.FMC_ExtendedMode            = FMC_ExtendedMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_WriteBurst              = FMC_WriteBurst_Disable;
  lFMC_NORSRAMInitStructure.FMC_ContinousClock          = FMC_CClock_SyncAsync;

  lFMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct   = &lNORSRAMTimingStructure_R;
  //lFMC_NORSRAMInitStructure.FMC_WriteTimingStruct       = &lNORSRAMTimingStructure_W;

#else
  // SRAM Timing configuration.
  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 2 * pDivider;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 4 * pDivider;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 2 * pDivider;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 1 * 1;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  lNORSRAMTimingStructure_W.FMC_AddressSetupTime        = 3 * pDivider;
  lNORSRAMTimingStructure_W.FMC_AddressHoldTime         = 6 * pDivider;
  lNORSRAMTimingStructure_W.FMC_DataSetupTime           = 3 * pDivider;
  lNORSRAMTimingStructure_W.FMC_BusTurnAroundDuration   = 1 * 1;
  lNORSRAMTimingStructure_W.FMC_CLKDivision             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_AccessMode              = FMC_AccessMode_A;

  // FMC SRAM control configuration.
  lFMC_NORSRAMInitStructure.FMC_Bank                    = FMC_Bank1_NORSRAM2;
  lFMC_NORSRAMInitStructure.FMC_DataAddressMux          = FMC_DataAddressMux_Disable;
  lFMC_NORSRAMInitStructure.FMC_MemoryType              = FMC_MemoryType_SRAM;
  lFMC_NORSRAMInitStructure.FMC_MemoryDataWidth         = FMC_NORSRAM_MemoryDataWidth_8b;
  lFMC_NORSRAMInitStructure.FMC_BurstAccessMode         = FMC_BurstAccessMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_AsynchronousWait        = FMC_AsynchronousWait_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalPolarity      = FMC_WaitSignalPolarity_Low;
  lFMC_NORSRAMInitStructure.FMC_WrapMode                = FMC_WrapMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignalActive        = FMC_WaitSignalActive_BeforeWaitState;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WriteOperation          = FMC_WriteOperation_Enable;
  lFMC_NORSRAMInitStructure.FMC_WaitSignal              = FMC_WaitSignal_Disable;
  lFMC_NORSRAMInitStructure.FMC_ExtendedMode            = FMC_ExtendedMode_Disable;
  lFMC_NORSRAMInitStructure.FMC_WriteBurst              = FMC_WriteBurst_Disable;
  lFMC_NORSRAMInitStructure.FMC_ContinousClock          = FMC_CClock_SyncAsync;
  lFMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct   = &lNORSRAMTimingStructure_R;
  lFMC_NORSRAMInitStructure.FMC_WriteTimingStruct       = &lNORSRAMTimingStructure_W;
#endif

  /*
  // Timing configuration.
  lNORSRAMTimingStructure_R.FMC_AddressSetupTime        = 8 * pDivider;
  lNORSRAMTimingStructure_R.FMC_AddressHoldTime         = 44 * pDivider;
  lNORSRAMTimingStructure_R.FMC_DataSetupTime           = 36 * pDivider;
  lNORSRAMTimingStructure_R.FMC_BusTurnAroundDuration   = 2 * 1;
  lNORSRAMTimingStructure_R.FMC_CLKDivision             = 2 * 1;
  lNORSRAMTimingStructure_R.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_R.FMC_AccessMode              = FMC_AccessMode_A;

  lNORSRAMTimingStructure_W.FMC_AddressSetupTime        = 8 * pDivider;
  lNORSRAMTimingStructure_W.FMC_AddressHoldTime         = 44 * pDivider;
  lNORSRAMTimingStructure_W.FMC_DataSetupTime           = 36 * pDivider;
  lNORSRAMTimingStructure_W.FMC_BusTurnAroundDuration   = 2 * 1;
  lNORSRAMTimingStructure_W.FMC_CLKDivision             = 2 * 1;
  lNORSRAMTimingStructure_W.FMC_DataLatency             = 0 * 1;
  lNORSRAMTimingStructure_W.FMC_AccessMode              = FMC_AccessMode_A;
*/

  /* SRAM configuration */
  FMC_NORSRAMInit(&lFMC_NORSRAMInitStructure);

  /* Enable FMC Bank1_SRAM2 Bank */
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM2, ENABLE);

#if 0
  ArcadeIT_Terminal_Debug( "\n\rChecking Expansion slots.");
  uint8_t lValue = 0;

  // Slots.
  uint32_t lSlot = 1, lError = 0;
  uint8_t *SlotPtr = gExpansionSlotAddress[lSlot];
  for (uint16_t lrow = 0; lrow < sizeof(gTestCart)/8; lrow++)
  {
    sprintf(gString, "\n\r%08X: ", (uint8_t*)(SlotPtr + (lrow * 8)), lSlot);
    ArcadeIT_Terminal_Debug(gString);
    for (uint8_t lBytes = 0; lBytes < 8; lBytes++)
    {

      //*(__IO uint8_t *)(0x60000000 + lBytes) = lBytes & 0xFF;
      lValue = SlotPtr[0x8000 + (lrow * 8) + lBytes];
      if (gTestCart[(lrow * 8) + lBytes] != lValue)
      {
        lError++;
        sprintf(gString, "-- ", lValue);
      }
      else
        sprintf(gString, "%02X ", lValue);

      ArcadeIT_Terminal_Debug(gString);


    } // End for.

    ArcadeIT_Terminal_Debug(" | ");

    for (uint8_t lBytes = 0; lBytes < 8; lBytes++)
    {
      //*(__IO uint8_t *)(0x60000000 + lBytes) = lBytes & 0xFF;
      lValue = SlotPtr[0x8000 + (lrow * 8) + lBytes];
      sprintf(gString, "%c", (lValue >= ASCII_SPACE && lValue < ASCII_DELETE) ? lValue : '.');
      ArcadeIT_Terminal_Debug(gString);

    } // End for.

  } // End for.

  sprintf(gString, "\n\rErrors: %d\n\r", lError);
  ArcadeIT_Terminal_Debug(gString);

  while(1) {};
#endif

#if 0
  // LCDs
  for (uint32_t lLCD = 0; lLCD < 2; lLCD++)
  {
    __IO uint8_t* lLCDPtr = gDisplayAddress[lLCD];

    for (uint32_t lBytes = 0; lBytes < 16; lBytes++)
    {
      //*(__IO uint8_t *)(0x60000000 + lBytes) = lBytes & 0xFF;
      lLCDPtr[lBytes] = lBytes & 0xFF;

    } // End for.

    // RS = 1
    for (uint32_t lBytes = (1<<16); lBytes < (1<<16)+16; lBytes++)
    {
      //*(__IO uint8_t *)(0x60000000 + lBytes) = lBytes & 0xFF;
      lLCDPtr[lBytes] = lBytes & 0xFF;

    } // End for.

  } // End for.
#endif

  if (gDevices & ARCADEIT_DEVICE_SERIAL_PORT)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_EXPANSION_PORT_SET);

  } // End if.

}; // End ArcadeIT_Bus_Port_Expansion.
#endif
// /////////////////////////////////////////////////////////////////////////////
#if 0
void ArcadeIT_Bus_Port_Slot
(
  uint8_t pSlot // The slot to select.
)
{
  /*
  * DESCRIPTION: This function is used to manually select the slot CS line of the
  *              Expansion bus.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  switch (pSlot)
  {
    case 0:
      GPIOE->BSRRH = GPIO_Pin_6;
      GPIOE->BSRRH = GPIO_Pin_2;
      GPIOG->BSRRH = GPIO_Pin_13;
    break;

    case 1:
      GPIOE->BSRRL = GPIO_Pin_6;
      GPIOE->BSRRH = GPIO_Pin_2;
      GPIOG->BSRRH = GPIO_Pin_13;
    break;

    case 2:
      GPIOE->BSRRH = GPIO_Pin_6;
      GPIOE->BSRRL = GPIO_Pin_2;
      GPIOG->BSRRH = GPIO_Pin_13;
    break;

    case 3:
      GPIOE->BSRRL = GPIO_Pin_6;
      GPIOE->BSRRL = GPIO_Pin_2;
      GPIOG->BSRRH = GPIO_Pin_13;
    break;

    case 4:
      GPIOE->BSRRH = GPIO_Pin_6;
      GPIOE->BSRRH = GPIO_Pin_2;
      GPIOG->BSRRL = GPIO_Pin_13;
    break;

    case 5:
      GPIOE->BSRRL = GPIO_Pin_6;
      GPIOE->BSRRH = GPIO_Pin_2;
      GPIOG->BSRRL = GPIO_Pin_13;
    break;

    case 6:
      GPIOE->BSRRH = GPIO_Pin_6;
      GPIOE->BSRRL = GPIO_Pin_2;
      GPIOG->BSRRL = GPIO_Pin_13;
    break;

    case 7:
      GPIOE->BSRRL = GPIO_Pin_6;
      GPIOE->BSRRL = GPIO_Pin_2;
      GPIOG->BSRRL = GPIO_Pin_13;
    break;

  } // End switch.

}; // End ArcadeIT_Bus_Port_Slot_Set.

// /////////////////////////////////////////////////////////////////////////////
#endif
