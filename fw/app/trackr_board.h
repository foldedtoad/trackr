/* 
 *  trackr_board.h    hardware layout for trackr device
 *  Copyright (c) 2016 Robin Callender. All Rights Reserved.
 */
#ifndef TRACKR_BOARD_H
#define TRACKR_BOARD_H
 
#include "nrf51_pins.h"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define TRACKR_RX              P0_10
#define TRACKR_BUTTON          P0_17
#define TRACKR_LED             P0_19
#define TRACKR_TX              P0_23


/* NOTE: Subverting the buzzer GPIOs to be UART GPIOs */

//#define TRACKR_BUZZER_R        P0_10
//#define TRACKR_BUZZER_L        P0_23

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define TX_PIN_NUMBER       TRACKR_TX
#define RX_PIN_NUMBER       TRACKR_RX

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define LEDS_NUMBER         1

#define LED_BLUE            TRACKR_LED
#define BSP_LED_0           LED_BLUE

#define BSP_LED_0_MASK      (1 << BSP_LED_0)
#define LEDS_MASK           (BSP_LED_0_MASK)
#define LEDS_INV_MASK       0x00000000

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define BUTTONS_NUMBER      1

#define BUTTON_0            TRACKR_BUTTON
#define BSP_BUTTON_0        BUTTON_0

#define BUTTON_PULL         NRF_GPIO_PIN_PULLUP
#define BSP_BUTTON_0_MASK   (1 << BUTTON_0)
#define BUTTONS_MASK        (BSP_BUTTON_0_MASK)
#define BUTTONS_LIST        {BUTTON_0}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define LEDS_OFF(leds_mask) \
    do { NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK);  \
         NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); \
    } while (0)

#define LEDS_ON(leds_mask) \
    do {  NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK);  \
          NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); \
    } while (0)

#define LED_IS_ON(leds_mask) ((leds_mask) & (NRF_GPIO->OUT ^ LEDS_INV_MASK) )

#define LEDS_INVERT(leds_mask) \
    do { uint32_t gpio_state = NRF_GPIO->OUT;            \
         NRF_GPIO->OUTSET = ((leds_mask) & ~gpio_state); \
         NRF_GPIO->OUTCLR = ((leds_mask) & gpio_state);  \
    } while (0)

#define LEDS_CONFIGURE(leds_mask) \
    do { uint32_t pin;                        \
        for (pin = 0; pin < 32; pin++)        \
            if ( (leds_mask) & (1 << pin) )   \
                nrf_gpio_cfg_output(pin);     \
    } while (0)


#endif /* TRACKR_BOARD_H */