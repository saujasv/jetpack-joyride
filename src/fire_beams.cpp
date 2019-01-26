#include "main.h"
#include "fire_beams.h"

#define N 100

FireBeam::FireBeam(float x, float y, float length, double speed, double upper_lim) {
    this->position = glm::vec3(x, y, 0);
    this->ball_positions[0] = glm::vec3(-length / 2, 0, 0);
    this->ball_positions[1] = glm::vec3(length / 2, 0, 0);

    rotation = 0;
    x_speed = 0;
    x_acc = 0;
    y_speed = speed;
    visible = true;
    this->length = length;
    this->upper_lim = y + upper_lim;
    this->lower_lim = y;


    static GLfloat g_vertex_buffer_data1[10 * N];

	float incr = 2 * M_PI / N;
	float angle = 0;
	GLfloat last_row[] = {this->ball_positions[0].x + 0.25f, this->ball_positions[0].y + 0.0f, 0.0f};
	GLfloat curr_row[3];

	for (int i = 0; i <= N; i++)
	{
		curr_row[0] = this->ball_positions[0].x + 0.25f * cos(angle);
		curr_row[1] = this->ball_positions[0].y + 0.25f * sin(angle);
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

    this->balls[0] = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data1, COLOR_BLACK, GL_FILL);

    angle = 0;
	last_row[0] = this->ball_positions[1].x + 0.25f;
    last_row[1] = this->ball_positions[1].y + 0.0f;
    last_row[2] = 0.0f;

    static GLfloat g_vertex_buffer_data2[10 * N];
    for (int i = 0; i <= N; i++)
	{
		curr_row[0] = this->ball_positions[1].x + 0.25f * cos(angle);
		curr_row[1] = this->ball_positions[1].y + 0.25f * sin(angle);
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

    this->balls[1] = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data2, COLOR_BLACK, GL_FILL);

    GLfloat g_vertex_buffer_data3[] = {
         - length / 2,  - 0.1, 0.0f,
         + length / 2,  - 0.1, 0.0f,
         + length / 2,  + 0.1, 0.0f,
         - length / 2,  - 0.1, 0.0f,
         - length / 2,  + 0.1, 0.0f,
         + length / 2,  + 0.1, 0.0f,
    };

    this->fire = create3DObject(GL_TRIANGLES, 2*3, g_vertex_buffer_data3, COLOR_BRIGHT_ORANGE, GL_FILL);
}

void FireBeam::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void FireBeam::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if (visible) {
        draw3DObject(this->fire);
        draw3DObject(this->balls[0]);
        draw3DObject(this->balls[1]);
    }
}

void FireBeam::tick_input(int left, int right, double rate) {
    this->position.x += (right - left) * rate;
}

bounding_box_t FireBeam::get_fire_bounding_box() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y;
    b.width = length;
    b.height = 0.2;

    return b;
}

bool FireBeam::tick() {
    this->position.y += y_speed;
    if (!(this->position.y < upper_lim && this->position.y > lower_lim)) {
        y_speed *= -1;
    }
}