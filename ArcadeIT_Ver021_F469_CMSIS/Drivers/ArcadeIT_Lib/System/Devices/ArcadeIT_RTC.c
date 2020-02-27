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
 *           (C)2020 www.digimorf.com, www.arcadeit.net
 *
 * @author  Francesco De Simone
 * @file    ArcadeIT_RT.h
 * @version V0.01C
 * @date    11-10-2017
 * @last    26-02-2020
 * @brief   This library is used to handle the RTC system of the ArcadeIT board
 *
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

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// Select the system board.
#include <System/Devices/ArcadeIT_RTC.h>
#include <System/Devices/ArcadeIT_Serial_Port.h>

// FATFS functions.
#include <ff.h>
#include <diskio.h>

// /////////////////////////////////////////////////////////////////////////////
// Defines
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Global elements
// /////////////////////////////////////////////////////////////////////////////
char gTimeStamp[SYSTEM_TIMESTAMP_LEN];

__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
__IO uint32_t uwTimeDisplay = 0;

uint32_t uwErrorIndex = 0;
uint32_t uwIndex = 0;

uint32_t aBKPDataReg[RTC_BKP_DR_NUMBER] =
  {
    RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2,
    RTC_BKP_DR3, RTC_BKP_DR4, RTC_BKP_DR5,
    RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8,
    RTC_BKP_DR9, RTC_BKP_DR10, RTC_BKP_DR11,
    RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14,
    RTC_BKP_DR15, RTC_BKP_DR16, RTC_BKP_DR17,
    RTC_BKP_DR18,  RTC_BKP_DR19
  };

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////
static uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint8_t bcdhigh = 0;

  while (Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }

  return  ((uint8_t)(bcdhigh << 4) | Value);
}
// -----------------------------------------------------------------------------
static uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint8_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}
// -----------------------------------------------------------------------------
uint32_t ArcadeIT_RTC_ReadBackupRegister
(
    uint32_t RTC_BKP_DR // RTC Backup data Register number.
)
{
  /*
   * DESCRIPTION: Reads data from the specified RTC Backup data Register.
   * PARAMETERS:  See above.
   * RETURNS:     Nothing.
   */

  __IO uint32_t tmp = 0;

  tmp = RTC_BASE + 0x50;
  tmp += (RTC_BKP_DR * 4);

  /* Read the specified register */
  return (*(__IO uint32_t *)tmp);

} // end ArcadeIT_RTC_ReadBackupRegister

// -----------------------------------------------------------------------------
void ArcadeIT_RTC_WriteBackupRegister(
    uint32_t RTC_BKP_DR, // RTC Backup data Register number.
    uint32_t Data        // Data to be written in the specified RTC Backup data register.
)
{
  __IO uint32_t tmp = 0;

  tmp = RTC_BASE + 0x50;
  tmp += (RTC_BKP_DR * 4);

  /* Write the specified register */
  *(__IO uint32_t *)tmp = (uint32_t)Data;

} // end RTC_WriteBackupRegister

// -----------------------------------------------------------------------------
uint32_t ArcadeIT_RCC_LSEConfig
(
    uint8_t RCC_LSE // specifies the new state of the LSE.
)
{
 /*
  * DESCRIPTION: Configures the External Low Speed oscillator (LSE).
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
  /* Reset LSEON bit */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Reset LSEBYP bit */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) */
  switch (RCC_LSE)
  {
    case RCC_LSE_ON:
      /* Set LSEON bit */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;

    case RCC_LSE_Bypass:
      /* Set LSEBYP and LSEON bits */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;

    default:
      break;

  } // end switch

  return 0;

} // end ArcadeIT_RCC_LSEConfig

