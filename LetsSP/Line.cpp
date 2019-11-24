#include "Line.h"

float change_gl_coord_x(int value) {
	return (value / float(CLIENT_WIDTH) * 2) - 1;
}

float change_gl_coord_y(int value) {
	return -((value / float(CLIENT_HIEGHT) * 2) - 1);
}

CLine::CLine(int mouse_x, int mouse_y)
{
	m_start.x = mouse_x;
	m_start.y = mouse_y;
	m_end.x = mouse_x;
	m_end.y = mouse_y;
}


CLine::~CLine()
{
}

void CLine::get_point(POINT* start, POINT* end)
{
	*start = m_start;
	*end = m_end;
}

void CLine::update()
{
	position_buffer[0] = change_gl_coord_x(m_start.x);
	position_buffer[1] = change_gl_coord_y(m_start.y);
	position_buffer[2] = change_gl_coord_x(m_end.x);
	position_buffer[3] = change_gl_coord_y(m_end.y);
}

void CLine::draw(GLuint* vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position_buffer), position_buffer, GL_STATIC_DRAW);
	glDrawArrays(GL_LINES, 0, 4);
}

void CLine::mouse_event(int mouse_x, int mouse_y)
{
	m_end.x = mouse_x;
	m_end.y = mouse_y;
}

