#pragma once
#include "pch.h"
#include "ObjectManager.h"

class Cframework
{
private:
	CObjectManager* m_obj_manager;
	clock_t frametime = 0;	// ms
	clock_t cur_time = 0;


public:
	Cframework();
	~Cframework();


	void Initialize();

	void Release();

	void Draw();

	void Update();

	void KeyEvent(Event a_event);

	void MouseEvent(Event a_event, int mouse_x, int mouse_y);
};

