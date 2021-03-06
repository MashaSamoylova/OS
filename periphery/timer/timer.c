#include "timer.h"
#include "interrupts.h"
#include "helpers.h"
#include "print.h"

void init_timer() {
    /* Get the PIT value: hardware clock at 1193180 Hz */
    unsigned int divisor = 1193180;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)((divisor >> 8) & 0xFF);
    /* Send the command */
    send_byte_to_port(0x43, 0x36); /* Command port  00{channel 0}11{acces mode lobyte/hibyte }011{mode 3}0{format 16 or 10} */
    send_byte_to_port(0x40, low);
    send_byte_to_port(0x40, high);

    char imr;
    imr = read_byte_from_port(0x21);
    io_wait();
    imr = imr & (0xff - 1); // enable pit interrupt (pin 0)
    send_byte_to_port(0x21, imr);
    io_wait();

    print("timer on\n");
}
