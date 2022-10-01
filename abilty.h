#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "mainroleh.h"

enum abEnum
{
	flashenum,
	speed_up_enum,
	flying1enum,
	totalabilty
};
void flash(Uint32& framestart, mainrole&, mainrole&);
void speedup(Uint32& framestart, mainrole&, mainrole&);
/*class flying
{
public:
	flying(mainrole&,mainrole&);
	
	flying::~flying();
	SDL_Rect flyrect;
	
	bool rightward;

	int originx;
	mainrole& ene;

	
};
/*
flying::flying(mainrole& playe, mainrole&enem)
{
	if (playe.rightward)
	{
		rightward = 1;
		flyrect.x = playe.rect.x+ playe.rect.w;
	}
	else
	{
		rightward = 0;
		flyrect.x = playe.rect.x ;
	}
	originx = flyrect.x;
	flyrect.y = playe.rect.y;
	
	ene = enem;
	
}

flying::~flying()
{
	
	
}
class curtainflying1 :public flying
{
public:
	curtainflying1(mainrole& playe, mainrole& ene):flying( playe, ene);
	void fly();
	void flyhit();
	SDL_Texture* texture;
	int speed;
	int attack;
};
curtainflying1::curtainflying1(mainrole& playe, mainrole& ene) : flying(playe,  ene)
{
	flyrect.w = 100;
	flyrect.h = 100;
	speed = 30;
	attack = 30;
	texture = loadTexture("flying1.png");
	playe.throwtexture = texture;
	playe.throwrect = &flyrect;
	playe.state[throwing] = 1;
}
*/