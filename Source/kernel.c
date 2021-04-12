#include <stdio.h>
#include "iolib.h"

void _start()
{
	set_screen(BLACK, GREEN);
	clear_screen();
	print_string(">>");
	while(1)
	{
		char key = get_key();
		if(key == 27)
			break;
		if(key != 0)
		{
			if(key == '\n')
			{
				get_command();
			}
			else if(key == '\b')
			{
				delete_last_char();
			}
			else
			{
				print_char(key);
			}
			Sleep(1000);
		}
	}

	print_string("\nKernel executed successfully");
	return;
}