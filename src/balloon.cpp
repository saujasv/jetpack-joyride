#include "main.h"
#include "balloon.h"

#define N 100
Balloon::Balloon(float x, float y) {
    this->position = glm::vec3(x, y, 0);

    rotation = 0;
    x_speed = -0.04;
    y_speed = 0;
    y_acc = -GRAVITY;
    visible = true;
    this->type = type;

    color_t col;
    if (type == 0)
        col = COLOR_YELLOW;
    else if (type == 1)
        col = COLOR_PURPLE;

    static GLfloat g_vertex_buffer_data[10 * N];

	
	float incr = 2 * M_PI / N;
	float angle = 0;
	GLfloat last_row[] = {0.3f, 0.0f, 0.0f};
	GLfloat curr_row[3];

	for (int i = 0; i <= N; i++)
	{
		curr_row[0] = 0.3f * cos(angle);
		curr_row[1] = 0.3f * sin(angle);
		curr_row[2] = 0.0f;
		g_vertex_buffer_data[9 * i] = 0.0f;
		g_vertex_buffer_data[9 * i + 1] = 0.0f;
		g_vertex_buffer_data[9 * i + 2] = 0.0f;
		g_vertex_buffer_data[9 * i + 3] = last_row[0];
		g_vertex_buffer_data[9 * i + 4] = last_row[1];
		g_vertex_buffer_data[9 * i + 5] = last_row[2];
		g_vertex_buffer_data[9 * i + 6] = curr_row[0];
		g_vertex_buffer_data[9 * i + 7] = curr_row[1];
		g_vertex_buffer_data[9 * i + 8] = curr_row[2];
		last_row[0] = curr_row[0];
		last_row[1] = curr_row[1];
		last_row[2] = curr_row[2];
		angle += incr;
	}

    this->object = create3DObject(GL_TRIANGLES, (N + 1) * 3, g_vertex_buffer_data, COLOR_AQUA, GL_FILL);
}

void Balloon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Balloon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if (this->visible) {
        draw3DObject(this->object);
    }
}

bounding_box_t Balloon::get_bounding_box() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y;
    b.width = 0.3;
    b.height = 0.3;

    return b;
}

void Balloon::tick() {
    this->y_speed += this->y_acc;
    this->position.y += y_speed;
    // this->position.x += x_speed;
}