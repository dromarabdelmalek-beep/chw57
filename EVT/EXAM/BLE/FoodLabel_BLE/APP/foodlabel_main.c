/********************************** (C) COPYRIGHT *******************************
 * File Name          : foodlabel_main.c
 * Author             : Converted from ESP32-C3 to CH57x
 * Version            : V1.0
 * Date               : 2025-12-05
 * Description        : Smart Food Label - Advanced Cold Chain Monitoring
 *                      CH57x + 2.9" E-Paper + SHT4x Sensor + BLE Broadcast
 *********************************************************************************
 * Features:
 * - Auto-calculate expiry dates
 * - Low power sleep mode (wake every 10 min)
 * - Cold chain temperature monitoring
 * - Waste alert system
 * - BLE broadcast of temperature and status
 *******************************************************************************/

#include "CONFIG.h"
#include "HAL.h"
#include "foodlabel.h"
#include "gattprofile.h"
#include "CH57x_common.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0xF0, 0x0D};  // Custom MAC for Food Label
#endif

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   Main event loop
 *
 * @return  none
 */
__HIGH_CODE
__attribute__((noinline))
void Main_Circulation()
{
    while(1)
    {
        TMOS_SystemProcess();
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main entry point
 *
 * @return  none
 */
int main(void)
{
    // Disable debug interface
    R16_PIN_ALTERNATE &= ~RB_PIN_DEBUG_EN;

    // Configure HSE capacitance and set system clock to 100MHz
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);

    // Configure all GPIOs as input with pull-up for sleep mode
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif

#ifdef DEBUG
    // Configure UART for debugging
    GPIOA_SetBits(bTXD_0);
    GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA);
    UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
    UART_DefInit();
#endif

    PRINT("\n=== SmartFood Cold Chain Monitor ===\n");
    PRINT("%s\n", VER_LIB);

    // Initialize BLE stack
    CH57x_BLEInit();

    // Initialize HAL (RTC, timers, etc.)
    HAL_Init();

    // Initialize GAP Role as Peripheral
    GAPRole_PeripheralInit();

    // Initialize Food Label application
    FoodLabel_Init();

    // Enter main event loop
    Main_Circulation();

    return 0;
}

/******************************** endfile @ main ******************************/
