/* 
** Manages GUI set up and updating.
*/

#include <pebble.h>
#include "gui.h"

#define DATE_BUFFER_LENGTH 12
#define TIME_FONT FONT_KEY_ROBOTO_BOLD_SUBSET_49
#define DATE_FONT FONT_KEY_GOTHIC_24
    
/* Windows */
static Window *sw_main_window;

/* Bitmaps */
static GBitmap *st_battery_shell_bitmap;
static GBitmap *st_battery_charging_bitmap;
static GBitmap *st_bluetooth_connected_bitmap;

/* Graphics layers */
static Layer *st_background_layer;
static BitmapLayer *st_battery_shell_layer;
static Layer *st_battery_level_layer;
static BitmapLayer *st_battery_charging_layer;
static BitmapLayer *st_bluetooth_connected_layer;
static TextLayer *st_time_layer;
static TextLayer *st_date_layer;

/* Graphics state management */
static uint8_t su8_battery_level_width;
    
/* Private forward declarations */
static void main_window_load(Window *w_window);
static void main_window_unload(Window *w_window);
static void background_layer_update(Layer *t_layer, GContext *t_context);
static void battery_level_layer_update(Layer *t_layer, GContext *t_context);

/* 
** Initialises the GUI. 
*/
void gui_init(void)
{
    /* Create main window element 
    and set handlers to manage the elements inside it */
    sw_main_window = window_create();
    window_set_window_handlers(sw_main_window, (WindowHandlers)
    {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    /* Show the window on the watch as animated */
    window_stack_push(sw_main_window, true);
    
    /* Show the current time and date */
    gui_update_time();
    gui_update_date();
}

/* 
** Releases resources.
*/
void gui_deinit(void)
{
    /* Destroy the main window */
    window_destroy(sw_main_window);
}

/* 
** Updates the battery level on the GUI.
** u8_battery_level_tenths - The number of tenths of the battery level left.
** b_is_charging - A value indicating whether the battery is charging.
*/
void gui_update_battery_level(uint8_t u8_battery_level_tenths, bool b_is_charging)
{
    /* Determine the number of pixels for the battery level */
    su8_battery_level_width = u8_battery_level_tenths;
    
    /* Refresh the battery level and hide the charging image if required */
    layer_mark_dirty(st_battery_level_layer);
    layer_set_hidden(bitmap_layer_get_layer(st_battery_charging_layer), !b_is_charging);
}

/* 
** Updates the bluetooth connection status on the GUI.
** b_is_connected - A value indicating whether bluetoth is connected.
*/
void gui_update_connection_status(bool b_is_connected)
{
	layer_set_hidden(bitmap_layer_get_layer(st_bluetooth_connected_layer), !b_is_connected);
}

/* 
** Updates the time on the GUI.
*/
void gui_update_time(void)
{
    /* Get a time structure */
    time_t temp = time(NULL); 
    struct tm *t_current_time = localtime(&temp);

    /* Create a long-lived buffer for displaying the time */
    static char ac_buffer[] = "00:00";

    /* Write the current hours and minutes into the buffer */
    if (clock_is_24h_style() == true)
    {
        /* Use 24 hour format */
		strftime(ac_buffer, sizeof(ac_buffer), "%H:%M", t_current_time);
    }
    else
    {
        /* Use 12 hour format */
		strftime(ac_buffer, sizeof(ac_buffer), "%I:%M", t_current_time);
    }

    /* Display this time on the text layer */
	text_layer_set_text(st_time_layer, ac_buffer);
}

/* 
** Updates the date on the GUI. 
*/
void gui_update_date(void)
{
    /* Get a time structure */
    time_t temp = time(NULL); 
	struct tm *t_current_time = localtime(&temp);
    
    /* Create a long-lived buffer for displaying the date */
	static char ac_buffer[DATE_BUFFER_LENGTH];

    /* Write the current month and day of the month into the buffer */
	strftime(ac_buffer, DATE_BUFFER_LENGTH, "%B %e", t_current_time);

    /* Display this date on the text layer */
	text_layer_set_text(st_date_layer, ac_buffer);
}

/**************************** Private functions *****************************/

/* 
** Loads the main window.
*/
static void main_window_load(Window *w_window)
{
    /* Create and add the background (drawing) layer */
    st_background_layer = layer_create(GRect(0, 0, 144, 168));
    layer_set_update_proc(st_background_layer, background_layer_update);
    layer_add_child(window_get_root_layer(w_window), st_background_layer);
    
    /* Create and add a battery shell bitmap layer */
    st_battery_shell_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
    st_battery_shell_layer = bitmap_layer_create(GRect(126, 4, 15, 8));
    bitmap_layer_set_bitmap(st_battery_shell_layer, st_battery_shell_bitmap);
    layer_add_child(window_get_root_layer(w_window), bitmap_layer_get_layer(st_battery_shell_layer));
    
    /* Create and add the battery level layer */
    st_battery_level_layer = layer_create(GRect(128, 6, 10, 4));
    layer_set_update_proc(st_battery_level_layer, battery_level_layer_update);
    layer_add_child(window_get_root_layer(w_window), st_battery_level_layer);
    
    /* Create the battery charging symbol */
    st_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGING);
    st_battery_charging_layer = bitmap_layer_create(GRect(117, 4, 7, 8));
    bitmap_layer_set_bitmap(st_battery_charging_layer, st_battery_charging_bitmap);
    layer_add_child(window_get_root_layer(w_window), bitmap_layer_get_layer(st_battery_charging_layer));
    
    /* Create the bluetooth connected symbol */
    st_bluetooth_connected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);
    st_bluetooth_connected_layer = bitmap_layer_create(GRect(3, 3, 7, 10));
    bitmap_layer_set_bitmap(st_bluetooth_connected_layer, st_bluetooth_connected_bitmap);
    layer_add_child(window_get_root_layer(w_window), bitmap_layer_get_layer(st_bluetooth_connected_layer));
    
    /* Create and add a time text layer */
    st_time_layer = text_layer_create(GRect(0, 46, 144, 50));
    text_layer_set_background_color(st_time_layer, GColorBlack);
    text_layer_set_text_color(st_time_layer, GColorWhite);
    text_layer_set_font(st_time_layer, fonts_get_system_font(TIME_FONT));
    text_layer_set_text_alignment(st_time_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(w_window), text_layer_get_layer(st_time_layer));
    
    /* Create and add a date text layer */
    st_date_layer = text_layer_create(GRect(0, 96, 144, 30));
    text_layer_set_background_color(st_date_layer, GColorBlack);
    text_layer_set_text_color(st_date_layer, GColorWhite);
    text_layer_set_font(st_date_layer, fonts_get_system_font(DATE_FONT));
    text_layer_set_text_alignment(st_date_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(w_window), text_layer_get_layer(st_date_layer));
}

/* 
** Unloads the main window. 
*/
static void main_window_unload(Window *w_window)
{
    /* Destroy the text layers */
    text_layer_destroy(st_time_layer);
    
    /* Destroy the status layers */
    bitmap_layer_destroy(st_bluetooth_connected_layer);
    bitmap_layer_destroy(st_battery_charging_layer);
    layer_destroy(st_battery_level_layer);
    bitmap_layer_destroy(st_battery_shell_layer);
    
    /* Destroy the background layer */
    layer_destroy(st_background_layer);
}

/* 
** Draws the background layer. 
*/
static void background_layer_update(Layer *t_layer, GContext *t_context)
{
    graphics_context_set_fill_color(tContext, GColorBlack);
    graphics_fill_rect(tContext, GRect(0, 0, 144, 168), 0, GCornerNone);
}

/* 
** Draws the battery level layer. 
*/
static void battery_level_layer_update(Layer *t_layer, GContext *t_context)
{
    if (su8_battery_level_width > 0)
    {
        graphics_context_set_fill_color(tContext, GColorWhite);
        graphics_fill_rect(tContext, GRect(0, 0, su8_battery_level_width, 4), 0, GCornerNone);
    }
}