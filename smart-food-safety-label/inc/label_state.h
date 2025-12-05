/**
 * @file label_state.h
 * @brief Runtime state management for Smart Food-Safety Label
 *
 * Stores all persistent runtime state including:
 * - Product information
 * - Expiration tracking
 * - Temperature history
 * - Staff initials
 * - Violation flags
 */

#ifndef LABEL_STATE_H
#define LABEL_STATE_H

#include <stdint.h>
#include <stdbool.h>

/* Product types */
typedef enum {
    PRODUCT_SALAD = 0,
    PRODUCT_MEAT,
    PRODUCT_FISH,
    PRODUCT_DAIRY,
    PRODUCT_PREPARED_FOOD,
    PRODUCT_VEGETABLES,
    PRODUCT_SAUCE,
    PRODUCT_MAX
} product_type_t;

/* Label status flags */
typedef struct {
    bool expired;
    bool out_of_fridge;
    bool temp_violation;
    bool low_battery;
} label_flags_t;

/* Runtime state */
typedef struct {
    /* Product info */
    product_type_t product_index;
    uint8_t days_shelf_life;

    /* Date tracking */
    uint32_t creation_timestamp;
    uint32_t expiry_timestamp;
    int16_t days_left;

    /* Temperature */
    int16_t current_temp_f;
    int16_t max_temp_f;
    uint32_t violation_start_time;

    /* Staff */
    char initials[2];

    /* Status flags */
    label_flags_t flags;

    /* Energy */
    uint8_t battery_level;  // 0-100%

} label_state_t;

/* Function prototypes */
void label_state_init(void);
void label_state_reset(void);
void label_state_save(void);
void label_state_load(void);
label_state_t* label_state_get(void);
void label_state_update_days_left(void);
void label_state_set_product(product_type_t product, uint8_t days);
void label_state_set_initials(char c1, char c2);
void label_state_set_temp_violation(bool violated);
void label_state_set_out_of_fridge(bool out);
void label_state_set_expired(bool expired);

#endif /* LABEL_STATE_H */
