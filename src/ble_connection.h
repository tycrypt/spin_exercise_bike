#ifndef BLE_CONNECTION_H
#define BLE_CONNECTION_H

#include <Arduino.h>
#include <BLEDevice.h>

// Just a blueprint line. Notice the semicolon at the end instead of brackets {}
bool connectToDevice(BLEAddress target_device);

#endif