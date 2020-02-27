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
 *                           S Y S T E M S
 * -----------------------------------------------------------------
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_RTC.h
 * @version V0.13
 * @date    11-10-2017
 * @last    26-02-2020
 * @brief   This library is used to handle the RTC system of the ArcadeIT board.
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY
 11-10-2017:
 - Creation date of the library structure.

 26-02-2020:
 - converted to CMSIS and ported to GitHUB

 ******************************************************************************
*/

#ifndef _ARCADEIT_RTC_H_
#define _ARCADEIT_RTC_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////
#define RTC_BKP_DR0             ((uint32_t)0x00000000)
#define RTC_BKP_DR1             ((uint32_t)0x00000001)
#define RTC_BKP_DR2             ((uint32_t)0x00000002)
#define RTC_BKP_DR3             ((uint32_t)0x00000003)
#define RTC_BKP_DR4             ((uint32_t)0x00000004)
#define RTC_BKP_DR5             ((uint32_t)0x00000005)
#define RTC_BKP_DR6             ((uint32_t)0x00000006)
#define RTC_BKP_DR7             ((uint32_t)0x00000007)
#define RTC_BKP_DR8             ((uint32_t)0x00000008)
#define RTC_BKP_DR9             ((uint32_t)0x00000009)
#define RTC_BKP_DR10            ((uint32_t)0x0000000A)
#define RTC_BKP_DR11            ((uint32_t)0x0000000B)
#define RTC_BKP_DR12            ((uint32_t)0x0000000C)
#define RTC_BKP_DR13            ((uint32_t)0x0000000D)
#define RTC_BKP_DR14            ((uint32_t)0x0000000E)
#define RTC_BKP_DR15            ((uint32_t)0x0000000F)
#define RTC_BKP_DR16            ((uint32_t)0x00000010)
#define RTC_BKP_DR17            ((uint32_t)0x00000011)
#define RTC_BKP_DR18            ((uint32_t)0x00000012)
#define RTC_BKP_DR19            ((uint32_t)0x00000013)

#define RTC_HourFormat_24       ((uint32_t)0x00000000)
#define RTC_HourFormat_12       ((uint32_t)0x00000040)

#define RTC_Format_BIN          ((uint32_t)0x000000000)
#define RTC_Format_BCD          ((uint32_t)0x000000001)

#define RTC_FLAG_RECALPF        ((uint32_t)0x00010000)
#define RTC_FLAG_TAMP1F         ((uint32_t)0x00002000)
#define RTC_FLAG_TAMP2F         ((uint32_t)0x00004000)
#define RTC_FLAG_TSOVF          ((uint32_t)0x00001000)
#define RTC_FLAG_TSF            ((uint32_t)0x00000800)
#define RTC_FLAG_WUTF           ((uint32_t)0x00000400)
#define RTC_FLAG_ALRBF          ((uint32_t)0x00000200)
#define RTC_FLAG_ALRAF          ((uint32_t)0x00000100)
#define RTC_FLAG_INITF          ((uint32_t)0x00000040)
#define RTC_FLAG_RSF            ((uint32_t)0x00000020)
#define RTC_FLAG_INITS          ((uint32_t)0x00000010)
#define RTC_FLAG_SHPF           ((uint32_t)0x00000008)
#define RTC_FLAG_WUTWF          ((uint32_t)0x00000004)
#define RTC_FLAG_ALRBWF         ((uint32_t)0x00000002)
#define RTC_FLAG_ALRAWF         ((uint32_t)0x00000001)

#define RTC_TR_RESERVED_MASK    ((uint32_t)0x007F7F7F)
#define RTC_DR_RESERVED_MASK    ((uint32_t)0x00FFFF3F)
#define RTC_INIT_MASK           ((uint32_t)0xFFFFFFFF)
#define RTC_RSF_MASK            ((uint32_t)0xFFFFFF5F)
#define RTC_FLAGS_MASK          ((uint32_t)(RTC_FLAG_TSOVF  | RTC_FLAG_TSF     | RTC_FLAG_WUTF   | \
                                            RTC_FLAG_ALRBF  | RTC_FLAG_ALRAF   | RTC_FLAG_INITF  | \
                                            RTC_FLAG_RSF    | RTC_FLAG_INITS   | RTC_FLAG_WUTWF  | \
                                            RTC_FLAG_ALRBWF | RTC_FLAG_ALRAWF  | RTC_FLAG_TAMP1F | \
                                            RTC_FLAG_TAMP2F | RTC_FLAG_RECALPF | RTC_FLAG_SHPF))

#define INITMODE_TIMEOUT        ((uint32_t) 0x00010000)
#define SYNCHRO_TIMEOUT         ((uint32_t) 0x00020000)
#define RECALPF_TIMEOUT         ((uint32_t) 0x00020000)
#define SHPF_TIMEOUT            ((uint32_t) 0x00001000)

