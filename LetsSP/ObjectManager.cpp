#include "ObjectManager.h"

using namespace std;

int get_world_coord_x(float value) {
	return (int)((value + 1) * float(CLIENT_WIDTH) / 2.0f);
}

int get_world_coord_y(float value) {
	return (int)((-value + 1) * float(CLIENT_HIEGHT) / 2.0f);
}

bool GetIntersectPoint(const POINT& AP1, const POINT& AP2,
	const POINT& BP1, const POINT& BP2, POINT* IP)
{
	double t;
	double s;
	double under = (BP2.y - BP1.y) * (AP2.x - AP1.x) - (BP2.x - BP1.x) * (AP2.y - AP1.y);
	if (under == 0) return false;

	double _t = (BP2.x - BP1.x) * (AP1.y - BP1.y) - (BP2.y - BP1.y) * (AP1.x - BP1.x);
	double _s = (AP2.x - AP1.x) * (AP1.y - BP1.y) - (AP2.y - AP1.y) * (AP1.x - BP1.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	IP->x = AP1.x + t * (double)(AP2.x - AP1.x); 
	IP->y = AP1.y + t * (double)(AP2.y - AP1.y);

	return true;
}

CObjectManager::CObjectManager()
{

	init_shader_program(&line_shader_id, "Line.vert", "Line.frag");
	glUseProgram(line_shader_id);

	glGenVertexArrays(1, &line_vao);
	glGenBuffers(1, &line_vbo_pos);

	glBindVertexArray(line_vao);
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo_pos);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	
	init_shader_program(&shape_shader_id, "Shape.vert", "Shape.frag");
	glUseProgram(shape_shader_id);

	glGenVertexArrays(1, &shape_vao);
	glGenBuffers(1, &shape_vbo_pos);
	glGenBuffers(1, &shape_vbo_clr);

	glBindVertexArray(shape_vao);
	glBindBuffer(GL_ARRAY_BUFFER, shape_vbo_pos);
	GLint pAttribute = glGetAttribLocation(shape_shader_id, "vPos");
	glVertexAttribPointer(pAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);
	
	glBindBuffer(GL_ARRAY_BUFFER, shape_vbo_clr);
	GLint cAttribute = glGetAttribLocation(shape_shader_id, "vColor");
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);

	transformLocation = glGetUniformLocation(shape_shader_id, "modelTransform");

}


CObjectManager::~CObjectManager()
{
}

void CObjectManager::create_shape(clock_t frametime)
{
	shape_gen_timer -= frametime;

	if (shape_gen_timer > 0) return;
	else {
		shape_gen_timer = SHAPE_GEN_TIME;
		CShape sh{ rand() % 3 + 3 };
		//CShape sh{ 6 };

		m_shapes.push_back(sh);
	}
}

void CObjectManager::create_cutted_shape(CShape* shape)
{
	POINTFLOAT sh1_pts[MAX_POLYGON];
	POINTFLOAT sh2_pts[MAX_POLYGON];
	memset(sh1_pts, 0, sizeof(POINTFLOAT) * MAX_POLYGON);
	memset(sh2_pts, 0, sizeof(POINTFLOAT) * MAX_POLYGON);

	int sh1_num = 0;

	sh1_pts[0].x = change_gl_coord_x(intersect_pt[0].x - get_world_coord_x(shape->get_center_pt().x) + 400);
	sh1_pts[0].y = change_gl_coord_y(intersect_pt[0].y - get_world_coord_y(shape->get_center_pt().y) + 300);

	for (int i = 1; i < MAX_POLYGON - 1; i++) {
		
		int index = (intersect_pos[0] + i) % shape->get_vert_num();
		sh1_pts[i] = shape->get_pts()[index];

		if (index == intersect_pos[1]) {
			
			sh1_pts[i + 1].x = change_gl_coord_x(intersect_pt[1].x - get_world_coord_x(shape->get_center_pt().x) + 400);
			sh1_pts[i + 1].y = change_gl_coord_y(intersect_pt[1].y - get_world_coord_y(shape->get_center_pt().y) + 300);
			sh1_num = i + 2;
			break;
		}
	}

	int sh2_num = 0;

	sh2_pts[0].x = change_gl_coord_x(intersect_pt[1].x - get_world_coord_x(shape->get_center_pt().x) + 400);
	sh2_pts[0].y = change_gl_coord_y(intersect_pt[1].y - get_world_coord_y(shape->get_center_pt().y) + 300);

	for (int i = 1; i < MAX_POLYGON - 1; i++) {
		int index = (intersect_pos[1] + i) % shape->get_vert_num();
		sh2_pts[i] = shape->get_pts()[index];

		if (index == intersect_pos[0]) {

			sh2_pts[i + 1].x = change_gl_coord_x(intersect_pt[0].x - get_world_coord_x(shape->get_center_pt().x) + 400);
			sh2_pts[i + 1].y = change_gl_coord_y(intersect_pt[0].y - get_world_coord_y(shape->get_center_pt().y) + 300);
			sh2_num = i + 2;
			break;
		}
	}

	cout << "sh1: " << sh1_num << endl;
	cout << "sh2: " << sh2_num << endl;

	CShape sh1{ shape, sh1_pts, sh1_num, 1};
	CShape sh2{ shape, sh2_pts, sh2_num, -1};

	m_shapes.push_back(sh1);
	m_shapes.push_back(sh2);

}

