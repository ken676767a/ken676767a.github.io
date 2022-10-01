#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <enet/enet.h>
enum keyenum
{
	keyenum_default,

	keyenum_left,
	keyenum_right,
	keyenum_standleft,
	keyenum_standright,
	keyenum_up,
	keyenum_down,
	keyenum_total,


};
enum playerstate
{
	speeduping,
	throwing,
	totalstate
};
class mainrole
{
public:
	bool rightward;
	int speed;
	int lastmove;
	int health;
	int attack;
	bool state[totalstate];
	Uint32 laststatetime[totalstate];
	Uint32 statecountinuetime[totalstate];
	Uint32 attlasttime;
	Uint32 attcooldown;
	Uint32 ab1_lasttime;
	Uint32 ab2_lasttime;
	Uint32 ab1_cooldown;
	Uint32 ab2_cooldown;
	Uint32 changeimageframe;
	SDL_Rect rect;
	SDL_Rect healthbarrect;
	SDL_Rect healthrect;
	SDL_Rect iconrect;
	SDL_Texture* stateicon[totalstate];
	
	SDL_Texture* mainrole_Texture;
	SDL_Texture* throwtexture;
	SDL_Texture* gkeyevent[keyenum_total];
	SDL_Texture* standleft2 = NULL;
	SDL_Texture* standright2 = NULL;
	SDL_Texture* weapon = NULL;
	SDL_Texture* weaponnotattck = NULL;
	SDL_Texture* weaponswingattack1 = NULL;
	SDL_Texture* weaponswingattack2 = NULL;
	SDL_Rect weaponRect;
	SDL_Rect cooldownRect1;
	SDL_Rect cooldownbarRect1;
	SDL_Rect cooldownbarRect2;
	SDL_Rect cooldownRect2;
	mainrole();
	mainrole(int a,int b);
	~mainrole();
	void setup();
	
	void changeimage(Uint32& framestart, mainrole&player1, mainrole&player2);
	void goleft(Uint32& framestart, mainrole&, mainrole&);
	void goright(Uint32& framestart, mainrole&, mainrole&);
	void godown(Uint32& framestart, mainrole&, mainrole&);
	void goup(Uint32& framestart, mainrole&, mainrole&);
	void swingattack( Uint32& framestart,mainrole& );
	void (*ab1)(Uint32& framestart, mainrole&, mainrole&);
	void (*ab2)(Uint32& framestart, mainrole&, mainrole&);
	void distate(int i, mainrole&plater, mainrole&ene);
};