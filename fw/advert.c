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

static const char * device_name = DEVICE_NAME;

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

    ble_gap_addr_t  mac = { 0 };

    flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    ble_uuid_t scan_uuids[] = {
        {EDDY_UUID_SERVICE,                   g_eddy_service.uuid_type},
        {BLE_UUID_BATTERY_SERVICE,            BLE_UUID_TYPE_BLE},
        {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE},
    };

        /* Set the device name */
    {
        ble_gap_conn_sec_mode_t sec_mode;

        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

        PRINTF("name: \"%s\"\n", device_name);

        err_code = sd_ble_gap_device_name_set(&sec_mode,
                                              (const uint8_t *) device_name,
                                              strlen(device_name));
        APP_ERROR_CHECK(err_code);
    }

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

    /* Set MAC address */
    err_code = sd_ble_gap_address_get( &mac );
    APP_ERROR_CHECK(err_code);

    mac.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;

    PRINTF("MAC address: %d, %02X:%02X:%02X:%02X:%02X:%02X\n", mac.addr_type,
            mac.addr[5], mac.addr[4], mac.addr[3],
            mac.addr[2], mac.addr[1], mac.addr[0] );

    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &mac);
    APP_ERROR_CHECK(err_code);
}

/*---------------------------------------------------------------------------*/
/*  Function for starting advertising: allow connections.                    */
/*---------------------------------------------------------------------------*/
void advertising_start_connectable(void)
{
    PUTS(__func__);

    advertising_connectable_init();

    APP_ERROR_CHECK( sd_ble_gap_adv_start(&m_adv_params_connectable) );

    APP_ERROR_CHECK( bsp_indication_set(BSP_INDICATE_ADVERTISING) );
}

/*---------------------------------------------------------------------------*/
/*  Function for starting advertising: disallow connections.                 */
/*---------------------------------------------------------------------------*/
void advertising_start_nonconnectable(void)
{
    PUTS(__func__);

    eddystone_init();

    APP_ERROR_CHECK( bsp_indication_set(BSP_INDICATE_ADVERTISING_DONE) );

    APP_ERROR_CHECK( sd_ble_gap_adv_start(&m_adv_params_nonconnectable) );
}

/*---------------------------------------------------------------------------*/
/*  Function for initializing the Advertising functionality.                 */
/*---------------------------------------------------------------------------*/
void advertising_init(void)
{
    /* TBD */
}
