#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "stdio.h"

#include "power-relay-control.h"
// #include "pico-serial-message.h"

int main() {
    stdio_init_all();

    serial_msg_t msg;

    while(1) {
        tight_loop_contents();

        msg_error_t status = ReadSerialMsg(&msg);
        if (SUCCESS == status) {
            ParseMsg(&msg);
        } else if (ERR_WRONG_SYNC != status) {
            putchar_raw(0xFF);
        }
    }
}