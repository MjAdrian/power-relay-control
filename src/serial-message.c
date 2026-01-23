#include "serial-message.h"
#include "crc8.h"

/* ----------------------------- LOCAL FUNCTIONS ---------------------------- */

static uint8_t GetMsgCRC(serial_msg_t *msg) {
    //TODO: this is cursed, find a better way
    uint8_t arr[PAYLOAD_MAX_LEN+2];
    arr[0] = msg->action;
    arr[1] = msg->payload_len;
    
    for (int i = 0; i < msg->payload_len; i++) {
        arr[2+i] = msg->payload[i];
    }

    return crc8(arr, msg->payload_len+2, NULL);
}

/* ---------------------------- GLOBAL FUNCTIONS ---------------------------- */

error_t ReadSerialMsg(serial_msg_t *msg) {

    msg->sync.bytes.high = getchar_timeout_us(0);
    msg->sync.bytes.low = getchar_timeout_us(1);
    if (SYNC_VALUE != msg->sync.value) {
        return ERR_WRONG_SYNC;
    }

    msg->action = getchar_timeout_us(1);

    msg->payload_len = getchar_timeout_us(1);

    if (msg->payload_len > PAYLOAD_MAX_LEN) {
        return ERR_PAYLOAD_LENGTH_EXCEEDED;
    }

    for (uint8_t i = 0; i < msg->payload_len; i++) {
        msg->payload[i] = getchar_timeout_us(1);
    }

    msg->crc = getchar_timeout_us(1);

    uint8_t calculatedCRC = GetMsgCRC(msg);

    if (calculatedCRC != msg->crc) {
        return ERR_WRONG_CHECKSUM;
    }
    
    return SUCCESS;
}

void SendSerialMsg(const serial_msg_t *msg) {
    if (msg == NULL) {
        return;
    }

    putchar_raw(msg->sync.bytes.high);
    putchar_raw(msg->sync.bytes.low);

    putchar_raw(msg->action);

    putchar_raw(msg->payload_len);

    for (int i = 0; i < msg->payload_len; i++) {
        putchar_raw(msg->payload[i]);
    }

    putchar_raw(msg->crc);
}
