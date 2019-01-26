#include "main.h"
#include "boomerang.h"

Boomerang::Boomerang(float x, float y) {
    // std::cout << "Boomerang" << std::endl;
    this->position = glm::vec3(x, y, 0);

    rotation = 0;
    x_speed = -0.1;
    x_acc = +0.001;
    y_speed = -0.007;
    visible = true;


    GLfloat g_vertex_buffer_data[] = {
        0.0f - 0.1, 0.0f + 0.2, 0.0f,
        0.0f, 0.0f + 0.2, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f + 0.1, 0.0f, 0.0f,
        0.0f, 0.0f + 0.2, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f - 0.1, 0.0f - 0.2, 0.0f,
        0.0f, 0.0f - 0.2, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f + 0.1, 0.0f, 0.0f,
        0.0f, 0.0f - 0.2, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 4*3, g_vertex_buffer_data, COLOR_BROWN, GL_FILL);
}

void Boomerang::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boomerang::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if (this->visible)
        draw3DObject(this->object);
}

bounding_box_t Boomerang::get_bounding_box() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y;
    b.width = 0.1;
    b.height = 0.1;

    return b;
}

void Boomerang::tick() {
    this->position.y += y_speed;
    this->x_speed += this->x_acc;
    this->position.x += x_speed;
    this->rotation = (this->rotation + 10);
    // if (x_speed > 0.01)
    //     this->visible = false;
}