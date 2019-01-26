#ifndef COLOR_H
#define COLOR_H

struct color_t {
    int r;
    int g;
    int b;
};

extern const color_t COLOR_RED = { 236, 100, 75 };
extern const color_t COLOR_GREEN = { 135, 211, 124 };
extern const color_t COLOR_BLACK = { 52, 73, 94 };
extern const color_t COLOR_BACKGROUND = { 242, 241, 239 };
extern const color_t COLOR_BLUE = { 0, 51, 204 };
extern const color_t COLOR_BROWN = { 153, 102, 51 };
extern const color_t COLOR_ORANGE = { 255, 102, 0 };

#endif
