#ifndef OPTICAL_FLOW_HPP
#define OPTICAL_FLOW_HPP

#include <Arduino.h>
#include "common.h"
#include "Bitcraze_PMW3901.h"

// see https://docs.m5stack.com/en/app/Stamp%20Fly
#define PIN_CS2  12
#define PIN_MISO 43
#define PIN_MOSI 14
#define PIN_SCK  44

void optical_flow_init(void);
void optical_flow_get_offset(int16_t &dx, int16_t &dy, uint8_t &quality);

#endif 