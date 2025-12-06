/********************************** (C) COPYRIGHT *******************************
 * File Name          : CONFIG.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2025-12-05
 * Description        : Configuration file for Food Label BLE application
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

/*********************************************************************
 * CHIP CONFIGURATION
 */
#define CHIP_ID                             ID_CH572

/*********************************************************************
 * BLE LIBRARY
 */
#ifdef CH57xBLE_ROM
#include "CH57xBLE_ROM.h"
#else
#include "CH572BLEPeri_LIB.h"
#endif

#include "CH57x_common.h"

/*********************************************************************
 * APPLICATION CONFIGURATION
 */

// MAC Address Configuration
#ifndef BLE_MAC
#define BLE_MAC                             TRUE    // Use custom MAC address
#endif

// Sleep Configuration
#ifndef HAL_SLEEP
#define HAL_SLEEP                           FALSE   // Disable sleep for now (TODO: enable for production)
#endif

#ifndef SLEEP_RTC_MIN_TIME
#define SLEEP_RTC_MIN_TIME                  US_TO_RTC(1000)
#endif

#ifndef SLEEP_RTC_MAX_TIME
#define SLEEP_RTC_MAX_TIME                  (RTC_MAX_COUNT - 1000 * 1000 * 30)
#endif

#ifndef WAKE_UP_RTC_MAX_TIME
#define WAKE_UP_RTC_MAX_TIME                US_TO_RTC(1600)
#endif

// Peripheral Configuration
#ifndef HAL_KEY
#define HAL_KEY                             FALSE
#endif

#ifndef HAL_LED
#define HAL_LED                             FALSE
#endif

// Temperature Calibration
#ifndef TEM_SAMPLE
#define TEM_SAMPLE                          TRUE
#endif

// BLE Calibration
#ifndef BLE_CALIBRATION_ENABLE
#define BLE_CALIBRATION_ENABLE              TRUE
#endif

#ifndef BLE_CALIBRATION_PERIOD
#define BLE_CALIBRATION_PERIOD              120000
#endif

// SNV (Storage Non-Volatile)
#ifndef BLE_SNV
#define BLE_SNV                             TRUE
#endif

#ifndef BLE_SNV_ADDR
#define BLE_SNV_ADDR                        0x77E00
#endif

#ifndef BLE_SNV_BLOCK
#define BLE_SNV_BLOCK                       256
#endif

#ifndef BLE_SNV_NUM
#define BLE_SNV_NUM                         1
#endif

// RTC Configuration
#ifndef CLK_OSC32K
#define CLK_OSC32K                          1       // Internal 32KHz
#endif

// Memory Configuration
#ifndef BLE_MEMHEAP_SIZE
#define BLE_MEMHEAP_SIZE                    (1024*6)
#endif

// Data Buffer Configuration
#ifndef BLE_BUFF_MAX_LEN
#define BLE_BUFF_MAX_LEN                    27
#endif

#ifndef BLE_BUFF_NUM
#define BLE_BUFF_NUM                        5
#endif

#ifndef BLE_TX_NUM_EVENT
#define BLE_TX_NUM_EVENT                    1
#endif

#ifndef BLE_TX_POWER
#define BLE_TX_POWER                        LL_TX_POWEER_0_DBM
#endif

// Connection Configuration
#ifndef PERIPHERAL_MAX_CONNECTION
#define PERIPHERAL_MAX_CONNECTION           1
#endif

#ifndef CENTRAL_MAX_CONNECTION
#define CENTRAL_MAX_CONNECTION              0       // Not used in this application
#endif

/*********************************************************************
 * DEBUG CONFIGURATION
 */
#ifndef DEBUG
#define DEBUG                               1       // Enable debug output
#endif

/*********************************************************************
 * CONVERSION MACROS
 */
#define US_TO_RTC(us)                       ((us) * 32 / 1000)  // Microseconds to RTC ticks
#define MS1_TO_SYSTEM_TIME(ms)              ((ms) * 1600)        // Milliseconds to TMOS system time

#endif /* __CONFIG_H */
