/**
 * @file label_config.h
 * @brief Configuration defaults and preconfigured values
 *
 * Provides default settings for:
 * - Product shelf-life defaults
 * - Temperature thresholds
 * - Timing parameters
 * - BLE configuration
 */

#ifndef LABEL_CONFIG_H
#define LABEL_CONFIG_H

#include <stdint.h>

/* Hardware pin definitions */
#define PIN_BUTTON_EDIT    6   // PA6
#define PIN_BUTTON_UP      7   // PA7
#define PIN_BUTTON_DOWN    8   // PA8

#define PIN_WS2812_DATA    9   // PA9 (example)

/* SPI pins for E-paper */
#define PIN_EPAPER_CS      10  // PA10
#define PIN_EPAPER_DC      11  // PA11
#define PIN_EPAPER_RST     12  // PA12
#define PIN_EPAPER_BUSY    13  // PA13

/* I2C pins for SHT4x */
#define PIN_I2C_SDA        14  // PA14
#define PIN_I2C_SCL        15  // PA15

/* Timing configuration */
#define BUTTON_LONG_PRESS_MS        5000    // 5 seconds for edit mode
#define BUTTON_DEBOUNCE_MS          50      // 50ms debounce
#define TEMP_SAMPLE_INTERVAL_MS     300000  // 5 minutes
#define TEMP_VIOLATION_TIMEOUT_MS   1800000 // 30 minutes
#define LED_BLINK_INTERVAL_MS       600000  // 10 minutes
#define EXPIRY_CHECK_INTERVAL_MS    3600000 // 1 hour

/* Temperature thresholds (Fahrenheit) */
#define TEMP_SAFE_MAX_F             41      // Default safe max temp
#define TEMP_DANGER_F               45      // Danger zone

/* Product defaults (shelf-life in days) */
typedef struct {
    const char* name;
    uint8_t default_days;
    int16_t max_temp_f;
} product_config_t;

extern const product_config_t product_defaults[];
extern const uint8_t num_products;

/* Warning thresholds */
#define DAYS_WARNING_THRESHOLD      1       // Yellow LED when <1 day
#define BATTERY_LOW_THRESHOLD       10      // Low battery at 10%

/* BLE configuration */
#define BLE_DEVICE_NAME             "FoodLabel"
#define BLE_ADV_INTERVAL_MS         1000    // 1 second
#define BLE_CONN_TIMEOUT_MS         20000   // 20 seconds

/* Function prototypes */
void label_config_init(void);
const product_config_t* label_config_get_product(uint8_t index);
const char* label_config_get_product_name(uint8_t index);
uint8_t label_config_get_product_days(uint8_t index);
int16_t label_config_get_product_max_temp(uint8_t index);

#endif /* LABEL_CONFIG_H */
