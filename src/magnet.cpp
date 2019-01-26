#include "main.h"
#include "magnet.h"

#define N 100

Magnet::Magnet(float x, float y) {
    this->position = glm::vec3(x, y, 0);

    rotation = 0;
    x_speed = -0.03;
    x_acc = +0.001;
    y_speed = -0.02;
    visible = true;


    static GLfloat g_vertex_buffer_data1[10 * N];

	float incr = M_PI / N;
	float angle = -M_PI/2;
	GLfloat last_row[] = {0.3f, 0.0f, 0.0f};
	GLfloat curr_row[3];

	for (int i = 0; i <= N; i++)
	{
		curr_row[0] = this->ball_positions[0].x + 0.3f * cos(angle);
		curr_row[1] = this->ball_positions[0].y + 0.3f * sin(angle);
		curr_row[2] = 0.0f;
		g_vertex_buffer_data1[9 * i] = this->ball_positions[0].x;
		g_vertex_buffer_data1[9 * i + 1] = this->ball_positions[0].y;
		g_vertex_buffer_data1[9 * i + 2] = 0.0f;
		g_vertex_buffer_data1[9 * i + 3] = last_row[0];
		g_vertex_buffer_data1[9 * i + 4] = last_row[1];
		g_vertex_buffer_data1[9 * i + 5] = last_row[2];
		g_vertex_buffer_data1[9 * i + 6] = curr_row[0];
		g_vertex_buffer_data1[9 * i + 7] = curr_row[1];
		g_vertex_buffer_data1[9 * i + 8] = curr_row[2];
		last_row[0] = curr_row[0];
		last_row[1] = curr_row[1];
		last_row[2] = curr_row[2];
		angle += incr;
	}

    this->balls[0] = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data1, COLOR_RED, GL_FILL);

    angle = -M_PI/2;
	last_row[0] = 0.15f;
    last_row[1] = 0.0f;
    last_row[2] = 0.0f;

    static GLfloat g_vertex_buffer_data2[10 * N];
    for (int i = 0; i <= N; i++)
	{
		curr_row[0] = this->ball_positions[1].x + 0.15f * cos(angle);
		curr_row[1] = this->ball_positions[1].y + 0.15f * sin(angle);
		curr_row[2] = 0.0f;
		g_vertex_buffer_data2[9 * i] = this->ball_positions[1].x;
		g_vertex_buffer_data2[9 * i + 1] = this->ball_positions[1].y;
		g_vertex_buffer_data2[9 * i + 2] = 0.0f;
		g_vertex_buffer_data2[9 * i + 3] = last_row[0];
		g_vertex_buffer_data2[9 * i + 4] = last_row[1];
		g_vertex_buffer_data2[9 * i + 5] = last_row[2];
		g_vertex_buffer_data2[9 * i + 6] = curr_row[0];
		g_vertex_buffer_data2[9 * i + 7] = curr_row[1];
		g_vertex_buffer_data2[9 * i + 8] = curr_row[2];
		last_row[0] = curr_row[0];
		last_row[1] = curr_row[1];
		last_row[2] = curr_row[2];
		angle += incr;
	}

    this->balls[1] = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data2, COLOR_GRAY, GL_FILL);

    GLfloat g_vertex_buffer_data3[] = {
        -0.3f, 0.3f, 0.0f,
        0.0f, 0.3f, 0.0f,
        0.0f, 0.15f, 0.0f,
        -0.3f, 0.3f, 0.0f,
        -0.3f, 0.15f, 0.0f,
        0.0f, 0.15f, 0.0f,
        -0.3f, -0.3f, 0.0f,
        0.0f, -0.3f, 0.0f,
        0.0f, -0.15f, 0.0f,
        -0.3f, -0.3f, 0.0f,
        -0.3f, -0.15f, 0.0f,
        0.0f, -0.15f, 0.0f,
    };

    this->bars = create3DObject(GL_TRIANGLES, 4*3, g_vertex_buffer_data3, COLOR_BRIGHT_ORANGE, GL_FILL);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->balls[0]);
    draw3DObject(this->balls[1]);
    draw3DObject(this->bars);
}

bounding_box_t Magnet::get_bounding_box() {
    bounding_box_t b;
    b.x = - 0.15;
    b.y = 0;
    b.width = 0.9;
    b.height = 2.0;

    return b;
}

void Magnet::tick() {
    this->position.y += y_speed;
    this->x_speed += this->x_acc;
    this->position.x += x_speed;
    this->rotation = (this->rotation + 10);
    if (x_speed > 0.01)
        this->visible = false;
}