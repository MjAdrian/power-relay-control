#ifndef __POWER_RELAY_CONTROL_H__
#define __POWER_RELAY_CONTROL_H__

#include "pico/stdlib.h"
#include "pico-serial-message.h"

/* --------------------------------- MACROS --------------------------------- */

#define TOGGLE_PIN 26

/* ------------------------------- PROTOTYPES ------------------------------- */

void PowerPinInit(uint8_t toogle_pin);

/**
 * @brief Function that gets a message and performs the action in it. This is typically project-specific.
 * 
 * @param msg (const serial_msg_t *): pointer to message that need to be parsed.
 * @return uint8_t: error status of function.
 * @retval 0: sucess, any other number is an error.
 */
uint8_t ParseMsg(const serial_msg_t *msg);

#endif // #ifndef __POWER_RELAY_CONTROL_H__

