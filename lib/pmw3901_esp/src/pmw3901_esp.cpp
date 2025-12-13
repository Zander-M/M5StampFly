#include <FreeRTOS.h>
#include "pmw3901_esp.hpp"
#include "esp_log.h"
#include "driver/gpio.h"
#include <string.h>

#define TAG "PMW3901"

#define ets_delay(ms) ets_delay_us((ms) * 1000)

spi_host_device_t host=SPI2_HOST;

PMW3901_ESP::PMW3901_ESP(uint8_t cs) 
 : pin_cs(cs)
{ }

// ---------- Init ----------
bool PMW3901_ESP::init(void)
{
    // SPI 
    spi_device_interface_config_t devcfg = {};
    devcfg.mode = 3;                  // PMW3901 requires MODE3
    devcfg.clock_speed_hz = 4 * 1000 * 1000;
    devcfg.spics_io_num = 12;
    devcfg.queue_size = 1;
    devcfg.flags = 0;

    esp_err_t ret = spi_bus_add_device(host, &devcfg, &spidev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_add_device failed");
        return false;
    }
    ESP_LOGI(TAG, "added optical flow to spi");

    // Power-up reset
    regWrite(0x3A, 0x5A);
    ets_delay(5);

    uint8_t chip = regRead(0x00);
    uint8_t inv  = regRead(0x5F);

    ESP_LOGI(TAG, "ChipID=0x%02X Inv=0x%02X", chip, inv);

    // if (chip != 0x49 || inv != 0xB8) {
    //     ESP_LOGE(TAG, "PMW3901 ID mismatch");
    //     return false;
    // }
    regRead(0x02);
    regRead(0x03);
    regRead(0x04);
    regRead(0x05);
    regRead(0x06);
    ets_delay(1);

    initRegisters();
    return true;
}

uint8_t PMW3901_ESP::regRead(uint8_t reg)
{
    uint8_t tx[2] = { uint8_t(reg & 0x7F), 0x00 };
    uint8_t rx[2] = { 0, 0 };

    spi_transaction_t t = {};
    t.length = 16;                 // 2 bytes
    t.tx_buffer = tx;
    t.rx_buffer = rx;

    ESP_ERROR_CHECK(
        spi_device_polling_transmit(spidev, &t)
    );

    return rx[1];
}

void PMW3901_ESP::regWrite(uint8_t reg, uint8_t value)
{
    uint8_t tx[2] = { uint8_t(reg | 0x80), value };

    spi_transaction_t t = {};
    t.length = 16;
    t.tx_buffer = tx;

    ESP_ERROR_CHECK(
        spi_device_polling_transmit(spidev, &t)
    );
}


void PMW3901_ESP::readMotionCount(int16_t *dx, int16_t *dy)
{
    regRead(0x02);  // motion register latch
    *dx = ((int16_t)regRead(0x04) << 8) | regRead(0x03);
    *dy = ((int16_t)regRead(0x06) << 8) | regRead(0x05);
}

uint8_t PMW3901_ESP::readQuality()
{
    return regRead(0x07);  // SQUAL
}

// Performance optimisation registers
void PMW3901_ESP::initRegisters()
{
  regWrite(0x7F, 0x00);
  regWrite(0x61, 0xAD);
  regWrite(0x7F, 0x03);
  regWrite(0x40, 0x00);
  regWrite(0x7F, 0x05);
  regWrite(0x41, 0xB3);
  regWrite(0x43, 0xF1);
  regWrite(0x45, 0x14);
  regWrite(0x5B, 0x32);
  regWrite(0x5F, 0x34);
  regWrite(0x7B, 0x08);
  regWrite(0x7F, 0x06);
  regWrite(0x44, 0x1B);
  regWrite(0x40, 0xBF);
  regWrite(0x4E, 0x3F);
  regWrite(0x7F, 0x08);
  regWrite(0x65, 0x20);
  regWrite(0x6A, 0x18);
  regWrite(0x7F, 0x09);
  regWrite(0x4F, 0xAF);
  regWrite(0x5F, 0x40);
  regWrite(0x48, 0x80);
  regWrite(0x49, 0x80);
  regWrite(0x57, 0x77);
  regWrite(0x60, 0x78);
  regWrite(0x61, 0x78);
  regWrite(0x62, 0x08);
  regWrite(0x63, 0x50);
  regWrite(0x7F, 0x0A);
  regWrite(0x45, 0x60);
  regWrite(0x7F, 0x00);
  regWrite(0x4D, 0x11);
  regWrite(0x55, 0x80);
  regWrite(0x74, 0x1F);
  regWrite(0x75, 0x1F);
  regWrite(0x4A, 0x78);
  regWrite(0x4B, 0x78);
  regWrite(0x44, 0x08);
  regWrite(0x45, 0x50);
  regWrite(0x64, 0xFF);
  regWrite(0x65, 0x1F);
  regWrite(0x7F, 0x14);
  regWrite(0x65, 0x60);
  regWrite(0x66, 0x08);
  regWrite(0x63, 0x78);
  regWrite(0x7F, 0x15);
  regWrite(0x48, 0x58);
  regWrite(0x7F, 0x07);
  regWrite(0x41, 0x0D);
  regWrite(0x43, 0x14);
  regWrite(0x4B, 0x0E);
  regWrite(0x45, 0x0F);
  regWrite(0x44, 0x42);
  regWrite(0x4C, 0x80);
  regWrite(0x7F, 0x10);
  regWrite(0x5B, 0x02);
  regWrite(0x7F, 0x07);
  regWrite(0x40, 0x41);
  regWrite(0x70, 0x00);

  ets_delay(100);

  regWrite(0x32, 0x44);
  regWrite(0x7F, 0x07);
  regWrite(0x40, 0x40);
  regWrite(0x7F, 0x06);
  regWrite(0x62, 0xf0);
  regWrite(0x63, 0x00);
  regWrite(0x7F, 0x0D);
  regWrite(0x48, 0xC0);
  regWrite(0x6F, 0xd5);
  regWrite(0x7F, 0x00);
  regWrite(0x5B, 0xa0);
  regWrite(0x4E, 0xA8);
  regWrite(0x5A, 0x50);
  regWrite(0x40, 0x80);
}