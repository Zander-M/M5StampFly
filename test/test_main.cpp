/*
 P3901 optical flow test
*/
#include <Arduino.h>
#include "pmw3901_esp.hpp"
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

// Using digital 12 for chip select
PMW3901_ESP flow(PIN_CS2);

spi_bus_config_t buscfg = {
    .mosi_io_num = PIN_MOSI,
    .miso_io_num = PIN_MISO,
    .sclk_io_num = PIN_SCK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4096*2,
};

void setup() {
  USBSerial.begin(115200);
  while (!USBSerial) {}
  vTaskDelay(pdMS_TO_TICKS(1500));
  USBSerial.println("Test Started.");
  esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
  if (!flow.init()) {
    USBSerial.println("Initialization of the flow sensor failed");
    while(1) { }
  }
  USBSerial.println("OF Initialized.");
}

int16_t dX,dY;
uint8_t quality;

void loop() {
  // Get motion count since last call
  flow.readMotionCount(&dX, &dY);
  quality = flow.readQuality();

  // dX = 10;
  // dY = 10;

  USBSerial.print("X: ");
  USBSerial.print(dX);
  USBSerial.print(", Y: ");
  USBSerial.print(dY);
  USBSerial.print(", quality: ");
  USBSerial.print(quality);

  USBSerial.print("\n");

  delay(10);
}