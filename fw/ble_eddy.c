/* 
 *  ble_eddy.c   BLE service and characteristics for Eddystone
 *  Copyright (c) 2016 Robin Callender. All Rights Reserved.
 */
#include <stdio.h>
#include <string.h>

#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"

#include "config.h"
#include "ble_eddy.h"
#include "dbglog.h"

static char     m_eddy_url [URL_MAX_LENGTH];

static int      m_eddy_url_len = URL_MAX_LENGTH;

/* Eddystone Service Handle */
ble_eddy_t      g_eddy_service;

/*
 *  Getter functions for URL
 */

char * eddy_url_str_get(void)
{
    return (char*) &m_eddy_url;
}

int eddy_url_len_get(void)
{
    return m_eddy_url_len;
}

/* 
 *  Function for handling the Connect event.
 *
 *  param[in]   p_eddy      eddy structure.
 *  param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_eddy_t * p_eddy, ble_evt_t * p_ble_evt)
{
    p_eddy->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/*
 *  Function for handling the Disconnect event.
 *
 *  param[in]   p_eddy      eddy structure.
 *  param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_eddy_t * p_eddy, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_eddy->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/** Function for handling write events to the URL characteristic.
 *
 *  param[in]   p_evt_write   Write event received from the BLE stack.
 */
static void on_eddy_url_write(ble_eddy_t * p_eddy, ble_gatts_evt_write_t * p_evt_write)
{
    char new_url [URL_MAX_LENGTH];

    if (p_evt_write->len < sizeof(new_url)) {

        strncpy((char*)&new_url, (char*)&p_evt_write->data, URL_MAX_LENGTH);
    }
}

/*
 *  Function for handling the Write event.
 *
 *  param[in]   p_eddy      eddy structure.
 *  param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_eddy_t * p_eddy, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t  * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    switch (p_evt_write->context.char_uuid.uuid) {

        case EDDY_UUID_URL_CHAR:
            on_eddy_url_write(p_eddy, p_evt_write);
            break;

        default:
            /* Quietly ignore these events. */
            break;
    }  
}


/*
 *  Function for handling the BLE events.
 *
 *  param[in]   p_eddy      eddy Service structure.
 *  param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_eddy_on_ble_evt(ble_eddy_t * p_eddy, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id) {

        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_eddy, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_eddy, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_eddy, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
        case BLE_GATTS_EVT_HVC:
        case BLE_GATTS_EVT_SC_CONFIRM:
        case BLE_GATTS_EVT_TIMEOUT:
        default:
            break;
    }
}

/*
 *  Function for adding the URL set/get characteristic.
 */
static uint32_t url_char_add(ble_eddy_t * p_eddy)
{
    ble_gatts_char_md_t  char_md;
    ble_gatts_attr_t     attr_char_value;
    ble_uuid_t           ble_uuid;
    ble_gatts_attr_md_t  attr_md;
    ble_gatts_char_pf_t  char_pf;
    ble_gatts_attr_md_t  desc_md;

    static const uint8_t user_desc[] = "URL";

    memset(&desc_md, 0, sizeof(desc_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
    desc_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_pf, 0, sizeof(char_pf));
    char_pf.format = BLE_GATT_CPF_FORMAT_UTF8S;  /* UTF8 String */

    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read         = 1;
    char_md.char_props.write        = 1;
    char_md.p_char_user_desc        = (uint8_t*) &user_desc;
    char_md.char_user_desc_size     = m_eddy_url_len;
    char_md.char_user_desc_max_size = URL_MAX_LENGTH;
    char_md.p_char_pf               = &char_pf;
    char_md.p_user_desc_md          = &desc_md;
    char_md.p_cccd_md               = NULL;
    char_md.p_sccd_md               = NULL;

    ble_uuid.type = p_eddy->uuid_type;
    ble_uuid.uuid = EDDY_UUID_URL_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_USER;              /* NOTE using app storage */
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = m_eddy_url_len;
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = URL_MAX_LENGTH;
    attr_char_value.p_value      = (uint8_t*) &m_eddy_url;

    return sd_ble_gatts_characteristic_add(p_eddy->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_eddy->url_char_handles);
}

/*
 *  Function for initializing eddy's BLE usage.
 */
uint32_t ble_eddy_init(ble_eddy_t * p_eddy)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    PUTS(__func__);

    /* Initialize default URL. */
    strncpy(m_eddy_url, URL_DEFAULT_STRING, URL_MAX_LENGTH);
    m_eddy_url_len = strlen(URL_DEFAULT_STRING);

    /* Initialize service structure */
    p_eddy->conn_handle    = BLE_CONN_HANDLE_INVALID;

    /* Add service */
    ble_uuid128_t base_uuid = {EDDY_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_eddy->uuid_type);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }
    
    ble_uuid.type = p_eddy->uuid_type;
    ble_uuid.uuid = EDDY_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, 
                                        &ble_uuid, 
                                        &p_eddy->service_handle);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    err_code = url_char_add(p_eddy);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    return NRF_SUCCESS;
}
