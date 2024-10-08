#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* here the tutorial says to put some safety checks
but who needs those anyway :)
*/

/* hardware text mode color constants */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/* puts the foreground and background colors into one value to pass to the vga driver */

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

/* puts the desired character and character color into one value to send to the vga driver */

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

/* implementation of strlen since the compiler doesnt include it */

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

/* width + height of teh screen (in characters) */

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;

	/* make the terminal the worst possible color combination to illustrate the solemnity of this project */

	terminal_color = vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_GREEN);
	terminal_buffer = (uint16_t*) 0xB8000;

	/* creates the terminal buffer and fills it with a buncha spaces */

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

/* pute is french for hooker
using derogatory terms from other languages is considered good practice when writing function names*/

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c,color);
}

void terminal_checknln(void)
{
	if (++terminal_row == VGA_HEIGHT)
	{
		terminal_row = terminal_row - 1;
		for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t outindex = y * VGA_WIDTH + x;
				const size_t inindex = (y + 1) * VGA_WIDTH + x;
				terminal_buffer[outindex] = terminal_buffer[inindex];
			}
		}
 	}
}

void terminal_putchar(char c)
{
	/*checks for newlines, if so skips putting the character and adds one to the column, resets the row */
	if (c == '\n') {
		terminal_column = 0;
		terminal_checknln();
	}

	else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

		/* makes sure to not write outside of the screen because that is impossible
		also increments the counters for column and row i believe */

		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			terminal_checknln();
		}
	}
}

/* increments through the string you send it using the length you provide and sends one character at a time to terminal_putchar */

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);

}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void kernel_main(void)
{
	terminal_initialize();

	terminal_writestring("Hello, colonel world!\nI am on a new line!\n\n\n\n\n\n\n\nehlhekeh\n\n\n\ndhska\nahjd\nhweui\nwhoehwuef\nwhuifga\neu\ngd\n\n\nashdfaskd\nasdfhj\nshd\nf\nsa\nsjdfe\n");
}
