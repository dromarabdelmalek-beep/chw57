/**
 * @file label_app.h
 * @brief Main application state machine and logic
 *
 * Implements the core application behavior:
 * - Normal operation mode
 * - Edit mode
 * - Out-of-fridge mode
 * - Temperature monitoring
 * - Expiration tracking
 */

#ifndef LABEL_APP_H
#define LABEL_APP_H

#include <stdint.h>
#include <stdbool.h>

/* Application states */
typedef enum {
    APP_STATE_INIT = 0,
    APP_STATE_NORMAL,
    APP_STATE_EDIT_PRODUCT,
    APP_STATE_EDIT_DAYS,
    APP_STATE_EDIT_INITIAL1,
    APP_STATE_EDIT_INITIAL2,
    APP_STATE_EDIT_CONFIRM,
    APP_STATE_OUT_OF_FRIDGE,
    APP_STATE_EXPIRED,
    APP_STATE_VIOLATION,
    APP_STATE_LOW_POWER
} app_state_t;

/* Edit mode field */
typedef enum {
    EDIT_FIELD_PRODUCT = 0,
    EDIT_FIELD_DAYS,
    EDIT_FIELD_INITIAL1,
    EDIT_FIELD_INITIAL2,
    EDIT_FIELD_DONE
} edit_field_t;

/* Function prototypes */
void label_app_init(void);
void label_app_main_loop(void);
void label_app_process_event(void);
void label_app_handle_button_event(uint8_t button_id, bool long_press);
void label_app_enter_edit_mode(void);
void label_app_exit_edit_mode(bool save);
void label_app_toggle_fridge_mode(void);
void label_app_update_temperature(int16_t temp_f);
void label_app_update_expiration(void);
void label_app_check_violations(void);
app_state_t label_app_get_state(void);

#endif /* LABEL_APP_H */
