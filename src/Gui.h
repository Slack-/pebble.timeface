/* 
** Manages GUI set up and updating.
*/

#pragma once

#include <pebble.h>

/* 
** Initialises the GUI. 
*/
void GuiInit(void);

/* 
** Releases resources. 
*/
void GuiDeinit(void);

/* 
** Updates the battery level on the GUI. 
** u8BatteryLevelTenths - The number of tenths of the battery level left.
** bIsCharging - A value indicating whether the battery is charging.
*/
void GuiUpdateBatteryLevel(uint8_t u8BatteryLevelTenths, bool bIsCharging);

/* 
** Updates the bluetooth connection status on the GUI.
** bIsCharging - A value indicating whether bluetoth is connected.
*/
void GuiUpdateConnectionStatus(bool bIsConnected);

/* 
** Updates the time on the GUI. 
*/
void GuiUpdateTime(void);

/* 
** Updates the date on the GUI. 
*/
void GuiUpdateDate(void);