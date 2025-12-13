#include <Arduino.h>
#include "common.h"
#include "optical_flow.hpp"
#include "pmw3901_esp.hpp"

// Structure create

PMW3901_ESP flow(PIN_CS2);

void optical_flow_init() {
    USBSerial.printf("Start Optical Flow Initialize!\n\r");
    if (!flow.init()) {
        USBSerial.printf("Optical Flow Initialization Failed!\n\r");
        while (1);
    }
    USBSerial.printf("Optical Flow Initialized!\n\r");
}

void optical_flow_get_offset(int16_t *dx, int16_t *dy, uint8_t *quality) {
    flow.readMotionCount(dx, dy);
    *quality = flow.readQuality();
}