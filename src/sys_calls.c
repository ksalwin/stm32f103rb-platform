#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

// Exit the program (you might want to loop indefinitely or reset the system)
void _exit(int status) {
    while (1) {
        // Optionally, you could trigger a system reset here
    }
}

// Close a file (no files are used, so just return an error)
int _close(int file) {
    return -1;
}

// Seek within a file (no files are used, so return error)
off_t _lseek(int file, off_t ptr, int dir) {
    return -1;
}

// Read from a file (no files are used, so set errno and return error)
int _read(int file, char *ptr, int len) {
    errno = EBADF;
    return -1;
}

// Write to a file (e.g., for debugging, you might send data to a UART)
int _write(int file, char *ptr, int len) {
    // Implement this function to send data to your desired output
    // For example, sending characters via UART:
    for (int i = 0; i < len; i++) {
        // Add `uart_send` with your UART send function
    }
    return len;
}

// Provide memory for the heap (if not using heap, you can return an error or a fixed address)
caddr_t _sbrk(int incr) {
    extern char _end; // Defined by the linker script
    static char *heap_end = NULL;

    if (heap_end == NULL) {
        heap_end = &_end;
    }

    char *prev_heap_end = heap_end;

    // If you don't want to use heap, you can disable it by returning an error
    // errno = ENOMEM;
    // return (caddr_t)-1;

    // Alternatively, you can define a fixed heap size or manage it as needed
    heap_end += incr;
    return (caddr_t)prev_heap_end;
}
