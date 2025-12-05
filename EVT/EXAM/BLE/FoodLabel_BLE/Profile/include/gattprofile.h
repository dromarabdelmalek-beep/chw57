/********************************** (C) COPYRIGHT *******************************
 * File Name          : gattprofile.h
 * Author             : WCH (Modified for Food Label)
 * Version            : V1.0
 * Date               : 2025-12-05
 * Description        : GATT profile header (simplified for beacon mode)
 *******************************************************************************/

#ifndef GATTPROFILE_H
#define GATTPROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Food Label Service UUID: 0xFFF0 (same as SimpleProfile for compatibility)
#define FOODLABEL_SERV_UUID                   0xFFF0

// Characteristic UUIDs
#define FOODLABEL_TEMP_UUID                   0xFFF1  // Temperature characteristic
#define FOODLABEL_STATUS_UUID                 0xFFF2  // Status characteristic

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * FUNCTIONS
 */

// Note: For beacon-only mode, we don't need GATT services
// This header is provided for future expansion if GATT connectivity is needed

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GATTPROFILE_H */