#define RTC_BKP_DR_NUMBER       0x14
#define FIRST_DATA              0x32F2

#define RCC_OFFSET              (RCC_BASE - PERIPH_BASE)
#define PWR_OFFSET              (PWR_BASE - PERIPH_BASE)

#define CR_OFFSET               (PWR_OFFSET + 0x00)
#define DBP_BitNumber           0x08
#define CR_DBP_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

#define BDCR_OFFSET             (RCC_OFFSET + 0x70)
#define RTCEN_BitNumber         0x0F
#define BDCR_RTCEN_BB           (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

#define BDRST_BitNumber         0x10
#define BDCR_BDRST_BB           (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

#define CSR_OFFSET              (RCC_OFFSET + 0x74)
#define LSION_BitNumber         0x00
#define CSR_LSION_BB            (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

#define RCC_FLAG_HSIRDY         ((uint8_t)0x21)
#define RCC_FLAG_HSERDY         ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY         ((uint8_t)0x39)
#define RCC_FLAG_PLLI2SRDY      ((uint8_t)0x3B)
#define RCC_FLAG_PLLSAIRDY      ((uint8_t)0x3D)
#define RCC_FLAG_LSERDY         ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY         ((uint8_t)0x61)
#define RCC_FLAG_BORRST         ((uint8_t)0x79)
#define RCC_FLAG_PINRST         ((uint8_t)0x7A)
#define RCC_FLAG_PORRST         ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST         ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST        ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST        ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST        ((uint8_t)0x7F)

#define RCC_RTCCLKSource_LSE    ((uint32_t)0x00000100)

#define BDCR_ADDRESS            (PERIPH_BASE + BDCR_OFFSET)
#define RCC_LSE_OFF             ((uint8_t)0x00)
#define RCC_LSE_ON              ((uint8_t)0x01)
#define RCC_LSE_Bypass          ((uint8_t)0x04)

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////
typedef struct {
  uint8_t   month;  // 1..12
  uint8_t   mday;   // 1..31
  uint8_t   year;   // 0...99
  uint8_t   hour;   // 0..23
  uint8_t   min;    // 0..59
  uint8_t   sec;    // 0..59

} rtc_time_t;

// /////////////////////////////////////////////////////////////////////////////
// External elements
// /////////////////////////////////////////////////////////////////////////////
extern char gTimeStamp[SYSTEM_TIMESTAMP_LEN];

// /////////////////////////////////////////////////////////////////////////////
// Global elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
uint32_t ArcadeIT_RTC_ReadBackupRegister
(
    uint32_t RTC_BKP_DR // RTC Backup data Register number.
);
// -----------------------------------------------------------------------------
uint32_t ArcadeIT_RTC_CheckBackupReg
(
  uint16_t FirstBackupData  // The pointer to the string.
);
// -----------------------------------------------------------------------------
void ArcadeIT_RTC_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_RTC_TimeDate_String(void);
// -----------------------------------------------------------------------------
rtc_time_t ArcadeIT_RTC_TimeDate_Get(void);
// -----------------------------------------------------------------------------
void ArcadeIT_RTC_TimeDate_Set
(
  rtc_time_t *pTime   // The pointer to the string where to store the information.
);

// /////////////////////////////////////////////////////////////////////////////

/*
  From: RM0386, Reference manual, page 962

  28. Real-time clock (RTC)

  The real-time clock (RTC) is an independent BCD timer/counter. The RTC provides a
  time-of-day clock/calendar, two programmable alarm interrupts, and a periodic
  programmable wakeup flag with interrupt capability.

  -

  From: RM0386, Reference manual, page 963

  Figure 319. RTC block diagram

  -
  The RTC clock source (RTCCLK) is selected through the clock controller among the LSE
  clock, the LSI oscillator clock, and the HSE clock. For more information on the RTC clock
  source configuration, refer to Section 6: Reset and clock control (RCC).

  The ArcadeIT has a LSE clock driven by a 32.768 kHz crystal.

  The RTC calendar time and date registers are accessed through shadow registers which
  are synchronized with PCLK1 (APB1 clock). They can also be accessed directly in order to
  avoid waiting for the synchronization duration. ArcadeIT firmware provides a high
  level function to set/get current time, and connecting a battery to the specific
  connector it's possible to keep the time and date alive as long as the battery
  lives.

  The use of the RTC comes useful where time and date are needed, for example
  files creation or time driven application.

  The RTC peripheral of the STM32 line of MCU is quite complex, and we decided
  to provide only the simple time and date function, but you are free to improve
  this driver as you need.
 */

#endif // _ARCADEIT_RTC_H_
