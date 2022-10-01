#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "mainroleh.h"
#include"abilty.h"
void flash(Uint32& framestart, mainrole& player, mainrole&ene)
{
	
	switch (player.lastmove)
	{
	case 1:
		
		player.rect.y -= 100;
		player.weaponRect.y -= 100;
		if (player.rect.y < 0)
		{
			player.rect.y = 0;
			player.weaponRect.y = 0;
		}
		break;
	case 2:

		player.rect.y += 100;
		player.weaponRect.y += 100;
		if (player.rect.y+player.rect.h >480)
		{
			player.weaponRect.y = 480-player.rect.h;
			player.rect.y = 480-player.rect.h;
		}
		break;
	case 3:

		player.rect.x -= 100;
		player.weaponRect.x -= 100;
		if (player.rect.x <0)
		{
			player.rect.x=0;
			player.weaponRect.x = 0;
		}
		break;
	case 4:
		
		player.rect.x += 100; 
		player.weaponRect.x += 100;
		if (player.rect.x+player.rect.w >640)
		{
			player.rect.x = 640-player.rect.w;
				player.weaponRect.x = 640-player.rect.w;;
		}
		break;
	default:
		break;

	}
	
}
void speedup(Uint32& framestart, mainrole& player, mainrole&ene)
{
	player.speed += 50;
	player.laststatetime[speeduping] = SDL_GetTicks();
	player.state[speeduping] = true;
}

