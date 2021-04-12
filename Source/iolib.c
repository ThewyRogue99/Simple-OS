#include "iolib.h"

#define VIDEO_MEMORY_CONST 0xB8000
#define VIDEO_MEMORY_END 0xB8FA0

char* video_memory = (char*)VIDEO_MEMORY_CONST;

void set_cursor_to_address(uint16_t address);
void set_cursor(int x, int y);

void print_string(char* str)
{
	for(int i = 0; *(str + i) != '\0'; i++)
	{
		print_char(*(str + i));
	}
}

void print_char(char character)
{
	handle_scroll();
	if(character == '\n')
		video_memory += 0xA0 - ((unsigned int)video_memory - VIDEO_MEMORY_CONST) % 0xA0;
	else if(character == '\r')
		video_memory -= ((unsigned int)video_memory - VIDEO_MEMORY_CONST) % 0xA0;
	else
	{
		*video_memory = character;
		video_memory += 2;
	}
	set_cursor_to_address((unsigned int)video_memory);
}

void delete_last_char()
{
	char *current_char = video_memory - 2;
	if(*current_char != '>')
	{
		*current_char = 0;
		video_memory -= 2;
	}
	set_cursor_to_address((unsigned int)video_memory);
}

void print_int(int number)
{
	print_char(number / 10 + 48);
	print_char(number % 10 + 48);
}

void clear_screen()
{
	char* cont = (char*)VIDEO_MEMORY_CONST;
	while(cont != (char*)VIDEO_MEMORY_END)
	{
		*cont = (char)BLACK;
		cont += 2;
	}
	video_memory = (char*)VIDEO_MEMORY_CONST;
	set_cursor(0, 0);
}

void set_screen(enum COLORS background_color, enum COLORS text_color)
{
	char* cont = (char*)VIDEO_MEMORY_CONST;
	while(cont != (char*)VIDEO_MEMORY_END + 0xA0)
	{
		*(cont + 1) = text_color & (*(cont + 1) & 0x0F) | ((char)background_color << 4);
		cont += 2;
	}
}

uint8_t port_byte_in ( uint16_t port ) {
	// "=a" ( result ) put AL register in variable RESULT
	// "d" ( port ) load EDX with port
	uint8_t result ;
	__asm__ ("in %%dx , %% al" : "=a" ( result ) : "d" ( port ));
	return result ;
}
void port_byte_out ( uint16_t port , uint8_t data ) {
	// "a" ( data ) load EAX with data
	// "d" ( port ) load EDX with port
	__asm__ (" out %%al , %% dx" : :"a" ( data ), "d" ( port ));
}
uint16_t port_word_in ( uint16_t port ) {
	uint16_t result ;
	__asm__ ("in %%dx , %% ax" : "=a" ( result ) : "d" ( port ));
	return result ;
}
void port_word_out ( uint16_t port , uint16_t data ) {
	__asm__ (" out %%ax , %% dx" : :"a" ( data ), "d" ( port ));
}

char* process_command(char* command, int length);

void get_command()
{
	char* command_start = video_memory -
	((unsigned int)video_memory - VIDEO_MEMORY_CONST) % 0xA0 + 4;

	int length = 0;
	for(int i = 0; *(command_start + i) != 0; i += 2)
		++length;

	char command[length + 1];
	for(int i = 0, x = 0; x < length; i += 2, x++)
		command[x] = *(command_start + i);
	command[length] = '\0';

	char* command_result = process_command(command, length);
	if(!command_result)
	{
		print_string("\n>>");
	}
	else
	{
		print_char('\n');
		print_string(command_result);
		print_string("\n>>");
	}
}

void memcopy(char* src, char* dst, int length)
{
	for(int i = 0; i < length; i++)
		*(dst + i) = *(src + i);
}

int strcmp(char* first, char* second, int length)
{
	int result = 1;
	for(int i = 0; i < length; i++)
	{
		if(*(first + i) != *(second + i))
		{
			result = 0;
			break;
		}
	}

	return result;
}

void set_cursor(int x, int y)
{
	uint16_t pos = y * 80 + x;
 
	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (uint8_t)(pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

//Work in progress (Works fine with 4.3ghz processors)
void Sleep(int milliseconds)
{
	milliseconds *= 38000;
	for(int i = 0; i < milliseconds; i++);
}

void set_cursor_to_address(uint16_t address)
{
	uint16_t pos = (address - (uint16_t)VIDEO_MEMORY_CONST) / 2;

	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (uint8_t)(pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

char* process_command(char* command, int length)
{
	if(strcmp(command, "clear", length))
	{
		clear_screen();
		return NULL;
	}
	else
	{
		char* error = "Failed to execute command";
		return error;
	}
}

char scan_code_US [] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', /* <-- Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\',
  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void handle_scroll()
{
	if((unsigned int)video_memory >= VIDEO_MEMORY_END)
	{
		for (int i = 0xA0; VIDEO_MEMORY_CONST + i <= VIDEO_MEMORY_END; i += 0xA0)
		{
			memcopy(VIDEO_MEMORY_CONST + i, (VIDEO_MEMORY_CONST + i) - 0xA0, 0xA0);
		}
		video_memory = VIDEO_MEMORY_END - 0xA0;
	}
}

int get_key()
{
	int key = port_byte_in(0x60);

	if(key >= 0x60)
		key = 0;

	key = scan_code_US[key];

	return key;
}
