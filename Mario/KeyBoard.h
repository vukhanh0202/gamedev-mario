#pragma once
#include <Windows.h>

class KeyEventHandler;
typedef KeyEventHandler * LPKeyEventHandler;

class KeyEventHandler
{
public:
	virtual void KeyState(BYTE *state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

