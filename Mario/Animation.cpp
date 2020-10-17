#include "Animation.h"


/*
	Add frame to Animation;
	1 Animation include some Frame
*/
void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSprite sprite = Sprites::GetInstance()->Get(spriteId);
	LPAnimationFrame frame = new AnimationFrame(sprite, t);
	listFrames.push_back(frame);
}

/*
	Render Animation
*/
void Animation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		// Get time current frame happen	
		DWORD t = listFrames[currentFrame]->GetTime();

		// Check lastframe have done ?
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			
			// Check current frame last -> reset current frame
			if (currentFrame == listFrames.size()) currentFrame = 0;
		}

	}

	// Draw current frame
	listFrames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}



/*
	List Animation to manage
*/
Animations * Animations::__instance = NULL;

Animations * Animations::GetInstance()
{
	if (__instance == NULL) __instance = new Animations();
	return __instance;
}

/*
	Get animation
*/
LPAnimation Animations::Get(int id)
{
	return listAnimations[id];
}

/*
	Add animation to list animation
*/
void Animations::Add(int id, LPAnimation ani)
{
	listAnimations[id] = ani;
}