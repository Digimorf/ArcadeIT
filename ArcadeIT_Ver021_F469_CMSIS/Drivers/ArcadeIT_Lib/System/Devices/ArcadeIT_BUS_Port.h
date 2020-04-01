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

#ifndef _ARCADEIT_BUS_PORT_H_
#define _ARCADEIT_BUS_PORT_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include "System/ArcadeIT_Common.h"
#include "System/ArcadeIT_Utilities.h"
#include "System/ArcadeIT_Firmware.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

#define GPIO_AF_FMC         ((uint8_t)0xC)  /* FMC Alternate Function mapping */

// GPIOD
#define FMC_D2_BUS_D2       0
#define FMC_D3_BUS_D3       1
#define FMC_NOE_BUS_RD      4
#define FMC_NWE_BUS_WR      5
#define FMC_A16_BUS_A16     11
#define FMC_A17_BUS_A17     12
#define FMC_A18_BUS_A18     13
#define FMC_D0_BUS_D0       14
#define FMC_D1_BUS_D1       15
#define FMC_NE1_BUS_LCDEN   7
// GPIOE
#define FMC_A19_BUS_A19     3
#define FMC_A20_BUS_A20     4
#define FMC_A21_BUS_A21     5
#define FMC_D4_BUS_D4       7
#define FMC_D5_BUS_D5       8
#define FMC_D6_BUS_D6       9
#define FMC_D7_BUS_D7       10
#define FMC_A23_BUS_MUX1    2
#define FMC_A22_BUS_MUX0    6
// GPIOF
#define FMC_A00_BUS_A00     0
#define FMC_A01_BUS_A01     1
#define FMC_A02_BUS_A02     2
#define FMC_A03_BUS_A03     3
#define FMC_A04_BUS_A04     4
#define FMC_A05_BUS_A05     5
#define FMC_A06_BUS_A06     12
#define FMC_A07_BUS_A07     13
#define FMC_A08_BUS_A08     14
#define FMC_A09_BUS_A09     15
// GPIOG
#define FMC_A10_BUS_A10     0
#define FMC_A11_BUS_A11     1
#define FMC_A12_BUS_A12     2
#define FMC_A13_BUS_A13     3
#define FMC_A14_BUS_A14     4
#define FMC_A15_BUS_A15     5
#define FMC_NE2_SRAMEN      9
#define FMC_A24_MUX2        13
// GPIOI
#define BUS_IO              7

// -----------------------------------------------------------------------------
#define FMC_AccessMode_A                        ((uint32_t)0x00000000)
#define FMC_AccessMode_B                        ((uint32_t)0x10000000)
#define FMC_AccessMode_C                        ((uint32_t)0x20000000)
#define FMC_AccessMode_D                        ((uint32_t)0x30000000)

#define FMC_Bank1_NORSRAM1                      ((uint32_t)0x00000000)
#define FMC_Bank1_NORSRAM2                      ((uint32_t)0x00000002)
#define FMC_Bank1_NORSRAM3                      ((uint32_t)0x00000004)
#define FMC_Bank1_NORSRAM4                      ((uint32_t)0x00000006)

#define FMC_DataAddressMux_Disable              ((uint32_t)0x00000000)

#define FMC_MemoryType_SRAM                     ((uint32_t)0x00000000)
#define FMC_MemoryType_PSRAM                    ((uint32_t)0x00000004)
#define FMC_MemoryType_NOR                      ((uint32_t)0x00000008)

#define FMC_NORSRAM_MemoryDataWidth_8b          ((uint32_t)0x00000000)

#define FMC_BurstAccessMode_Disable             ((uint32_t)0x00000000)
#define FMC_BurstAccessMode_Enable              ((uint32_t)0x00000100)

#define FMC_WriteOperation_Disable              ((uint32_t)0x00000000)
#define FMC_WriteOperation_Enable               ((uint32_t)0x00001000)

#define FMC_ExtendedMode_Disable                ((uint32_t)0x00000000)
#define FMC_ExtendedMode_Enable                 ((uint32_t)0x00004000)

#define FMC_AsynchronousWait_Disable            ((uint32_t)0x00000000)
#define FMC_AsynchronousWait_Enable             ((uint32_t)0x00008000)

#define FMC_WaitSignal_Disable                  ((uint32_t)0x00000000)
#define FMC_WaitSignal_Enable                   ((uint32_t)0x00002000)

