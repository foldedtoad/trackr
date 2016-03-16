/*---------------------------------------------------------------------------*/
/*  config.h                                                                 */
/*  Copyright (c) 2015 Robin Callender. All Rights Reserved.                 */
/*---------------------------------------------------------------------------*/

#ifndef _CONFIG_H__
#define _CONFIG_H__

#include <stdint.h>

#include "app_timer.h"
#include "app_scheduler.h"
#include "trackr_bsp.h"

/* 
 *  Include or not the service_changed characteristic. 
 *  if not enabled, the server's database cannot be changed for 
 *  the lifetime of the device
 */
#define IS_SRVC_CHANGED_CHARACT_PRESENT 1

/*
 *  BLE advertised Device Name
 */
 #define DEVICE_NAME                    "Eddy"

/*
 *  GAP parameters
 */
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)
#define SLAVE_LATENCY                   0 
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)

/* 
 *  Time from initiating event (connect or start of notification) to first 
 *  time sd_ble_gap_conn_param_update is called (15 seconds).
 */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(20000, APP_TIMER_PRESCALER)

/* 
 *  Time between each call to sd_ble_gap_conn_param_update 
 *  after the first call (5 seconds).
 */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER) 

/* 
 *  Number of attempts before giving up the connection parameter negotiation.
 */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3

/*
 *  Security Parameters
 */
#define SEC_PARAM_TIMEOUT               30 
#define SEC_PARAM_BOND                  1 
#define SEC_PARAM_MITM                  0
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE
#define SEC_PARAM_OOB                   0 
#define SEC_PARAM_MIN_KEY_SIZE          7 
#define SEC_PARAM_MAX_KEY_SIZE          16

/* 
 *  The advertising interval for advertisement (100 ms).
 *  This value can vary between 100ms to 10.24s). 
 */
#define APP_ADV_TIMEOUT                 30
#define APP_ADV_INTERVAL_MS             100
#define APP_ADV_INTERVAL                MSEC_TO_UNITS(APP_ADV_INTERVAL_MS, UNIT_0_625_MS)

/*
 *  Timer parameters
 */
#define APP_TIMER_PRESCALER             0
#define APP_TIMER_MAX_TIMERS            4
#define APP_TIMER_OP_QUEUE_SIZE         10

/* 
 *  Maximum size of scheduler events. 
 *  Note that scheduler BLE stack events do not contain any data, as the 
 *  events are being pulled from the stack in the event handler.
 */
#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)

/* 
 *  Maximum number of events in the scheduler queue.
 */
#define SCHED_QUEUE_SIZE                10

/*
 *  The Beacon's measured RSSI at 1 meter distance in dBm.
 */
#define APP_MEASURED_RSSI               0xC3

#define VBAT_MAX_IN_MV                  3300

#define EDDYSTONE_UID                   0
#define EDDYSTONE_URL                   1
#define EDDYSTONE_TLM                   2

/* 
 *  Handle of first application specific service when when 
 *  service changed characteristic is present.
 */
#define APP_SERVICE_HANDLE_START        0x000C

/* 
 *  Max handle value in BLE.
 */
#define BLE_HANDLE_MAX                  0xFFFF

/* 
 *  Go to https://goo.gl for utility to shortening URL names.
 */
#define URL_MAX_LENGTH                  17
#define URL_DEFAULT_STRING              "goo.gl/CDgVgI"
#define URL_LENGTH                      (sizeof(URL_STRING) - 1)

/*
 *  73156B80-12AA-4E56-91A3-24C06CC3285F  UUID  (generated with uuidgen)
 *  73156B8024C06CC3285F                  Namespace
 */
#define UID_NAMESPACE                   {0x73,0x15,0x6B,0x80,0x24,0xC0,0x6C,0xC3,0x28,0x5F}

/*
 *  Misc values
 */
#define DEAD_BEEF                       0xDEADBEEF 

#endif /* _CONFIG_H__ */
