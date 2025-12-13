#ifndef OPTICAL_FLOW_HPP
#define OPTICAL_FLOW_HPP

#include <Arduino.h>
#include "common.h"

void optical_flow_init(void);
void optical_flow_get_offset(int16_t *dx, int16_t *dy, uint8_t *quality);

#endif 