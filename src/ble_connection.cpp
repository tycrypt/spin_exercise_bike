#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include "ble_connection.h"
#include <lvgl.h>

extern volatile int current_speed;

void onDataRecieved (BLERemoteCharacteristic *character, uint8_t* packet_data, size_t packet_size, bool isNotify);
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
    // register the notify callback function
    character->registerForNotify(onDataRecieved);
    Serial.printf("Successfully Connected to : %s", target_device.toString().c_str());
    return true;
}

/* == This func. will act as a callback function to parse bytes that the device is sending

*/
void onDataRecieved (BLERemoteCharacteristic *character, uint8_t* packet_data, size_t packet_size, bool isNotify) {
    // Sloop Phase : print the raw hex
    for ( int i = 0 ; i < packet_size ; i++ ){
        //print current packet
        Serial.printf("%02X", packet_data[i]);
        
    }
    Serial.println();

    // ensure that the packet has finished populating
    if (packet_size >= 11) {
        /* extract packet_data[2:3] : combine packets 2 and 3
         data[2] = 0x2A ---> 00000000 00101010
         data[2] >> 8 = 00101010 00000000
         data[3] = 0x3B ---> 00000000 00111011
         data[2] >> 8 | data[3] ---> 00101010 00000000
                                  OR 00000000 00111011
                                  ---------------------
        combined hex bytes           00101010 00111011  
        */

        uint16_t rawRate = (packet_data[2] << 8) | packet_data[3];
        float trueRate = rawRate / 5.0;
        // extract packet_data[9:10] : total revolutions
        uint16_t totalRevs = (packet_data[10] << 8) | packet_data[9];
        Serial.print("SENDING DATA TO MAIN speed = ");
        Serial.println(current_speed);
        current_speed = (int)trueRate;

        
    }
}