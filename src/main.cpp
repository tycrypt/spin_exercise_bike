#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BLEScan.h>
#include <BLEDevice.h>
#include <ble_connection.h>

// Initialize the display library instance
TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    Serial.println("Booting up main control system...");

    // force the backlight hardware pin ON immediately 
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH); 

    // initalize display
    tft.init();
    tft.setRotation(1); 
    tft.fillScreen(TFT_BLACK);  // white -> black screen

    // (Text Color, Background Color)
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 

    // print initial boot status to screen
    tft.drawString("Initializing BLE Radio System...", 15, 15, 2);

    // 4. Fire up the Bluetooth engine
    BLEDevice::init("Pooboo_spin_bike");

    // Update screen text before attempting handshake
    tft.fillScreen(TFT_BLACK); // Clear previous status line
    tft.drawString("Targeting Sensor Address [51:81:67:11:50:5a]...", 15, 15, 2);

    BLEAddress targetAddress("51:81:67:11:50:5a");
    bool connection = connectToDevice(targetAddress);
    
    // Clear screen one final time to display the connection results cleanly
    tft.fillScreen(TFT_BLACK); 

    if (connection) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK); // Green text for success!
        tft.drawString("SUCCESS: Linked to Spin Bike Engine!", 15, 15, 2);
        tft.drawString("System Status: ONLINE", 15, 40, 4); // Larger font for status
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);   // Red text for failures
        tft.drawString("ERROR: Connection Timeout.", 15, 15, 2);
        tft.drawString("Check bike switch / spin wheel to wake up.", 15, 40, 2);
    }
}

void loop() {
    delay(200); 
}