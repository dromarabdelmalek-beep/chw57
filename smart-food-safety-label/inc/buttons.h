/**
 * @file buttons.h
 * @brief Button input handling with debouncing
 *
 * Debounced button scanning and short/long press event generation
 * Supports 3 buttons: EDIT, UP, DOWN
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>
#include <stdbool.h>

/* Button IDs */
typedef enum {
    BUTTON_EDIT = 0,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_COUNT
} button_id_t;

/* Button events */
typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_RELEASE
} button_event_t;

/* Button state */
typedef struct {
    bool pressed;
    bool prev_state;
    uint32_t press_start_time;
    uint32_t debounce_time;
    button_event_t pending_event;
} button_state_t;

/* Button callback function type */
typedef void (*button_callback_t)(button_id_t button, button_event_t event);

/* Function prototypes */
void buttons_init(void);
void buttons_scan(void);
void buttons_register_callback(button_callback_t callback);
button_event_t buttons_get_event(button_id_t button);
bool buttons_is_pressed(button_id_t button);
void buttons_clear_event(button_id_t button);

#endif /* BUTTONS_H */
