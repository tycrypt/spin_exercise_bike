#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include "ble_connection.h"

// STD. bluetooth SIG UUID for cycling speed and cadence
static BLEUUID serviceUUID((uint16_t)0xFFF0);
static BLEUUID charUUID((uint16_t)0xFFF1);

/* == This func. will make the inital connection to the specified MAC address
        while also checking for errors in the connection.

        IN: device MAC address
        OUT: state of connection (BOOL)
*/
bool connectToDevice(BLEAddress target_device) {
    
    Serial.print("Forming point to point data link to address :");
    Serial.println(target_device.toString().c_str());

    // Set up client to communicate
    // utilize static factory method which returns a pointer
    BLEClient *client = BLEDevice::createClient(); 

    // Invoke connection
    if (!client->connect(target_device) ){
        Serial.print("Error: Could not connect to this MAC address!");
        return false;
    }
    // Service living on bike sensor device : Service
    BLERemoteService *service = nullptr;
    service = client->getService(serviceUUID);

    if (service == nullptr) {
        Serial.print("Error: Device does not have 0x1816 service!");
        return false;
    }

    // Find the data pipe : Characteristic
    BLERemoteCharacteristic *character = nullptr;
    character = service->getCharacteristic(charUUID);

    if (character == nullptr) {
        Serial.print("Error: Could not find 0x2A5B characteristic!");
        return false;
    }

    Serial.printf("Successfully Connected to : %s", target_device.toString().c_str());
    return true;
}