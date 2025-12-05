/**
 * @file ble_label.h
 * @brief BLE advertising and provisioning for food label
 *
 * Handles:
 * - Advertising payload with compressed status
 * - Provisioning interface
 * - History sync
 * - Configuration updates
 */

#ifndef BLE_LABEL_H
#define BLE_LABEL_H

#include <stdint.h>
#include <stdbool.h>

/* BLE advertising packet structure */
typedef struct __attribute__((packed)) {
    uint8_t product_index;
    uint8_t days_left;
    uint8_t temp_f;
    uint8_t status_flags;
    uint8_t energy_level;
} ble_adv_payload_t;

/* Status flag bits */
#define BLE_FLAG_EXPIRED        (1 << 0)
#define BLE_FLAG_OUT_OF_FRIDGE  (1 << 1)
#define BLE_FLAG_VIOLATION      (1 << 2)
#define BLE_FLAG_LOW_BATTERY    (1 << 3)

/* Provisioning data structure */
typedef struct {
    uint8_t product_index;
    uint8_t default_days;
    int16_t max_temp_f;
    char initials[2];
} ble_provision_data_t;

/* Function prototypes */
void ble_label_init(void);
void ble_label_start_advertising(void);
void ble_label_stop_advertising(void);
void ble_label_update_adv_data(void);
void ble_label_enter_provisioning_mode(void);
void ble_label_exit_provisioning_mode(void);
bool ble_label_is_connected(void);
void ble_label_send_history(void);
void ble_label_handle_provision_data(ble_provision_data_t* data);

#endif /* BLE_LABEL_H */
