#include "main.h"

#ifndef LINES_H
#define LINES_H

class Line {
public:
    Line(float x);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *object;
};

#endif