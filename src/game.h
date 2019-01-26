#include "main.h"
#include "fire_lines.h"
#include "coin.h"
#include "joyrider.h"
#include "bg.h"
#include "fire_beams.h"
#include "boomerang.h"
#include "magnet.h"
#include "projectile.h"
#include "lines.h"
#include "tunnel.h"
#include "balloon.h"

#ifndef GAME_H
#define GAME_H

class Game {
public:
    // Game() {}
    Game();
    double frame_pos, speed;
    Background bg;
    int points;
    int lives;
    Joyrider j;
    std::vector<FireLine> lines;
    std::vector<Coin> coins;
    std::vector<FireBeam> beams;
    std::vector<Boomerang> boomerangs;
    std::vector<Magnet> magnets;
    std::vector<Projectile> projectiles;
    std::vector<Line> bricks;
    std::vector<Balloon> balloons;
    int next_balloon;
    Tunnel tun;
    void tick();
    void tick_input(int left, int right, int up, int down, int space);
private:
    float min_spawn_pos;
    float last_line;
};

#endif