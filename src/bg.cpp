#include "main.h"
#include "bg.h"

Background::Background() {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat roof_vertex_buffer[] = {
        -4.0f, 4.0f, 0.0f,
        -4.0f, 3.2f, 0.0f,
        4.0f, 3.2f, 0.0f,
        -4.0f, 4.0f, 0.0f,
        4.0f, 4.0f, 0.0f,
        4.0f, 3.2f, 0.0f
    };

    static const GLfloat wall_vertex_buffer[] = {
        -4.0f, 3.2f, 0.0f,
        -4.0f, -2.8f, 0.0f,
        4.0f, -2.8f, 0.0f,
        -4.0f, 3.2f, 0.0f,
        4.0f, 3.2f, 0.0f,
        4.0f, -2.8f, 0.0f,
    };

    static const GLfloat floor_vertex_buffer[] = {
        -4.0f, -4.0f, 0.0f,
        -4.0f, -2.8f, 0.0f,
        4.0f, -2.8f, 0.0f,
        -4.0f, -4.0f, 0.0f,
        4.0f, -4.0f, 0.0f,
        4.0f, -2.8f, 0.0f,
    };

    this->roof = create3DObject(GL_TRIANGLES, 2*3, roof_vertex_buffer, COLOR_BLACK, GL_FILL);
    this->wall = create3DObject(GL_TRIANGLES, 2*3, wall_vertex_buffer, COLOR_GRAY, GL_FILL);
    this->floor = create3DObject(GL_TRIANGLES, 2*3, floor_vertex_buffer, COLOR_DARK_GRAY, GL_FILL);
}

void Background::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Background::tick(double rate) {
    this->position.x += rate;
}

void Background::tick_input(int left, int right, double rate) {
    this->position.x += (right - left) * rate;
}

void Background::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scale = glm::scale(glm::vec3(4.0f, 4.0f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->roof);
    draw3DObject(this->floor);
    draw3DObject(this->wall);
}

bounding_box_t Background::roof_bounds() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y + 3.6;
    b.width = 8.0;
    b.height = 0.8;

    return b;
}

bounding_box_t Background::floor_bounds() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y - 3.4;
    b.width = 8.0;
    b.height = 1.2;

    return b;
}

bounding_box_t Background::wall_bounds() {
    bounding_box_t b;
    b.x = this->position.x;
    b.y = this->position.y;
    b.width = 8.0;
    b.height = 6.0;

    return b;
}