// -----------------------------------------------------------------------------
uint32_t ArcadeIT_RTC_CheckBackupReg
(
  uint16_t FirstBackupData  // The pointer to the string.
)
{
 /*
  * DESCRIPTION: This function checks the backup registers of the RTC system.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  uint32_t index = 0;

  for (index = 0; index < RTC_BKP_DR_NUMBER; index++)
  {
    if (ArcadeIT_RTC_ReadBackupRegister(aBKPDataReg[index]) != (FirstBackupData + (index * 0x5A)))
    {
      return (index + 1);

    } // End if.

  } // End for.

  return 0;

} // End ArcadeIT_RTC_CheckBackupReg

// -----------------------------------------------------------------------------
FlagStatus ArcadeIT_RCC_GetFlagStatus
(
    uint8_t RCC_FLAG // specifies the flag to check.
)
{
  /*
  * DESCRIPTION: Checks whether the specified RCC flag is set or not.
  * PARAMETERS:  See above.
  * RETURNS:     A Flag Status.
  */

  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;

  /* Get the RCC register index */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* The flag to check is in CR register */
    statusreg = RCC->CR;
  else if (tmp == 2)          /* The flag to check is in BDCR register */
    statusreg = RCC->BDCR;
  else                        /* The flag to check is in CSR register */
    statusreg = RCC->CSR;

  /* Get the flag position */
  tmp = RCC_FLAG & ((uint8_t)0x1F);
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
    bitstatus = SET;
  else
    bitstatus = RESET;

  /* Return the flag status */
  return bitstatus;

} // end ArcadeIT_RCC_GetFlagStatus

// -----------------------------------------------------------------------------
ErrorStatus ArcadeIT_RTC_WaitForSynchro(void)
{
  /*
  * DESCRIPTION: Waits until the RTC Time and Date registers (RTC_TR and RTC_DR) are
  *              synchronized with RTC APB clock.
  * PARAMETERS:  None.
  * RETURNS:     A Status.
  */

  __IO uint32_t synchrocounter = 0;
  ErrorStatus status = ERROR;
  uint32_t synchrostatus = 0x00;

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Clear RSF flag */
  RTC->ISR &= (uint32_t)RTC_RSF_MASK;

  /* Wait the registers to be synchronised */
  do
  {
    synchrostatus = RTC->ISR & RTC_ISR_RSF;
    synchrocounter++;

  } while((synchrocounter != SYNCHRO_TIMEOUT) && (synchrostatus == 0x00));

  if ((RTC->ISR & RTC_ISR_RSF) != RESET)
    status = SUCCESS;
  else
    status = ERROR;

  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF;

  return (status);

} // end ArcadeIT_RTC_WaitForSynchro

// -----------------------------------------------------------------------------
ErrorStatus ArcadeIT_RTC_EnterInitMode(void)
{
  /*
  * DESCRIPTION: Enters the RTC Initialization mode.
  * PARAMETERS:  None.
  * RETURNS:     An ErrorStatus.
  */

  __IO uint32_t initcounter = 0x00;
  ErrorStatus status = ERROR;
  uint32_t initstatus = 0x00;

  /* Check if the Initialization mode is set */
  if ((RTC->ISR & RTC_ISR_INITF) == (uint32_t)RESET)
  {
    /* Set the Initialization mode */
    RTC->ISR = (uint32_t)RTC_INIT_MASK;

    /* Wait till RTC is in INIT state and if Time out is reached exit */
    do
    {
      initstatus = RTC->ISR & RTC_ISR_INITF;
      initcounter++;

    } while((initcounter != INITMODE_TIMEOUT) && (initstatus == 0x00));

    if ((RTC->ISR & RTC_ISR_INITF) != RESET)
      status = SUCCESS;
    else
      status = ERROR;
  }
  else
    status = SUCCESS;

  return (status);

} // end ArcadeIT_RTC_EnterInitMode

