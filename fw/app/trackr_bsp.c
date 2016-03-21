/* 
 *  trackr_bsp.c
 *  Copyright (c) 2016 Robin Callender. All Rights Reserved
 */
#include "config.h"

#include <stddef.h>
#include <stdio.h>

#include "nordic_common.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_error.h"

#include "app_timer.h"
#include "app_gpiote.h"
#include "app_button.h"
#include "dbglog.h"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define ADVERTISING_LED_ON_INTERVAL            200
#define ADVERTISING_LED_OFF_INTERVAL           1800

#define ADVERTISING_DIRECTED_LED_ON_INTERVAL   200
#define ADVERTISING_DIRECTED_LED_OFF_INTERVAL  200

#define ADVERTISING_WHITELIST_LED_ON_INTERVAL  200
#define ADVERTISING_WHITELIST_LED_OFF_INTERVAL 800

#define ADVERTISING_SLOW_LED_ON_INTERVAL       400
#define ADVERTISING_SLOW_LED_OFF_INTERVAL      4000

#define BONDING_INTERVAL                       100

#define SENT_OK_INTERVAL                       100
#define SEND_ERROR_INTERVAL                    500

#define RCV_OK_INTERVAL                        100
#define RCV_ERROR_INTERVAL                     500

#define ALERT_INTERVAL                         200

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

static bsp_indication_t m_stable_state        = BSP_INDICATE_IDLE;
static uint32_t         m_app_ticks_per_100ms = 0;
static uint32_t         m_indication_type     = 0;
static app_timer_id_t   m_leds_timer_id;

static bsp_event_callback_t m_registered_callback         = NULL;
static bsp_event_t          m_events_list[BUTTONS_NUMBER] = {BSP_EVENT_NOTHING};

static void bsp_button_event_handler(uint8_t pin_no, uint8_t button_action);

static const uint32_t m_buttons_list[BUTTONS_NUMBER] = BUTTONS_LIST;

static const app_button_cfg_t app_buttons[BUTTONS_NUMBER] =
{
    {BSP_BUTTON_0, false, BUTTON_PULL, bsp_button_event_handler},
};

#define BSP_MS_TO_TICK(MS) (m_app_ticks_per_100ms * (MS / 100))