#define FMC_WriteBurst_Disable                  ((uint32_t)0x00000000)
#define FMC_WriteBurst_Enable                   ((uint32_t)0x00080000)

#define FMC_CClock_SyncOnly                     ((uint32_t)0x00000000)
#define FMC_CClock_SyncAsync                    ((uint32_t)0x00100000)

#define FMC_WaitSignalPolarity_Low              ((uint32_t)0x00000000)
#define FMC_WaitSignalPolarity_High             ((uint32_t)0x00000200)

#define FMC_WaitSignalActive_BeforeWaitState    ((uint32_t)0x00000000)
#define FMC_WaitSignalActive_DuringWaitState    ((uint32_t)0x00000800)

// /////////////////////////////////////////////////////////////////////////////
// Macros.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types.
// /////////////////////////////////////////////////////////////////////////////
/**
  * Taken from STM32F4 Standard Peripheral Library 1.8 -------------------------
  *
  * @brief  Timing parameters For NOR/SRAM Banks
  */
typedef struct
{
  uint32_t FMC_AddressSetupTime;       /*!< Defines the number of HCLK cycles to configure
                                             the duration of the address setup time.
                                             This parameter can be a value between 0 and 15.
                                             @note This parameter is not used with synchronous NOR Flash memories. */

  uint32_t FMC_AddressHoldTime;        /*!< Defines the number of HCLK cycles to configure
                                             the duration of the address hold time.
                                             This parameter can be a value between 1 and 15.
                                             @note This parameter is not used with synchronous NOR Flash memories.*/

  uint32_t FMC_DataSetupTime;          /*!< Defines the number of HCLK cycles to configure
                                             the duration of the data setup time.
                                             This parameter can be a value between 1 and 255.
                                             @note This parameter is used for SRAMs, ROMs and asynchronous multiplexed NOR Flash memories. */

  uint32_t FMC_BusTurnAroundDuration;  /*!< Defines the number of HCLK cycles to configure
                                             the duration of the bus turnaround.
                                             This parameter can be a value between 0 and 15.
                                             @note This parameter is only used for multiplexed NOR Flash memories. */

  uint32_t FMC_CLKDivision;            /*!< Defines the period of CLK clock output signal, expressed in number of HCLK cycles.
                                             This parameter can be a value between 1 and 15.
                                             @note This parameter is not used for asynchronous NOR Flash, SRAM or ROM accesses. */

  uint32_t FMC_DataLatency;            /*!< Defines the number of memory clock cycles to issue
                                             to the memory before getting the first data.
                                             The parameter value depends on the memory type as shown below:
                                              - It must be set to 0 in case of a CRAM
                                              - It is don't care in asynchronous NOR, SRAM or ROM accesses
                                              - It may assume a value between 0 and 15 in NOR Flash memories
                                                with synchronous burst mode enable */

  uint32_t FMC_AccessMode;             /*!< Specifies the asynchronous access mode.
                                             This parameter can be a value of @ref FMC_Access_Mode */
}FMC_NORSRAMTimingInitTypeDef;

/**
  * Taken from STM32F4 Standard Peripheral Library 1.8 -------------------------
  *
  * @brief  FMC NOR/SRAM Init structure definition
  */
