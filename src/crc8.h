#ifndef __CRC16_H__
#define __CRC16_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -------------------------------------------------------------------------- */
/*                                   STRUCTS                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Struct for CRC8 configuration
 */
typedef struct {
  uint16_t initial;         /**< Initial CRC value */
  uint16_t polynome;        /**< CRC polynomial  */
  uint16_t final_xor_value; /**< Final XOR value of CRC8, not implemented yet  */
  bool reverse_input;       /**< Reverses inputed value before starting algorithm if True */
  bool reverse_output;      /**< Reverses Output after algorithm is done */
} crc8_config_t;

/* -------------------------------------------------------------------------- */
/*                                 PROTOTYPES                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Computes CRC-8 with custom configuration. The default uses the SMBUS configuration.
 * 
 * If `cfg` is NULL, it defaults to CRC-8 SMBUS (poly 0x07, init 0x00).
 * 
 * @param data (const uint8_t *) Pointer to the input data buffer
 * @param length (size_t) Number of bytes in the input data buffer.
 * @param cfg   (const crc_config_t *) Pointer to CRC config struct (nullable).
 * @return `[uint8_t]` : Computed 8 bit CRC value.
 */
uint8_t crc8(const uint8_t* data, size_t length, const crc8_config_t* cfg);

#endif // #ifndef __CRC16_H__
