/* 
** Manages the battery level. 
*/

#include <pebble.h>
#include "battery.h"
#include "gui.h"

/* Private forward declarations */
static void battery_state_update_handler(BatteryChargeState ts_charge_state);

/*
** Initialises the battery manager.
*/
void battery_init(void)
{
    /* Update with the current battery level */
    battery_state_update_handler(battery_state_service_peek());
        
    /* Register with the battery state service for further updates */
    battery_state_service_subscribe(battery_state_update_handler);
}

/*
** Releases resources.
*/
void battery_deinit(void)
{
    battery_state_service_unsubscribe();
}

/**************************** Private functions *****************************/

/* 
** Handles the battery state update event. 
*/
static void battery_state_update_handler(BatteryChargeState ts_charge_state)
{
    gui_update_battery_level(ts_charge_state.charge_percent / 10, ts_charge_state.is_charging);
}