#ifndef IOLIB_H
#define IOLIB_H

#include <stdint.h>
#include <stddef.h>

enum COLORS
{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PURPLE,
	BROWN,
	GRAY,
	DARK_GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_CYAN,
	LIGHT_RED,
	LIGHT_PURPLE,
	YELLOW,
	WHITE
};

void print_string(char* str);
void print_char(char character);
int get_key();
void get_command();
void handle_scroll();
void print_int(int number);
void clear_screen();
void Sleep(int milliseconds);
void delete_last_char();
void set_screen(enum COLORS background_color, enum COLORS text_color);

#endif