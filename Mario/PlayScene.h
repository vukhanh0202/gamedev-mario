#pragma once
#include "Scene.h"
#include "Mario.h"
#include "GameObject.h"
#include "BackGround.h"
#include "Special.h"

class PlayScene : public Scene
{
protected:
	Mario *player;					// A play scene has to have player, right? 

	boolean mergeObject;			// merge object (present is only box) ? merge status : do normally

	vector<LPGameObject> objects;

	BackGroundMotion *bgMotion;		// Start screen have background move

	vector<Special*> gifts;
	int currentSpecial;
	DWORD timeChangeSpecial;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void pushObject(LPGameObject LPObject) { this->objects.push_back(LPObject); }
	vector<LPGameObject> getObjects() { return objects; }
	Mario * GetPlayer() { return player; }
	BackGroundMotion * GetBGMotion() { return bgMotion; }
};

class PlaySceneKeyHandler : public SceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlaySceneKeyHandler(Scene *s) :SceneKeyHandler(s) {};
};

