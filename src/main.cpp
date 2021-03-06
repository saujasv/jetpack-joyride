#include "main.h"
#include "timer.h"
#include "coin.h"
#include "joyrider.h"
#include "fire_lines.h"
#include "bg.h"
#include "game.h"
#include "projectile.h"

#define N_COINS 100
#define N_LINES 100
#define LO -2.2f
#define HI 2.8f

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

float screen_zoom = 1, screen_center_x = -1.5, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    screen_zoom = fmax(screen_zoom - yoffset * 0.01, 1);
    reset_screen();
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw(Game &g) {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(g.frame_pos, 0, 1), glm::vec3(g.frame_pos, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    g.bg.draw(VP);
    for (int i = 0; i < g.bricks.size(); i++) {
        g.bricks[i].draw(VP);
    }
    g.tun.draw(VP);
    for (Coin coin : g.coins) {
        coin.draw(VP);
    }
    for (int i = 0; i < g.lines.size(); i++) {
        g.lines[i].draw(VP);
    }
    for (FireBeam beam : g.beams) {
        beam.draw(VP);
    }
    for (int i = 0; i < g.boomerangs.size(); i++) {
        g.boomerangs[i].draw(VP);
    }

    for (int i = 0; i < g.balloons.size(); i++) {
        g.balloons[i].draw(VP);
    }

    for (int i = 0; i < g.projectiles.size(); i++) {
        g.projectiles[i].draw(VP);
    }

    g.j.draw(VP);
}

void tick_input(GLFWwindow *window, Game &g) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up  = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);

    g.tick_input(left, right, up, down, space);

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;
    char title[100];

    window = initGLFW(width, height);

    glfwSetScrollCallback(window, scroll_callback);
    initGL (window, width, height);
    Game g = Game();

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        sprintf(title, "Score = %d, Lives Remaining = %d", g.points, g.lives);
        glfwSetWindowTitle(window, title);
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw(g);
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_input(window, g);
            g.tick();
        }

        if (g.lives == 0)
            break;

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 2.5 / screen_zoom;
    float right  = screen_center_x + 5.5 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

double get_random(double min, double max) {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
}
