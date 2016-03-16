/* 
 *  Copyright (c) 2016 Robin Callender. All Rights Reserved.
 */

#ifndef BLE_EDDY_H__
#define BLE_EDDY_H__

#include <stdint.h>
#include <stdbool.h>

#include "ble.h"
#include "ble_srv_common.h"

//
//  0000XXXX-0048-4e17-9068-11873cdbadae
// 
#define EDDY_UUID_BASE {0xae, 0xad, 0xdb, 0x3c, 0x87, 0x11, 0x68, 0x90, 0x17, 0x4e, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00}
#define EDDY_UUID_SERVICE            0xfad0
#define EDDY_UUID_URL_CHAR           0xfad1



// Forward declaration of the ble_eddy_t type. 
typedef struct _ble_eddy   ble_eddy_t;

/*  eddy Service structure. 
 *  This contains various status information for the service. 
 */
typedef struct _ble_eddy {
    uint16_t                       service_handle;
    ble_gatts_char_handles_t       url_char_handles;
    uint8_t                        uuid_type;
    uint16_t                       conn_handle;  
} ble_eddy_t;


/* Eddy Service Handle */
extern ble_eddy_t  g_eddy_service;


/*  Function for initializing the Eddy Service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_eddy_init(void);

/*  Function for handling the Application's BLE Stack events.
 *  Handles all events from the BLE stack of interest to the LED Button Service.
 *
 *   param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_eddy_on_ble_evt(ble_evt_t * p_ble_evt);

/*
 * Getter functions for URL related items.
 */
char * eddy_url_str_get(void);
int    eddy_url_len_get(void);



#endif // BLE_EDDY_H__
