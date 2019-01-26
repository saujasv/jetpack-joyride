#include "main.h"

#ifndef FIREBEAM_H
#define FIREBEAM_H

class FireBeam {
public:
    FireBeam(float x, float y, float length, double speed, double upper_lim);
    glm::vec3 position;
    glm::vec3 ball_positions[2];
    // float x_inc, y_inc;
    float rotation;
    bool firing;
    bool visible;
    float length;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool tick();
    void tick_input(int left, int right, double rate);
    bounding_box_t get_fire_bounding_box();
    double x_speed, y_speed, upper_lim, lower_lim;
    double x_acc;
private:
    VAO *balls[2];
    VAO *fire;
};

#endif