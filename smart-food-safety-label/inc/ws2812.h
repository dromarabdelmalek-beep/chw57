/**
 * @file ws2812.h
 * @brief WS2812 RGB LED control
 *
 * Controls single WS2812 RGB LED for visual status indication
 * - Green: Safe
 * - Yellow: Warning
 * - Red: Expired/Violation
 * - Blinking patterns
 */

#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>
#include <stdbool.h>

/* LED color definitions */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color_t;

/* Predefined colors */
#define COLOR_OFF       {0, 0, 0}
#define COLOR_RED       {255, 0, 0}
#define COLOR_GREEN     {0, 255, 0}
#define COLOR_YELLOW    {255, 255, 0}
#define COLOR_BLUE      {0, 0, 255}
#define COLOR_WHITE     {255, 255, 255}

/* LED status modes */
typedef enum {
    LED_STATUS_SAFE = 0,        // Green solid
    LED_STATUS_WARNING,         // Yellow solid
    LED_STATUS_EXPIRED,         // Red solid
    LED_STATUS_VIOLATION,       // Red blinking
    LED_STATUS_OUT_OF_FRIDGE,   // Slow pulse
    LED_STATUS_OFF
} led_status_t;

/* Function prototypes */
void ws2812_init(void);
void ws2812_set_color(rgb_color_t color);
void ws2812_set_rgb(uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_status(led_status_t status);
void ws2812_update(void);  // Call periodically for blinking
void ws2812_off(void);

#endif /* WS2812_H */
