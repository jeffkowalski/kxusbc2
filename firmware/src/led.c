#include "twi.h"

#define LP5815_ADDR 0x2D

static bool led_write_register(uint8_t reg, uint8_t value) {
    return twi_send_bytes(LP5815_ADDR, (uint8_t[]){reg, value}, 2);
}

void led_init(void) {
    // Enable chip, instant blinking disable
    led_write_register(0x00, 0x03);

    // Set maximum current = 25.5 mA
    led_write_register(0x01, 0x00);

    // Set maximum current 5 mA on green (OUT0)
    led_write_register(0x14, 50);

    // Set maximum current 20 mA on blue (OUT1)
    led_write_register(0x15, 200);

    // Set maximum current 10 mA on red (OUT2)
    led_write_register(0x16, 100);

    // Enable all three outputs
    led_write_register(0x02, 0x07);

    // Send update command
    led_write_register(0x0F, 0x55);
}

void led_set_color(uint8_t red, uint8_t green, uint8_t blue) {
    led_write_register(0x18, green);
    led_write_register(0x19, blue);
    led_write_register(0x1A, red);
    if (red == 0 && green == 0 && blue == 0) {
        // Special case: put chip in standby to reduce power consumption
        led_write_register(0x00, 0x02);
    } else {
        // Ensure chip is enabled
        led_write_register(0x00, 0x03);
    }
}
