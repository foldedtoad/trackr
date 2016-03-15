/*---------------------------------------------------------------------------*/
/*  advert.h                                                                 */
/*  Copyright (c) 2015 Robin Callender. All Rights Reserved.                 */
/*---------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "nrf51.h"
#include "nrf_soc.h"
#include "softdevice_handler.h"
#include "app_scheduler.h"
#include "ble_advdata.h"
#include "ble_gap.h"

#include "config.h"
#include "ble_eddy.h"
#include "advert.h"
#include "eddystone.h"
#include "dbglog.h"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

static const ble_gap_adv_params_t   m_adv_params_connectable = {
    .type         = BLE_GAP_ADV_TYPE_ADV_IND,
    .p_peer_addr  = NULL,
    .fp           = 0,
    .p_whitelist  = NULL,
    .interval     = APP_ADV_INTERVAL,
    .timeout      = APP_ADV_TIMEOUT,
};

static const ble_gap_adv_params_t   m_adv_params_nonconnectable = {
    .type         = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND,
    .p_peer_addr  = NULL,
    .fp           = 0,
    .p_whitelist  = NULL,
    .interval     = APP_ADV_INTERVAL,
    .timeout      = 0,
};

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void advertising_connectable_init(void)
{
    uint32_t      err_code;
    uint8_t       flags;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    ble_uuid_t scan_uuids[] = {
        {EDDY_UUID_SERVICE,                   g_eddy_service.uuid_type},
        {BLE_UUID_BATTERY_SERVICE,            BLE_UUID_TYPE_BLE},
        {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE},
    };

    /* Build ADVertisement data */
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type    = BLE_ADVDATA_FULL_NAME;
    advdata.flags.size   = sizeof(flags);
    advdata.flags.p_data = &flags;

    /* Build SCAN response data */
    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(scan_uuids) / sizeof(scan_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = scan_uuids;

    /* Set ADV and SCAN data */
    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void advertising_set_device_name(void)
{
    uint32_t        err_code;
    ble_gap_addr_t  mac = { 0 };

    err_code = sd_ble_gap_address_get( &mac );
    APP_ERROR_CHECK(err_code);

    mac.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;

    PRINTF("MAC address: %d, %02X:%02X:%02X:%02X:%02X:%02X\n", mac.addr_type,
            mac.addr[5], mac.addr[4], mac.addr[3],
            mac.addr[2], mac.addr[1], mac.addr[0] );

    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &mac);
    APP_ERROR_CHECK(err_code);

#if 0
    /* Device name unique-ifier: Use lower 3 bytes of MAC as extension to device name */
    {
        char                    device_name [20];
        ble_gap_conn_sec_mode_t sec_mode;

        sprintf(device_name, "%s_%02X%02X%02X", DEVICE_NAME,
                mac.addr[2], mac.addr[1], mac.addr[0] );

        PRINTF("device name: %s\n", device_name);

        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

        err_code = sd_ble_gap_device_name_set(&sec_mode,
                                              (const uint8_t *) &device_name,
                                              strlen(device_name));
        APP_ERROR_CHECK(err_code);
    }
#endif
}

/*---------------------------------------------------------------------------*/
/*  Function for starting advertising: allow connections.                    */
/*---------------------------------------------------------------------------*/
void advertising_start_connectable(void)
{
    advertising_set_device_name();

    advertising_connectable_init();

    APP_ERROR_CHECK( sd_ble_gap_adv_start(&m_adv_params_connectable) );

    APP_ERROR_CHECK( bsp_indication_set(BSP_INDICATE_ADVERTISING) );
}

/*---------------------------------------------------------------------------*/
/*  Function for starting advertising: disallow connections.                 */
/*---------------------------------------------------------------------------*/
void advertising_start_nonconnectable(void)
{
    APP_ERROR_CHECK( bsp_indication_set(BSP_INDICATE_ADVERTISING_DONE) );

    APP_ERROR_CHECK( sd_ble_gap_adv_start(&m_adv_params_nonconnectable) );
}

/*---------------------------------------------------------------------------*/
/*  Function for initializing the Advertising functionality.                 */
/*---------------------------------------------------------------------------*/
void advertising_init(void)
{
    /* Build (refresh) all Eddystone frames. */
    eddystone_init();
}
