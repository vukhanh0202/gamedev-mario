#pragma once
#include "Scene.h"
#include "Mario.h"
#include "GameObject.h"

class PlayScene : public Scene
{
protected:
	Mario *player;					// A play scene has to have player, right? 

	vector<LPGameObject> objects;

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

	Mario * GetPlayer() { return player; }

};

class PlaySceneKeyHandler : public SceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlaySceneKeyHandler(Scene *s) :SceneKeyHandler(s) {};
};

