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
    Json_loader loader;


    std::cout << "working directory is: " << get_current_dir() << "\n";

    GLFWwindow* window;
    bool running = true;
    controller.set_keepalive(&running);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (true) // fullscreen modes
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
        }
    else
        {
        window = glfwCreateWindow(1920, 1200, "", NULL, NULL);
        if (!window)
            {
                glfwTerminate();
                return -1;
            }
        }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(-1); //1 = VSYNC, -1 = ??

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init fail!\n";
    else
        std::cout << "GLEW ok\n";


    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glClearColor(255.0f, 0.0f, 255.0f, 1.0f));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LEQUAL));

    glEnable(GL_MULTISAMPLE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    std::cout << glGetString(GL_VERSION) << "  -  " << glGetString(GL_VENDOR) << "  -  " << glGetString(GL_RENDERER) << "   -  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    {
        glm::mat4 projection_matrix = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        glm::mat4 zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
        int width, height, width_old, height_old, zoom_x, zoom_y;
        width_old = 0;
        height_old = 0;

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        double xpos, ypos;
        float xpos1, ypos1;

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
        flowers.resize(1000);
        for (int i = 0; i < 1000; i++)
        {
            flowers[i] = new Tile(&things.vertex_buffer, &loader, "1");
            flowers[i]->translate((rand() % 100) * 32, (rand() % 100) * 32);
        }


        controller.set_player(&player);
        
        while (!glfwWindowShouldClose(window) && running)
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            controller.tick();
            player.tick();


            
            main_map.draw(*player.get_trans_matrix() * zoom_matrix); /* Has pointer to projection_matrix */
            player_render.draw(projection_matrix * zoom_matrix);
            things.draw(projection_matrix * *main_map.get_trans_matrix() * *player.get_trans_matrix() * zoom_matrix);

            
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos -= width / 2;
            ypos -= height / 2;
            ypos *= -1;
            xpos1 = (float)xpos;
            ypos1 = (float)ypos;
            
            //glfwGetFramebufferSize(window, &width, &height);
            if (width != width_old || height != height_old)
            {
                glViewport(0, 0, width, height);
               
                std::cout << width << "x" << height << std::endl;

                projection_matrix = glm::ortho(round(-0.5f * width), round(0.5f * width), round(-0.5f * height), round(0.5f * height) + 0.01f, -1.0f, 1.0f);

                width_old = width;
                height_old = height;
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

        }

        for (int i = 0; i < 1000; i++)
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