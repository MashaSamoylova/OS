#include "pic8259.h"

#include "../helpers/helpers.h"

void init_pic8259() {
    send_byte_to_port(0x20, 0x11);
    io_wait();
    send_byte_to_port(0xa0, 0x11);
    io_wait();

    send_byte_to_port(0x21, 0x20); //offsets for interupts
    io_wait();
    send_byte_to_port(0xa1, 0x28); //offsets for interrupts
    io_wait();

    send_byte_to_port(0x21, 0x04); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    send_byte_to_port(0xa1, 0x02); // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    send_byte_to_port(0x21, 0x01);
    io_wait();
    send_byte_to_port(0xa1, 0x01);
    io_wait();

    send_byte_to_port(0x21, 0x00);
    io_wait();
    send_byte_to_port(0xa1, 0x00);
    io_wait();
}