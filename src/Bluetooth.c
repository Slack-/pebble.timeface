/* 
** Manages the bluetooth connection status. 
*/

#include <pebble.h>
#include "Bluetooth.h"
#include "Gui.h"
    
/* Private forward declarations */
static void BluetoothConnectionUpdateHandler(bool bIsConnected);

/*
** Initialises the bluetooth manager.
*/
void BluetoothInit(void)
{
    /* Update with the current connection status */
    BluetoothConnectionUpdateHandler(bluetooth_connection_service_peek());
        
    /* Register with the bluetooth connection service for further updates */
    bluetooth_connection_service_subscribe(BluetoothConnectionUpdateHandler);
}

/*
** Releases resources.
*/
void BluetoothDeinit(void)
{
    bluetooth_connection_service_unsubscribe();
}

/**************************** Private functions *****************************/

/* 
** Handles the bluetooth connection update event. 
*/
static void BluetoothConnectionUpdateHandler(bool bIsConnected)
{
    GuiUpdateConnectionStatus(bIsConnected);
}