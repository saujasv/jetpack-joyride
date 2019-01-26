#include "main.h"
#include "lines.h"

Line::Line(float x) {
    this->position = glm::vec3(x, -3.4, 0);

    rotation = 0;

    GLfloat box_vertex_buffer[] = {
         -0.1f, -0.6f, 0.0f,
         +0.1f, -0.6f, 0.0f,
         +0.1f, 0.6f, 0.0f,
         -0.1f, -0.6f, 0.0f,
         -0.1f, 0.6f, 0.0f,
         +0.1f, 0.6f, 0.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, box_vertex_buffer, COLOR_BLACK, GL_FILL);
}

void Line::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Line::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 scaling = glm::scale(glm::vec3(0.7f, 0.7f, 1.0f));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}