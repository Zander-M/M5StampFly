#pragma once
#include "driver/spi_master.h"
#include <stdint.h>

struct FlowDelta {
    int16_t dx;
    int16_t dy;
};

class PMW3901_ESP {
public:

    PMW3901_ESP(uint8_t cs);
    bool init();
    void readMotionCount(int16_t *dx, int16_t *dy);
    uint8_t readQuality();
    void initRegisters(void);

private:
    uint8_t pin_cs; 
    spi_device_handle_t spidev = nullptr;
    uint8_t regRead(uint8_t reg);
    void regWrite(uint8_t reg, uint8_t val);
};
