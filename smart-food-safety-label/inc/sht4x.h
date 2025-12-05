/**
 * @file sht4x.h
 * @brief Sensirion SHT4x temperature and humidity sensor driver
 *
 * I2C interface to SHT4x sensor for:
 * - Temperature measurement
 * - Humidity measurement (optional)
 * - Low-power operation
 */

#ifndef SHT4X_H
#define SHT4X_H

#include <stdint.h>
#include <stdbool.h>

/* SHT4x I2C address */
#define SHT4X_I2C_ADDR      0x44

/* Measurement precision modes */
typedef enum {
    SHT4X_PRECISION_HIGH = 0,
    SHT4X_PRECISION_MEDIUM,
    SHT4X_PRECISION_LOW
} sht4x_precision_t;

/* Measurement data */
typedef struct {
    int16_t temp_c;      // Temperature in Celsius * 100
    int16_t temp_f;      // Temperature in Fahrenheit * 100
    uint16_t humidity;   // Relative humidity * 100
} sht4x_data_t;

/* Function prototypes */
void sht4x_init(void);
bool sht4x_measure(sht4x_precision_t precision, sht4x_data_t* data);
bool sht4x_read_serial(uint32_t* serial);
void sht4x_soft_reset(void);
int16_t sht4x_celsius_to_fahrenheit(int16_t temp_c);

#endif /* SHT4X_H */
