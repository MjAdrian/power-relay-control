#include "power-relay-control.h"
#include "hardware/gpio.h"

/* ------------------------- LOCAL GLOBAL? VARIABLES ------------------------ */

/* ----------------------------- LOCAL FUNCTIONS ---------------------------- */

/* ---------------------------- GLOBAL FUNCTIONS ---------------------------- */

void PowerPinInit(uint8_t toogle_pin) {
    gpio_init(toogle_pin);
    gpio_set_dir(toogle_pin, GPIO_OUT);
}

uint8_t ParseMsg(const serial_msg_t *msg) {

    if (NULL == msg) {
        return 0;
    }

    uint8_t action = msg->action;
    uint8_t length = msg->payload_len;

    bool power_val = 0;

    serial_msg_t new_msg = {.sync.value = SYNC_VALUE};

    switch (action) {
        case ACT_META:
            // shouldnt have received ACT_META from computer
            return SendSerialMsg(&nak_msg);
        case ACT_POWER_REQ:
            power_val = !!(msg->payload[0]);
            gpio_put(TOGGLE_PIN, power_val);

            if (gpio_get(TOGGLE_PIN) == power_val) {
                return SendSerialMsg(&ack_msg);
            } else {
                return SendSerialMsg(&nak_msg);
            }

        case ACT_STATUS_REQ:
            new_msg.action = ACT_STATUS_RX;
            new_msg.payload_len = 1;

            new_msg.payload[0] = gpio_get(TOGGLE_PIN);

            new_msg.crc = GetMsgCRC(&new_msg);

            return SendSerialMsg(&new_msg);

        case ACT_STATUS_RX:
            // should never receive ACT_STATUS_RX, this is meant to be from us
            return SendSerialMsg(&nak_msg);

        default:
            return ERR_MISC;
    }
}
