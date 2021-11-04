#include "FadeToBlack.h"

#include "Render.h"
#include "App.h"
#include "Window.h"

#include "External/SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

FadeToBlack::FadeToBlack(App* application, bool start_enabled) : Module(application, start_enabled)
{
	if (app != nullptr) {
		screenRect = { 0,0,app->win->width * app->win->scale,app->win->height * app->win->scale };
	}
}
FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	if (currentStep == Fade_Step::NONE) 
	{ 
		return true;
	}

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			/*moduleToDisable->Disable();
			moduleToEnable->Enable();*/

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if(frameCount<=0)
		{
			currentStep = Fade_Step::NONE;
		}
	}
	return true;
}

bool FadeToBlack::PostUpdate()
{
	if(currentStep == Fade_Step::NONE)
	{
		return true;
	}
	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::FadeToBlackThis(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
		ret = true;
	}
	return ret;
}