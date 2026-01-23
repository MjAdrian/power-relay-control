#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "crc8.h" 

static const uint8_t test_data[] = "123456789";

int main(void) {
    uint8_t crc;
    int failures = 0;

    // CRC-8 SMBUS
    crc8_config_t crc8_std = {
        .initial = 0x00,
        .polynome = 0x07,
        .final_xor_value = 0x00,
        .reverse_input = false,
        .reverse_output = false
    };

    crc = crc8(test_data, strlen((const char *)test_data), &crc8_std);

    printf("CRC-8 (0x07): 0x%02X  ", crc);
    if (crc == 0xF4) {
        printf("[PASS]\n");
    } else {
        printf("[FAIL]\n");
        failures++;
    }

    // CRC-8/MAXIM
    crc8_config_t crc8_maxim = {
        .initial = 0x00,
        .polynome = 0x31,
        .final_xor_value = 0x00,
        .reverse_input = true,
        .reverse_output = true
    };

    crc = crc8(test_data, strlen((const char *)test_data), &crc8_maxim);

    printf("CRC-8/MAXIM:  0x%02X  ", crc);
    if (crc == 0xA1) {
        printf("[PASS]\n");
    } else {
        printf("[FAIL]\n");
        failures++;
    }

    /* ------------------------------------------------------------ */
    /* Summary                                                      */
    /* ------------------------------------------------------------ */
    if (failures == 0) {
        printf("\nAll CRC-8 tests PASSED.\n");
        return 0;
    } else {
        printf("\nCRC-8 tests FAILED (%d failure(s)).\n", failures);
        return 1;
    }
}
