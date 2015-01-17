/* 
** Manages the tick timer. 
*/

#include <pebble.h>
#include "Timer.h"
#include "Gui.h"

/* Private forward declarations */
static void TickTimerHandler(struct tm *tTickTime, TimeUnits tUnitsChanged);

/*
** Initialises the tick timer.
*/
void TimerInit(void)
{
    /* Register with the tick timer service for updates */
    tick_timer_service_subscribe(MINUTE_UNIT, TickTimerHandler);
}

/*
** Releases resources.
*/
void TimerDeinit(void)
{
    tick_timer_service_unsubscribe();
}

/**************************** Private functions *****************************/

/*
** Handles the timer tick event.
*/
static void TickTimerHandler(struct tm *tTickTime, TimeUnits tUnitsChanged)
{
    /* Refresh the time on the GUI */
    GuiUpdateTime();
    
    /* Refresh the date on the GUI every day */
    if (tUnitsChanged & DAY_UNIT)
    {
        GuiUpdateDate();
    }
}