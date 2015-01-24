/* 
** Initialises and runs the application.
*/

#include <pebble.h>
#include "gui.h"
#include "battery.h"
#include "bluetooth.h"
#include "timer.h"

/* Private forward declarations */
static void init(void);
static void deinit(void);

/* 
** Entry point for the application. 
*/
int main(void)
{
    init();
    app_event_loop();
    deinit();
}
    
/**************************** Private functions *****************************/
    
/* 
** Application intialisation. 
*/
static void init(void)
{
    /* Initialize the comms and GUI */
    gui_init();
    battery_init();
    bluetooth_init();
    timer_init();
}

/* 
** Releases application resources. 
*/
static void deinit(void)
{
    /* Destroy the comms and GUI */
    timer_deinit();
    bluetooth_deinit();
    battery_deinit();
    gui_deinit();
}
