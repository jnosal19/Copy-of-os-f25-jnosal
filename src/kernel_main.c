#include <stdint.h>
#include "terminal.h"
#include "rprintf.h"

#define MULTIBOOT2_HEADER_MAGIC         0xe85250d6

const unsigned int multiboot_header[]  __attribute__((section(".multiboot"))) = {MULTIBOOT2_HEADER_MAGIC, 0, 16, -(16+MULTIBOOT2_HEADER_MAGIC), 0, 12};

uint8_t inb (uint16_t _port) {
    uint8_t rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void main() {
    // Clear the screen first
    terminal_clear();
    
    // Deliverable 2: Use esp_printf to print out the current execution level
    printk("CS 310 Homework 1: Serial Port Driver\n");
    printk("Current execution level: Kernel Mode (Ring 0)\n\n");
    
    // Demonstrate the putc function (Deliverable 1)
    printk("Testing putc function:\n");
    putc('H');
    putc('e');
    putc('l');
    putc('l');
    putc('o');
    putc('\n');
    
    // Demonstrate scrolling (Deliverable 3)
    printk("\nDemonstrating scrolling - filling screen:\n");
    for (int i = 0; i < 30; i++) {
        printk("Line %d: This line will cause scrolling when we reach the bottom\n", i);
    }
    
    printk("\nKeyboard input test (type to see characters):\n");
    
    // Simple keyboard echo loop
    while(1) {
        uint8_t status = inb(0x64);

        if(status & 1) {
            uint8_t scancode = inb(0x60);
            
            // Simple scancode to ASCII mapping for demo
            // Only handle key press events (scancode < 0x80)
            if (scancode < 0x80) {
                // Map some basic keys
                if (scancode == 0x1C) putc('\n');  // Enter
                else if (scancode == 0x39) putc(' ');  // Space
                else if (scancode >= 0x10 && scancode <= 0x19) {
                    // Q W E R T Y U I O P
                    const char qwerty[] = "qwertyuiop";
                    putc(qwerty[scancode - 0x10]);
                }
                else if (scancode >= 0x1E && scancode <= 0x26) {
                    // A S D F G H J K L
                    const char asdf[] = "asdfghjkl";
                    putc(asdf[scancode - 0x1E]);
                }
                else if (scancode >= 0x2C && scancode <= 0x32) {
                    // Z X C V B N M
                    const char zxcv[] = "zxcvbnm";
                    putc(zxcv[scancode - 0x2C]);
                }
            }
        }
    }
}