#define ALERT_LED_MASK BSP_LED_0_MASK

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_buttons_state_get(uint32_t * p_buttons_state)
{
    uint32_t result = NRF_SUCCESS;

    *p_buttons_state = 0;

    uint32_t buttons = ~NRF_GPIO->IN;
    uint32_t cnt;

    for (cnt = 0; cnt < BUTTONS_NUMBER; cnt++) {
        if (buttons & (1 << m_buttons_list[cnt])) {
            *p_buttons_state |= 1 << cnt;
        }
    }

    return result;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_button_is_pressed(uint32_t button, bool * p_state)
{
    uint32_t buttons = ~NRF_GPIO->IN;

    *p_state = (buttons & (1 << m_buttons_list[button])) ? true : false;
    
    return NRF_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void bsp_button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    bsp_event_t event  = BSP_EVENT_NOTHING;
    uint32_t    button = 0;

    if ((button_action == APP_BUTTON_PUSH) && (m_registered_callback != NULL)) {
        while (m_buttons_list[button] != pin_no) {
            button++;
        }

        if (button < BUTTONS_NUMBER) {
            event = m_events_list[button];
        }

        if (event != BSP_EVENT_NOTHING) {
            m_registered_callback(event);
        }
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static uint32_t bsp_led_indication(bsp_indication_t indicate)
{
    uint32_t err_code   = NRF_SUCCESS;
    uint32_t next_delay = 0;

    switch (indicate) {

        case BSP_INDICATE_IDLE:
            LEDS_OFF(LEDS_MASK & ~ALERT_LED_MASK);
            m_stable_state = indicate;
            break;

        case BSP_INDICATE_ADVERTISING:
            LEDS_OFF(LEDS_MASK & ~BSP_LED_0_MASK & ~ALERT_LED_MASK);
            if (LED_IS_ON(BSP_LED_0_MASK)) {
                LEDS_OFF(BSP_LED_0_MASK);
                next_delay = indicate ==
                             BSP_INDICATE_ADVERTISING ? ADVERTISING_LED_OFF_INTERVAL :
                             ADVERTISING_SLOW_LED_OFF_INTERVAL;
            }
            else {
                LEDS_ON(BSP_LED_0_MASK);
                next_delay = indicate ==
                             BSP_INDICATE_ADVERTISING ? ADVERTISING_LED_ON_INTERVAL :
                             ADVERTISING_SLOW_LED_ON_INTERVAL;
            }
            m_stable_state = indicate;
            err_code = app_timer_start(m_leds_timer_id, BSP_MS_TO_TICK(next_delay), NULL);
            break;

        case BSP_INDICATE_ADVERTISING_DONE:
            LEDS_OFF(BSP_LED_0_MASK);
            m_stable_state = indicate;
            err_code = app_timer_stop(m_leds_timer_id);
            break;

        case BSP_INDICATE_CONNECTED:
            LEDS_OFF(LEDS_MASK & ~BSP_LED_0_MASK & ~ALERT_LED_MASK);
            LEDS_ON(BSP_LED_0_MASK);
            m_stable_state = indicate;
            break;

        case BSP_INDICATE_USER_STATE_ON:
            LEDS_OFF(LEDS_MASK & ~BSP_LED_0_MASK & ~ALERT_LED_MASK);
            LEDS_ON(BSP_LED_0_MASK);
            m_stable_state = indicate;
            break;

        case BSP_INDICATE_USER_STATE_OFF:
            LEDS_OFF(BSP_LED_0_MASK);
            m_stable_state = indicate;
            break;

        case BSP_INDICATE_SENT_OK:
        case BSP_INDICATE_SEND_ERROR:
        case BSP_INDICATE_RCV_OK:
        case BSP_INDICATE_RCV_ERROR:
        case BSP_INDICATE_FATAL_ERROR:
            break;

        default:
            break;
    }

    return err_code;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void leds_timer_handler(void * p_context)
{
    if (m_indication_type & BSP_INIT_LED) {
        bsp_led_indication(m_stable_state);
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_indication_set(bsp_indication_t indicate)
{
    uint32_t err_code = NRF_SUCCESS;

    if (m_indication_type & BSP_INIT_LED) {
        err_code = bsp_led_indication(indicate);
    }

    return err_code;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_indication_text_set(bsp_indication_t indicate, char const * p_text)
{
    return bsp_indication_set(indicate);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_init(uint32_t type, uint32_t ticks_per_100ms, 
                  bsp_event_callback_t callback)
{
    uint32_t err_code = NRF_SUCCESS;

    m_app_ticks_per_100ms = ticks_per_100ms;
    m_indication_type     = type;

    m_registered_callback = callback;

    if (type & BSP_INIT_BUTTON) {
        uint32_t button_id;

        for (button_id = 0; (button_id < BUTTONS_NUMBER); button_id++) {
            bsp_event_t event_id;

            event_id = (bsp_event_t)(BSP_EVENT_KEY_0 + button_id);

            err_code = bsp_event_to_button_assign( button_id, event_id );
            if (err_code != NRF_SUCCESS) return err_code;
        }

        APP_BUTTON_INIT((app_button_cfg_t *)app_buttons, 
                         BUTTONS_NUMBER,
                         ticks_per_100ms / 2,
                         false);

        err_code = app_button_enable();
    }

    if (type & BSP_INIT_LED) {
        NRF_GPIO->DIRCLR = BUTTONS_MASK;
        LEDS_OFF(LEDS_MASK);
        NRF_GPIO->DIRSET = LEDS_MASK;
    }

    if (err_code == NRF_SUCCESS) {
        err_code = app_timer_create(&m_leds_timer_id, 
                                    APP_TIMER_MODE_SINGLE_SHOT,
                                    leds_timer_handler);
    }

    return err_code;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_event_to_button_assign(uint32_t button, bsp_event_t event)
{
    uint32_t err_code = NRF_SUCCESS;

    if (button < BUTTONS_NUMBER) {
        m_events_list[button] = event;
    }
    else {
        err_code = NRF_ERROR_INVALID_PARAM;
    }

    return err_code;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
uint32_t bsp_buttons_enable(uint32_t buttons)
{
    uint32_t button_no;
    uint32_t pin_no;

    for (button_no = 0; button_no < BUTTONS_NUMBER; button_no++) {
        pin_no = m_buttons_list[button_no];

        if (buttons & (1 << button_no)) {
            NRF_GPIO->PIN_CNF[pin_no] &= ~GPIO_PIN_CNF_SENSE_Msk;
            NRF_GPIO->PIN_CNF[pin_no] |= GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos;
        }
        else {
            NRF_GPIO->PIN_CNF[pin_no] &= ~GPIO_PIN_CNF_SENSE_Msk;
            NRF_GPIO->PIN_CNF[pin_no] |= GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos;
        }
    }

    return NRF_SUCCESS;
}
