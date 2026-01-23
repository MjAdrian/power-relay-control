#include "pico/stdlib.h"
#include "pico/stdio.h"

#include "serial-message.h"

int main() {
    stdio_init_all();

    while(1) {
        tight_loop_contents();
    }
}