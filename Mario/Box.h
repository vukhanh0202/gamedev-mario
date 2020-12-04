#pragma once
#include "GameObject.h"

#define BOX_STATE_ENABLE 100
#define BOX_STATE_DISABLE 200

#define BOX_BBOX_WIDTH  16
#define BOX_BBOX_HEIGHT 16

#define OBJECT_TYPE_BOX		5
#define OBJECT_TYPE_BOXS	6

class Box : public GameObject
{
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BOX;
	}
};

class Boxs : public GameObject
{
	boolean mergeComplete;
	float width;
	float height;

	vector<Box*> listBox;
public:

	Boxs() { mergeComplete = false; width = 0; height = 0; }
	boolean getMergeComplete() { return mergeComplete; }
	vector<Box*> getListBox() { return listBox; }
	void setWidth(float width) { this->width = width; }
	void setHeight(float height) { this->height = height; }
	double getWidth() { return this->width; }
	double getHeight() { return this->height; }
	void setMergeComplete(boolean mergeComplete) { this->mergeComplete = mergeComplete; }

	void PushBox(Box* box);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BOXS;
	}

};