bool CObjectManager::out_client(CShape* shape)
{
	POINTFLOAT center = shape->get_center_pt();

	if (center.x > 1.2) return true;
	else if (center.x < -1.2) return true;
	else if (center.y > 1.2) return true;
	else if (center.y < -1.2) return true;

	return false;
}

bool CObjectManager::collide_with_line(CShape* shape)
{

	if (m_line == nullptr) return false;
	if (shape->get_vert_num() > MAX_POLYGON - 1) return false;	// max_polygon - 1까지만 자름


	POINTFLOAT center = shape->get_center_pt();
	POINT world_c = { get_world_coord_x(center.x), get_world_coord_y(center.y) };

	POINTFLOAT pts[MAX_POLYGON];
	for (int i = 0; i < shape->get_vert_num(); i++) {
		pts[i] = shape->get_pts()[i];
		world_pts[i] = { get_world_coord_x(pts[i].x) + world_c.x - 400, get_world_coord_y(pts[i].y) + world_c.y - 300};
	}


	int collidsion_line = 0;
	m_line->get_point(&line_pts[0], &line_pts[1]);


	for (int i = 1; i <= shape->get_vert_num(); i++) {
		if (GetIntersectPoint(line_pts[0], line_pts[1], world_pts[i - 1], world_pts[i % shape->get_vert_num()], 
			&intersect_pt[collidsion_line])) 
		{
			intersect_pos[collidsion_line] = i - 1;
			collidsion_line++;
		}
	}
	//cout << collidsion_line << endl;
	if (collidsion_line < 2) return false;
	else return true;
}

void CObjectManager::draw_path(CShape* shape)
{
	POINT* tmp = shape->get_control_point();
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++) {
		glVertex2f(change_gl_coord_x(tmp[i].x), change_gl_coord_y(tmp[i].y));
	}
	glEnd();

}


void CObjectManager::draw()
{
	glUseProgram(shape_shader_id);
	glBindVertexArray(shape_vao);

	for (list<CShape>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++) {
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(it->get_transfrom_matrix()));
		it->draw(&shape_vbo_pos, &shape_vbo_clr, draw_mode);


	}

	glUseProgram(line_shader_id);
	glBindVertexArray(line_vao);


	//glBegin(GL_POINTS);
	//for (int i = 0; i < 2; i++) {
	//	glVertex2f(change_gl_coord_x(intersect_pt[i].x), change_gl_coord_y(intersect_pt[i].y));
	//}
	//glEnd();
	if (is_draw_path) {
		for (list<CShape>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++) {
			glBegin(GL_POINTS);
			for (int i = 0; i < 4; i++) {
				glVertex2f(change_gl_coord_x(it->get_control_point()[i].x), change_gl_coord_y(it->get_control_point()[i].y));

			}
			glEnd();
		}
	}
	if (m_line != nullptr)
		m_line->draw(&line_vbo_pos);


}

void CObjectManager::update(clock_t frametime)
{
	if (m_line != nullptr)
		m_line->update();

	for (list<CShape>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++) {
		it->update(frametime);
	}

	create_shape(frametime);

	for (list<CShape>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++) {
		if (!is_cut) {
			// 사각형 까지만 잘림
			if (collide_with_line(&(*it))) {
				is_cut = true;
				create_cutted_shape(&(*it));
				it = m_shapes.erase(it);
				cout << "잘림!" << endl;
				break;
			}
		}

		if (out_client(&(*it))) {
			it = m_shapes.erase(it);
			cout << "밖에 나감!" << endl;
			break;
		}
		
	}

}

void CObjectManager::mouse_event(Event a_event, int mouse_x, int mouse_y)
{
	switch (a_event) {
	case LMOUSE_DOWN:
		m_line = new CLine(mouse_x, mouse_y);
		is_cut = false;
		cout << "instance line" << endl;
		break;
	case LMOUSE_UP:
		delete m_line;
		m_line = nullptr;
		cout << "delete line" << endl;
		break;
	case MOUSE_MOVE:
		if (m_line != nullptr)
			m_line->mouse_event(mouse_x, mouse_y);
		break;
	}
}

void CObjectManager::key_event(Event a_event)
{
	switch (a_event) {
	case A_KEY_DOWN:
		// 속도 업
		for (list<CShape>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++) {
			it->speed_up();
		}

		break;
	case S_KEY_DOWN:
		// 속도 다운
		for (list<CShape>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++) {
			it->speed_down();
		}
		break;
	case Z_KEY_DOWN:
		// 도형 그리기 모드 바꾸기
		if (draw_mode == GL_POLYGON)
			draw_mode = GL_LINE_LOOP;
		else draw_mode = GL_POLYGON;
		break;
	case X_KEY_DOWN:
		if (is_draw_path) is_draw_path = false;
		else is_draw_path = true;
		cout << "경로" << endl;
		break;
	}

}
