/* 
** Initialises and runs the application.
*/

#include <pebble.h>
#include "Gui.h"
#include "Battery.h"
#include "Bluetooth.h"
#include "Timer.h"

/* Private forward declarations */
static void Init(void);
static void Deinit(void);

/* 
** Entry point for the application. 
*/
int main(void)
{
    Init();
    app_event_loop();
    Deinit();
}
    
/**************************** Private functions *****************************/
    
/* 
** Application intialisation. 
*/
static void Init(void)
{
    /* Initialize the comms and GUI */
    GuiInit();
    BatteryInit();
    BluetoothInit();
    TimerInit();
}

/* 
** Releases application resources. 
*/
static void Deinit(void)
{
    /* Destroy the comms and GUI */
    TimerDeinit();
    BluetoothDeinit();
    BatteryDeinit();
    GuiDeinit();
}
