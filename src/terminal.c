#include "terminal.h"

static unsigned short *vga_buffer = (unsigned short*)0xb8000;
static int cursor_offset = 0; // Tracks position in video memory (0 to 80*25-1)

// Deliverable 1: Write a function called putc that writes a single character to the terminal
void putc(int data) {
    const unsigned char color = 0x07; // Light grey on black
    char c = (char)data;
    
    // Handle special characters
    if (c == '\n') {
        // Move to beginning of next line
        cursor_offset = ((cursor_offset / 80) + 1) * 80;
    } else if (c == '\r') {
        // Carriage return - move to beginning of current line
        cursor_offset = (cursor_offset / 80) * 80;
    } else {
        // Write the character to video memory
        vga_buffer[cursor_offset] = (color << 8) | c;
        cursor_offset++;
    }
    
    // Deliverable 3: Scroll when reaching bottom of screen (past 24th row)
    if (cursor_offset >= 80 * 25) {
        // Scroll up by one line
        for (int i = 0; i < 80 * 24; i++) {
            vga_buffer[i] = vga_buffer[i + 80];
        }
        
        // Clear the last line
        for (int i = 80 * 24; i < 80 * 25; i++) {
            vga_buffer[i] = (color << 8) | ' ';
        }
        
        // Move cursor to beginning of last line
        cursor_offset = 80 * 24;
    }
}

void terminal_clear(void) {
    const unsigned char color = 0x07;
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (color << 8) | ' ';
    }
    cursor_offset = 0;
}