typedef struct
{
  uint32_t FMC_Bank;                /*!< Specifies the NOR/SRAM memory bank that will be used.
                                          This parameter can be a value of @ref FMC_NORSRAM_Bank */

  uint32_t FMC_DataAddressMux;      /*!< Specifies whether the address and data values are
                                          multiplexed on the databus or not.
                                          This parameter can be a value of @ref FMC_Data_Address_Bus_Multiplexing */

  uint32_t FMC_MemoryType;          /*!< Specifies the type of external memory attached to
                                          the corresponding memory bank.
                                          This parameter can be a value of @ref FMC_Memory_Type */

  uint32_t FMC_MemoryDataWidth;     /*!< Specifies the external memory device width.
                                          This parameter can be a value of @ref FMC_NORSRAM_Data_Width */

  uint32_t FMC_BurstAccessMode;     /*!< Enables or disables the burst access mode for Flash memory,
                                          valid only with synchronous burst Flash memories.
                                          This parameter can be a value of @ref FMC_Burst_Access_Mode */

  uint32_t FMC_WaitSignalPolarity;  /*!< Specifies the wait signal polarity, valid only when accessing
                                          the Flash memory in burst mode.
                                          This parameter can be a value of @ref FMC_Wait_Signal_Polarity */

  uint32_t FMC_WaitSignalActive;    /*!< Specifies if the wait signal is asserted by the memory one
                                          clock cycle before the wait state or during the wait state,
                                          valid only when accessing memories in burst mode.
                                          This parameter can be a value of @ref FMC_Wait_Timing */

  uint32_t FMC_WriteOperation;      /*!< Enables or disables the write operation in the selected bank by the FMC.
                                          This parameter can be a value of @ref FMC_Write_Operation */

  uint32_t FMC_WaitSignal;          /*!< Enables or disables the wait state insertion via wait
                                          signal, valid for Flash memory access in burst mode.
                                          This parameter can be a value of @ref FMC_Wait_Signal */

  uint32_t FMC_ExtendedMode;        /*!< Enables or disables the extended mode.
                                          This parameter can be a value of @ref FMC_Extended_Mode */

  uint32_t FMC_AsynchronousWait;     /*!< Enables or disables wait signal during asynchronous transfers,
                                          valid only with asynchronous Flash memories.
                                          This parameter can be a value of @ref FMC_AsynchronousWait */

  uint32_t FMC_WriteBurst;          /*!< Enables or disables the write burst operation.
                                          This parameter can be a value of @ref FMC_Write_Burst */

  uint32_t FMC_ContinousClock;       /*!< Enables or disables the FMC clock output to external memory devices.
                                          This parameter is only enabled through the FMC_BCR1 register, and don't care
                                          through FMC_BCR2..4 registers.
                                          This parameter can be a value of @ref FMC_Continous_Clock */


  FMC_NORSRAMTimingInitTypeDef* FMC_ReadWriteTimingStruct; /*!< Timing Parameters for write and read access if the  Extended Mode is not used*/

  FMC_NORSRAMTimingInitTypeDef* FMC_WriteTimingStruct;     /*!< Timing Parameters for write access if the  Extended Mode is used*/

}FMC_NORSRAMInitTypeDef;

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
int ArcadeIT_BUS_Port_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_BUS_Port_LCD(
    uint8_t pScaler // speed divider to slow down the access time to the memory bank
    );
// -----------------------------------------------------------------------------
void ArcadeIT_BUS_Port_SRAM(
    uint8_t pScaler // speed divider to slow down the access time to the memory bank
    );
// -----------------------------------------------------------------------------
void ArcadeIT_BUS_Port_Expansion(
    uint8_t pScaler // spoeed divider to slow down the access time to the memory bank
    );
// -----------------------------------------------------------------------------

// /////////////////////////////////////////////////////////////////////////////
#endif // _ARCADEIT_BUS_PORT_H_

