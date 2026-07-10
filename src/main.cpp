#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BLEScan.h>
#include <BLEDevice.h>
#include <ble_connection.h>
#include <lvgl.h>

volatile int current_speed = 0;
int last_speed = -1;
// Initialize the display library instance
TFT_eSPI tft = TFT_eSPI();

// 1. Declare the LVGL drawing buffer object
static lv_disp_draw_buf_t draw_buf;
// 2. Allocate the raw RAM for it (1/10th of a 480x320 screen is standard)
static lv_color_t buf[ 480 * 10 ];

lv_obj_t *arc;
/* Display flushing function for TFT_eSPI */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushImage(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv); // Tell LVGL the flush is done
}


void setup() {
    Serial.begin(115200);
    Serial.println("Booting up main control system...");

    // force the backlight hardware pin ON immediately 
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH); 

    tft.init();
    tft.setRotation(1);
    lv_init();
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    // Point LVGL to the RAM buffer we created globally
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);

    // Initialize the display driver struct
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    
    // Tell LVGL your exact screen dimensions
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    
    // Link the buffer and your custom flush function to the driver
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    
    // Register the driver (LVGL now knows the screen exists!)
    lv_disp_drv_register(&disp_drv);

    // initalize display
    lv_obj_t *dashboard = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(dashboard, lv_color_hex(0x434927), LV_PART_MAIN);
    lv_scr_load(dashboard);
    
    
    arc = lv_arc_create(dashboard);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(arc, 150, 150);
    lv_arc_set_range(arc, 0, 50);


    // print initial boot status to screen
    

    // 4. Fire up the Bluetooth engine
    BLEDevice::init("Pooboo_spin_bike");

    // Update screen text before attempting handshake
    
    

    BLEAddress targetAddress("51:81:67:11:50:5a");
    bool connection = connectToDevice(targetAddress);
    Serial.println("==== Finished connecting, entering LOOP ");
    // Clear screen one final time to display the connection results cleanly
    

    // if (connection) {
        
    
    
    // } else {
    
    
    
    // }
}

void loop() {
    if (current_speed != last_speed) {
        if (arc != NULL) {
            Serial.print("Updating speed to : ");
            Serial.println(current_speed);
            lv_arc_set_value(arc, current_speed);
        }
        last_speed = current_speed;
    }
    lv_tick_inc(5);
    lv_timer_handler();
    delay(5); 
}