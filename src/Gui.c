/* 
** Manages GUI set up and updating.
*/

#include <pebble.h>
#include "Gui.h"

#define DATE_BUFFER_LENGTH 12
    
/* Windows */
static Window *wMainWindow;

/* Bitmaps */
static GBitmap *tBatteryShellBitmap;
static GBitmap *tBatteryChargingBitmap;
static GBitmap *tBluetoothConnectedBitmap;

/* Graphics layers */
static Layer *tBackgroundLayer;
static BitmapLayer *tBatteryShellLayer;
static Layer *tBatteryLevelLayer;
static BitmapLayer *tBatteryChargingLayer;
static BitmapLayer *tBluetoothConnectedLayer;
static TextLayer *tTimeLayer;
static TextLayer *tDateLayer;

/* Graphics state management */
static uint8_t u8BatteryLevelWidth;
    
/* Private forward declarations */
static void MainWindowLoad(Window *wWindow);
static void MainWindowUnload(Window *wWindow);
static void BackgroundLayerUpdate(Layer *tLayer, GContext *tContext);
static void BatteryLevelLayerUpdate(Layer *tLayer, GContext *tContext);

/* 
** Initialises the GUI. 
*/
void GuiInit(void)
{
    /* Create main window element 
    and set handlers to manage the elements inside it */
    wMainWindow = window_create();
    window_set_window_handlers(wMainWindow, (WindowHandlers)
    {
        .load = MainWindowLoad,
        .unload = MainWindowUnload
    });
    
    /* Show the window on the watch as animated */
    window_stack_push(wMainWindow, true);
    
    /* Show the current time and date */
    GuiUpdateTime();
    GuiUpdateDate();
}

/* 
** Releases resources.
*/
void GuiDeinit(void)
{
    /* Destroy the main window */
    window_destroy(wMainWindow);
}

/* 
** Updates the battery level on the GUI.
** u8BatteryLevelTenths - The number of tenths of the battery level left.
** bIsCharging - A value indicating whether the battery is charging.
*/
void GuiUpdateBatteryLevel(uint8_t u8BatteryLevelTenths, bool bIsCharging)
{
    /* Determine the number of pixels for the battery level */
    u8BatteryLevelWidth = u8BatteryLevelTenths;
    
    /* Refresh the battery level and hide the charging image if required */
    layer_mark_dirty(tBatteryLevelLayer);
    layer_set_hidden(bitmap_layer_get_layer(tBatteryChargingLayer), !bIsCharging);
}

/* 
** Updates the bluetooth connection status on the GUI.
** bIsCharging - A value indicating whether bluetoth is connected.
*/
void GuiUpdateConnectionStatus(bool bIsConnected)
{
    layer_set_hidden(bitmap_layer_get_layer(tBluetoothConnectedLayer), !bIsConnected);
}

/* 
** Updates the time on the GUI.
*/
void GuiUpdateTime(void)
{
    /* Get a time structure */
    time_t temp = time(NULL); 
    struct tm *tCurrentTime = localtime(&temp);

    /* Create a long-lived buffer for displaying the time */
    static char acBuffer[] = "00:00";

    /* Write the current hours and minutes into the buffer */
    if (clock_is_24h_style() == true)
    {
        /* Use 24 hour format */
        strftime(acBuffer, sizeof(acBuffer), "%H:%M", tCurrentTime);
    }
    else
    {
        /* Use 12 hour format */
        strftime(acBuffer, sizeof(acBuffer), "%I:%M", tCurrentTime);
    }

    /* Display this time on the text layer */
    text_layer_set_text(tTimeLayer, acBuffer);
}

/* 
** Updates the date on the GUI. 
*/
void GuiUpdateDate(void)
{
    /* Get a time structure */
    time_t temp = time(NULL); 
    struct tm *tCurrentTime = localtime(&temp);
    
    /* Create a long-lived buffer for displaying the date */
    static char acBuffer[DATE_BUFFER_LENGTH];

    /* Write the current month and day of the month into the buffer */
    strftime(acBuffer, DATE_BUFFER_LENGTH, "%B %e", tCurrentTime);

    /* Display this date on the text layer */
    text_layer_set_text(tDateLayer, acBuffer);
}

