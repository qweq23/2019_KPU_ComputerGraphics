#pragma once
#include "pch.h"
#include "vector"

#define MAX_POLYGON 6

using namespace glm;

const POINTFLOAT f_init_pts[8] = {
	{ cos(radians(float(45 * 0))) / 5.0, sin(radians(float(45 * 0))) / 5.0 },
	{ cos(radians(float(45 * 1))) / 5.0, sin(radians(float(45 * 1))) / 5.0 },
	{ cos(radians(float(45 * 2))) / 5.0, sin(radians(float(45 * 2))) / 5.0 },
	{ cos(radians(float(45 * 3))) / 5.0, sin(radians(float(45 * 3))) / 5.0 },
	{ cos(radians(float(45 * 4))) / 5.0, sin(radians(float(45 * 4))) / 5.0 },
	{ cos(radians(float(45 * 5))) / 5.0, sin(radians(float(45 * 5))) / 5.0 },
	{ cos(radians(float(45 * 6))) / 5.0, sin(radians(float(45 * 6))) / 5.0 },
	{ cos(radians(float(45 * 7))) / 5.0, sin(radians(float(45 * 7))) / 5.0 },

};

const POINT i_init_pts[8] = {
	{ cos(radians(float(45 * 0))) * 30, sin(radians(float(45 * 0))) * 20 },
	{ cos(radians(float(45 * 1))) * 30, sin(radians(float(45 * 1))) * 20 },
	{ cos(radians(float(45 * 2))) * 30, sin(radians(float(45 * 2))) * 20 },
	{ cos(radians(float(45 * 3))) * 30, sin(radians(float(45 * 3))) * 20 },
	{ cos(radians(float(45 * 4))) * 30, sin(radians(float(45 * 4))) * 20 },
	{ cos(radians(float(45 * 5))) * 30, sin(radians(float(45 * 5))) * 20 },
	{ cos(radians(float(45 * 6))) * 30, sin(radians(float(45 * 6))) * 20 },
	{ cos(radians(float(45 * 7))) * 30, sin(radians(float(45 * 7))) * 20 },

};


class CShape
{
	int vertex_num;
	float position_buffer[2 * MAX_POLYGON] = { 0, }; // 5각형까지
	float color_buffer[3 * MAX_POLYGON] = {1,};

	POINTFLOAT center;
	POINTFLOAT points[MAX_POLYGON];


	glm::mat4 transform_matrix = glm::mat4(1.0f);
	
	POINTFLOAT speed = { -0.2, -0.2 };
	float xspeed = 1.0;

	POINT path_control_point[4];
	float t = 0;
	int interval = 0;

	bool cutted = false;

public:
	CShape(int a_vertex_num);
	CShape(CShape* mother, POINTFLOAT*, int num, int dir);
	~CShape();

	float* get_color_buf() {
		return color_buffer;
	}

	POINT* get_control_point() {
		return path_control_point;
	}

	POINTFLOAT get_center_pt() {
		return center;
	}

	POINTFLOAT* get_pts() {
		return points;
	}

	void set_color();
	
	void set_control_points();

	void update(clock_t frametime);

	void draw(GLuint*, GLuint*, GLenum);

	void draw_path() {
		glBegin(GL_POINTS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(change_gl_coord_x(path_control_point[i].x), change_gl_coord_x(path_control_point[i].y));
		}
		glEnd();
	}

	glm::mat4 get_transfrom_matrix() {
		return transform_matrix;
	}

	int get_vert_num() {
		return vertex_num;
	}

	float change_gl_coord_x(int value) {
		return (value / float(CLIENT_WIDTH) * 2) - 1;
	}

	float change_gl_coord_y(int value) {
		return -((value / float(CLIENT_HIEGHT) * 2) - 1);
	}

	void speed_up();
	void speed_down();
	
};

