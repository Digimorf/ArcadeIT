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
 * @file    ArcadeIT_Scheduler.c
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

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ArcadeIT Libraries.
#include <System/ArcadeIT_Common.h>
#include <System/ArcadeIT_Utilities.h>
#include <System/ArcadeIT_Firmware.h>

// ArcadeIT! System clocks and counters.
#include <System/Units/ArcadeIT_Scheduler.h>
//#include "System/Peripherals/ArcadeIT_Serial_Port.h"

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Externals
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Globals
// /////////////////////////////////////////////////////////////////////////////
ArcadeIT_Scheduler_Task_t gArcadeITTasks[ARCADEIT_SCHEDULER_MAX_TASKS];

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Scheduler_Task_Init(void)
{
  /*
  DESCRIPTION:  Initialize the scheduler system.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  uint8_t lTask = 0;
  for (lTask = 0; lTask < ARCADEIT_SCHEDULER_MAX_TASKS; lTask++)
  {
    gArcadeITTasks[lTask].current_cycle = 0;
    gArcadeITTasks[lTask].cycles = 0;
    gArcadeITTasks[lTask].time_period = 0;
    gArcadeITTasks[lTask].time_current = 0;
    gArcadeITTasks[lTask].function = NULL;

  } // End for.

  if (gDevices & ARCADEIT_DEVICE_SERIAL)
  {
    //ArcadeIT_Serial_Port_String_Send(TEXT_SCHEDULER_INITED);

  } // End if.

} // End ArcadeIT_Scheduler_Task_Init.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Scheduler_Task_Set
(
  uint8_t id,                // The id of the task in the array.
  void *pFunctionPtr,        // The pointer to the function to execute.
  void *pParameters,         // The pointer to the parameters used by the function.
  uint32_t pCycles,          // The number of time that this task has to be executed. 0 = infinite.
  uint32_t pTimePeriod       // The time period in ms to elapse before the next execution.
)
{
  /*
  DESCRIPTION:  Setup a scheduled task.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  if (pParameters != NULL)
  {
    gArcadeITTasks[id].num_parameters = ((uint32_t*)pParameters)[PAR_1];

    // Copy parameters.
    uint8_t lPars = 0;
    for (lPars = 0; lPars < gArcadeITTasks[id].num_parameters+1; lPars++)
    {
      gArcadeITTasks[id].parameters[lPars] = ((uint32_t*)pParameters)[lPars];

    } // End for.

  } // End if.

  gArcadeITTasks[id].current_cycle = 0;
  gArcadeITTasks[id].cycles = pCycles;
  gArcadeITTasks[id].time_period = pTimePeriod;
  gArcadeITTasks[id].time_current = gArcadeITTasks[id].time_period;
  gArcadeITTasks[id].function = pFunctionPtr;
  gArcadeITTasks[id].execute = FALSE;

} // End ArcadeIT_Scheduler_Task_Set.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Scheduler_Task_Unset
(
  uint8_t id   // The id of the task in the array.
)
{
  /*
  DESCRIPTION:  Reset a scheduled task.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  gArcadeITTasks[id].current_cycle = 0;
  gArcadeITTasks[id].cycles = 0;
  gArcadeITTasks[id].time_period = 0;
  gArcadeITTasks[id].time_current = 0;
  gArcadeITTasks[id].function = NULL;
  gArcadeITTasks[id].execute = FALSE;

} // End ArcadeIT_Scheduler_Task_Unset.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Scheduler_Update(void)
{
  /*
  DESCRIPTION:  This function is called every Systick count period and executes
                the tasks which time period reaches 0.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  uint8_t lTask = 0;
  for (lTask = 0; lTask < ARCADEIT_SCHEDULER_MAX_TASKS; lTask++)
  {
    // If the task "lTask" is set there must be a function set, so we process this task.
    if (gArcadeITTasks[lTask].function != NULL)
    {
      if (gArcadeITTasks[lTask].execute == FALSE)
      {
        // Update the time period.
        gArcadeITTasks[lTask].time_current--;
        if (gArcadeITTasks[lTask].time_current == 0)
        {
          // Here we call the function set for this task.
          gArcadeITTasks[lTask].execute = TRUE;

          // If the timer is over we check if to execute the task. If the number of
          // cycles is 0 the task is always executed, otherwise we count the number
          // of cycles. When we reach the maximum number we reset this task.
          if (gArcadeITTasks[lTask].cycles)
          {
            gArcadeITTasks[lTask].time_current = gArcadeITTasks[lTask].time_period;

            gArcadeITTasks[lTask].current_cycle++;
            if (gArcadeITTasks[lTask].current_cycle == gArcadeITTasks[lTask].cycles)
            {
              ArcadeIT_Scheduler_Task_Unset(lTask);

            } // End if.
          }
          else
          {
            gArcadeITTasks[lTask].time_current = gArcadeITTasks[lTask].time_period;

          } // End if.

        } // End if.

      } // End if.

    } // End if.

  } // End for.

} // End ArcadeIT_Scheduler_Update.

// /////////////////////////////////////////////////////////////////////////////
void ArcadeIT_Scheduler_Run(void)
{
  /*
  DESCRIPTION:  This function is called every Systick count period and executes
                the tasks which time period reaches 0.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  uint8_t lTask = 0;
  for (lTask = 0; lTask < ARCADEIT_SCHEDULER_MAX_TASKS; lTask++)
  {
    if (gArcadeITTasks[lTask].execute)
    {
        // Here we call the function set for this task.
        gArcadeITTasks[lTask].function(gArcadeITTasks[lTask].parameters);

        // Unmark the function.
        gArcadeITTasks[lTask].execute = FALSE;

    } // End if.

  } // End for.

} // End ArcadeIT_Scheduler_Run.

// /////////////////////////////////////////////////////////////////////////////
uint8_t ArcadeIT_Scheduler_Task_Running
(
  uint8_t id   // The id of the task in the array.
)
{
  /*
  DESCRIPTION:  Check if the task number 'id' of the scheduler is running or not.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  return (gArcadeITTasks[id].function != NULL) ? ON : OFF;

} // End ArcadeIT_Scheduler_Task_Running.

// /////////////////////////////////////////////////////////////////////////////
