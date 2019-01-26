#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y);
    glm::vec3 position;
    float rotation;
    int type;
    int reward;
    float x_speed, x_acc;
    bool claimed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(bounding_box_t joyrider);
    void tick_input(int left, int right, double rate);
    bounding_box_t get_bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
