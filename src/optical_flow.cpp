#include <Arduino.h>
#include "Bitcraze_PMW3901.h"
#include "optical_flow.hpp"

// Structure create

int16_t dX, dY;
uint8_t quality;
Bitcraze_PMW3901 flow(PIN_CS2);

void optical_flow_init() {
    USBSerial.printf("Start Optical Flow Initialize!\n\r");
    if (!flow.begin(PIN_SCK, PIN_MISO, PIN_MOSI)) {
        USBSerial.printf("Optical Flow Initialization Failed!\n\r");
        while (1);
    }
}

void optical_flow_get_offset(int16_t &dx, int16_t &dy, uint8_t &quality) {
    flow.readMotionCount(&dx, &dy);
    quality = flow.getQuality();
}