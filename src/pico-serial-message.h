#ifndef __PICO_SERIAL_MESSAGE_H__
#define __PICO_SERIAL_MESSAGE_H__

#include "pico/stdlib.h"

/* --------------------------------- MACROS --------------------------------- */

#define PAYLOAD_MAX_LEN 8

#define SYNC_VALUE 0x6721
#define SYNC_HI (uint8_t) (SYNC_VALUE >> 8)
#define SYNC_LO (uint8_t) (SYNC_VALUE & 0xFF)

/* ------------------------ STRUCTS, UNIONS and ENUMS ----------------------- */

typedef struct {
    union {
        struct {
            uint8_t low;
            uint8_t high;
        } bytes;
        uint16_t value;
    }sync;
    uint8_t action;
    uint8_t payload_len;
    uint8_t payload[PAYLOAD_MAX_LEN];
    uint8_t crc;
} serial_msg_t;

typedef enum {
    SUCCESS = 0,
    ERR_WRONG_SYNC = 1,
    ERR_WRONG_CHECKSUM = 2,
    ERR_PAYLOAD_LENGTH_EXCEEDED = 3
} error_t;

/* ------------------------------- PROTOTYPES ------------------------------- */

/**
 * @brief Reads the Serial Buffer 
 * 
 * @param msg (serial msg_t *): pointer to the message variable
 * @return error_t
 */
error_t ReadSerialMsg(serial_msg_t *msg);

/**
 * @brief Sends a message to the Serial Buffer
 * 
 * @param msg (serial msg_t *): pointer to the message variable
 */
void SendSerialMsg(const serial_msg_t *msg);

#endif // #ifndef __PICO_SERIAL_MESSAGE_H__
