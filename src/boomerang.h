#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H

class Boomerang {
public:
    Boomerang(float x, float y);
    glm::vec3 position;
    float rotation;
    bool visible;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    // void tick_input(int left, int right, double rate);
    bounding_box_t get_bounding_box();
    double x_speed, y_speed;
    double x_acc;
private:
    VAO *object;
};

#endif