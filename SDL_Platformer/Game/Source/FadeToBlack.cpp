//#include "FadeToBlack.h"
//
//#include "Render.h"
//#include "App.h"
//
//#include "External/SDL/include/SDL_render.h"
//
//ModuleFadeToBlack::ModuleFadeToBlack(bool startEnabled) : Module(startEnabled)
//	{
//		screenRect = {0,0,SCREEN_WIDTH * SCREEN_SIZE,SCREEN_HEIGHT * SCREEN_SIZE};
//	}
//ModuleFadeToBlack::~ModuleFadeToBlack()
//{
//
//}
//bool ModuleFadeToBlack::Start()
//{
//	LOG("Preparing Fade Screen");
//
//	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
//	return true;
//}
//
//update_status ModuleFadeToBlack::Update()
//{
//	if (currentStep == Fade_Step::NONE) 
//	{ 
//		return update_status::UPDATE_CONTINUE;
//	}
//
//	if (currentStep == Fade_Step::TO_BLACK)
//	{
//		++frameCount;
//		if (frameCount >= maxFadeFrames)
//		{
//			moduleToDisable->Disable();
//			moduleToEnable->Enable();
//
//			currentStep = Fade_Step::FROM_BLACK;
//		}
//	}
//	else
//	{
//		--frameCount;
//		if(frameCount<=0)
//		{
//			currentStep = Fade_Step::NONE;
//		}
//	}
//	return update_status::UPDATE_CONTINUE;
//}
//
//update_status ModuleFadeToBlack::PostUpdate()
//{
//	if(currentStep == Fade_Step::NONE)
//	{
//		return update_status::UPDATE_CONTINUE;
//	}
//	float fadeRatio = (float)frameCount / (float)maxFadeFrames;
//
//	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
//	SDL_RenderFillRect(App->render->renderer, &screenRect);
//
//	return update_status::UPDATE_CONTINUE;
//}
//
//bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
//{
//	bool ret = false;
//
//	if (currentStep == Fade_Step::NONE)
//	{
//		currentStep = Fade_Step::TO_BLACK;
//		frameCount = 0;
//		maxFadeFrames = frames;
//
//		this->moduleToDisable = moduleToDisable;
//		this->moduleToEnable = moduleToEnable;
//		ret = true;
//	}
//	return ret;
//}