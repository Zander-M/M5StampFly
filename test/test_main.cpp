/*
 P3901 optical flow test
*/
#include <Arduino.h>
#include <SPI.h>
#include <Bitcraze_PMW3901.h>
#include "USB.h"

// -------- PIN CONFIG (ADJUST THESE FOR YOUR BOARD) --------
#define PIN_CS  46
#define PIN_CS2  12
#define PIN_MISO 43
#define PIN_MOSI 14
#define PIN_SCK  44

//------ SPI SETTING -------------

// -------- REGISTER DEFINITIONS --------
#define REG_MOTION            0x02
#define REG_DELTA_X_L         0x03
#define REG_DELTA_X_H         0x04
#define REG_DELTA_Y_L         0x05
#define REG_DELTA_Y_H         0x06
#define REG_SQUAL             0x07
#define REG_POWER_UP_RESET    0x3A

//------------------------------------------------------------

// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow(PIN_CS2);

void setup() {
  USBSerial.begin(115200);

  if (!flow.begin(PIN_SCK, PIN_MISO, PIN_MOSI)) {
    USBSerial.println("Initialization of the flow sensor failed");
    while(1) { }
  }
}

int16_t dX,dY;

void loop() {
  // Get motion count since last call
  flow.readMotionCount(&dX, &dY);

  USBSerial.print("X: ");
  USBSerial.print(dX);
  USBSerial.print(", Y: ");
  USBSerial.print(dY);
  USBSerial.print("\n");

  delay(100);
}