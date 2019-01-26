#include <vector>

#include "main.h"

#ifndef FIRELINE_H
#define FIRELINE_H

#define F 40

class FireLine {
public:
    FireLine() {}
    FireLine(float x1, float y1, float x2, float y2);
    glm::vec3 position;
    glm::vec3 ball_positions[2];
    glm::vec3 fire_positions[F];
    float x_inc, y_inc;
    float rotation;
    bool firing;
    bool visible;
    bounding_box_t boxes[F];
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    // bool tick();
    // void tick_input(int left, int right, double rate);
    bool check_collision(bounding_box_t b);
    double x_speed;
    double x_acc;
private:
    VAO *balls[2];
    VAO *fire[F];
    bounding_box_t get_fire_box(int i);
};

#endif