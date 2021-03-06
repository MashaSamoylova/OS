static inline void send_byte_to_port(unsigned short port, unsigned char value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline void io_wait(void) {
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

static inline char read_byte_from_port(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0": "=a"(ret): "dN"(port) );
    return ret;
}

static inline __attribute__((always_inline)) save_state() {
    asm volatile("pushf");
    asm volatile("cli");
}

static inline __attribute__((always_inline)) restore_state() {
    asm volatile("popf");
}