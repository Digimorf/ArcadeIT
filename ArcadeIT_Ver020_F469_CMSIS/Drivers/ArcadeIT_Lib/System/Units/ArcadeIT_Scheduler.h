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
 * @file    ArcadeIT_Scheduler.h
 * @version V0.13
 * @date    26-07-2017
 * @last    10-02-2020
 * @brief   This library implements a custom time scheduled task manager.
 *          It's driven by counters updated by the Systick timer. Each task can
 *          be configured to run forever or a specific number of times at
 *          regular intervals of time (the unit configured in the Systick system)
 *
 ******************************************************************************
 TOADD

 ******************************************************************************
 TOFIX

 ******************************************************************************
 HISTORY

 05-30-2018:
 - Creation date of the library structure.

 06-02-2020:
 - Converted to pure CMSIS.
 - Moved to GitHub.

 ******************************************************************************
*/

#ifndef _ARCADEIT_SCHEDULER_H
#define _ARCADEIT_SCHEDULER_H

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
#define ARCADEIT_SCHEDULER_MAX_TASKS          5
#define ARCADEIT_SCHEDULER_REPEAT_INFINITELY  0
#define TASK_MAX_PARAMETERS                   4

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////
typedef struct ArcadeIT_Scheduler_Task
{
  uint32_t cycles;                          // The number of time that this task has to be executed. 0 = infinite.
  uint32_t current_cycle;                   // The counter that keep track of the cycles executed.
  uint32_t time_period;                     // The time period in ms to elapse before the next execution.
  uint32_t time_current;                    // The time counter in ms.
  func function;                            // The function to execute when the time_period reaches 0.
  uint8_t num_parameters;
  uint32_t parameters[TASK_MAX_PARAMETERS]; // The parameters to use in the task.
  uint8_t execute;

} ArcadeIT_Scheduler_Task_t, *ArcadeIT_Scheduler_Task_ptr;

// /////////////////////////////////////////////////////////////////////////////
// Exported globals.
// /////////////////////////////////////////////////////////////////////////////
extern ArcadeIT_Scheduler_Task_t gArcadeITTasks[ARCADEIT_SCHEDULER_MAX_TASKS];

// /////////////////////////////////////////////////////////////////////////////
// Exported functions.
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Scheduler_Task_Init(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Scheduler_Task_Set
(
  uint8_t id,                // The id of the task in the array.
  void *pFunctionPtr,        // The pointer to the function to execute.
  void *pParameters,         // The pointer to the parameters used by the function.
  uint32_t pCycles,          // The number of time that this task has to be executed. 0 = infinite.
  uint32_t pTimePeriod       // The time period in ms to elapse before the next execution.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Scheduler_Task_Unset
(
  uint8_t id                 // The id of the task in the array.
);
// -----------------------------------------------------------------------------
uint8_t ArcadeIT_Scheduler_Task_Running
(
  uint8_t id                 // The id of the task in the array.
);
// -----------------------------------------------------------------------------
void ArcadeIT_Scheduler_Update(void);
// -----------------------------------------------------------------------------
void ArcadeIT_Scheduler_Run(void);

// /////////////////////////////////////////////////////////////////////////////
#endif // _ARCADEIT_SCHEDULER_H
