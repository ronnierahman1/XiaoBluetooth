#include "TFT_eSPI.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
// #include "EPaper.h"

#ifdef EPAPER_ENABLE // Only compile this code if the EPAPER_ENABLE is defined in User_Setup.h
EPaper epaper;
#endif


#define CMD_NEXT      0x01
#define CMD_PREV      0x02
#define CMD_M1        0x11
#define CMD_M2        0x12
#define CMD_M3        0x13
#define CMD_M4        0x14

// UUIDs must match Android:
static NimBLEUUID serviceUUID("0000abcd-0000-1000-8000-00805f9b34fb");
static NimBLEUUID charUUID   ("0000abce-0000-1000-8000-00805f9b34fb");

// EPaper epaper;

int currentIndex = 0;
const int NUM_METHODS = 4;

// Forward declarations
void runCurrentMethod();
void method1();
void method2();
void method3();
void method4();

class CmdCallback : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override{
        std::string value =  pCharacteristic->getValue();
        if (value.empty()) return;

        uint8_t cmd = (uint8_t)value[0];

        switch (cmd) {
            case CMD_NEXT:
                currentIndex = (currentIndex + 1) % NUM_METHODS;
                runCurrentMethod();
                break;
            case CMD_PREV:
                currentIndex = (currentIndex - 1 + NUM_METHODS) % NUM_METHODS;
                runCurrentMethod();
                break;
            case CMD_M1: currentIndex = 0; method1(); break;
            case CMD_M2: currentIndex = 1; method2(); break;
            case CMD_M3: currentIndex = 2; method3(); break;
            case CMD_M4: currentIndex = 3; method4(); break;
            default:
                break;
        }
    }
};

void setup() {
    Serial.begin(115200);

    // e-paper init
    epaper.begin();
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setTextColor(TFT_BLACK, TFT_WHITE, true);
    epaper.setCursor(10, 40);
    epaper.print("Waiting for BLE...");
    epaper.update();

    // BLE init
    NimBLEDevice::init("XIAO_EPAPER");
    NimBLEServer *server = NimBLEDevice::createServer();
    NimBLEService *service = server->createService(serviceUUID);
    NimBLECharacteristic *cmdChar = service->createCharacteristic(
        charUUID,
        NIMBLE_PROPERTY::WRITE
    );
    cmdChar->setCallbacks(new CmdCallback());
    service->start();

    NimBLEAdvertising *adv = NimBLEDevice::getAdvertising();
    adv->addServiceUUID(serviceUUID);
    adv->start();
}

void loop() {
    // nothing – BLE callbacks handle everything
}

// Helpers

void runCurrentMethod() {
    switch (currentIndex) {
        case 0: method1(); break;
        case 1: method2(); break;
        case 2: method3(); break;
        case 3: method4(); break;
    }
}

// Example methods – replace with your real ones
void method1() {
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setTextDatum(TL_DATUM);
    epaper.setCursor(10, 40);
    epaper.print("Method 1");
    epaper.update();
}

void method2() {
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setCursor(10, 40);
    epaper.print("Method 2");
    epaper.update();
}

void method3() {
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setCursor(10, 40);
    epaper.print("Method 3");
    epaper.update();
}

void method4() {
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setCursor(10, 40);
    epaper.print("Method 4");
    epaper.update();
}
