#include "main.h"
#include "joyrider.h"

Joyrider::Joyrider(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    rot_speed = 0;
    x_speed = 0;
    y_speed = 0;
    x_acc = -0.001;
    y_acc = -GRAVITY;
    rot_acc = 0;
    firing = false;

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat body_vertex_buffer[] = {
        0.18f, 0.0f, 0.0f,
        0.18f, 0.4f, 0.0f, 
        -0.18f, 0.4f, 0.0f,
        -0.18f, 0.0f, 0.0f,
        -0.18f, 0.4f, 0.0f, 
        0.18f,  0.0f, 0.0f
    };

    static const GLfloat head_vertex_buffer[] = {
        -0.15f, 0.4f, 0.0f,
        -0.15f, 0.6f, 0.0f,
        0.15f, 0.4f, 0.0f,
        0.15f, 0.4f, 0.0f,
        -0.15f, 0.6f, 0.0f,
        0.15f, 0.6f, 0.0f
    };

    static const GLfloat arm_vertex_buffer[] = {
        0.06f, 0.06f, 0.0f,
        -0.06f, 0.06f, 0.0f,
        -0.06f, 0.21f, 0.0f,
        -0.06f, 0.21f, 0.0f,
        0.06f, 0.21f, 0.0f,
        0.06f, 0.06f, 0.0f
    };

    static const GLfloat leg_vertex_buffer[] = {
        -0.15f, 0.0f, 0.0f,
        0.15f, 0.0f, 0.0f,
        0.15f, -0.6f, 0.0f,
        -0.15f, 0.0f, 0.0f,
        0.15f, -0.6f, 0.0f,
        -0.15f, -0.6f, 0.0f
    };

    static const GLfloat jetpack_vertex_buffer[] = {
        -0.18f, 0.06f, 0.0f,
        -0.36f, 0.06f, 0.0f,
        -0.36f, 0.42f, 0.0f,
        -0.18f, 0.06f, 0.0f,
        -0.36f, 0.42f, 0.0f,
        -0.18f, 0.42f, 0.0f
    };

    static const GLfloat flame_vertex_buffer[] = {
        -0.24f,  -0.06f, 0.0f,
        -0.3f,  -0.06f, 0.0f,
        -0.3f,  0.06f, 0.0f,
        -0.3f,  0.06f, 0.0f,
        -0.24f,  -0.06f, 0.0f,
        -0.24f,  0.06f, 0.0f
    };

    this->body = create3DObject(GL_TRIANGLES, 2*3, body_vertex_buffer, COLOR_RED, GL_FILL);
    this->head = create3DObject(GL_TRIANGLES, 2*3, head_vertex_buffer, COLOR_BROWN, GL_FILL);
    this->arm = create3DObject(GL_TRIANGLES, 2*3, arm_vertex_buffer, COLOR_BROWN, GL_FILL);
    this->leg = create3DObject(GL_TRIANGLES, 2*3, leg_vertex_buffer, COLOR_BLUE, GL_FILL);
    this->jetpack = create3DObject(GL_TRIANGLES, 2*3, jetpack_vertex_buffer, COLOR_BLACK, GL_FILL);
    this->flame = create3DObject(GL_TRIANGLES, 2*3, flame_vertex_buffer, COLOR_ORANGE, GL_FILL);
}

void Joyrider::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Joyrider::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->body);
    draw3DObject(this->head);
    draw3DObject(this->arm);
    draw3DObject(this->leg);
    draw3DObject(this->jetpack);
    if (this->firing)
        draw3DObject(this->flame);
}

void Joyrider::tick_input(int left, int right, int space, double rate) {
    if (space)
        y_speed = 0.02;

    if (space || left || right)
        this->firing = true;
    else
        this->firing = false;

    this->position.x += rate;
}

bounding_box_t Joyrider::get_bounding_box() {
    bounding_box_t b;
    b.x = this->position.x - 0.09;
    b.y = this->position.y;
    b.width = 0.54;
    b.height = 1.2;

    return b;
}

bool Joyrider::tick(bounding_box_t roof, bounding_box_t floor, bounding_box_t wall) {
    bounding_box_t b = get_bounding_box();
    if (detect_collision(b, roof) && y_speed >= 0) {
        y_speed = 0;
    }
    else if (detect_collision(b, floor) && y_speed <= 0) {
        y_speed = 0;
        return true;
    }

    // if (!detect_collision(b, wall)) {
    //     x_speed *= -1;
    // }

    // else if (x_speed > 0)
    //     x_speed += x_acc;
    // else if (x_speed < 0)
    //     x_speed -= x_acc;

    y_speed += y_acc;

    // this->position.x += x_speed;
    this->position.y += y_speed;

    return false;
}