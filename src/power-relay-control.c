#include "power-relay-control.h"
#include "hardware/gpio.h"

/* ------------------------- LOCAL GLOBAL? VARIABLES ------------------------ */

/* ----------------------------- LOCAL FUNCTIONS ---------------------------- */

/* ---------------------------- GLOBAL FUNCTIONS ---------------------------- */

uint8_t ParseMsg(const serial_msg_t *msg) {

    if (NULL == msg) {
        return 0;
    }

    uint8_t action = msg->action;
    uint8_t length = msg->payload_len;

    bool power_val = 0;

    serial_msg_t new_msg;

    switch (action) {
        case ACT_META:
            // shouldnt have received ACT_META from computer
            SendSerialMsg(&nak_msg);
            return 0;
        case ACT_POWER_REQ:
            power_val = msg->payload[0];
            gpio_put(TOGGLE_PIN, power_val);

            if (gpio_get(TOGGLE_PIN) == power_val) {
                SendSerialMsg(&ack_msg);
            } else {
                SendSerialMsg(&nak_msg);
            }

            return 0;
        case ACT_STATUS_REQ:

            return 0;
        case ACT_STATUS_RX:
            // should never receive ACT_STATUS_RX, this is meant to be from us
            SendSerialMsg(&nak_msg);
            return 0;
        default:
            return 1;
    }
}
