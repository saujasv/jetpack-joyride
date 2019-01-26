#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct color_t {
    int r;
    int g;
    int b;
};

#define GRAVITY 0.0005
#define DAMP 0.0008

// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float width;
    float height;
};

bool detect_collision(bounding_box_t a, bounding_box_t b);

extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();
double get_random(double min, double max);

// ---- Colors ----
const color_t COLOR_RED = { 236, 100, 75 };
const color_t COLOR_GREEN = { 135, 211, 124 };
const color_t COLOR_BLACK = { 52, 73, 94 };
const color_t COLOR_BACKGROUND = { 242, 241, 239 };
const color_t COLOR_BLUE = { 0, 51, 204 };
const color_t COLOR_BROWN = { 153, 102, 51 };
const color_t COLOR_ORANGE = { 255, 102, 0 };
const color_t COLOR_GRAY = { 179, 179, 179 };
const color_t COLOR_DARK_GRAY = { 89, 89, 89 };
const color_t COLOR_GOLD = { 255, 204, 0 };
const color_t COLOR_BRIGHT_ORANGE = { 255, 51, 0 };
const color_t COLOR_PURPLE = { 153, 51, 255 };
const color_t COLOR_YELLOW = { 255, 255, 0 };
const color_t COLOR_PINK = { 255, 0, 102 };
const color_t COLOR_AQUA = { 51, 204, 255 };

#endif
