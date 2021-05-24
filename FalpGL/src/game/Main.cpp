#include "../renderer/RendererIncludes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

#include "Player.h"
#include "Terminal.h"
#include "OverMap.h"


#include <math.h>



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1200, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        out::error("GLEW init fail!");
    else
        out::info("GLEW ok");


    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    std::cout << glGetString(GL_VERSION) << "  -  " << glGetString(GL_VENDOR) << "  -  " << glGetString(GL_RENDERER) << "   -  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; // put into terminal
    {
        glm::mat4 projection_matrix = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        int width, height, width_old, height_old;
        width_old = 0;
        height_old = 0;

        TileMap map;

        BatchRenderer test(3, "res/shaders/basic.shader");
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
        /* Loop until the user closes the window */

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        double xpos, ypos;

        boy.quad.teleport(100, 100);
        
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            map.tick(projection_matrix, 0, 0);

            test.draw(projection_matrix);
            
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos -= width / 2;
            ypos -= height / 2;
            ypos *= -1;

            //std::cout << xpos << "    -    " << ypos << std::endl;

            boy.quad.teleport(xpos, ypos);
            boy.quad.rotate(1, Point(0, 0));
            //boy.quad.scale(1.0005f);
            
            glfwGetFramebufferSize(window, &width, &height);
            if (width != width_old || height != height_old)
            {
                glViewport(0, 0, width, height);
               
                std::cout << width << "x" << height << std::endl;

                projection_matrix = glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, -1.0f, 1.0f);

                map.expand(width, height);

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
    std::cout << key << " " << scancode << " " << action << std::endl;
}