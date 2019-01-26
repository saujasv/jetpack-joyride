#include "main.h"

#ifndef BALLOON_H
#define BALLOON_H

class Balloon {
public:
    Balloon(float x, float y);
    glm::vec3 position;
    glm::vec3 ball_positions[2];
    float rotation;
    bool visible;
    int type;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    // void tick_input(int left, int right, double rate);
    bounding_box_t get_bounding_box();
    double x_speed, y_speed;
    double y_acc;
private:
    VAO *object;
};

#endif