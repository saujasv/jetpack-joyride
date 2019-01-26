#include "main.h"
#include "projectile.h"

Projectile::Projectile(float x, float y, int type) {
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

    GLfloat box_vertex_buffer[] = {
        -0.15f, 0.15f, 0.0f,
        0.15f, 0.15f, 0.0f,
        0.15f, -0.15f, 0.0f,
        -0.15f, 0.15f, 0.0f,
        -0.15f, -0.15f, 0.0f,
        0.15f, -0.15f, 0.0f,
    };

    this->box = create3DObject(GL_TRIANGLES, 2*3, box_vertex_buffer, col, GL_FILL);

    GLfloat handle_vertex_buffer[] = {
        0.0f, 0.1f, 0.0f,
        0.1f, 0.1f, 0.0f,
        0.1f, 0.0f, 0.0f
    };

    this->handle = create3DObject(GL_TRIANGLES, 1*3, handle_vertex_buffer, COLOR_GRAY, GL_FILL);

    GLfloat cap_vertex_buffer[] = {
        -0.05f, 0.15f, 0.0f,
        -0.08f, 0.15f, 0.0f,
        -0.11f, 0.18f, 0.0f
    };

    this->cap = create3DObject(GL_TRIANGLES, 1*3, cap_vertex_buffer, COLOR_BRIGHT_ORANGE, GL_FILL);
}

void Projectile::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Projectile::draw(glm::mat4 VP) {
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
        draw3DObject(this->box);
        draw3DObject(this->handle);
        draw3DObject(this->cap);
    }
}

bounding_box_t Projectile::get_bounding_box() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y;
    b.width = 0.3;
    b.height = 0.3;

    return b;
}

void Projectile::tick() {
    this->y_speed += this->y_acc;
    this->position.y += y_speed;
    this->position.x += x_speed;
}