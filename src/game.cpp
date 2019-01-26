#include <vector>

#include "main.h"
#include "fire_lines.h"
#include "coin.h"
#include "joyrider.h"
#include "game.h"
#include "bg.h"
#include "fire_beams.h"
#include "boomerang.h"
#include "projectile.h"
#include "lines.h"
#include "tunnel.h"
#include "balloon.h"

Game::Game() {
    frame_pos = 0;
    points = 0;
    lives = 3;
    this->bg = Background();
    this->j = Joyrider(-1.5f, -1.5f);
    this->speed = 0.01;
    this->min_spawn_pos = 6.0;
    this->bricks.push_back(-4.0f);
    this->bricks.push_back(-3.0f);
    this->bricks.push_back(-2.0f);
    this->bricks.push_back(-1.0f);
    this->bricks.push_back(0.0f);
    this->bricks.push_back(1.0f);
    this->bricks.push_back(2.0f);
    this->bricks.push_back(3.0f);
    this->bricks.push_back(4.0f);
    last_line = 4;
    next_balloon = 0;
    this->tun = Tunnel(20);
}

void Game::tick() {
    bounding_box_t joy_b = this->j.get_bounding_box();

    if (next_balloon > 0)
        next_balloon--;

    for (int i = 0; i < this->coins.size(); i++) {
        if (!this->coins[i].claimed && 
            detect_collision(joy_b, this->coins[i].get_bounding_box())) {
            this->coins[i].claimed = true;
            this->points += coins[i].reward;
        }
    }

    // for (int i = 0; i < this->lines.size(); i++) {
    //     bool dead = false;
    //     if (lines[i].visible) {
    //         for (int j = 0; j < 40; j++) {
    //             if (detect_collision(joy_b, lines[i].boxes[j])) {
    //                 // this->lives--;
    //                 // lines[i].visible = false;
    //                 dead = true;
    //                 break;
    //             }
    //         }
    //         if (dead)
    //             continue;
    //     }
    // }

    for (int i = 0; i < this->beams.size(); i++) {
        beams[i].tick();
        if (beams[i].visible && detect_collision(joy_b, beams[i].get_fire_bounding_box())) {
            if (!detect_collision(joy_b, this->tun.get_bounding_box())) {
                this->lives--;
                beams[i].visible = false;
                continue;
            }
        }
    }

    // std::cout << this->boomerangs.size() << std::endl;
    for (int i = 0; i < this->boomerangs.size(); i++) {
        this->boomerangs[i].tick();
        if (this->boomerangs[i].visible &&
            detect_collision(this->boomerangs[i].get_bounding_box(), joy_b)) {
                if (!detect_collision(joy_b, this->tun.get_bounding_box())) {
                    this->boomerangs[i].visible = false;
                    this->points -= 10;
                }
        }

        if (detect_collision(this->bg.floor_bounds(), this->boomerangs[i].get_bounding_box())) {
            boomerangs[i].visible = false;
        }
    }

    // this->j.y_acc = -GRAVITY;
    // int n_magnets = 0;
    // float diff = 0;
    // for (int i = 0; i < this->magnets.size(); i++) {
    //     if (this->magnets[i].visible &&
    //         this->magnets[i].position.x < frame_pos + 4 && 
    //         this->magnets[i].position.x > frame_pos - 4) {
    //         n_magnets++;
    //         diff += (this->j.position.y - magnets[i].position.y) * 0.05;
    //     }
    // }
    // this->speed += n_magnets * 0.004;
    // this->j.y_acc += diff;
    // this->j.position.x += n_magnets * 0.004;


    for (int i = 0; i < this->projectiles.size(); i++) {
        if (projectiles[i].visible && 
            detect_collision(joy_b, this->projectiles[i].get_bounding_box())) {
            projectiles[i].visible = false;
            if (projectiles[i].type == 0)
                this->points += 10;
            else if (projectiles[i].type == 1)
                this->lives += 1;
        }

        if (detect_collision(this->bg.floor_bounds(), this->projectiles[i].get_bounding_box())) {
            projectiles[i].visible = false;
        }

        this->projectiles[i].tick();
    }

    for (int i = 0; i < this->balloons.size(); i++) {
        this->balloons[i].tick();
        if (detect_collision(this->bg.floor_bounds(), this->balloons[i].get_bounding_box())) {
            this->balloons[i].visible = false;
        }

        for (int j = 0; j < this->beams.size(); j++) {
            if (fabs(this->beams[j].position.x - this->balloons[i].position.x) < 5) {
                if ((detect_collision(this->beams[j].get_fire_bounding_box(), this->balloons[i].get_bounding_box()))) {
                    this->balloons[i].visible = false;
                    this->beams[j].visible = false;
                    this->points += 20;
                }
            }
        }
    }

    if (frame_pos + 4 - last_line > 1) {
        this->bricks.push_back(Line(last_line + 1));
        last_line += 1;
    }

    frame_pos += this->speed;
    this->bg.tick(this->speed);

    if (detect_collision(joy_b, this->tun.get_bounding_box()) 
        && !(abs(this->j.position.x - this->tun.position.x) > 2.915)) {
        this->j.position.y = sqrt(fabs(8.5 - (this->j.position.x - this->tun.position.x) * (this->j.position.x - this->tun.position.x))) - 2.2f;
    }
    else
        this->j.tick(this->bg.roof_bounds(), this->bg.floor_bounds(), this->bg.wall_bounds());
}

void Game::tick_input(int left, int right, int up, int down, int space) {
    if (right)
        this->speed = 0.03;
    else if (left)
        this->speed = -0.03;
    else if (this->speed > 0)
        this->speed -= 0.0001;
    else if (this->speed < 0)
        this->speed += 0.0001;

    this->j.tick_input(left, right, space, this->speed);
    // this->bg.tick_input(left, right, this->speed); 
    this->min_spawn_pos = fmax(this->min_spawn_pos, frame_pos + 6.0);

    if ((next_balloon == 0) && down) {
        this->balloons.push_back(Balloon(this->j.position.x, 
                                            this->j.position.y));
        next_balloon = 60;
    }

    double die = get_random(0, 1);
    if (right && die < 0.01) {
        this->coins.push_back(Coin(this->min_spawn_pos, get_random(-2.2, 2.8)));
    }
    // else if (right && die > 0.997) {
    //     this->lines.push_back(FireLine(this->min_spawn_pos, 
    //                                     get_random(-2.2, 2.8), 
    //                                     this->min_spawn_pos + get_random(1.0, 3.0), 
    //                                     get_random(-2.2, 2.8)));
    // }
    else if (right && die > 0.998) {
        float h = get_random(-1.0, 1.5);
        this->beams.push_back(FireBeam(this->min_spawn_pos, 
                                        h,
                                        get_random(1.0, 2.0),
                                        0.01,
                                        fmin(get_random(1.0, 1.5), 2.8 - h)));
    }
    else if (right && die > 0.996) {
        this->boomerangs.push_back(Boomerang(this->min_spawn_pos, 
                                                get_random(-2.2, 2.8)));
    }
    // else if (right && die > 0.990) {
    //     this->magnets.push_back(Magnet(this->min_spawn_pos, 
    //                                             get_random(-2.2, 2.8)));
    // }
    else if (right && die > 0.990) {
        int t;
        if (get_random(0, 1) > 0.9)
            t = 1;
        else
            t = 0;
        this->projectiles.push_back(Projectile(this->min_spawn_pos,
                                                get_random(0, 2.2), t));
    }
}