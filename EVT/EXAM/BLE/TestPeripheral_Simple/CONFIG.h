/********************************** (C) COPYRIGHT *******************************
 * File Name          : CONFIG.h
 * Author             : Simple Test
 * Version            : V1.0
 * Date               : 2025-12-05
 * Description        : Minimal BLE configuration
 *******************************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * CHIP Configuration
 */
#define CH57x                           1
#define CH572                           1

/*********************************************************************
 * DEBUG Configuration
 */
#define DEBUG                           1

/*********************************************************************
 * BLE LIB Configuration
 */
#include "CH572BLEPeri_LIB.h"

/* BLE stack memory size (6KB minimum for basic BLE) */
#define BLE_MEMHEAP_SIZE                (1024 * 6)

/* BLE MAC address source */
#define BLE_MAC                         TRUE

/* Connection parameters */
#define BLE_SNV                         DISABLE
#define CLK_OSC32K                      0   // 0: internal RC, 1: external crystal

/*********************************************************************
 * PERIPHERAL Role Configuration
 */
#define BLE_ROLE                        PERIPHERAL_ROLE

/*********************************************************************
 * HAL Configuration
 */
#define HAL_SLEEP                       FALSE
#define DCDC_ENABLE                     FALSE

#ifdef __cplusplus
}
#endif

#endif