/*
ArcadeIT! Memory mapped periherals


.--------------------------------------------------------------------------------.
|                                 Address bits 31     24      16      8       0  |
|                                              |      |       |       |       |  |
|                                       #########################################|
|    .---------------------------------------->0010............................ #|
|+---|--------+-------+                 #                                       #|
|| 0x20000000 |       |                 #                                       #|
||          : | RAM   |                 #                                       #|
|| 0x20000000 |       |                 #                                       #|
|+------------+-------+                 #########################################|
'--------------------------------------------------------------------------------'

.--------------------------------------------------------------------------------.
|                                                                                |
|                                 Address bits 31     24      16      8       0  |
|                                              |      |       |       |       |  |
|                                       #########################################|
|    .---------------------------------------->0110............................ #|
|+---|--------+-------+                 #                                       #|
|| 0x60400000 |       | LCD_EN---->NE1 ------->....00.......................... #|
|| 0x60410000 | LCD1  |             ^   #      |      |       |       |       | #|
||            |       | MUX_1-------------Cmd->.......001.....0................ #|
|+------------+-------+             |   # Dat->.......001.....1................ #|
|| 0x60800000 |       | LCD_EN------+   #      |      |       |       |       | #|
|| 0x60810000 | LCD2  |                 #      |      |       |       |       | #|
||            |       | MUX_2-------------Cmd->.......010.....0................ #|
|+------------+-------+                 # Dat->.......010.....1................ #|
|                                       #########################################|
| Typical LCD display driver access:                                             |
|                                                                                |
| *((uint8_t*)0x60400000) = Command;                                             |
| *((uint8_t*)0x60410000) = Data0;                                               |
| *((uint8_t*)0x60410000) = Data1;                                               |
|            -                                                                   |
| *((uint8_t*)0x60410000) = Datan;                                               |
|                                                                                |
|================================================================================|
|                                                                                |
|                                 Address bits 31     24      16      8       0  |
|                                              |      |       |       |       |  |
|                                       #########################################|
|    .---------------------------------------->0110............................ #|
|+---|--------+-------+                 #                                        |
||   |        |       | SRAM_EN--->NE2 ------->....01.......................... #|
||   |        | Exp.  |                 #      |      |       |       |       | #|
|| 0x64000000 | SRAM  |       0-512 kB ------->...........00xxxxxxxxxxxxxxxxxxx #|
||          : |       |    512-1024 kB ------->...........01xxxxxxxxxxxxxxxxxxx #|
||          : |       |   1024-1536 kB ------->...........10xxxxxxxxxxxxxxxxxxx #|
|| 0x641FFFFF |       |   1536-2048 kB ------->...........11xxxxxxxxxxxxxxxxxxx #|
||            |       |                 #      |      |       |       |       | #|
|+------------+-------+                 #########################################|
|                                                                                |
| Typical access to SRAM expansion:                                              |
|                                                                                |
| *((uint8_t*)0x64000000) = Data0;                                               |
| *((uint8_t*)0x64000001) = Data1;                                               |
|            -                                                                   |
| *((uint8_t*)0x641FFFFF) = Datan;                                               |
|                                                                                |
|================================================================================|
|                                                                                |
|                                 Address bits 31     24      16      8       0  |
|                                              |      |       |       |       |  |
|                                       #########################################|
|    .---------------------------------------->0110............................ #|
|+---|--------+-------+                 #                                        |
|| 0x68400000 | Exp.  | BUS_EN---->NE3 ------->....10.......................... #|
||          : | SLOT0 |             ^   #      |      |       |       |       | #|
|| 0x683FFFFF |       | EN_SLOT_0------------->.......001xxxxxxxxxxxxxxxxxxxxxx #|
|+------------+-------+             |   #      |      |       |       |       | #|
|| 0x68400000 | Exp.  | BUS_EN------+   #      |      |       |       |       | #|
||          : | SLOT1 |             |   #      |      |       |       |       | #|
|| 0x687FFFFF |       | EN_SLOT_1------------->.......01xxxxxxxxxxxxxxxxxxxxxxx #|
|+------------+-------+             |   #      |      |       |       |       | #|
|| 0x68800000 | Exp.  | BUS_EN------+   #      |      |       |       |       | #|
||          : | SLOT2 |             |   #      |      |       |       |       | #|
|| 0x68FFFFFF |       | EN_SLOT_2------------->.......011xxxxxxxxxxxxxxxxxxxxxx #|
|+------------+-------+             |   #      |      |       |       |       | #|
|| 0x69000000 | Exp.  | BUS_EN------+   #      |      |       |       |       | #|
||          : | SLOT3 |                 #      |      |       |       |       | #|
|| 0x69FFFFFF |       | EN_SLOT_3------------->.......100xxxxxxxxxxxxxxxxxxxxxx #|
|+------------+-------+                 #                                       #|
|                                       #########################################|
| Typical access to SRAM expansion:                                              |
|                                                                                |
| *((uint8_t*)0x64000000) = Data0;                                               |
| *((uint8_t*)0x64000001) = Data1;                                               |
|            -                                                                   |
| *((uint8_t*)0x641FFFFF) = Datan;                                               |
|                                                                                |
'--------------------------------------------------------------------------------'

.--------------------------------------------------------------------------------.
|                                 Address bits 31     24      16      8       0  |
|                                              |      |       |       |       |  |
|                                       #########################################|
|    .---------------------------------------->1000............................ #|
|+---|--------+-------+                 #                                       #|
|| 0x80000000 |       |                 #                                       #|
||          : | FLASH |                 #                                       #|
|| 0x801FFFFF |       |                 #                                       #|
|+------------+-------+                 #########################################|
'--------------------------------------------------------------------------------'

 */
