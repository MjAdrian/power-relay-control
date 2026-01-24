#include "crc8.h"

/* -------------------------------------------------------------------------- */
/*                                  FUNCTIONS                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Function that reverses word passed
 * 
 * @param b (uint8_t)
 * @return `uint8_t`
 */
static uint8_t reverse_byte(uint8_t b) {
    uint8_t r = 0;
    for (int i = 0; i < 8; i++) {
        r = (uint8_t)((r << 1) | (b & 1));
        b >>= 1;
    }
    return r;
}

uint8_t crc8(const uint8_t* data, size_t length, const crc8_config_t* cfg) {
    uint8_t crc = 0x00;
    uint8_t polynome = 0x07;
    uint8_t xorout = 0x0000;
    bool refin = false;
    bool refout = false;

    if (cfg != NULL) {
        crc = cfg->initial;
        polynome = cfg->polynome;
        xorout = cfg->final_xor_value;
        refin = cfg->reverse_input;
        refout = cfg->reverse_output;
    }

    if (!refin) {
        for (size_t i = 0; i < length; i++) {
            crc ^= data[i];

            for (uint8_t j = 0; j < 8; j++) {
                if (crc & 0x80) {
                    crc = (uint8_t)((crc << 1) ^ polynome);
                } else {
                    crc <<= 1;
                }
            }
        }
    } else {
        uint8_t rev_poly = reverse_byte(polynome);

        for (size_t i = 0; i < length; i++) {
            crc ^= data[i];

            for (uint8_t j = 0; j < 8; j++) {
                if (crc & 0x01) {
                    crc = (uint8_t)((crc >> 1) ^ rev_poly);
                } else {
                    crc >>= 1;
                }
            }
        }
    }

    if (refout && !refin) {
        crc = reverse_byte(crc);
    }

    crc ^= xorout;
    return crc;
}
