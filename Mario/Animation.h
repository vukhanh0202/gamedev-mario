#pragma once
#include "Sprite.h"

/*
	Definition Animation Frame
*/
typedef AnimationFrame *LPAnimationFrame;
class AnimationFrame
{
	LPSprite sprite;
	DWORD time;

public:
	AnimationFrame(LPSprite sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSprite GetSprite() { return sprite; }
};

/*
	Definition Animation include Animation Frame
*/
typedef Animation *LPAnimation;
class Animation
{
public:
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPAnimationFrame> listFrames;

public:
	/*
		Constructor
	*/
	Animation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);
};

/*
	Manage animation
*/
class Animations
{
	static Animations * __instance;

	unordered_map<int, LPAnimation> listAnimations;

public:
	void Add(int id, LPAnimation ani);
	LPAnimation Get(int id);

	static Animations * GetInstance();
};
