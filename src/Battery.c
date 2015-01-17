/* 
** Manages the battery level. 
*/

#include <pebble.h>
#include "Battery.h"
#include "Gui.h"

/* Private forward declarations */
static void BatteryStateUpdateHandler(BatteryChargeState tsChargeState);

/*
** Initialises the battery manager.
*/
void BatteryInit(void)
{
    /* Update with the current battery level */
    BatteryStateUpdateHandler(battery_state_service_peek());
        
    /* Register with the battery state service for further updates */
    battery_state_service_subscribe(BatteryStateUpdateHandler);
}

/*
** Releases resources.
*/
void BatteryDeinit(void)
{
    battery_state_service_unsubscribe();
}

/**************************** Private functions *****************************/

/* 
** Handles the battery state update event. 
*/
static void BatteryStateUpdateHandler(BatteryChargeState tsChargeState)
{
    GuiUpdateBatteryLevel(tsChargeState.charge_percent / 10, tsChargeState.is_charging);
}