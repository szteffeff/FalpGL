#include "../renderer/RendererIncludes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

#include "Player.h"
#include "Terminal.h"
#include "OverMap.h"
#include "Map.h"
#include "Input.h"


#include <math.h>

/*
* VertexBuffer - use local buffer
* Quad - clean up functions
* Entity
* 
* 
* 
* 
* 
* 
* 
* 
* 
*/

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Input controller;

int main(void)
{

    GLFWwindow* window;
    bool running = true;
    controller.set_keepalive(&running);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (false) // fullscreen modes
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
        out::error("GLEW init fail!");
    else
        out::info("GLEW ok");


    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    glfwWindowHint(GLFW_SAMPLES, 16);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    std::cout << glGetString(GL_VERSION) << "  -  " << glGetString(GL_VENDOR) << "  -  " << glGetString(GL_RENDERER) << "   -  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    {
        glm::mat4 projection_matrix = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        int width, height, width_old, height_old;
        width_old = 0;
        height_old = 0;


        //TileMap map;

        Map map;

        BatchRenderer test(300, "res/shaders/basic.shader");
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);
        test.add_layout(layout);
        
        Player boy(test.vertex_buffer);

        Tile red(0, 0, test.vertex_buffer);
        red.index_change(2.0f); 
        red.update_quad();
        red.data();

        Texture red_grass("res/textures/Rrass32.png");
        red_grass.Bind(2);

        boy.TBind();

        controller.set_tile(&red);

        /*quad test_tile(test.vertex_buffer, 32, 32, 32);
        test_tile.texture_index(2);
        test_tile.translate(500, 0);*/

        /* Loop until the user closes the window */

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        double xpos, ypos;
        float xpos1, ypos1;
        //boy.quad.teleport(100, 100);
        
        while (!glfwWindowShouldClose(window) && running)
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            map.draw(projection_matrix);
            test.draw(projection_matrix);

            
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos -= width / 2;
            ypos -= height / 2;
            ypos *= -1;
            xpos1 = (float)xpos;
            ypos1 = (float)ypos;

            boy.quad.teleport(xpos1, ypos1);
            boy.quad.rotate(1, Point(0, 0));
            //boy.quad.scale(1.0005f);
            
            glfwGetFramebufferSize(window, &width, &height);
            if (width != width_old || height != height_old)
            {
                glViewport(0, 0, width, height);
               
                std::cout << width << "x" << height << std::endl;

                projection_matrix = glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, -1.0f, 1.0f);

                //map.expand(width, height);

                width_old = width;
                height_old = height;
            }


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

        }
    }
    glfwTerminate();
    return (0);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    controller.recive(key, scancode, action, mods);
}