/*
 *  trackr_bsp.h
 *  Copyright (c) 2016 Robin Callender. All Rights Reserved.
 */
#ifndef TRACKR_BSP_H_
#define TRACKR_BSP_H_

#include <stdint.h>
#include <stdbool.h>

#include "trackr_board.h"

#define BUTTON_ERASE_BONDING BSP_BUTTON_0_MASK
#define BUTTON_ERASE_ALL     BSP_BUTTON_1_MASK
#define BUTTON_ADVERTISE     BSP_BUTTON_0_MASK
#define BUTTON_CLEAR_EVT     BSP_BUTTON_1_MASK
#define BUTTON_CAPSLOCK      BSP_BUTTON_2_MASK
#define BSP_BUTTONS_ALL      0xFFFFFFFF
#define BSP_BUTTONS_NONE     0

#define BSP_APP_TIMERS_NUMBER 2

#define BSP_INIT_NONE    0
#define BSP_INIT_LED     (1 << 0)
#define BSP_INIT_BUTTON  (1 << 1)

typedef enum {
    BSP_INDICATE_FIRST = 0,
    BSP_INDICATE_IDLE  = BSP_INDICATE_FIRST,
    BSP_INDICATE_SCANNING,
    BSP_INDICATE_ADVERTISING,
    BSP_INDICATE_ADVERTISING_WHITELIST,
    BSP_INDICATE_ADVERTISING_SLOW,
    BSP_INDICATE_ADVERTISING_DIRECTED,
    BSP_INDICATE_ADVERTISING_DONE,
    BSP_INDICATE_BONDING,
    BSP_INDICATE_CONNECTED,
    BSP_INDICATE_SENT_OK,
    BSP_INDICATE_SEND_ERROR,
    BSP_INDICATE_RCV_OK,
    BSP_INDICATE_RCV_ERROR,
    BSP_INDICATE_FATAL_ERROR,
    BSP_INDICATE_ALERT_0,
    BSP_INDICATE_ALERT_1,
    BSP_INDICATE_ALERT_2,
    BSP_INDICATE_ALERT_3,
    BSP_INDICATE_ALERT_OFF,
    BSP_INDICATE_USER_STATE_OFF,
    BSP_INDICATE_USER_STATE_0,
    BSP_INDICATE_USER_STATE_1,
    BSP_INDICATE_USER_STATE_2,
    BSP_INDICATE_USER_STATE_3,
    BSP_INDICATE_USER_STATE_ON,
    BSP_INDICATE_LAST = BSP_INDICATE_USER_STATE_ON
} bsp_indication_t;


typedef enum {
    BSP_EVENT_NOTHING = 0,
    BSP_EVENT_CLEAR_BONDING_DATA,
    BSP_EVENT_CLEAR_ALERT,
    BSP_EVENT_DISCONNECT,
    BSP_EVENT_ADVERTISING_START,
    BSP_EVENT_ADVERTISING_STOP,
    BSP_EVENT_BOND,
    BSP_EVENT_RESET,
    BSP_EVENT_SLEEP,
    BSP_EVENT_WAKEUP,
    BSP_EVENT_DFU,
    BSP_EVENT_KEY_0,
    BSP_EVENT_KEY_1,
    BSP_EVENT_KEY_2,
    BSP_EVENT_KEY_3,
    BSP_EVENT_KEY_4,
    BSP_EVENT_KEY_5,
    BSP_EVENT_KEY_6,
    BSP_EVENT_KEY_7,
    BSP_EVENT_KEY_LAST = BSP_EVENT_KEY_7,
} bsp_event_t;

typedef void (* bsp_event_callback_t)(bsp_event_t);

uint32_t bsp_init(uint32_t type, uint32_t ticks_per_100ms, bsp_event_callback_t callback);
uint32_t bsp_buttons_state_get(uint32_t * p_buttons_state);
uint32_t bsp_button_is_pressed(uint32_t button, bool * p_state);
uint32_t bsp_event_to_button_assign(uint32_t button, bsp_event_t event);
uint32_t bsp_indication_set(bsp_indication_t indicate);
uint32_t bsp_indication_text_set(bsp_indication_t indicate, const char * p_text);
uint32_t bsp_buttons_enable(uint32_t buttons);

#endif /* TRACKR_BSP_H_ */
