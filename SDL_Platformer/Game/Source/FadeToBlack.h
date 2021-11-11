#pragma once
#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "External/SDL/include/SDL_rect.h"
class FadeToBlack : public Module
{
public:
	FadeToBlack(App* application, bool start_enabled = true);

	~FadeToBlack();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();
	//call another module and starts the proces to fade
	bool StartFadeToBlack(Module* toDisable, Module* toEnable, float frames = 60);
private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	}currentStep = Fade_Step::NONE;

	//frame count
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;
	//rectangle of the screen
	SDL_Rect screenRect;

	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;
};

#endif //__MODULEFADETOBLACK_H__