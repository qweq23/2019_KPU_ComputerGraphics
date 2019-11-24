#pragma once
#include "Line.h"
#include "Shape.h"
#include "shader.h"

#include <list>

#define SHAPE_GEN_TIME 3000
#define MAX_POLYGON 6


class CObjectManager
{
	CLine* m_line = nullptr;
	std::list<CShape> m_shapes;
	bool is_cut = false;

	GLuint line_shader_id, shape_shader_id;
	GLuint line_vao, shape_vao;
	GLuint line_vbo_pos, shape_vbo_pos, shape_vbo_clr;

	GLuint transformLocation;

	clock_t shape_gen_timer = 0; //ms

	POINT intersect_pt[2];
	int intersect_pos[2];

	POINT line_pts[2];
	POINT world_pts[5] = {0,};

	GLenum draw_mode = GL_POLYGON;
	bool is_draw_path = false;

public:
	CObjectManager();
	~CObjectManager();

	void create_shape(clock_t frametime);

	void create_cutted_shape(CShape*);

	bool out_client(CShape*);

	bool collide_with_line(CShape*);

	void draw_path(CShape* shape);

	void draw();

	void update(clock_t frametime);

	void mouse_event(Event a_event, int mouse_x, int mouse_y);

	void key_event(Event a_event);

	float change_gl_coord_x(int value) {
		return (value / float(CLIENT_WIDTH) * 2) - 1;
	}

	float change_gl_coord_y(int value) {
		return -((value / float(CLIENT_HIEGHT) * 2) - 1);
	}

};

