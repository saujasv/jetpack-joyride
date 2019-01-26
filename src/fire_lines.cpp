#include <vector>

#include "main.h"
#include "fire_lines.h"

#define N 100
#define F 40

FireLine::FireLine(float x1, float y1, float x2, float y2) {
    std::cout << "Fire Line" << std::endl;
    float x = (x2 - x1) / 2, y = (y2 - y1) / 2;
    this->position = glm::vec3(x, y, 0);
    this->ball_positions[0] = glm::vec3(this->position.x - x1, this->position.y - y1, 0);
    this->ball_positions[1] = glm::vec3(this->position.x - x2, this->position.y - y2, 0);
    
    x_inc = (this->ball_positions[1].x - this->ball_positions[0].x) / F;
    y_inc = (this->ball_positions[1].y - this->ball_positions[0].y) / F;
    rotation = 0;
    x_speed = 0;
    x_acc = 0;
    visible = true;

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

    this->balls[0] = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data1, COLOR_DARK_GRAY, GL_FILL);

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

    this->balls[1] = create3DObject(GL_TRIANGLES, (N + 1)*3, g_vertex_buffer_data2, COLOR_DARK_GRAY, GL_FILL);

    float p = x - x1, q = y - y1;
    for (int i = 0; i < F; i++) {
        this->boxes[i].x = p;
        this->boxes[i].y = q;
        this->boxes[i].width = 0.2;
        this->boxes[i].height = 0.2;
        fire_positions[i] = glm::vec3(p, q, 0);
        GLfloat g_vertex_buffer_data3[] = {
            p - 0.1f, q + 0.1f, 0.0f,
            p + 0.1f, q + 0.1f, 0.0f,
            p + 0.1f, q - 0.1f, 0.0f,
            p - 0.1f, q + 0.1f, 0.0f,
            p - 0.1f, q - 0.1f, 0.0f,
            p + 0.1f, q - 0.1f, 0.0f
        };
        this->fire[i] = create3DObject(GL_TRIANGLES, 2*3, g_vertex_buffer_data3, COLOR_ORANGE, GL_FILL);
        p += x_inc;
        q += y_inc;
    }
}

void FireLine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void FireLine::draw(glm::mat4 VP) {
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
        for (int i = 0; i < F; i++) {
            draw3DObject(this->fire[i]);
        }
        draw3DObject(this->balls[0]);
        draw3DObject(this->balls[1]);
    }
}

// void FireLine::tick_input(int left, int right, double rate) {
//     this->position.x += (right - left) * rate;
// }

bounding_box_t FireLine::get_fire_box(int i) {
    bounding_box_t b;
    b.x = -this->position.x + this->ball_positions[0].x + i * x_inc;
    b.y = -this->position.y + this->ball_positions[0].y + i * y_inc;
    b.width = 0.2;
    b.height = 0.2;
}

bool FireLine::check_collision(bounding_box_t b) {
   for (int i = 0; i < F; i++) {
        if (detect_collision(b, this->get_fire_box(i))) {
            std::cout << "DEAD" << std::endl;
            return true;
        }
   }

   return false;
}

// bool FireLine::tick() {
//     this->position.x -= x_speed;
//     return false;
// }