/**
 * @file ui_epaper.h
 * @brief E-paper display driver and UI rendering
 *
 * Graphics and rendering to GDEY029T94 2.9" e-paper display
 * - Full and partial refresh support
 * - Layout rendering
 * - Icon and text drawing
 */

#ifndef UI_EPAPER_H
#define UI_EPAPER_H

#include <stdint.h>
#include <stdbool.h>

/* Display dimensions */
#define EPAPER_WIDTH    296
#define EPAPER_HEIGHT   128

/* Display colors */
#define COLOR_WHITE     0xFF
#define COLOR_BLACK     0x00

/* UI element positions */
#define TEMP_BOX_X      240
#define TEMP_BOX_Y      10
#define TEMP_BOX_W      50
#define TEMP_BOX_H      30

/* Function prototypes */
void epaper_init(void);
void epaper_power_on(void);
void epaper_power_off(void);
void epaper_clear(void);
void epaper_refresh_full(void);
void epaper_refresh_partial(void);
void epaper_sleep(void);

/* High-level UI functions */
void epaper_display_main_screen(void);
void epaper_display_edit_screen(uint8_t field_index);
void epaper_display_expired_screen(void);
void epaper_display_violation_screen(void);

/* Drawing primitives */
void epaper_draw_text(uint16_t x, uint16_t y, const char* text, uint8_t size);
void epaper_draw_number(uint16_t x, uint16_t y, int32_t number, uint8_t size);
void epaper_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool filled);
void epaper_draw_icon_fridge(uint16_t x, uint16_t y, bool out_of_fridge);
void epaper_draw_icon_warning(uint16_t x, uint16_t y);
void epaper_draw_icon_expired(uint16_t x, uint16_t y);

/* Field update functions (for partial refresh) */
void epaper_update_product_name(const char* name);
void epaper_update_days_left(int16_t days);
void epaper_update_temperature(int16_t temp_f);
void epaper_update_initials(char c1, char c2);
void epaper_update_dates(uint32_t prep_date, uint32_t expiry_date);

#endif /* UI_EPAPER_H */
