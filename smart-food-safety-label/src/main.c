/**
 * @file main.c
 * @brief Main entry point for Smart Food-Safety Label
 *
 * Initializes all subsystems and enters main application loop
 */

#include "CH57x_common.h"
#include "label_app.h"
#include "label_state.h"
#include "label_config.h"
#include "ui_epaper.h"
#include "buttons.h"
#include "ws2812.h"
#include "sht4x.h"

#ifdef ENABLE_BLE
#include "ble_label.h"
#endif

/*********************************************************************
 * @fn      main
 * @brief   Main program entry point
 * @return  Does not return
 */
int main(void)
{
    /* System initialization */
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    /* Initialize TMOS */
    TMOS_Init();

    /* Initialize hardware peripherals */
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);  // Default all pins to input with pull-up

    /* Initialize application modules */
    label_config_init();
    label_state_init();
    buttons_init();
    ws2812_init();
    epaper_init();
    sht4x_init();

#ifdef ENABLE_BLE
    ble_label_init();
#endif

    /* Initialize application logic */
    label_app_init();

    /* Display startup screen */
    epaper_clear();
    epaper_display_main_screen();
    epaper_refresh_full();

    /* Set initial LED status */
    ws2812_set_status(LED_STATUS_SAFE);

    /* Enter main loop */
    while(1)
    {
        TMOS_SystemProcess();  // Process TMOS events
        label_app_main_loop(); // Run application logic
    }
}

/*********************************************************************
 * @fn      HAL_ProcessPoll
 * @brief   HAL polling process - called periodically by TMOS
 */
tmosTaskID HAL_ProcessPoll(tmosTaskID task_id, tmosEvents events)
{
    /* Scan buttons */
    buttons_scan();

    /* Update LED state */
    ws2812_update();

    return 0;
}
