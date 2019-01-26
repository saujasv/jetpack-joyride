#include "main.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background {
public:
    Background();
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick(double rate);
    void tick_input(int left, int right, double rate);
    void set_position(float x, float y);
    bounding_box_t roof_bounds();
    bounding_box_t floor_bounds();
    bounding_box_t wall_bounds();
private:
    VAO *roof, *wall, *floor;
};

#endif