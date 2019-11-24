#pragma once
#include "pch.h"


class CLine
{
	POINT m_start;
	POINT m_end;

	float position_buffer[4];

public:
	CLine(int mouse_x, int mouse_y);
	~CLine();

	void get_point(POINT* start, POINT* end);

	void update();

	void draw(GLuint* vbo);

	void mouse_event(int mouse_x, int mouse_y);
};

