#include "main.h"

#ifndef JOYRIDER_H
#define JOYRIDER_H

class Joyrider {
public:
    Joyrider() {}
    Joyrider(float x, float y);
    glm::vec3 position;
    float rotation;
    bool firing;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool tick(bounding_box_t roof, bounding_box_t floor, bounding_box_t wall);
    void tick_input(int left, int right, int space, double rate);
    bounding_box_t get_bounding_box();
    double x_speed, y_speed, rot_speed;
    double x_acc, y_acc, rot_acc;
private:
    VAO *body, *head, *arm, *leg, *jetpack, *flame;
};

#endif