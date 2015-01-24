/* 
** Manages the bluetooth connection status. 
*/

#include <pebble.h>
#include "bluetooth.h"
#include "gui.h"
    
/* Private forward declarations */
static void bluetooth_connection_update_handler(bool b_is_connected);

/*
** Initialises the bluetooth manager.
*/
void bluetooth_init(void)
{
    /* Update with the current connection status */
    bluetooth_connection_update_handler(bluetooth_connection_service_peek());
        
    /* Register with the bluetooth connection service for further updates */
    bluetooth_connection_service_subscribe(bluetooth_connection_update_handler);
}

/*
** Releases resources.
*/
void bluetooth_deinit(void)
{
    bluetooth_connection_service_unsubscribe();
}

/**************************** Private functions *****************************/

/* 
** Handles the bluetooth connection update event. 
*/
static void bluetooth_connection_update_handler(bool b_is_connected)
{
    gui_update_connection_status(b_is_connected);
}