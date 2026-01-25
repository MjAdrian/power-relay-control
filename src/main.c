#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "stdio.h"

#include "power-relay-control.h"
// #include "pico-serial-message.h"

int main() {
    stdio_init_all();

    PowerPinInit(TOGGLE_PIN);

    serial_msg_t msg;
    msg_error_t status;

    while(1) {
        tight_loop_contents();

        status = ReadSerialMsg(&msg);
        
        if (SUCCESS == status) {
            status = ParseMsg(&msg);
            if (SUCCESS != status) {
                putchar_raw(status);
            }
        } else if (ERR_WRONG_SYNC != status) {
            putchar_raw(0xFF);
        }
    }
}