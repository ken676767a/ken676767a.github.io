
#include <SDL.h>
#include <SDL_image.h>
#include <enet/enet.h>
#include<typeinfo>
#include <stdio.h>
#include <iostream>
#include <string>
#include<thread>
#include<vector>
#include<omp.h>
#include"mainroleh.h"
#include"abilty.h"
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const int fps = 20;
const int framedelay = 1000 / fps;
int playerid=0;
Uint32 framestart;
bool disturbframe;
//Starts up SDL and creates window
bool init();
void frame(Uint32 &framestart, int framedelay);
//Loads media
bool loadMedia();
void update(mainrole&, mainrole&);
//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
SDL_Texture* p1win = NULL;
SDL_Texture* p2win = NULL;
SDL_Texture* ab_to_select[totalabilty];
std::string p1a1 = " ";
std::string p1a2 = " ";
std::string p2a1 = " ";
std::string p2a2 = " ";

void action(int, mainrole&, mainrole&, Uint32&, Uint32&, Uint32&, ENetPeer*);
//Current displayed texture
//SDL_Texture* gTexture = NULL;

mainrole::mainrole(int a,int b)
{
	 rightward=0;
	speed = 20;
	health = 1000;
	attack = 50;
	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 100;
	weaponRect.x = rect.x+40;
	weaponRect.y = rect.y;
	weaponRect.w = 90;
	weaponRect.h = 60;
	healthbarrect.x = 0;
	healthbarrect.y = 0;
	healthbarrect.w = 200;
	healthbarrect.h = 50;
	healthrect.x = 0;
	healthrect.y = 0;
	cooldownRect1.x = 0;
	cooldownRect2.x = 0;
	cooldownRect1.y = 55;
	cooldownRect2.y = 85;
	cooldownbarRect1.x = 0;
	cooldownbarRect2.x = 0;
	cooldownbarRect1.y = 55;
	cooldownbarRect2.y = 85;
	cooldownbarRect1.w = 200;
	cooldownbarRect2.w = 200;
	cooldownRect2.h = 20;
	cooldownRect1.h = 20;
	cooldownbarRect1.h = 20;
	cooldownbarRect2.h = 20;
	iconrect.y = 120;
	iconrect.h = 20;
	iconrect.w = 20;
	healthrect.w = 200;
	healthrect.h = 50;
	for (int i = 0; i < totalstate; i++)
	{
		state[i] = 0;
	}
	statecountinuetime[speeduping] = 3000;
}
mainrole::~mainrole()
{
	SDL_DestroyTexture(weapon);
	weapon = NULL;
	SDL_DestroyTexture(weaponnotattck);
	weaponnotattck = NULL;
	SDL_DestroyTexture(weaponswingattack1);
		weaponswingattack1 = NULL;
		SDL_DestroyTexture(weaponswingattack2);
		weaponswingattack2 = NULL;
	SDL_DestroyTexture(standright2);
	SDL_DestroyTexture(standleft2);
	standleft2 = NULL;
	standright2 = NULL;
	SDL_DestroyTexture(mainrole_Texture);
	for (int i = keyenum_total; i >= 0; i--)
	{
		SDL_DestroyTexture(gkeyevent[i]);
		gkeyevent[i] = NULL;
	}
	for (int i = totalstate; i >= 0; i--)
	{
		SDL_DestroyTexture(stateicon[i]);
		stateicon[i] = NULL;
	}
	
}
void mainrole::goleft(Uint32 & framestart, mainrole& player1, mainrole& player2)
{
	lastmove = 3;
	disturbframe = true;
	rightward = 0;
	mainrole_Texture = gkeyevent[keyenum_left];
	
	if (rect.x >0)
	{
		rect.x -= speed;
		weaponRect.x = rect.x - 30;
		//weaponRect.y = rect.y;
	}

	//Render texture to screen
	//update(player1,player2 );
	//frame(framestart, framedelay);
	mainrole_Texture = gkeyevent[keyenum_standleft];

	//SDL_RenderSetScale(gRenderer, 1000, 1000);
	//Render texture to screen
	//update(player1, player2);
	//frame(framestart, framedelay);
	
}
void mainrole::distate(int i, mainrole&player, mainrole&ene)
{
	switch (i)
	{
	case speeduping: 
		player.speed -= 50;
		player.state[speeduping] = false;
		break;
		
	default:
		break;
	}
}
void mainrole::setup()
{
	attcooldown = 300;
	attlasttime = SDL_GetTicks();
	stateicon[speeduping] = loadTexture("swiftness.png");
	standleft2 = loadTexture("Knight-standleft4.png");
	//standright2 = loadTexture("Knight-standright2.png");
	//weapon = loadTexture("daggers (1).png");
	weaponnotattck = loadTexture("daggers (1).png");
	weaponswingattack1 = loadTexture("daggers(1)225.png");
	weaponswingattack2 = loadTexture("daggers(1)45.png");
	bool success = true;
	gkeyevent[keyenum_left] = loadTexture("Knight-leftWalk1.png");
	if (gkeyevent[keyenum_left] == NULL)
	{
		printf("Failed to load gkeyevent[1] image!\n");
		success = false;
	}
	gkeyevent[keyenum_right] = loadTexture("Knight-rightWalk1.png");
	if (gkeyevent[keyenum_right] == NULL)
	{
		printf("Failed to load gkeyevent[2] image!\n");
		success = false;
	}
	gkeyevent[keyenum_standleft] = loadTexture("Knight-standleft3.png");
	if (gkeyevent[keyenum_standleft] == NULL)
	{
		printf("Failed to load gkeyevent[1] image!\n");
		success = false;
	}
	weapon = weaponnotattck;
}
void mainrole::goright(Uint32 &framestart, mainrole&player1, mainrole&player2)
{
	lastmove = 4;
	disturbframe = true;
	rightward = 1;
	mainrole_Texture = gkeyevent[keyenum_left];
	;
	if (rect.x < SCREEN_WIDTH)
	{
		rect.x += speed;
		 weaponRect.x = rect.x + 40;
		//printf("%d\n", weaponRect.y);
		//weaponRect.y = rect.y ;
	}


	//Render texture to screen
	//update(player1, player2);
	//frame(framestart, framedelay);
	mainrole_Texture = gkeyevent[keyenum_standleft];
//	update(player1, player2);
	//frame(framestart, framedelay);
	//Render texture to screen
	
	
}
void mainrole::goup(Uint32 &framestart,mainrole&player1, mainrole&player2)
{
	lastmove = 1;
	disturbframe = true;
	if (rect.y> 0)
	{
		rect.y -= speed;
		//weaponRect.x = rect.x - rect.w / 3;
		weaponRect.y = rect.y;
		//printf("%d\n", weaponRect.y);
	}
	
	//Render texture to screen
	
	//update(player1, player2);
	//frame(framestart, framedelay);
	
	
}
void mainrole::godown(Uint32& framestart, mainrole&player1, mainrole&player2)
{
	lastmove = 2;
	
	disturbframe = true;
	if (rect.y+rect.h< SCREEN_HEIGHT)
	{
		rect.y += speed;
		//weaponRect.x = rect.x - rect.w / 3;
		weaponRect.y = rect.y ;
	}
	
	//Render texture to screen
	//update(player1, player2);
	
	
	//SDL_RenderSetScale(gRenderer, 1000, 1000);
	//Update screen
	
	
}
void mainrole::changeimage(Uint32& framestart, mainrole&player1, mainrole&player2)
{
	if (this->changeimageframe > 200)
	{
		if (mainrole_Texture == standleft2)
		{
			mainrole_Texture = gkeyevent[keyenum_standleft];
		}
		else
		{
			mainrole_Texture = standleft2;
		}
		changeimageframe = 0;
	}
	
	
	//frame(framestart, framedelay);
	
}
void mainrole::swingattack(Uint32& framestart,mainrole& ene)
{
	disturbframe = true;
	weapon = weaponswingattack1;

	update(*this, ene);
	frame(framestart, framedelay);
	if (weaponRect.x<ene.rect.x+ene.rect.w&&weaponRect.x >ene.rect.x&&weaponRect.y<ene.rect.y+ ene.rect.h&&weaponRect.y+ weaponRect.h>ene.rect.y)
	{
		ene.health -= attack;
		printf("hurt");

	}
	else
	{
		if (weaponRect.x+ weaponRect.w  >ene.rect.x &&weaponRect.x + weaponRect.w  <ene.rect.x + ene.rect.w&&weaponRect.y<ene.rect.y + ene.rect.h&&weaponRect.y + weaponRect.h>ene.rect.y)
		{
			ene.health -= attack;
			printf("hurt");
		}
		else {
			if (weaponRect.x < ene.rect.x &&weaponRect.x + weaponRect.w > ene.rect.x + ene.rect.w&&weaponRect.y<ene.rect.y + ene.rect.h&&weaponRect.y + weaponRect.h>ene.rect.y)
			{
				ene.health -= attack;
				printf("hurt");
			}
		}
	}
	
	SDL_RenderClear(gRenderer);
	weapon = weaponswingattack2;
	update(*this, ene);

	frame(framestart, framedelay);
	weapon = weaponnotattck;
}
void select(ENetPeer* peer, ENetHost *client, ENetEvent event, mainrole& player, mainrole& player2);
void select(ENetPeer* peer,ENetHost *client, ENetEvent event,mainrole& player, mainrole& player2)
{
	ab_to_select[flashenum] = loadTexture("select_flash.png");
	ab_to_select[speed_up_enum] = loadTexture("select_speedup.png");

	SDL_Texture* selecttexture=ab_to_select[0];
	int i = 0;
	int n = 1;
	bool quit = false;
	bool bp1a1;
	bool bp1a2;
	bool bp2a1;
	bool bp2a2;
	bp1a1 = bp1a2 = bp2a1 = bp2a2 = false;
	
	SDL_Event e;
	while (!quit)
	{
		
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_RIGHT)
				{
					if (i < totalabilty)
					{
						i++;
						selecttexture = ab_to_select[i];
					}


				}
				if (e.key.keysym.sym == SDLK_LEFT)
				{
					if (i >= 0)
					{
						i--;
						selecttexture = ab_to_select[i];
					}


				}
				if (e.key.keysym.sym == SDLK_DOWN)
				{
					if (n == 1)
					{
						printf("player %d %d\n", i, n);
						switch (i)
						{
						case flashenum:
						
							if (playerid == 1)
							{
								p1a1 = "flash";
								bp1a1 = true;
								ENetPacket *packet = enet_packet_create("p1_ab1=flash", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							if (playerid == 2)
							{
								bp2a1 = true;
								p2a1 = "flash";
								ENetPacket *packet = enet_packet_create("p2_ab1=flash", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							
							break;
						case speed_up_enum:
						
							if (playerid == 1)
							{
								bp1a1 = true;
								p1a1 = "speedup";
								ENetPacket *packet = enet_packet_create("p1_ab1=speedup", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							if (playerid == 2)
							{
								bp2a1 = true;
								p2a1 = "speedup";
								ENetPacket *packet = enet_packet_create("p2_ab1=speedup", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							break;
						default:
							break;
						}
						n++;
						i = 0;
					}
					else
					{
						printf("player %d %d\n", i, n);
						switch (i)
						{
						case flashenum:
							
							if (playerid == 1)
							{
								bp1a2 = true;
								p1a2 = "flash";
								ENetPacket *packet = enet_packet_create("p1_ab2=flash", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							if (playerid == 2)
							{
								bp2a2 = true;
								p2a2 = "flash";
								ENetPacket *packet = enet_packet_create("p2_ab2=flash", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							break;
						case speed_up_enum:
							
							if (playerid == 1)
							{
								bp1a2 = true;
								p1a2 = "speedup";
								ENetPacket *packet = enet_packet_create("p1_ab2=speedup", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							if (playerid == 2)
							{
								p2a2 = "speedup";
								bp2a2 = true;
							
								ENetPacket *packet = enet_packet_create("p2_ab2=speedup", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
							}
							break;
						default:
							break;
						}
						
					}
			}
			
			}
			SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
			SDL_RenderCopy(gRenderer, selecttexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);



		}
		while (enet_host_service(client, &event, 10) > 0)
		{
			if (event.type == ENET_EVENT_TYPE_RECEIVE)
			{
				if (event.type == ENET_EVENT_TYPE_RECEIVE)
				{
					std::string s = static_cast<std::string>(reinterpret_cast<char *>(event.packet->data));
					if (playerid == 1)
					{
						if (s == "p2_ab1=flash")
						{
							printf("p2a1");
							bp2a1 = true;
							p2a1 = "flash";
						}
						if (s == "p2_ab1=speedup")
						{
							p2a1 = "speedup";
							bp2a1 = true;
							printf("p2a1");
						
						}
						if (s == "p2_ab2=flash")
						{
							printf("p2a2");
							p2a2 = "flash";
							bp2a2 = true;
						
							
						}
						if (s == "p2_ab2=speedup")
						{
							p2a2 = "speedup";
							printf("p2a2");
							bp2a2 = true;
							
						}
					}
					if (playerid == 2)
					{
						if (s == "p1_ab1=flash")
						{
							printf("p1a1");
							p1a1 = "flash";
							bp1a1 = true;
							

						}
						if (s == "p1_ab1=speedup")
						{
							p1a1 = "speedup";
							printf("p1a1");
							bp1a1 = true;
							
						}
						if (s == "p1_ab2=flash")
						{
							p1a2 = "flash";
							printf("p1a2");
							bp1a2 = true;
							
							
						}
						if (s == "p1_ab2=speedup")
						{
							p1a2 = "speedup";
							printf("p1a2");
							bp1a2 = true;
				
							
						}
						break;
					}






				}
			}
		}
		if (bp1a1&&bp1a2&&bp2a1&&bp2a2)
		{
			quit = true;

		}
	}
	printf("\n");
	if (p1a1 == "flash")
	{
		printf("p1a1");
		player.ab1 = flash;
		player.ab1_cooldown = (UINT32)10000;

	}
	if (p1a1 == "speedup")
	{
		printf("p1a1");
		player.ab1 = speedup;
		player.ab1_cooldown = (UINT32)5000;

	}
	if (p1a2 == "flash")
	{
		printf("p1a2");
		player.ab2 = flash;
		player.ab2_cooldown = (UINT32)10000;

	}
	if (p1a2 == "speedup")
	{
		printf("p1a2");
		player.ab2 = speedup;
		player.ab2_cooldown = (UINT32)5000;

	}
	if (p2a1 == "flash")
	{
		printf("p2a1");
		player2.ab1 = flash;
		player2.ab1_cooldown = (UINT32)10000;

	}
	if (p2a1 == "speedup")
	{
		printf("p2a1");
		player2.ab1 = speedup;
		player2.ab1_cooldown = (UINT32)5000;

	}
	if (p2a2 == "flash")
	{
		printf("p2a2");
		player2.ab2 = flash;
		player2.ab2_cooldown = (UINT32)10000;

	}
	if (p2a2 == "speedup")
	{
		printf("p2a2");
		player2.ab2 = speedup;
		player2.ab2_cooldown = (UINT32)5000;

	}
}
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED );
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 255, 0xFF, 0xFF, 255);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void update(mainrole&player, mainrole& player2)
{
	
	SDL_Rect windowrect;
	
	SDL_RenderClear(gRenderer);
	int p1ststatenumb = 0;
	int p2ststatenumb = 0;
	player.iconrect.x = 0;
	
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(gRenderer);
	player.healthrect.w = (player.health / 1000.0)*200;
	player2.healthrect.w = (player2.health / 1000.0)*200;
	
	if (SDL_GetTicks() - player.ab1_lasttime > player.ab1_cooldown)
	{
		player.cooldownRect1.w = 200;
	}
	else
	{
		player.cooldownRect1.w = 1.0*(SDL_GetTicks() - player.ab1_lasttime) / player.ab1_cooldown * 200;
	}
	if (SDL_GetTicks() - player.ab2_lasttime > player.ab2_cooldown)
	{

		player.cooldownRect2.w = 200;
	}
	else
	{
		SDL_RenderClear(gRenderer);
		//SDL_Delay(1000);
		player.cooldownRect2.w = 1.0*(SDL_GetTicks() - player.ab2_lasttime) / player.ab2_cooldown * 200;
	}
	if (SDL_GetTicks() - player2.ab1_lasttime > player2.ab1_cooldown)
	{
		player2.cooldownRect1.w = 200;
	}
	else
	{
		player2.cooldownRect1.w = 1.0*(SDL_GetTicks() - player2.ab1_lasttime) / player2.ab1_cooldown * 200;
	}
	if (SDL_GetTicks() - player2.ab2_lasttime > player2.ab2_cooldown)
	{
		player2.cooldownRect2.w = 200;
	}
	else
	{
		player2.cooldownRect2.w = 1.0*(SDL_GetTicks() - player2.ab2_lasttime) / player2.ab2_cooldown * 200;
	}
	//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_SetRenderDrawColor(gRenderer, 255, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(gRenderer, &player.healthbarrect);
	
	SDL_RenderFillRect(gRenderer, &player2.healthbarrect);//SDL_RenderFillRect(gRenderer, &player.healthrect);
	SDL_RenderFillRect(gRenderer, &player.cooldownbarRect1);
	SDL_RenderFillRect(gRenderer, &player.cooldownbarRect2);
	SDL_RenderFillRect(gRenderer, &player2.cooldownbarRect1);
	SDL_RenderFillRect(gRenderer, &player2.cooldownbarRect2);
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(gRenderer, &player.healthrect);
	
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(gRenderer, &player2.healthrect);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(gRenderer, &player.cooldownRect1);
	SDL_RenderFillRect(gRenderer, &player.cooldownRect2);
	SDL_RenderFillRect(gRenderer, &player2.cooldownRect1);
	SDL_RenderFillRect(gRenderer, &player2.cooldownRect2);
	
	
	if (player.rightward)
	{
		SDL_RenderCopy(gRenderer, player.weapon, NULL, &player.weaponRect);
		SDL_RenderCopyEx(gRenderer, player.mainrole_Texture, NULL, &player.rect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		if (player.state[throwing])
		{
			//SDL_RenderCopyEx(gRenderer, player.throwtexture, NULL, player.throwrect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		}
	}
	else
	{
		SDL_RenderCopy(gRenderer, player.mainrole_Texture, NULL, &player.rect);
		SDL_RenderCopyEx(gRenderer, player.weapon, NULL, &player.weaponRect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		if (player.state[throwing])
		{
			//SDL_RenderCopyEx(gRenderer, player.throwtexture, NULL, player.throwrect, NULL, NULL, SDL_FLIP_NONE);
		}
	}
	if (player2.rightward)
	{
		SDL_RenderCopy(gRenderer, player2.weapon, NULL, &player2.weaponRect);
		SDL_RenderCopyEx(gRenderer, player2.mainrole_Texture, NULL, &player2.rect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		if (player2.state[throwing])
		{
			//SDL_RenderCopyEx(gRenderer, player2.throwtexture, NULL, player2.throwrect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		}
	}
	else
	{
		SDL_RenderCopy(gRenderer, player2.mainrole_Texture, NULL, &player2.rect);
		SDL_RenderCopyEx(gRenderer, player2.weapon, NULL, &player2.weaponRect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		if (player2.state[throwing])
		{
			//SDL_RenderCopyEx(gRenderer, player2.throwtexture, NULL, player2.throwrect, NULL, NULL, SDL_FLIP_NONE);
		}
	}
	player2.iconrect.x = 600;
	for (int i = 0; i < totalstate; i++)
	{
		if (player.state[i])
		{
			SDL_RenderCopy(gRenderer, player.stateicon[i], NULL, &player.iconrect);
			p1ststatenumb++;
			player.iconrect.x += 25;
		}
		if (player2.state[i])
		{
			SDL_RenderCopy(gRenderer, player2.stateicon[i], NULL, &player2.iconrect);
			p2ststatenumb++;
			player2.iconrect.x -= 25;
		}
	}
	SDL_RenderPresent(gRenderer);
	
	
}

void close()
{
	//Free loaded image

	SDL_DestroyTexture(p1win);
	SDL_DestroyTexture(p2win);
	p1win = NULL;
	p2win = NULL;
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//printf("cc\\\\%s",  path.c_str());
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}
void frame(Uint32 &framestart, int framedelay)
{
	

	int frametime = SDL_GetTicks() - framestart;
	while(framedelay > SDL_GetTicks() - framestart) //&& disturbframe==false)
	{
		

	}
	
	framestart = SDL_GetTicks();


}

void action(int playerid, mainrole&player, mainrole&player2, Uint32& framekeystart, Uint32& framekeystart2, Uint32& framestart, ENetPeer* peer)
{

	bool quit = false;
	static SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit

		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{

			int keey = e.key.keysym.sym;
			bool p1swingattack = false;
			bool p2swingattack = false;
			bool p1ab1 = false;
			bool p1ab2 = false;
			bool p2ab1 = false;
			bool p2ab2 = false;
			//#pragma omp parallel
			{
				//#pragma omp parallel for



				if (playerid == 2)
				{

					//if(SDL_GetTicks() - framekeystart2>framedelay * 5)
					{

						if (keey == SDLK_w && e.key.repeat == 0)
						{


							player2.goup(std::ref(framestart), std::ref(player), std::ref(player2));

							//SDL_RenderCopy(gRenderer, player2.mainrole_Texture, NULL, &(player2.rect));
							

							framekeystart2 = SDL_GetTicks();
							ENetPacket *packet = enet_packet_create("p2_goup", 50, ENET_PACKET_FLAG_RELIABLE);
							enet_peer_send(peer, 0, packet);

						}
						else
						{
							if (keey == SDLK_a&& e.key.repeat == 0)
							{


								player2.goleft(framestart, player, player2);

								
								ENetPacket *packet = enet_packet_create("p2_goleft", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
								SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
								framekeystart2 = SDL_GetTicks();
								//continue;
							}
							else
							{
								if (keey == SDLK_d && e.key.repeat == 0)
								{



									player2.goright(framestart, player, player2);

									
									ENetPacket *packet = enet_packet_create("p2_goright", 50, ENET_PACKET_FLAG_RELIABLE);
									enet_peer_send(peer, 0, packet);
									SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
									framekeystart2 = SDL_GetTicks();
									//continue;

								}
								else
								{
									if (keey == SDLK_s && e.key.repeat == 0)
									{

										player2.godown(framestart, player, player2);

										//continue;
										SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
										ENetPacket *packet = enet_packet_create("p2_godown", 50, ENET_PACKET_FLAG_RELIABLE);
										enet_peer_send(peer, 0, packet);
										
										framekeystart2 = SDL_GetTicks();
									}
									else
									{
										if (keey == SDLK_t && e.key.repeat == 0)
										{
											p2swingattack = true;

											//flash(player,player.lastmove);
											framekeystart2 = SDL_GetTicks();
											
											SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
											//continue;
										}
										else
										{
											if (keey == SDLK_y && e.key.repeat == 0)
											{
												p2ab1 = true;
												//flash(player,player.lastmove);
												framekeystart2 = SDL_GetTicks();
												
												SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
												//continue;
											}
											else
											{
												if (keey == SDLK_g && e.key.repeat == 0)
												{
													printf("ab2 %d \n", SDL_GetTicks() - player2.ab2_lasttime);

													p2ab2 = true;

													//curtainflying1 throow =curtainflying1(player, player2);
													//throow.fly();
													framekeystart2 = SDL_GetTicks();
													SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
													//continue;
												}
											}
										}
									}
								}
							}
						}

					}
					//else {


				}
				if (playerid == 1)
				{

					if (keey == SDLK_UP && e.key.repeat == 0)
					{
						ENetPacket *packet = enet_packet_create("p1_goup", 50, ENET_PACKET_FLAG_RELIABLE);
						enet_peer_send(peer, 0, packet);
						player.goup(std::ref(framestart), std::ref(player), std::ref(player2));

						//SDL_RenderCopy(gRenderer, player2.mainrole_Texture, NULL, &(player2.rect));
						

						framekeystart = SDL_GetTicks();

					}
					else
					{
						if (keey == SDLK_LEFT && e.key.repeat == 0)
						{

							ENetPacket *packet = enet_packet_create("p1_goleft", 50, ENET_PACKET_FLAG_RELIABLE);
							enet_peer_send(peer, 0, packet);
							player.goleft(framestart, player, player2);

							
							SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
							framekeystart = SDL_GetTicks();
							//continue;
						}
						else
						{
							if (keey == SDLK_RIGHT && e.key.repeat == 0)
							{



								player.goright(framestart, player, player2);
								ENetPacket *packet = enet_packet_create("p1_goright", 50, ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(peer, 0, packet);
								

								SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
								framekeystart = SDL_GetTicks();
								//continue;

							}
							else
							{
								if (keey == SDLK_DOWN && e.key.repeat == 0)
								{
									ENetPacket *packet = enet_packet_create("p1_godown", 50, ENET_PACKET_FLAG_RELIABLE);
									enet_peer_send(peer, 0, packet);
									player.godown(framestart, player, player2);

									//continue;
									SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
									
									framekeystart = SDL_GetTicks();
								}
								else
								{
									if (keey == SDLK_p && e.key.repeat == 0)
									{
										p1swingattack = true;
										//continue;
									}
									else
									{
										if (keey == SDLK_o && e.key.repeat == 0)
										{

											p1ab1 = true;

											//flash(player,player.lastmove);
											framekeystart = SDL_GetTicks();
											SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
											//continue;
										}
										else
										{
											if (keey == SDLK_i && e.key.repeat == 0)
											{
												printf("ab2 %d \n", SDL_GetTicks() - player.ab2_lasttime);

												p1ab2 = true;
												//curtainflying1 throow =curtainflying1(player, player2);
												//throow.fly();
												framekeystart = SDL_GetTicks();
												SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
												//continue;
											}
										}
									}
								}
							}
						}
					}

				}





				SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

				//printf("%d\n", SDL_PeepEvents(&e, 100, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT));




			}
			if (p1swingattack)
			{
				if (SDL_GetTicks() - player.attlasttime > player.attcooldown)
				{
					ENetPacket *packet = enet_packet_create("p1_swingattack", 50, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					player.swingattack(framestart, player2);
					player.attlasttime = SDL_GetTicks();

				}

				p1swingattack = false;
			}
			if (p1ab1)
			{
				if (SDL_GetTicks() - player.ab1_lasttime > player.ab1_cooldown)
				{
					ENetPacket *packet = enet_packet_create("p1_ab1", 50, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					player.ab1(framestart, player, player2);
					player.ab1_lasttime = SDL_GetTicks();
				}
				p1ab1 = false;
			}
			if (p1ab2)
			{
				if (SDL_GetTicks() - player.ab2_lasttime > player.ab2_cooldown)
				{
					ENetPacket *packet = enet_packet_create("p1_ab2", 50, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					player.ab2(framestart, player, player2);
					player.ab2_lasttime = SDL_GetTicks();
				}
				p1ab2 = false;
			}
			if (p2ab1)
			{
				if (SDL_GetTicks() - player2.ab1_lasttime > player2.ab1_cooldown)
				{
					ENetPacket *packet = enet_packet_create("p2_ab1", 50, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					player2.ab1(framestart, player2, player);
					player2.ab1_lasttime = SDL_GetTicks();
				}
				p2ab1 = false;
			}
			if (p2ab2)
			{
				if (SDL_GetTicks() - player2.ab2_lasttime > player2.ab2_cooldown)
				{
					ENetPacket *packet = enet_packet_create("p2_ab2", 50, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					player2.ab2(framestart, player2, player);
					player2.ab2_lasttime = SDL_GetTicks();
				}
				p2ab2 = false;
			}
			if (p2swingattack)
			{
				if (SDL_GetTicks() - player2.attlasttime > player2.attcooldown)
				{
					ENetPacket *packet = enet_packet_create("p2_swingattack", 50, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					player2.swingattack(framestart, player);
					player2.attlasttime = SDL_GetTicks();

				}
				p2swingattack = false;
			}
		}

	}

}

int main(int argc, char* args[])
{
	std::string addressstring;
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{

		if (enet_initialize() != 0)
		{
			fprintf(stderr, "An error occurred while initializing ENet.\n");
			return EXIT_FAILURE;
		}
		atexit(enet_deinitialize);
		ENetAddress address;

		ENetHost * client;
		client = enet_host_create(NULL, // create a client host ,
			1,
			2,
			0,
			0);
		if (client == NULL)
		{
			fprintf(stderr,
				"An error occurred while trying to create an ENet client host.\n");
			exit(EXIT_FAILURE);
		}
		ENetEvent event;
		ENetPeer* peer;
		std::string s;
		scanf("%s", s);
		
		const char * c = s.c_str();
		printf("%s", c);
		enet_address_set_host(&address, c);
		address.port = 7777;

		peer = enet_host_connect(client, &address, 1, 0);
		if (peer == NULL)
		{
			fprintf(stderr, "no avaliable peer");

		}

		if (enet_host_service(client, &event, 5000) >= 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts("connect succyytjyess kkkk");
		}
		else
		{

			enet_peer_reset(peer);
			puts("conect to 127.0.0.1 fail kkk");

			SDL_Delay(3000);

		}

		//Main loop flag
		p1win = loadTexture("player1win.png");
		p2win = loadTexture("player2win.png");

		mainrole player = mainrole(0, 0);
		mainrole player2 = mainrole(0, 0);

		bool quit = false;
		framestart = SDL_GetTicks();
		Uint32 framekeystart = SDL_GetTicks();
		Uint32 framekeystart2 = SDL_GetTicks();

		SDL_RenderClear(gRenderer);
		player.setup();
		player2.setup();
		SDL_Rect massagerect;
		massagerect.x = 100;
		massagerect.y = 100;
		massagerect.w = 300;
		massagerect.h = 300;
		player.mainrole_Texture = player.standleft2;
		player2.mainrole_Texture = player2.standleft2;
		player2.rect.x = 500;
		player2.weaponRect.x = 500;
		player2.healthbarrect.x = 420;
		player2.healthrect.x = 420;
		player2.cooldownbarRect1.x = 420;
		player2.cooldownbarRect2.x = 420;
		player2.cooldownRect1.x = 420;
		player2.cooldownRect2.x = 420;
		//While application is running
		int player1lastmove = 0;
		int player2lastmove = 0;

		int playernum = 0;
		int abnum = 0;
		printf("wait for opponent");
		printf("playerid,%d\n", playerid);
		while (playernum < 2)
		{

			while (enet_host_service(client, &event, 10) > 0)
			{
				if (event.type == ENET_EVENT_TYPE_RECEIVE)
				{

					printf("first%s\n", event.packet->data);
					std::string s = static_cast<std::string>(reinterpret_cast<char *>(event.packet->data));
					printf("get  %s\n", s);
				
					if (s == "p2")
					{
						if (playerid == 0)
						{
							playerid = 2;
							printf("\np2playerid,%d", playerid);
						}
						playernum = 2;
					}
					if (s == "p1")
					{
						if (playerid == 0)
						{
							playerid = 1;
							printf("\np1playerid,%d", playerid);
						}

					}


				}
			}



		}
		select(peer,client,event,player,player2); 
	
		
	
		
		while (player.health > 0 && player2.health > 0)//)
		{

			for (int i = 0; i < totalstate; i++)
			{
				if (player.state[i])
				{
					if (SDL_GetTicks() - player.laststatetime[i] > player.statecountinuetime[i])
					{
						player.distate(i, player, player2);
					}
				}
				if (player2.state[i])
				{
					if (SDL_GetTicks() - player2.laststatetime[i] > player2.statecountinuetime[i])
					{
						player2.distate(i, player2, player);
					}
				}
			}
			player.changeimageframe++;
			player2.changeimageframe++;
			framestart = SDL_GetTicks();


			SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
			player.changeimage(framestart, player, player2);
			player2.changeimage(framestart, player, player2);

			action(playerid, player, player2, framekeystart, framekeystart2, framestart, peer);
			update(player, player2);
			disturbframe = false;

			while (enet_host_service(client, &event, 10) > 0)
			{


				if (event.type == ENET_EVENT_TYPE_RECEIVE)
				{

					printf("%s\n", event.packet->data);
					std::string ss = static_cast<std::string>(reinterpret_cast<char *>(event.packet->data));
					if (playerid == 1)
					{
						if (ss == "p2_goup")
						{
							player2.goup(framestart, player, player2);
						}
						if (ss == "p2_goleft")
						{
							player2.goleft(framestart, player, player2);
						}
						if (ss == "p2_godown")
						{
							player2.godown(framestart, player, player2);
						}
						if (ss == "p2_goright")
						{
							player2.goright(framestart, player, player2);
						}

						if (ss == "p2_swingattack")
						{
							player2.swingattack(framestart, player);
						}
						if (ss == "p2_ab1")
						{
							printf("p2ab1");
							player2.ab1(framestart, player2, player);
							player2.ab1_lasttime = SDL_GetTicks();
						}
						if (ss == "p2_ab2")
						{
							printf("p2ab2");
							player2.ab2(framestart, player2, player);
							player2.ab2_lasttime = SDL_GetTicks();
						}
					}
					if (playerid == 2)
					{
						if (ss == "p1_goup")
						{
							player.goup(framestart, player, player2);
						}
						if (ss == "p1_goleft")
						{
							player.goleft(framestart, player, player2);
						}
						if (ss == "p1_godown")
						{
							player.godown(framestart, player, player2);
						}
						if (ss == "p1_goright")
						{
							player.goright(framestart, player, player2);
						}

						if (ss == "p1_swingattack")
						{
							player.swingattack(framestart, player2);
						}
						if (ss == "p1_ab1")
						{
							printf("p1ab1");
							player.ab1(framestart, player, player2);
							player.ab1_lasttime = SDL_GetTicks();
						}
						if (ss == "p1_ab2")
						{
							printf("p1ab2");
							player.ab2(framestart, player, player2);
							player.ab2_lasttime = SDL_GetTicks();
						}
					}

				}
				break;

			}
		}
		if (player.health <= 0)
		{
			SDL_RenderCopy(gRenderer, p2win, NULL, NULL);
		}
		if (player2.health <= 0)
		{
			SDL_RenderCopy(gRenderer, p1win, NULL, NULL);
			printf("p2win");


		}

		printf("out");
		SDL_RenderPresent(gRenderer);
		enet_peer_disconnect(peer, 0);

		while (enet_host_service(client, &event, 3000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnection succeeded.");
				break;
			}
		}
		enet_host_destroy(client);
		return EXIT_SUCCESS;
		//Free resources and close SDL
		SDL_Delay(3000);
		close();



	}


	return 0;
}