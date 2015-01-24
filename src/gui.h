/* 
** Manages GUI set up and updating.
*/

#pragma once

#include <pebble.h>

/* 
** Initialises the GUI. 
*/
void gui_init(void);

/* 
** Releases resources. 
*/
void gui_deinit(void);

/* 
** Updates the battery level on the GUI. 
** u8_battery_level_tenths - The number of tenths of the battery level left.
** b_is_charging - A value indicating whether the battery is charging.
*/
void gui_update_battery_level(uint8_t u8_battery_level_tenths, bool b_is_charging);

/* 
** Updates the bluetooth connection status on the GUI.
** b_is_connected - A value indicating whether bluetoth is connected.
*/
void gui_update_connection_status(bool b_is_connected);

/* 
** Updates the time on the GUI. 
*/
void gui_update_time(void);

/* 
** Updates the date on the GUI. 
*/
void gui_update_date(void);