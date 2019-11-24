#include "framework.h"

using namespace std;

Cframework::Cframework()
{
}


Cframework::~Cframework()
{
}

void Cframework::Initialize()
{
	m_obj_manager = new CObjectManager;
}

void Cframework::Release()
{
	delete m_obj_manager;
}

void Cframework::Draw()
{
	m_obj_manager->draw();
}

void Cframework::Update()
{	
	// frametime °è»ê
	frametime = clock() - cur_time;
	cur_time = clock();
	m_obj_manager->update(frametime);
}

void Cframework::KeyEvent(Event a_event)
{
	m_obj_manager->key_event(a_event);
}

void Cframework::MouseEvent(Event a_event, int mouse_x, int mouse_y)
{
	m_obj_manager->mouse_event(a_event, mouse_x, mouse_y);
}