// -----------------------------------------------------------------------------
void ArcadeIT_RTC_Init(void)
{
  /*
  * DESCRIPTION: This function modify the string provided to upper case.
  * PARAMETERS:  None.
  * RETURNS:     Nothing.
  */

  if (ArcadeIT_RTC_ReadBackupRegister(RTC_BKP_DR0) != FIRST_DATA)
  {
    // Enable the PWR clock if not yet.
    if ((RCC->APB1ENR & RCC_APB1Periph_PWR) == FALSE) RCC->APB1ENR |= RCC_APB1Periph_PWR;

    // Allow access to RTC.
    *(__IO uint32_t *) CR_DBP_BB = (uint32_t)ENABLE;
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Reset RTC Domain.
    *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)(ENABLE);
    *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)(DISABLE);

    // Enable the LSE OSC.
    *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)DISABLE;
    ArcadeIT_RCC_LSEConfig(RCC_LSE_ON);

    // Wait till LSE is ready.
    while(ArcadeIT_RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {};

    // Select the RTC Clock Source.
    uint32_t tmpreg = 0;
    if ((RCC_RTCCLKSource_LSE & 0x00000300) == 0x00000300)
    { /* If HSE is selected as RTC clock source, configure HSE division factor for RTC clock */
      tmpreg = RCC->CFGR;

      /* Clear RTCPRE[4:0] bits */
      tmpreg &= ~RCC_CFGR_RTCPRE;

      /* Configure HSE division factor for RTC clock */
      tmpreg |= (RCC_RTCCLKSource_LSE & 0xFFFFCFF);

      /* Store the new value */
      RCC->CFGR = tmpreg;
    }

    /* Select the RTC clock source */
    RCC->BDCR |= (RCC_RTCCLKSource_LSE & 0x00000FFF);

    // Enable the RTC Clock.
    *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)ENABLE;

    // Wait for RTC APB registers synchronization.
    ArcadeIT_RTC_WaitForSynchro();

    /* Disable the write protection for RTC registers */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    /* Set Initialization mode */
    if (ArcadeIT_RTC_EnterInitMode() != ERROR)
    {
      /* Clear RTC CR FMT Bit */
      RTC->CR &= ((uint32_t)~(RTC_CR_FMT));

      /* Set RTC_CR register */
      RTC->CR |=  ((uint32_t)(RTC_HourFormat_24));

      /* Configure the RTC PRER */
      RTC->PRER = (uint32_t)(255);
      RTC->PRER |= (uint32_t)(127 << 16);

      /* Exit Initialization mode */
      RTC->ISR &= (uint32_t)~RTC_ISR_INIT;

    } // end if

    /* Enable the write protection for RTC registers */
    RTC->WPR = 0xFF;

    // Indicator for the RTC configuration.
    ArcadeIT_RTC_WriteBackupRegister(RTC_BKP_DR0, FIRST_DATA);
  }
  else
  {
    // Wait for RTC APB registers synchronization.
    ArcadeIT_RTC_WaitForSynchro();

    // Backup SRAM.
    // Enable BKPSRAM Clock.
    RCC->AHB1ENR |= RCC_AHB1Periph_BKPSRAM;

  } // End if.

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    ArcadeIT_Serial_Port_String_Send(TEXT_RTC_INITED);

  } // End if.

} // End ArcadeIT_RTC_Init.

// -----------------------------------------------------------------------------
ErrorStatus RTC_SetTime(uint8_t pHours, uint8_t pMinutes, uint8_t pSeconds, uint8_t pH12)
{
  uint32_t tmpreg = 0;
  ErrorStatus status = ERROR;

  if ((RTC->CR & RTC_CR_FMT) != 0)
    tmpreg = RTC_Bcd2ToByte(pHours);
  else
    pH12 = 0x00;

  tmpreg = (((uint32_t)(pHours) << 16) | \
           ((uint32_t)(pMinutes) << 8) | \
           ((uint32_t)pSeconds) | \
           ((uint32_t)(pH12) << 16));

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Set Initialization mode */
  if (ArcadeIT_RTC_EnterInitMode() == ERROR)
  {
    status = ERROR;
  }
  else
  {
    /* Set the RTC_TR register */
    RTC->TR = (uint32_t)(tmpreg & RTC_TR_RESERVED_MASK);

    /* Exit Initialization mode */
    RTC->ISR &= (uint32_t)~RTC_ISR_INIT;

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
    {
    if(ArcadeIT_RTC_WaitForSynchro() == ERROR)
      status = ERROR;
    else
      status = SUCCESS;
    }
    else
      status = SUCCESS;
  }
  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF;

  return status;

} // end RTC_SetTime

