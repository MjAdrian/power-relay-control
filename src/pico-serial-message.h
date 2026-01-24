#ifndef __PICO_SERIAL_MESSAGE_H__
#define __PICO_SERIAL_MESSAGE_H__

#include "pico/stdlib.h"

/* --------------------------------- MACROS --------------------------------- */

#define PAYLOAD_MAX_LEN 8

#define SYNC_VALUE 0x6721
#define SYNC_HI (uint8_t) (SYNC_VALUE >> 8)
#define SYNC_LO (uint8_t) (SYNC_VALUE & 0xFF)

#define ACK_VALUE 0x01
#define NAK_VALUE 0x00

#define ACK_CRC 0x15
#define NAK_CRC 0x12

/* ------------------------ STRUCTS, UNIONS and ENUMS ----------------------- */

/**
 * @brief Struct for a serial message that is send between device and computer.
 * 
 */
typedef struct {
    union {
        struct {
            uint8_t low;
            uint8_t high;
        } bytes;                        /**<Using a union-struct combo to easily swap between two uint8 and a uint16. 
                                            However, this is not portable and this code only works for processors that use little endian. If big endian, place high before low. 
                                            TODO: see if C has a preprocessor macro that says endian and have preprocessor #if check */
        uint16_t value;
    }sync;                              /**< Sync value (union of a uint16 and two uint8s) that is used to indicate start of message, aka. start delimiter. 
                                            Can get uint16 value with .value or high or low byte with .bytes.high or .bytes.low */
    uint8_t action;                     /**< Action byte (uint8), it is used to determine the purpose of a message */
    uint8_t payload_len;                /**< Length of payload array, max is based on PAYLOAD_MAX_LEN macro */
    uint8_t payload[PAYLOAD_MAX_LEN];   /**< Payload array (uint8 []) to store message contents */
    uint8_t crc;                        /**< CRC8 value (uint8) for message to ensure message is sent correctly */
} serial_msg_t;

/**
 * @brief Enum of Error/Status message during serial message Tx or Rx.
 */
typedef enum {
    SUCCESS = 0,
    ERR_WRONG_SYNC,
    ERR_WRONG_CHECKSUM,
    ERR_WRONG_ACTION,
    ERR_WRONG_PAYLOAD_LEN,
    ERR_PAYLOAD_LENGTH_EXCEEDED,
    ERR_MISC
} msg_error_t;

/**
 * @brief Enum that list all current serial message actions that can be done.
 * 
 */
typedef enum {
    ACT_META        = 0x00,
    ACT_POWER_REQ   = 0x01,
    ACT_STATUS_REQ  = 0x02,
    ACT_STATUS_RX   = 0x03
} actions_options_t;

/**
 * @brief Enum that list possible variable options for a payload. TODO: Update and Implement
 * 
 */
typedef enum {
    VALUE_U8,
    VALUE_U16, 
    VALUE_FLOAT,
    VALUE_BOOL,
} payload_val_t;

/**
 * @brief Struct that describes message types using action_list_t and payload_val_t. 
 *        This will be use to store an array of all possible action message.
 * 
 */
typedef struct {
    actions_options_t type;                 /**< The action based on action_option_t */
    uint8_t min_length;                     /**< The minimum length of the payload for this action */
    uint8_t max_length;                     /**< The maximum length of the payload for this action */
    payload_val_t values[PAYLOAD_MAX_LEN];  /**< Array of payload variable types in the action */
} actions_desc_t;

//TODO: consider changing names of structs/enums, not too sold on them rn

/* --------------------------------- EXTERNS -------------------------------- */

// TODO: uncomment or delete this, idk if it is needed rn
// extern const actions_desc_t actions_list[];
// extern const size_t actions_list_size;

extern const serial_msg_t ack_msg;
extern const serial_msg_t nak_msg;

/* ------------------------------- PROTOTYPES ------------------------------- */

/**
 * @brief Reads the Serial Buffer 
 * 
 * @param msg (serial msg_t *): pointer to the message variable
 * @return msg_error_t: error status of message
 * @retval SUCCESS: message is valid. Any other value is an error, see msg_error_t.
 */
msg_error_t ReadSerialMsg(serial_msg_t *msg);

/**
 * @brief Sends a message to the Serial Buffer
 * 
 * @param msg (serial msg_t *): pointer to the message variable
 */
void SendSerialMsg(const serial_msg_t *msg);

#endif // #ifndef __PICO_SERIAL_MESSAGE_H__
