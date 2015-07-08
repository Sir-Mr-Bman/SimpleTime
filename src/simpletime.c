/*
    simpletime.c
    Part of the SimpleTime project for Pebble.
    Created by Brennan Macaig.
    Copyright (C) 2015 Brennan Macaig under the M.I.T. license.
*/

#include <pebble.h>


// WARNING! These imports are only needed for the Pebble Time.
#include <stdio.h>
#include <stdlib.h>
// END TIME IMPORTS


static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GColor s_color;
static GFont s_time_font;
static GFont s_date_font;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  time_t rawtime;
  struct tm *tick_time = localtime(&temp);
  struct tm *timeinfo;
  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char datebuffer[] = "10/27/1998";
  
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  
  strftime(datebuffer, sizeof("10/27/1998"), "%m/%d/%Y", timeinfo);
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
 
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_date_layer, datebuffer);
}
 
static void main_window_load(Window *window) {
  window_set_background_color(s_main_window, GColorBlack);
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  s_date_layer = text_layer_create(GRect(0, 130, 144, 25));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_date_layer, "10/27/1998");
  
  // --- COLORS ---
  #ifdef PBL_BW
  text_layer_set_background_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  #elif PBL_COLOR
    
  // Add a splash of color for Basalt platform.
  srand(time(NULL));
  int random = rand() % 5;
  switch (random) {
      case 0:
            s_color = GColorRed;
            break;
      case 1:
            s_color = GColorBlue;
            break;
      case 2:
            s_color = GColorChromeYellow;
            break;
      case 3:
            s_color = GColorIslamicGreen;
            break;
      case 4:
            s_color = GColorPurple;
            break;
      case 5:
            s_color = GColorOrange;
            break;
      default:
            s_color = GColorWhite;
            break;
      
  }
  text_layer_set_background_color(s_time_layer, s_color);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  #endif
  
    
  text_layer_set_text(s_time_layer, "00:00");
 
  // Improve the layout to be more like a watchface
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  text_layer_set_font(s_date_layer, s_date_font);
 
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  // Make sure the time is displayed from the start
  update_time();
}
 
static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
}
 
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
 
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
 
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}
 
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}
 
int main(void) {
  init();
  app_event_loop();
  deinit();
}