#include "pico-serial-message.h"
#include "crc8.h"


/* ------------------------- GLOBAL GLOBA? VARIABLES ------------------------ */

// TODO: see if there is a better way to have ack and nak msg. I would prefer not to hard code the crc.
const serial_msg_t ack_msg = {.sync.value=SYNC_VALUE, .action=ACT_META, .payload_len=1, .payload = {ACK_VALUE}, .crc=ACK_CRC};
const serial_msg_t nak_msg = {.sync.value=SYNC_VALUE, .action=ACT_META, .payload_len=1, .payload = {NAK_VALUE}, .crc=NAK_CRC};

/* ------------------------- LOCAL GLOBAL? VARIABLES ------------------------ */

const actions_desc_t actions_list[] = {
    {ACT_META,          1,  1,  {VALUE_U8}},
    {ACT_POWER_REQ,     1,  1,  {VALUE_BOOL}},
    {ACT_STATUS_REQ,    0,  0,  {0}},
    {ACT_STATUS_RX,     1,  8,  {VALUE_U8}}
};

const size_t actions_list_size = sizeof(actions_list) / sizeof(actions_list[0]);

/* ----------------------------- LOCAL FUNCTIONS ---------------------------- */

/**
 * @brief Linear search to see if the action passed is in action_list
 * 
 * @param action (actions_options_t): action enum value from message.
 * @return const actions_desc_t*: pointer to the action list the action passed is for.
 * @retval NULL: If NULL is returned, it means it is not in list.
 */
static const actions_desc_t *find_action_desc(actions_options_t action) {
    for (size_t i = 0; i < actions_list_size; i++) {
        if (actions_list[i].type == action) {
            return &actions_list[i];
        }
    }

    return NULL;
}

/**
 * @brief Helper function to validate a message.
 *        It checks if action is in action list and payload length of the message is the expected length in the corresponding action list index.
 * 
 * @param msg (const serial_msg_t *) pointer to message that needs to be validated.
 * @return msg_error_t: error status of message.
 * @retval SUCCESS: message is valid. Any other value is an error, see msg_error_t.
 */
static msg_error_t ValidateSerialMsg(const serial_msg_t *msg) {
    if (NULL == msg) {
        return ERR_MISC;
    }

    const actions_desc_t *desc = find_action_desc(msg->action);
    if (!desc) {
        return ERR_WRONG_ACTION;
    }

    if (msg->payload_len < desc->min_length || msg->payload_len > desc->max_length) {
        return ERR_WRONG_PAYLOAD_LEN;
    }

    uint8_t calculatedCRC = GetMsgCRC(msg);
    if (calculatedCRC != msg->crc) {
        return ERR_WRONG_CHECKSUM;
    }

    return SUCCESS;
}

/* ---------------------------- GLOBAL FUNCTIONS ---------------------------- */

uint8_t GetMsgCRC(const serial_msg_t *msg) {
    //TODO: this is cursed, find a better way
    uint8_t arr[PAYLOAD_MAX_LEN+2];
    arr[0] = msg->action;
    arr[1] = msg->payload_len;
    
    for (int i = 0; i < msg->payload_len; i++) {
        arr[2+i] = msg->payload[i];
    }

    return crc8(arr, msg->payload_len+2, NULL);
}


msg_error_t ReadSerialMsg(serial_msg_t *msg) {
    if (NULL == msg) {
        return ERR_MISC;
    }

    msg->sync.bytes.high = getchar_timeout_us(0);
    if (SYNC_HI != msg->sync.bytes.high) {
        return ERR_WRONG_SYNC;
    }
    
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
    
    return ValidateSerialMsg(msg);
}

msg_error_t SendSerialMsg(const serial_msg_t *msg) {
    if (NULL == msg) {
        return ERR_MISC;
    }

    msg_error_t valid_state = ValidateSerialMsg(msg);
    if (SUCCESS != valid_state) {
        return valid_state;
    }

    putchar_raw(msg->sync.bytes.high);
    putchar_raw(msg->sync.bytes.low);

    putchar_raw(msg->action);

    putchar_raw(msg->payload_len);

    for (int i = 0; i < msg->payload_len; i++) {
        putchar_raw(msg->payload[i]);
    }

    putchar_raw(msg->crc);

    return SUCCESS;
}