/**************************** Private functions *****************************/

/* 
** Loads the main window.
*/
static void MainWindowLoad(Window *wWindow)
{
    /* Create and add the background (drawing) layer */
    tBackgroundLayer = layer_create(GRect(0, 0, 144, 168));
    layer_set_update_proc(tBackgroundLayer, BackgroundLayerUpdate);
    layer_add_child(window_get_root_layer(wWindow), tBackgroundLayer);
    
    /* Create and add a battery shell bitmap layer */
    tBatteryShellBitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
    tBatteryShellLayer = bitmap_layer_create(GRect(126, 4, 15, 8));
    bitmap_layer_set_bitmap(tBatteryShellLayer, tBatteryShellBitmap);
    layer_add_child(window_get_root_layer(wWindow), bitmap_layer_get_layer(tBatteryShellLayer));
    
    /* Create and add the battery level layer */
    tBatteryLevelLayer = layer_create(GRect(128, 6, 10, 4));
    layer_set_update_proc(tBatteryLevelLayer, BatteryLevelLayerUpdate);
    layer_add_child(window_get_root_layer(wWindow), tBatteryLevelLayer);
    
    /* Create the battery charging symbol */
    tBatteryChargingBitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGING);
    tBatteryChargingLayer = bitmap_layer_create(GRect(117, 4, 7, 8));
    bitmap_layer_set_bitmap(tBatteryChargingLayer, tBatteryChargingBitmap);
    layer_add_child(window_get_root_layer(wWindow), bitmap_layer_get_layer(tBatteryChargingLayer));
    
    /* Create the bluetooth connected symbol */
    tBluetoothConnectedBitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);
    tBluetoothConnectedLayer = bitmap_layer_create(GRect(3, 3, 7, 10));
    bitmap_layer_set_bitmap(tBluetoothConnectedLayer, tBluetoothConnectedBitmap);
    layer_add_child(window_get_root_layer(wWindow), bitmap_layer_get_layer(tBluetoothConnectedLayer));
    
    /* Create and add a time text layer */
    tTimeLayer = text_layer_create(GRect(0, 52, 144, 50));
    text_layer_set_background_color(tTimeLayer, GColorBlack);
    text_layer_set_text_color(tTimeLayer, GColorWhite);
    text_layer_set_font(tTimeLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
    text_layer_set_text_alignment(tTimeLayer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(wWindow), text_layer_get_layer(tTimeLayer));
    
    /* Create and add a date text layer */
    tDateLayer = text_layer_create(GRect(0, 96, 144, 30));
    text_layer_set_background_color(tDateLayer, GColorBlack);
    text_layer_set_text_color(tDateLayer, GColorWhite);
    text_layer_set_font(tDateLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    text_layer_set_text_alignment(tDateLayer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(wWindow), text_layer_get_layer(tDateLayer));
}

/* 
** Unloads the main window. 
*/
static void MainWindowUnload(Window *wWindow)
{
    /* Destroy the text layers */
    text_layer_destroy(tTimeLayer);
    
    /* Destroy the status layers */
    bitmap_layer_destroy(tBluetoothConnectedLayer);
    bitmap_layer_destroy(tBatteryChargingLayer);
    layer_destroy(tBatteryLevelLayer);
    bitmap_layer_destroy(tBatteryShellLayer);
    
    /* Destroy the background layer */
    layer_destroy(tBackgroundLayer);
}

/* 
** Draws the background layer. 
*/
static void BackgroundLayerUpdate(Layer *tLayer, GContext *tContext)
{
    graphics_context_set_fill_color(tContext, GColorBlack);
    graphics_fill_rect(tContext, GRect(0, 0, 144, 168), 0, GCornerNone);
}

/* 
** Draws the battery level layer. 
*/
static void BatteryLevelLayerUpdate(Layer *tLayer, GContext *tContext)
{
    if (u8BatteryLevelWidth > 0)
    {
        graphics_context_set_fill_color(tContext, GColorWhite);
        graphics_fill_rect(tContext, GRect(0, 0, u8BatteryLevelWidth, 4), 0, GCornerNone);
    }
}