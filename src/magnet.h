#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet {
public:
    Magnet(float x, float y);
    glm::vec3 position;
    glm::vec3 ball_positions[2];
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
    VAO *balls[2];
    VAO *bars;
};

#endif