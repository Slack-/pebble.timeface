/* 
** Manages the tick timer. 
*/

#include <pebble.h>
#include "timer.h"
#include "gui.h"

/* Private forward declarations */
static void tick_timer_handler(struct tm *t_tick_time, TimeUnits t_units_changed);

/*
** Initialises the tick timer.
*/
void timer_init(void)
{
    /* Register with the tick timer service for updates */
    tick_timer_service_subscribe(MINUTE_UNIT, tick_timer_handler);
}

/*
** Releases resources.
*/
void timer_deinit(void)
{
    tick_timer_service_unsubscribe();
}

/**************************** Private functions *****************************/

/*
** Handles the timer tick event.
*/
static void tick_timer_handler(struct tm *t_tick_time, TimeUnits t_units_changed)
{
    /* Refresh the time on the GUI */
    gui_update_time();
    
    /* Refresh the date on the GUI every day */
    if (t_units_changed & DAY_UNIT)
    {
        gui_update_date();
    }
}