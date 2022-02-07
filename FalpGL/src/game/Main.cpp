#include <windows.h>
#include <direct.h>

#include "../renderer/RendererIncludes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <math.h>
#include <random>

#include "Map.h"
#include "Input.h"
#include "Entitiy.h"
#include "Json.h"

std::string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    _getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Input controller;

int main(void)
{

    /* Setup the window */

    std::cout << "working directory is: " << get_current_dir() << "\n";

    GLFWwindow* window;
    bool running = true;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    int width, height, width_old = 0, height_old = 0;
    double xpos, ypos, zoom = 2;

    glm::mat4 projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f));


    if (false) // true = fulscreen, false = windowed
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            /* Create a windowed mode window and its OpenGL context */
            window = glfwCreateWindow(mode->width, mode->height, "", glfwGetPrimaryMonitor(), NULL);
            if (!window)
                {
                    glfwTerminate();
                    return -1;
                }
            
            width = mode->width;
            height = mode->height;

            projection_matrix = glm::ortho(round(-0.5f * width), round(0.5f * width), round(-0.5f * height), round(0.5f * height), -1.0f, 1.0f);

        }
    else
        {
            window = glfwCreateWindow(1920, 1200, "", NULL, NULL);
            if (!window)
                {
                    glfwTerminate();
                    return -1;
                }

            int resolution_x = 1920, resolution_y = 1080, scale = 2;

            width = resolution_x / scale;
            height = resolution_y / scale;

            projection_matrix = glm::ortho((float)(-width / scale * zoom), (float)(width / scale * zoom), (float)(-height / scale * zoom), (float)(height / scale * zoom), -1.0f, 1.0f);
        }



    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init fail!\n";
    else
        std::cout << "GLEW ok\n";


    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LEQUAL));

    GLCall(glEnable(GL_MULTISAMPLE));
    GLCall(glfwWindowHint(GLFW_SAMPLES, 8));

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glViewport(0, 0, width, height);
    glfwSetWindowSize(window, width, height);

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << "  -  " << glGetString(GL_VENDOR) << "  -  " << glGetString(GL_RENDERER) << "   -  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    {
        /* Create all the things */
        /*
        *    MATRICES:
        * 
        * Zoom: scales everything 2x
        * Player Transform: Moves around everthing to follow player
        * Map Transform: Centers Everthing
        * Projection: Fits output to window
        * 
        */








        





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

        Map main_map(&projection_matrix, &loader);

        std::vector<Tile*> flowers;
        flowers.resize(500);
        for (int i = 0; i < 500; i++)
        {
            flowers[i] = new Tile(&things.vertex_buffer, &loader, "1");
            flowers[i]->translate((rand() % 320 - 170) * 32, (rand() % 240 - 120) * 32);
        }


        controller.set_player(&player);
        controller.set_keepalive(&running);
        controller.set_matrix(&projection_matrix);
        
        while (!glfwWindowShouldClose(window) && running)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            controller.tick();
            player.tick();
            main_map.shift(player.position_x(), player.position_y());


            
            main_map.draw(*player.get_trans_matrix() * zoom_matrix); /* Has pointer to projection_matrix */
            player_render.draw(projection_matrix * zoom_matrix * *player.get_trans_matrix());
            things.draw(projection_matrix** main_map.get_trans_matrix() * *player.get_trans_matrix() * zoom_matrix);

            
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos -= width / 2;
            ypos -= height / 2;
            ypos *= -1;

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