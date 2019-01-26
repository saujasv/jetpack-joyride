#include "main.h"

#ifndef TUNNEL_H
#define TUNNEL_H

class Tunnel {
public:
    Tunnel() {}
    Tunnel(float x);
    glm::vec3 position;
    float rotation;
    bool visible;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t get_bounding_box();
private:
    VAO *circle;
    VAO *box;
    VAO *tun;
};

#endif