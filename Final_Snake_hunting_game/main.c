#include <matrix.h>
#include <INTRINS.h>
#include <REG51.h>
#include <Snake.h>

#define COMMONPORTS P0

void main()
{
	unsigned char tab;
	unsigned char i;

	init_game();

	while(1)
	{
		Running_Game();
		// Refresh screen
		for(i = 0; i < Speed; i++) // The game speed is still kept the same
		{
			for(tab = 0; tab < 8; tab++)
			{
				Hc595SendByte(0x00);  // Clear old data
				COMMONPORTS = Cols[tab];
				Hc595SendByte(display_buffer[tab]); // Update the screen
			}
		}
	}
}
