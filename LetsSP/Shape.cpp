#include "Shape.h"

using namespace std;



CShape::CShape(int a_vertex_num)
{
	vertex_num = a_vertex_num;

	bool buf[8];
	memset(buf, false, sizeof(bool) * 8);

	for (int i = 0; i < vertex_num; i++) {
		int index;
		while (true) {
			index = rand() % 8;
			if (!buf[index]) {
				buf[index] = true;
				break;
			}
		}
	}

	int index = 0;
	for (int i = 0; i < 8; i++) {
		if (buf[i]) {
			position_buffer[index * 2 + 0] = f_init_pts[i].x;
			position_buffer[index * 2 + 1] = f_init_pts[i].y;
			points[index] = f_init_pts[i];
			index++;
			//cout << i << endl;
		}
	}

	set_color();

	set_control_points();

	center.x = change_gl_coord_x(path_control_point[0].x);
	center.y = change_gl_coord_y(path_control_point[0].y);

	transform_matrix = glm::translate(glm::mat4(1),
		glm::vec3(center.x, center.y, 0));

	cout << "Åº»ý!" << endl;
}

CShape::CShape(CShape* mother, POINTFLOAT* init_pts, int num, int dir)
{
	vertex_num = num;

	// µÑ ´Ù gl ÁÂÇ¥ÀÓ
	for (int i = 0; i < num; i++)
	{
		points[i] = init_pts[i];
		position_buffer[i * 2 + 0] = points[i].x;
		position_buffer[i * 2 + 1] = points[i].y;
	}

	memcpy(color_buffer, mother->get_color_buf(), sizeof(float) * 15);

	POINTFLOAT mother_center = mother->get_center_pt();
	center.x = mother_center.x;
	center.y = mother_center.y;

	t = dir;


	transform_matrix = glm::translate(glm::mat4(1),
		glm::vec3(center.x, center.y, 0));

	cutted = true;

	cout << "¾Æ±â Åº»ý!" << endl;
}


CShape::~CShape()
{

}

void CShape::set_color()
{
	float R = float(rand()) / float(RAND_MAX) / 2 + 0.5;
	float G = float(rand()) / float(RAND_MAX) / 2 + 0.5;
	float B = float(rand()) / float(RAND_MAX) / 2 + 0.5;

	for (int i = 0; i < MAX_POLYGON; i++) {
		color_buffer[i * 3 + 0] = R;
		color_buffer[i * 3 + 1] = G;
		color_buffer[i * 3 + 2] = B;
	}
}

void CShape::set_control_points()
{
	if (rand() % 2 == 0){
		for (int i = 0; i < 4; i++) {
			path_control_point[i].x = 250 * i + 10;
			path_control_point[i].y = rand() % 400 + 100;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			path_control_point[i].x = 250 * (3 - i) - 10;
			path_control_point[i].y = rand() % 400 + 100;
		}
	}
}




void CShape::update(clock_t frametime)
{
	if (!cutted) {
		POINTFLOAT tmp[4];
		for (int i = 0; i < 4; i++) {
			tmp[i] = { change_gl_coord_x(path_control_point[i].x), change_gl_coord_y(path_control_point[i].y) };
		}

		if (interval == 0) {
			center.x = (2 * (t * t) - 3 * t + 1) * tmp[0].x + (-4 * (t * t) + 4 * t) * tmp[1].x + (2 * (t * t) - t) * tmp[2].x;
			center.y = (2 * (t * t) - 3 * t + 1) * tmp[0].y + (-4 * (t * t) + 4 * t) * tmp[1].y + (2 * (t * t) - t) * tmp[2].y;
			t += (float)frametime / 2000.0 / 2.0 * xspeed;

			if (t > 0.5) {
				interval++;
				t = 0;
			}
		}

		else if (interval == 1) {
			center.x = ((-(t * t * t) + 2 * (t * t) - t) * tmp[0].x + (3 * (t * t * t) - 5 * (t * t) + 2) * tmp[1].x + (-3 * (t * t * t) + 4 * (t * t) + t) * tmp[2].x + ((t * t * t) - (t * t)) * tmp[3].x) / 2;
			center.y = ((-(t * t * t) + 2 * (t * t) - t) * tmp[0].y + (3 * (t * t * t) - 5 * (t * t) + 2) * tmp[1].y + (-3 * (t * t * t) + 4 * (t * t) + t) * tmp[2].y + ((t * t * t) - (t * t)) * tmp[3].y) / 2;
			t += (float)frametime / 2000.0 * xspeed;

			if (t > 1) {
				interval++;
				t = 0.5;
			}
		}
		else {
			center.x = (2 * (t * t) - 3 * t + 1) * tmp[1].x + (-4 * (t * t) + 4 * t) * tmp[2].x + (2 * (t * t) - t) * tmp[3].x;
			center.y = (2 * (t * t) - 3 * t + 1) * tmp[1].y + (-4 * (t * t) + 4 * t) * tmp[2].y + (2 * (t * t) - t) * tmp[3].y;
			t += (float)frametime / 2000.0 / 2.0 * xspeed;
		}
	}
	else {
		center.x -= t * ((float)frametime / 5000.0);
		center.y -= (float)frametime / 2000.0 * xspeed;	// Áß·Â

	}


	transform_matrix = glm::translate(glm::mat4(1),
		glm::vec3(center.x, center.y, 0));


}

void CShape::draw(GLuint* vbo_pos, GLuint* vbo_clr, GLenum draw_mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position_buffer), position_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_clr);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer), color_buffer, GL_STATIC_DRAW);

	glDrawArrays(draw_mode, 0, vertex_num);

}

void CShape::speed_up()
{
	if (xspeed < 1.5) xspeed += 0.1;
}

void CShape::speed_down()
{
	if (xspeed > 0.5) xspeed -= 0.1;
}