// -----------------------------------------------------------------------------
ErrorStatus RTC_SetDate(uint8_t pMonth, uint8_t pDate, uint8_t pYear)
{
  uint32_t tmpreg = 0;
  ErrorStatus status = ERROR;


  tmpreg = (((uint32_t)RTC_ByteToBcd2(pYear) << 16) | \
            ((uint32_t)RTC_ByteToBcd2(pMonth) << 8) | \
            ((uint32_t)RTC_ByteToBcd2(pDate)));

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Set Initialization mode */
  if (ArcadeIT_RTC_EnterInitMode() == ERROR)
  {
    status = ERROR;
  }
  else
  {
    /* Set the RTC_DR register */
    RTC->DR = (uint32_t)(tmpreg & RTC_DR_RESERVED_MASK);

    /* Exit Initialization mode */
    RTC->ISR &= (uint32_t)~RTC_ISR_INIT;

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
    {
      if(ArcadeIT_RTC_WaitForSynchro() == ERROR)
        status = ERROR;
      else
        status = SUCCESS;
    }
    else
      status = SUCCESS;
  }
  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF;

  return status;
}

// -----------------------------------------------------------------------------
rtc_time_t ArcadeIT_RTC_TimeDate_Get(void)
{
  /*
  * DESCRIPTION: This function returns the time and date of the system.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  rtc_time_t lTempTime;

  uint32_t tmpreg;

  tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
  lTempTime.hour = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16);
  lTempTime.min = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >>8);
  lTempTime.sec = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));

  tmpreg = (uint32_t)(RTC->DR & RTC_DR_RESERVED_MASK);
  lTempTime.month = (uint8_t)((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8);
  lTempTime.mday = (uint8_t)(tmpreg & (RTC_DR_DT | RTC_DR_DU));
  lTempTime.year = (uint8_t)((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16);

  return lTempTime;

} // End ArcadeIT_RTC_TimeDate_Get.

// -----------------------------------------------------------------------------
void ArcadeIT_RTC_TimeDate_String(void)
{
  /*
  * DESCRIPTION: This function gets the time and date of the system.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  rtc_time_t lTempTime = ArcadeIT_RTC_TimeDate_Get();

  // Display date Format : mm/dd/yy, time Format : hh:mm
  sprintf(gTimeStamp,"%c%c/%c%c/%c%c-%c%c:%c%c",
          (((uint8_t)(lTempTime.month & 0xF0) >> 0x04)+ 0x30),
          (((uint8_t)(lTempTime.month & 0x0F)) + 0x30),
          (((uint8_t)(lTempTime.mday & 0xF0) >> 0x04)+ 0x30),
          (((uint8_t)(lTempTime.mday & 0x0F)) + 0x30),
          (((uint8_t)(lTempTime.year & 0xF0) >> 0x04)+ 0x30),
          (((uint8_t)(lTempTime.year & 0x0F)) + 0x30),
          (((uint8_t)(lTempTime.hour & 0xF0) >> 0x04)+ 0x30),
          (((uint8_t)(lTempTime.hour & 0x0F)) + 0x30),
          (((uint8_t)(lTempTime.min & 0xF0) >> 0x04)+ 0x30),
          (((uint8_t)(lTempTime.min & 0x0F)) + 0x30)
          );

} // End ArcadeIT_RTC_TimeDate_String.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_RTC_TimeDate_Set
(
  rtc_time_t *pTime   // The pointer to the string where to store the information.
)
{
  /*
  * DESCRIPTION: This function gets the time and date of the system.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Allow access to RTC.
  *(__IO uint32_t *) CR_DBP_BB = ENABLE;
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  ArcadeIT_RTC_EnterInitMode();

  /* Set Time hh:mm:ss */
  RTC_SetTime(
      ((pTime->hour / 10) << 4) + (pTime->hour % 10),
      ((pTime->min / 10) << 4) + (pTime->min % 10),
      ((pTime->sec / 10) << 4) + (pTime->sec % 10),
      0);

  // Set the date:
  RTC_SetDate(
      ((pTime->month / 10) << 4) + (pTime->month % 10),
      ((pTime->mday / 10) << 4) + (pTime->mday % 10),
      ((pTime->year / 10) << 4) + (pTime->year % 10)
      );

  // Wait for RTC APB registers synchronization.
  ArcadeIT_RTC_WaitForSynchro();

  // Indicator for the RTC configuration.
  ArcadeIT_RTC_WriteBackupRegister(RTC_BKP_DR0, FIRST_DATA);

  RTC->ISR &= (uint32_t)~RTC_ISR_INIT;

} // End ArcadeIT_RTC_TimeDate_Set.

// /////////////////////////////////////////////////////////////////////////////
