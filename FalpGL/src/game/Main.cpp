#include <windows.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <math.h>
#include <random>

#include "../renderer/RendererIncludes.h"
#include "Map.h"
#include "Input.h"
#include "Entitiy.h"
#include "Json.h"

std::string get_current_dir() {
    void* v; //stops unused return value warning
    char buff[FILENAME_MAX]; //create string buffer to hold path
    v = _getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Input controller;

int main(void)
{

    /* Setup the window */
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    int window_width, window_height, width_old = 0, height_old = 0;
    const int resolution_x = 1920, resolution_y = 1080, window_scale = 2;
    double xpos, ypos;
    bool running = true;

    glm::mat4 projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);


    if (true) // true = fulscreen, false = windowed
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            /* Create a windowed mode window and its OpenGL context */
            window = glfwCreateWindow(mode->width, mode->height, "", glfwGetPrimaryMonitor(), NULL);
            if (!window) { glfwTerminate(); return -1;}
            
            window_width = mode->width;
            window_height = mode->height;

            projection_matrix = glm::ortho(
                round(-0.5f * window_width), 
                round(0.5f * window_width), 
                round(-0.5f * window_height), 
                round(0.5f * window_height), 
                -1.0f, 1.0f);
        }
    else
        {
           /* Create a windowed mode windowand its OpenGL context */
            window = glfwCreateWindow(1920, 1200, "", NULL, NULL);
            if (!window) { glfwTerminate(); return -1; }

            window_width = resolution_x / window_scale;
            window_height = resolution_y / window_scale;

            projection_matrix = glm::ortho(
                round(-0.5f * window_width),
                round(0.5f * window_width),
                round(-0.5f * window_height),
                round(0.5f * window_height),
                -1.0f, 1.0f);
        }


    std::cout << "working directory is: " << get_current_dir() << "\n";


    glfwMakeContextCurrent(window);
    std::cout << (glewInit() == GLEW_OK ? "GLEW ok\n" : "GLEW init fail!\n");


    std::cout << glGetString(GL_VERSION) << " - ";
    std::cout << glGetString(GL_VENDOR) << " - ";
    std::cout << glGetString(GL_RENDERER) << " - ";
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "\n";

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LEQUAL));

    GLCall(glEnable(GL_MULTISAMPLE));

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));


    glfwSetWindowTitle(window, "Test Text");

    glfwWindowHint(GLFW_SAMPLES, 8);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glViewport(0, 0, window_width, window_height);
    glfwSetWindowSize(window, window_width, window_height);

    glfwSetKeyCallback(window, key_callback);

    glfwSwapInterval(1);

    
    {
        /* Create all the things */

        Json_loader loader;

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);

        BatchRenderer player_render(10, "res/shaders/basic.shader");
        player_render.add_layout(layout);

        BatchRenderer things(1000, "res/shaders/basic.shader");
        things.add_layout(layout);
        
        Texture atlas_0 = Texture("res/gfx/atlas1.png");
        atlas_0.Bind(0);

        Player player(&player_render.vertex_buffer, &loader, 0);

        Map main_map(&projection_matrix, &loader, resolution_x, resolution_y);

        std::vector<Tile*> flowers;
        flowers.resize(500);
        for (int i = 0; i < 500; i++)
        {
            flowers[i] = new Tile(&things.vertex_buffer, &loader, "1");
            flowers[i]->translate((rand() % (int)ceil(resolution_x / 32)) * 32, (rand() % (int)ceil(resolution_y / 32)) * 32);
        }


        controller.set_player(&player);
        controller.set_keepalive(&running);
        controller.set_matrix(&projection_matrix);
        
        while (!glfwWindowShouldClose(window) && running)
        {
            /* Clear buffers and other OpenGL stuff */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* Handle inputs */
            controller.tick();

            glfwGetCursorPos(window, &xpos, &ypos);
            xpos -= window_width / 2;
            ypos -= window_height / 2;
            ypos *= -1;


            /* Tick things that need to be ticked */
            
            player.tick();
            main_map.shift(player.position_x(), player.position_y());


            /* Draw all the renderers */
            main_map.draw(*player.get_trans_matrix() ); /* Has pointer to projection_matrix */
            player_render.draw(projection_matrix  * *player.get_trans_matrix());
            things.draw(projection_matrix** main_map.get_trans_matrix() * *player.get_trans_matrix());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        for (int i = 0; i < 500; i++)
        {
            delete flowers[i];
        }
    }
    glfwTerminate();
    return (0);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    controller.recive(key, scancode, action, mods);
}