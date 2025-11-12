#include "rprintf.h"
#include "terminal.h"

// Deliverable 2: Use esp_printf to print to terminal
// This wrapper function allows esp_printf to use our putc function
void printk(charptr ctrl, ...) {
    va_list args;
    va_start(args, ctrl);
    esp_vprintf(putc, ctrl, args);
    va_end(args);
}
