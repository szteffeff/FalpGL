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
#include "Input.h"
#include "Entity.h"
#include "Json.h"
#include "UI.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "SoundDevice.h"
#include "NewMap.h"
#include "../renderer/Log.h"
#include "SFX.h"

std::string get_current_dir() {
    void* v; //stops unused return value warning
    char buff[FILENAME_MAX]; //create string buffer to hold path
    v = _getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}


/* Declarations */
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
Json_loader* Animation::loader;
Json_loader* Creature::loader;



/* Input needs to be global I think */
Input controller;

int main(void)
{
    console_log("[INFO]: Started clock");


    Json_loader loader;
    if (!loader.init()) { return -1; }

    /* Set static variables */
    Animation::loader = &loader;
    Creature::loader = &loader;
    

    /* Setup the window */
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwSwapInterval(1);

    int window_width, window_height;
    int resolution_x = 1920, resolution_y = 1080, window_scale = 1;
    double xpos, ypos;
    bool running = true;
    const bool fullscreen = false;
    bool pause = false;

    glm::mat4 projection_matrix;


    /* Create window and find resolution */
    if (fullscreen)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            /* Create a windowed mode window and its OpenGL context */
            window = glfwCreateWindow(mode->width, mode->height, "", glfwGetPrimaryMonitor(), NULL);
            if (!window) { glfwTerminate(); return -1;}
            
            resolution_x = mode->width;
            window_width = mode->width;

            resolution_y = mode->height;
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
                round(-0.5f * window_width * window_scale),
                round(0.5f * window_width * window_scale),
                round(-0.5f * window_height * window_scale),
                round(0.5f * window_height * window_scale),
                -1.0f, 1.0f);
        }
    /* Window size and resolution is now known */

    controller.set_height_width(window_width, window_height);

    /* Working directory for debug */
    console_log("[INFO]: working directory is: " + get_current_dir());

    /* Seed rand */
    srand(time(NULL));

    /* Start glew */
    glfwMakeContextCurrent(window);
    if (glewInit() == GLEW_OK)
    {
        console_log("[INFO]: GLEW ok");
    }
    else
    {
        console_log("[INFO]: GLEW init fail!");
        return -1;
    }

    /* OpenGL strings */
    {
        std::stringstream glstrings;
        glstrings << "[INFO]: ";
        glstrings << glGetString(GL_VERSION) << " - ";
        glstrings << glGetString(GL_VENDOR) << " - ";
        glstrings << glGetString(GL_RENDERER) << " - ";
        glstrings << glGetString(GL_SHADING_LANGUAGE_VERSION);
        console_log(glstrings.str());
    }


    /* OpenGL functions */
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LEQUAL));
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    glEnable(GL_PROGRAM_POINT_SIZE);
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glViewport(0, 0, window_width, window_height));
    GLCall(glEnable(GL_MULTISAMPLE));

    /* glfw functions */
    std::string names[18] = {
        "The Small Lands",
        "A Hobbit's Journey",
        "Splund",
        "Stop the Dungeon",
        "Catland",
        "Evergrow",
        "Unnamed Dungeon Game",
        "Spooklands",
        "Nowhere",
        "Gloom",
        "Live",
        "Maghide",
        "Inside the Dungeon",
        "Don't Die",
        "Bruh",
        "Archworks",
        "A Dungeon Story",
        "Dungeon Dive"
    };
    glfwSetWindowTitle(window, names[(rand() % 18)].c_str());
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowSize(window, window_width, window_height);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    /* setup cursor */
    {
        unsigned char* cursor_image_data;
        int c_height, c_width, c_bpp;
        cursor_image_data = stbi_load("res/gfx/textures/cursor.png", &c_height, &c_width, &c_bpp, 4);


        GLFWimage cursor_image;
        cursor_image.height = c_height;
        cursor_image.width = c_width;
        cursor_image.pixels = cursor_image_data;

        GLFWcursor* cursor = glfwCreateCursor(&cursor_image, 0, 0);

        glfwSetCursor(window, cursor);
    }
    
    /* setup icon */
    {
        unsigned char* icon_image_data;
        int c_height, c_width, c_bpp;
        icon_image_data = stbi_load("res/gfx/textures/incon2.png", &c_height, &c_width, &c_bpp, 4);


        GLFWimage icon_image;
        icon_image.height = c_height;
        icon_image.width = c_width;
        icon_image.pixels = icon_image_data;

        glfwSetWindowIcon(window, 1, &icon_image);
    }
    
    { /* OpenGL objects need to be created in this scope */

        { /* Loading screen */
            float vertices[24] = {
                 1.0f, -1.0f,  1.0f, 0.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                -1.0f,  1.0f,  0.0f, 1.0f,

                 1.0f,  1.0f,  1.0f, 1.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f,  0.0f, 1.0f
            };

            VertexArray va;
            VertexBuffer vb = VertexBuffer(sizeof(vertices), &vertices);
            VertexBufferLayout vbl;
            vbl.Push<float>(2);
            vbl.Push<float>(2);
            va.AddBuffer(vb, vbl);
            Shader s("res/shaders/quad.shader");
            s.Bind();
            s.SetUniform1i("u_Texture", 0);
            s.SetUniform1f("u_alpha", 1.0f);

            Texture loading_screen("res/gfx/textures/loading.jpeg");
            loading_screen.Bind();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDisable(GL_DEPTH_TEST);
            va.Bind();
            s.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glEnable(GL_DEPTH_TEST);
            glfwSwapBuffers(window);
        }

        console_log("[INFO]: Started constructing map");
        New_Map nmap;
        console_log("[INFO]: Finished constructing map");

        HSL_Framebuffer framebuffer(resolution_x, resolution_y, 14);
        Chroma_Framebuffer c_framebuffer(resolution_x, resolution_y, 15);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);

        BatchRenderer player_render(100, "res/shaders/basic.shader");
        player_render.add_layout(layout);

        BatchRenderer interface_renderer(1000, "res/shaders/ui.shader");
        interface_renderer.add_layout(layout);
        
        Texture atlas_0("res/gfx/atlas0.png");
        Texture atlas_1("res/gfx/atlas1.png");

        atlas_0.Bind(0);
        atlas_1.Bind(1);
        
        UserInterface ui(&interface_renderer.vertex_buffer);

        Player player(&player_render.vertex_buffer);
        Red_Slime red_slime(&player_render.vertex_buffer);
        Enemy_Ghost enemy_ghost(&player_render.vertex_buffer);
        Garfield garfield(&player_render.vertex_buffer);
        Bush_Boi Bush_boi(&player_render.vertex_buffer);
        Chompy_Slime Chompy_slime(&player_render.vertex_buffer);

        red_slime.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost.Get_player_position(player.get_position_x(), player.get_position_y());
        Chompy_slime.Get_player_position(player.get_position_x(), player.get_position_y());

        /*Sound crap*/
        SFX Sound_player;
        uint32_t intro = SoundBuffer::get()->addSoundEffect("files/SFX/intro.wav");
        Sound_player.Play_sound(intro);
        //SFX Sound_song;
        //uint32_t song = SoundBuffer::get()->addSoundEffect("files/SFX/red-test.wav");
        //Sound_song.Play_sound(song);
        SFX Background_sound;
        uint32_t background = SoundBuffer::get()->addSoundEffect("files/SFX/Spooky_Egyptian_Beat.wav");

        ui.SetHealth(player.GetHealth());
        ui.SetStamina(player.GetStamina());
        ui.SetPotion(player.GetPotion());
        controller.set_pause(&pause);
        controller.set_player(&player);
        controller.set_keepalive(&running);
        controller.set_matrix(&projection_matrix);

        player.set_active_map(&nmap);


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        while (!glfwWindowShouldClose(window) && running)
        {
            /* Handle inputs */
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos -= window_width / 2;
            ypos -= window_height / 2;
            ypos *= -1;

            controller.tick(xpos, ypos);

            /*SOUND THINGY*/
            
            Background_sound.Play_sound(background);

            //myspeaker.Play(intro);

            /* Tick things that need to be ticked */
            ui.UI_Tick();
            if (pause == false) {
                player.tick();
                //red_slime.tick();
                enemy_ghost.tick();
                garfield.tick();
                Bush_boi.tick();
                Chompy_slime.tick();
            }
           
            //console_log(std::string("[INFO]: Player on tile: ") + std::to_string(nmap.tile_at(*player.get_position_x(), *player.get_position_y())));

            /* ##### Draw ##### */

            if (!pause)
            {
                /* Setup hsl framebuffer */
                framebuffer.bind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, resolution_x, resolution_y);


                /* Draw everything but ui to framebuffer */
                glDepthMask(false);
                nmap.draw(projection_matrix * *player.get_trans_matrix());
                glDepthMask(true);

                player_render.draw(projection_matrix * *player.get_trans_matrix());


                /* Setup chromatic aberration framebuffer */
                c_framebuffer.bind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, resolution_x, resolution_y);

                /* Draw between buffers */
                framebuffer.set_hue(0.0f);
                framebuffer.set_saturation(*player.GetHealth());
                framebuffer.set_value(((*player.GetHealth() + 200) / 300.0f) * 100);
                framebuffer.draw();

            }

            /* Setup main buffer */
            c_framebuffer.unbind();
            c_framebuffer.set_chroma((100 - *player.GetHealth()) / 33.3333f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, window_width, window_height);

            /* Draw processed image and UI */
            c_framebuffer.draw();
            interface_renderer.draw(projection_matrix);




            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return (0);
}


void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type != GL_DEBUG_TYPE_ERROR) { return; }

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    controller.keys_recive(key, scancode, action, mods);
}


void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    controller.mouse_recive(button, action, mods);
}