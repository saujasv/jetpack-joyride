#include "main.h"
#include "tunnel.h"

#define N 100

Tunnel::Tunnel(float x) {
    this->position = glm::vec3(x, 0, 0);

    rotation = 0;

    static GLfloat g_vertex_buffer_data1[10 * N];

	float incr = M_PI / N;
	float angle = 0;
	GLfloat last_row[] = {2.915f, -2.8f, 0.0f};
	GLfloat curr_row[3];

	for (int i = 0; i <= N; i++)
	{
		curr_row[0] = 2.915f * cos(angle);
		curr_row[1] = -2.8f + 2.915f * sin(angle);
		curr_row[2] = 0.0f;
		g_vertex_buffer_data1[9 * i] = 0;
		g_vertex_buffer_data1[9 * i + 1] = -2.8f;
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

    this->circle = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data1, COLOR_RED, GL_FILL);

    GLfloat g_vertex_buffer_data2[] = {
        x - 2.915f, 0.3f, 0.0f,
        x + 2.915f, -2.2f, 0.0f,
        x + 2.915f, 0.3f, 0.0f,
        x - 2.915f, 0.3f, 0.0f,
        x + 2.915f, -2.2f, 0.0f,
        x - 2.915f, -2.2f, 0.0f,
    };

    this->box = create3DObject(GL_TRIANGLES, 2*3, g_vertex_buffer_data2, COLOR_GRAY, GL_FILL);

    GLfloat g_vertex_buffer_data3[] = {
         - 3.5f, -2.8f, 0.0f,
         + 3.5f, 3.2f, 0.0f,
         + 3.5f, -2.8f, 0.0f,
         - 3.5f, -2.8f, 0.0f,
         + 3.5f, 3.2f, 0.0f,
         - 3.5f, 3.2f, 0.0f,
    };

    this->tun = create3DObject(GL_TRIANGLES, 2*3, g_vertex_buffer_data3, COLOR_GREEN, GL_FILL);
}

void Tunnel::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Tunnel::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->tun);
    draw3DObject(this->circle);
    // draw3DObject(this->box);
}

bounding_box_t Tunnel::get_bounding_box() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = 0.2;
    b.width = 2 * 3.5;
    b.height = 6;

    return b;
}

void Tunnel::tick() {
    return;
}