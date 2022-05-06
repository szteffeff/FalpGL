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
#include "Presentation.h"


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
float* Creature::curser_x;
float* Creature::curser_y;



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
    float xpos, ypos;
    Creature::curser_x = &xpos;
    Creature::curser_y = &ypos;
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

    glfwSetWindowTitle(window, "RedCape");
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

        /* Loading screen */
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
            s.SetUniform1f("u_alpha", 0.5f);

            Texture loading_screen("res/gfx/textures/loading.jpeg");
            Texture pause_screen("res/gfx/textures/leo the danny(epic pause screen).png"); /*path to image*/
            
            pause_screen.Bind(8);
            loading_screen.Bind();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDisable(GL_DEPTH_TEST);
            va.Bind();
            s.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glEnable(GL_DEPTH_TEST);
            glfwSwapBuffers(window);
        

        console_log("[INFO]: Started constructing map");
        New_Map nmap;
        console_log("[INFO]: Finished constructing map");

        HSL_Framebuffer framebuffer(resolution_x, resolution_y, 14);
        Chroma_Framebuffer c_framebuffer(resolution_x, resolution_y, 15);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);

        BatchRenderer player_render(1000, "res/shaders/basic.shader");
        player_render.add_layout(layout);

        //std::vector<Creature*> bad_bois;

        BatchRenderer interface_renderer(1000, "res/shaders/ui.shader");
        interface_renderer.add_layout(layout);
        
        Texture atlas_0("res/gfx/atlas0.png");
        Texture atlas_1("res/gfx/atlas1.png");

        atlas_0.Bind(0);
        atlas_1.Bind(1);
        
        UserInterface ui(&interface_renderer.vertex_buffer);

        // npc and players
        Player player(&player_render.vertex_buffer);
        Garfield garfield(&player_render.vertex_buffer);
        Cow cow(&player_render.vertex_buffer);
        Perry perry(&player_render.vertex_buffer);
        Edgelord edgelord(&player_render.vertex_buffer);
        Clair_Of_Cavern clair_of_cavern(&player_render.vertex_buffer);
        Destus_Of_Cavern destus_of_cavern(&player_render.vertex_buffer);
        Del_Ibra_of_Hillsby del_ibra_of_hillsby(&player_render.vertex_buffer);
        Eloah_of_Minlet eloah_of_minlet(&player_render.vertex_buffer);
        Felix_of_Festria felix_of_festria(&player_render.vertex_buffer);
        Maban_of_Undermount maban_of_undermount(&player_render.vertex_buffer);

        // the bad bois

        // red slimes

        Red_Slime red_slime_0(&player_render.vertex_buffer);
        Red_Slime red_slime_1(&player_render.vertex_buffer);
        Red_Slime red_slime_2(&player_render.vertex_buffer);
        Red_Slime red_slime_3(&player_render.vertex_buffer);
        Red_Slime red_slime_4(&player_render.vertex_buffer);
        Red_Slime red_slime_5(&player_render.vertex_buffer);
        Red_Slime red_slime_6(&player_render.vertex_buffer);
        Red_Slime red_slime_7(&player_render.vertex_buffer);
        Red_Slime red_slime_8(&player_render.vertex_buffer);
        Red_Slime red_slime_9(&player_render.vertex_buffer);

        // chompy slimes

        Chompy_Slime chompy_slime_0(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_1(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_2(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_3(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_4(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_5(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_6(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_7(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_8(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_9(&player_render.vertex_buffer);
        Chompy_Slime chompy_slime_10(&player_render.vertex_buffer);

        // blue slimes

        Blue_Slime blue_slime_0(&player_render.vertex_buffer);
        Blue_Slime blue_slime_1(&player_render.vertex_buffer);
        Blue_Slime blue_slime_2(&player_render.vertex_buffer);
        Blue_Slime blue_slime_3(&player_render.vertex_buffer);

        // icy slimes

        Ice_Slime ice_slime_0(&player_render.vertex_buffer);
        Ice_Slime ice_slime_1(&player_render.vertex_buffer);
        Ice_Slime ice_slime_2(&player_render.vertex_buffer);
        Ice_Slime ice_slime_3(&player_render.vertex_buffer);
        Ice_Slime ice_slime_4(&player_render.vertex_buffer);
        Ice_Slime ice_slime_5(&player_render.vertex_buffer);

        // bushy da boi

        Bush_Boi bush_boi_0(&player_render.vertex_buffer);
        Bush_Boi bush_boi_1(&player_render.vertex_buffer);
        Bush_Boi bush_boi_2(&player_render.vertex_buffer);
        Bush_Boi bush_boi_3(&player_render.vertex_buffer);
        Bush_Boi bush_boi_4(&player_render.vertex_buffer);
        Bush_Boi bush_boi_5(&player_render.vertex_buffer);
        Bush_Boi bush_boi_6(&player_render.vertex_buffer);
        Bush_Boi bush_boi_7(&player_render.vertex_buffer);
        Bush_Boi bush_boi_8(&player_render.vertex_buffer);
        Bush_Boi bush_boi_9(&player_render.vertex_buffer);
        Bush_Boi bush_boi_10(&player_render.vertex_buffer);
        Bush_Boi bush_boi_11(&player_render.vertex_buffer);
        Bush_Boi bush_boi_12(&player_render.vertex_buffer);
        Bush_Boi bush_boi_13(&player_render.vertex_buffer);
        Bush_Boi bush_boi_14(&player_render.vertex_buffer);
        Bush_Boi bush_boi_15(&player_render.vertex_buffer);

        // enemy ghosty toasty

        Enemy_Ghost enemy_ghost_0(&player_render.vertex_buffer);
        Enemy_Ghost enemy_ghost_1(&player_render.vertex_buffer);
        Enemy_Ghost enemy_ghost_2(&player_render.vertex_buffer);
        Enemy_Ghost enemy_ghost_3(&player_render.vertex_buffer);
        Enemy_Ghost enemy_ghost_4(&player_render.vertex_buffer);
        Enemy_Ghost enemy_ghost_5(&player_render.vertex_buffer);

        // sussy vase acting kinda sus ;D

        Sussy_Vase sussy_vase_0(&player_render.vertex_buffer);
        Sussy_Vase sussy_vase_1(&player_render.vertex_buffer);
        Sussy_Vase sussy_vase_2(&player_render.vertex_buffer);
        Sussy_Vase sussy_vase_3(&player_render.vertex_buffer);
        Sussy_Vase sussy_vase_4(&player_render.vertex_buffer);
        Sussy_Vase sussy_vase_5(&player_render.vertex_buffer);
        Sussy_Vase sussy_vase_6(&player_render.vertex_buffer);


        //moving objects???
        Campfire campfire_1(&player_render.vertex_buffer);
        Campfire campfire_2(&player_render.vertex_buffer);
        
        //npc getting player stuff
        campfire_1.teleport(-4155.34, 3440.17);
        campfire_2.teleport(-6769.67, 4205.33);

        garfield.Get_player_position(player.get_position_x(), player.get_position_y());
        garfield.Get_talk(player.speaking());
        garfield.Get_point(player.check_points());

        cow.Get_player_position(player.get_position_x(), player.get_position_y());
        cow.Get_talk(player.speaking());
        cow.Get_point(player.check_points());

        perry.Get_player_position(player.get_position_x(), player.get_position_y());
        perry.Get_talk(player.speaking());
        perry.Get_point(player.check_points());

        edgelord.Get_player_position(player.get_position_x(), player.get_position_y());
        edgelord.Get_talk(player.speaking());
        edgelord.Get_point(player.check_points());

        clair_of_cavern.Get_player_position(player.get_position_x(), player.get_position_y());
        clair_of_cavern.Get_talk(player.speaking());

        del_ibra_of_hillsby.Get_player_position(player.get_position_x(), player.get_position_y());
        del_ibra_of_hillsby.Get_talk(player.speaking());

        eloah_of_minlet.Get_player_position(player.get_position_x(), player.get_position_y());
        eloah_of_minlet.Get_talk(player.speaking());

        felix_of_festria.Get_player_position(player.get_position_x(), player.get_position_y());
        felix_of_festria.Get_talk(player.speaking());
        
        maban_of_undermount.Get_player_position(player.get_position_x(), player.get_position_y());
        maban_of_undermount.Get_talk(player.speaking());

        destus_of_cavern.Get_player_position(player.get_position_x(), player.get_position_y());
        destus_of_cavern.Get_talk(player.speaking());

        // enemys..... OH GOOOD THERE SO MANY.....

        // red_slimes
        red_slime_0.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_0.Player_Health(player.GetHealth());
        red_slime_0.teleport(-817, -796);

        red_slime_1.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_1.Player_Health(player.GetHealth());
        red_slime_1.teleport(-499, 913);

        red_slime_2.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_2.Player_Health(player.GetHealth());
        red_slime_2.teleport(2095, 204);

        red_slime_3.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_3.Player_Health(player.GetHealth());
        red_slime_3.teleport(1839, 2639);

        red_slime_4.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_4.Player_Health(player.GetHealth());
        red_slime_4.teleport(10, 2888);

        red_slime_5.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_5.Player_Health(player.GetHealth());
        red_slime_5.teleport(-1677, 4785);

        red_slime_6.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_6.Player_Health(player.GetHealth());
        red_slime_6.teleport(-1777, 3125);

        red_slime_7.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_7.Player_Health(player.GetHealth());
        red_slime_7.teleport(-1621, 2227);

        red_slime_8.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_8.Player_Health(player.GetHealth());
        red_slime_8.teleport(-1877, 2163);

        red_slime_9.Get_player_position(player.get_position_x(), player.get_position_y());
        red_slime_9.Player_Health(player.GetHealth());
        red_slime_9.teleport(-5299, 2992);
        
        // chompy slime chomp

        chompy_slime_0.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_0.Player_Health(player.GetHealth());
        chompy_slime_0.teleport(-1812, -913);

        chompy_slime_1.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_1.Player_Health(player.GetHealth());
        chompy_slime_1.teleport(-592, -492);

        chompy_slime_2.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_2.Player_Health(player.GetHealth());
        chompy_slime_2.teleport(-3151, 600);

        chompy_slime_3.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_3.Player_Health(player.GetHealth());
        chompy_slime_3.teleport(-5454, 2129);

        chompy_slime_4.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_4.Player_Health(player.GetHealth());
        chompy_slime_4.teleport(-2770, 3055);

        chompy_slime_5.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_5.Player_Health(player.GetHealth());
        chompy_slime_5.teleport(-1598, 689);

        chompy_slime_6.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_6.Player_Health(player.GetHealth());
        chompy_slime_6.teleport(1613, -141);

        chompy_slime_7.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_7.Player_Health(player.GetHealth());
        chompy_slime_7.teleport(1744, 1167);

        chompy_slime_8.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_8.Player_Health(player.GetHealth());
        chompy_slime_8.teleport(75, 2160);

        chompy_slime_9.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_9.Player_Health(player.GetHealth());
        chompy_slime_9.teleport(-526, 2643);

        chompy_slime_10.Get_player_position(player.get_position_x(), player.get_position_y());
        chompy_slime_10.Player_Health(player.GetHealth());
        chompy_slime_10.teleport(1581, 4083);

        // bluey the slime

        blue_slime_0.Get_player_position(player.get_position_x(), player.get_position_y());
        blue_slime_0.Player_Health(player.GetHealth());
        blue_slime_0.teleport(-9458, 4308);

        blue_slime_1.Get_player_position(player.get_position_x(), player.get_position_y());
        blue_slime_1.Player_Health(player.GetHealth());
        blue_slime_1.teleport(-6515, 9485);

        blue_slime_2.Get_player_position(player.get_position_x(), player.get_position_y());
        blue_slime_2.Player_Health(player.GetHealth());
        blue_slime_2.teleport(-6257, 9584);

        blue_slime_3.Get_player_position(player.get_position_x(), player.get_position_y());
        blue_slime_3.Player_Health(player.GetHealth());
        blue_slime_3.teleport(-3633, 8211);

        // icelce slimecilce

        ice_slime_0.Get_player_position(player.get_position_x(), player.get_position_y());
        ice_slime_0.Player_Health(player.GetHealth());
        ice_slime_0.teleport(-8404, 4946);

        ice_slime_1.Get_player_position(player.get_position_x(), player.get_position_y());
        ice_slime_1.Player_Health(player.GetHealth());
        ice_slime_1.teleport(-8209, 7439);

        ice_slime_2.Get_player_position(player.get_position_x(), player.get_position_y());
        ice_slime_2.Player_Health(player.GetHealth());
        ice_slime_2.teleport(-9011, 9267);

        ice_slime_3.Get_player_position(player.get_position_x(), player.get_position_y());
        ice_slime_3.Player_Health(player.GetHealth());
        ice_slime_3.teleport(-6197, 8047);

        ice_slime_4.Get_player_position(player.get_position_x(), player.get_position_y());
        ice_slime_4.Player_Health(player.GetHealth());
        ice_slime_4.teleport(79, 6830);

        ice_slime_5.Get_player_position(player.get_position_x(), player.get_position_y());
        ice_slime_5.Player_Health(player.GetHealth());
        ice_slime_5.teleport(1191, 6862);

        // OH GOD ITS AN ARMY OF BUSHES

        bush_boi_0.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_0.Player_Health(player.GetHealth());
        bush_boi_0.teleport(1253, -137);

        bush_boi_1.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_1.Player_Health(player.GetHealth());
        bush_boi_1.teleport(401, 1489);

        bush_boi_2.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_2.Player_Health(player.GetHealth());
        bush_boi_2.teleport(-350, 1583);

        bush_boi_3.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_3.Player_Health(player.GetHealth());
        bush_boi_3.teleport(-3570, 1073);

        bush_boi_4.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_4.Player_Health(player.GetHealth());
        bush_boi_4.teleport(-2721, 2209);

        bush_boi_5.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_5.Player_Health(player.GetHealth());
        bush_boi_5.teleport(-274, 3120);

        bush_boi_6.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_6.Player_Health(player.GetHealth());
        bush_boi_6.teleport(-4145, 2801);

        bush_boi_7.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_7.Player_Health(player.GetHealth());
        bush_boi_7.teleport(-2230, 4365);

        bush_boi_8.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_8.Player_Health(player.GetHealth());
        bush_boi_8.teleport(-9712, 5742);

        bush_boi_9.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_9.Player_Health(player.GetHealth());
        bush_boi_9.teleport(-8110, 8496);

        bush_boi_10.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_10.Player_Health(player.GetHealth());
        bush_boi_10.teleport(-5422, 8786);

        bush_boi_11.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_11.Player_Health(player.GetHealth());
        bush_boi_11.teleport(-6195, 6512);

        bush_boi_12.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_12.Player_Health(player.GetHealth());
        bush_boi_12.teleport(-3697, 7150);

        bush_boi_13.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_13.Player_Health(player.GetHealth());
        bush_boi_13.teleport(-659, 7855);

        bush_boi_14.Get_player_position(player.get_position_x(), player.get_position_y());
        bush_boi_14.Player_Health(player.GetHealth());
        bush_boi_14.teleport(750, 8879);

        /// ghosty time

        enemy_ghost_0.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost_0.Player_Health(player.GetHealth());
        enemy_ghost_0.teleport(-468, -1104);

        enemy_ghost_1.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost_1.Player_Health(player.GetHealth());
        enemy_ghost_1.teleport(543, 1277);

        enemy_ghost_2.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost_2.Player_Health(player.GetHealth());
        enemy_ghost_2.teleport(-3279, 3350);

        enemy_ghost_3.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost_3.Player_Health(player.GetHealth());
        enemy_ghost_3.teleport(-5715, 6957);

        enemy_ghost_4.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost_4.Player_Health(player.GetHealth());
        enemy_ghost_4.teleport(-2153, 8263);

        enemy_ghost_5.Get_player_position(player.get_position_x(), player.get_position_y());
        enemy_ghost_5.Player_Health(player.GetHealth());
        enemy_ghost_5.teleport(142, 8431);
        
        // sussy vase vibin doe

        sussy_vase_0.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_0.Player_Health(player.GetHealth());
        sussy_vase_0.teleport(-1921, -543);

        sussy_vase_1.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_1.Player_Health(player.GetHealth());
        sussy_vase_1.teleport(247, 1053);

        sussy_vase_2.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_2.Player_Health(player.GetHealth());
        sussy_vase_2.teleport(-4237, 3408);

        sussy_vase_3.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_3.Player_Health(player.GetHealth());
        sussy_vase_3.teleport(1744, 3122);

        sussy_vase_4.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_4.Player_Health(player.GetHealth());
        sussy_vase_4.teleport(-5361, 6961);

        sussy_vase_5.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_5.Player_Health(player.GetHealth());
        sussy_vase_5.teleport(-2289, 8302);

        sussy_vase_6.Get_player_position(player.get_position_x(), player.get_position_y());
        sussy_vase_6.Player_Health(player.GetHealth());
        sussy_vase_6.teleport(-594, 8530);

        /*Sound crap*/
        SFX Sound_player;
        uint32_t intro = SoundBuffer::get()->addSoundEffect("files/SFX/intro.wav");
        Sound_player.Play_sound(intro);
        SFX Sound_song;
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

        ui.create_text("example\ntext", *player.get_position_x(), *player.get_position_y(), 60);
        ui.create_text("example\ntext", *player.get_position_x(), *player.get_position_y() - 64, 120);

        atlas_0.Bind(0);
        atlas_1.Bind(1);

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        while (!glfwWindowShouldClose(window) && running)
        {
            /* Handle inputs */
            {
                double tempx, tempy;
                glfwGetCursorPos(window, &tempx, &tempy);
                xpos = (float)tempx;
                ypos = (float)tempy;
            }
            
            xpos -= window_width / 2;
            ypos -= window_height / 2;
            ypos *= -1;

            controller.tick(xpos, ypos);

            /*SOUND THINGY*/
            
            Background_sound.Play_sound(background);

            //myspeaker.Play(intro);

            /* Tick things that need to be ticked */
            ui.UI_Tick();
            nmap.tick(*player.get_position_x(), *player.get_position_y());
            if (pause == false) {
                player.tick();
                garfield.tick();
                cow.tick();
                perry.tick();
                edgelord.tick();
                clair_of_cavern.tick();
                destus_of_cavern.tick();
                del_ibra_of_hillsby.tick();
                eloah_of_minlet.tick();
                felix_of_festria.tick();
                maban_of_undermount.tick();

                campfire_1.tick();
                campfire_2.tick();

                // ALOT OF MOBS
                // red slimes doe
                red_slime_0.tick();
                red_slime_1.tick();
                red_slime_2.tick();
                red_slime_3.tick();
                red_slime_4.tick();
                red_slime_5.tick();
                red_slime_6.tick();
                red_slime_7.tick();
                red_slime_8.tick();
                red_slime_9.tick();

                // chomper chompy slime
                chompy_slime_0.tick();
                chompy_slime_1.tick();
                chompy_slime_2.tick();
                chompy_slime_3.tick();
                chompy_slime_4.tick();
                chompy_slime_5.tick();
                chompy_slime_6.tick();
                chompy_slime_7.tick();
                chompy_slime_8.tick();
                chompy_slime_9.tick();
                chompy_slime_10.tick();

                // bluey slimgton
                blue_slime_0.tick();
                blue_slime_1.tick();
                blue_slime_2.tick();
                blue_slime_3.tick();

                //spikey icey slime

                ice_slime_0.tick();
                ice_slime_1.tick();
                ice_slime_2.tick();
                ice_slime_3.tick();
                ice_slime_4.tick();
                ice_slime_5.tick();

                //bushyist of bois

                bush_boi_0.tick();
                bush_boi_1.tick();
                bush_boi_2.tick();
                bush_boi_3.tick();
                bush_boi_4.tick();
                bush_boi_5.tick();
                bush_boi_6.tick();
                bush_boi_7.tick();
                bush_boi_8.tick();
                bush_boi_9.tick();
                bush_boi_10.tick();
                bush_boi_11.tick();
                bush_boi_12.tick();
                bush_boi_13.tick();
                bush_boi_14.tick();

                // spoopy ghosters
                enemy_ghost_0.tick();
                enemy_ghost_1.tick();
                enemy_ghost_2.tick();
                enemy_ghost_3.tick();
                enemy_ghost_4.tick();
                enemy_ghost_5.tick();
                
                // sussy vase
                sussy_vase_0.tick();
                sussy_vase_1.tick();
                sussy_vase_2.tick();
                sussy_vase_3.tick();
                sussy_vase_4.tick();
                sussy_vase_5.tick();
                sussy_vase_6.tick();

                if (player.am_attacking() == true) {
                    //red slimes
                    red_slime_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_0.give_health(), red_slime_0.give_x(), red_slime_0.give_y());
                    red_slime_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_1.give_health(), red_slime_1.give_x(), red_slime_1.give_y());
                    red_slime_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_2.give_health(), red_slime_2.give_x(), red_slime_2.give_y());
                    red_slime_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_3.give_health(), red_slime_3.give_x(), red_slime_3.give_y());
                    red_slime_4.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_4.give_health(), red_slime_4.give_x(), red_slime_4.give_y());
                    red_slime_5.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_5.give_health(), red_slime_5.give_x(), red_slime_5.give_y());
                    red_slime_6.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_6.give_health(), red_slime_6.give_x(), red_slime_6.give_y());
                    red_slime_7.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_7.give_health(), red_slime_7.give_x(), red_slime_7.give_y());
                    red_slime_8.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_8.give_health(), red_slime_8.give_x(), red_slime_8.give_y());
                    red_slime_9.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), red_slime_9.give_health(), red_slime_9.give_x(), red_slime_9.give_y());

                    //commpers the slimington

                    chompy_slime_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_0.give_health(), chompy_slime_0.give_x(), chompy_slime_0.give_y());
                    chompy_slime_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_1.give_health(), chompy_slime_1.give_x(), chompy_slime_1.give_y());
                    chompy_slime_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_2.give_health(), chompy_slime_2.give_x(), chompy_slime_2.give_y());
                    chompy_slime_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_3.give_health(), chompy_slime_3.give_x(), chompy_slime_3.give_y());
                    chompy_slime_4.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_4.give_health(), chompy_slime_4.give_x(), chompy_slime_4.give_y());
                    chompy_slime_5.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_5.give_health(), chompy_slime_5.give_x(), chompy_slime_5.give_y());
                    chompy_slime_6.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_6.give_health(), chompy_slime_6.give_x(), chompy_slime_6.give_y());
                    chompy_slime_7.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_7.give_health(), chompy_slime_7.give_x(), chompy_slime_7.give_y());
                    chompy_slime_8.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_8.give_health(), chompy_slime_8.give_x(), chompy_slime_8.give_y());
                    chompy_slime_9.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_9.give_health(), chompy_slime_9.give_x(), chompy_slime_9.give_y());
                    chompy_slime_10.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), chompy_slime_10.give_health(), chompy_slime_10.give_x(), chompy_slime_10.give_y());

                    // blooper the bluey slime

                    blue_slime_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), blue_slime_0.give_health(), blue_slime_0.give_x(), blue_slime_0.give_y());
                    blue_slime_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), blue_slime_1.give_health(), blue_slime_1.give_x(), blue_slime_1.give_y());
                    blue_slime_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), blue_slime_2.give_health(), blue_slime_2.give_x(), blue_slime_2.give_y());
                    blue_slime_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), blue_slime_3.give_health(), blue_slime_3.give_x(), blue_slime_3.give_y());

                    // icey the spikey slime

                    ice_slime_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), ice_slime_0.give_health(), ice_slime_0.give_x(), ice_slime_0.give_y());
                    ice_slime_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), ice_slime_1.give_health(), ice_slime_1.give_x(), ice_slime_1.give_y());
                    ice_slime_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), ice_slime_2.give_health(), ice_slime_2.give_x(), ice_slime_2.give_y());
                    ice_slime_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), ice_slime_3.give_health(), ice_slime_3.give_x(), ice_slime_3.give_y());
                    ice_slime_4.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), ice_slime_4.give_health(), ice_slime_4.give_x(), ice_slime_4.give_y());
                    ice_slime_5.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), ice_slime_5.give_health(), ice_slime_5.give_x(), ice_slime_5.give_y());

                    // bushy bois be dying
                    bush_boi_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_0.give_health(), bush_boi_0.give_x(), bush_boi_0.give_y());
                    bush_boi_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_1.give_health(), bush_boi_1.give_x(), bush_boi_1.give_y());
                    bush_boi_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_2.give_health(), bush_boi_2.give_x(), bush_boi_2.give_y());
                    bush_boi_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_3.give_health(), bush_boi_3.give_x(), bush_boi_3.give_y());
                    bush_boi_4.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_4.give_health(), bush_boi_4.give_x(), bush_boi_4.give_y());
                    bush_boi_5.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_5.give_health(), bush_boi_5.give_x(), bush_boi_5.give_y());
                    bush_boi_6.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_6.give_health(), bush_boi_6.give_x(), bush_boi_6.give_y());
                    bush_boi_7.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_7.give_health(), bush_boi_7.give_x(), bush_boi_7.give_y());
                    bush_boi_8.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_8.give_health(), bush_boi_8.give_x(), bush_boi_8.give_y());
                    bush_boi_9.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_9.give_health(), bush_boi_9.give_x(), bush_boi_9.give_y());
                    bush_boi_10.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_10.give_health(), bush_boi_10.give_x(), bush_boi_10.give_y());
                    bush_boi_11.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_11.give_health(), bush_boi_11.give_x(), bush_boi_11.give_y());
                    bush_boi_12.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_12.give_health(), bush_boi_12.give_x(), bush_boi_12.give_y());
                    bush_boi_13.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_13.give_health(), bush_boi_13.give_x(), bush_boi_13.give_y());
                    bush_boi_14.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), bush_boi_14.give_health(), bush_boi_14.give_x(), bush_boi_14.give_y());
                    
                    // spoopy ghost
                    enemy_ghost_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), enemy_ghost_0.give_health(), enemy_ghost_0.give_x(), enemy_ghost_0.give_y());
                    enemy_ghost_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), enemy_ghost_1.give_health(), enemy_ghost_1.give_x(), enemy_ghost_1.give_y());
                    enemy_ghost_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), enemy_ghost_2.give_health(), enemy_ghost_2.give_x(), enemy_ghost_2.give_y());
                    enemy_ghost_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), enemy_ghost_3.give_health(), enemy_ghost_3.give_x(), enemy_ghost_3.give_y());
                    enemy_ghost_4.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), enemy_ghost_4.give_health(), enemy_ghost_4.give_x(), enemy_ghost_4.give_y());
                    enemy_ghost_5.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), enemy_ghost_5.give_health(), enemy_ghost_5.give_x(), enemy_ghost_5.give_y());
                    
                    // sussy vase
                    sussy_vase_0.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_0.give_health(), sussy_vase_0.give_x(), sussy_vase_0.give_y());
                    sussy_vase_1.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_1.give_health(), sussy_vase_1.give_x(), sussy_vase_1.give_y());
                    sussy_vase_2.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_2.give_health(), sussy_vase_2.give_x(), sussy_vase_2.give_y());
                    sussy_vase_3.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_3.give_health(), sussy_vase_3.give_x(), sussy_vase_3.give_y());
                    sussy_vase_4.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_4.give_health(), sussy_vase_4.give_x(), sussy_vase_4.give_y());
                    sussy_vase_5.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_5.give_health(), sussy_vase_5.give_x(), sussy_vase_5.give_y());
                    sussy_vase_6.attacked(player.weapon_x(), player.weapon_y(), 58, player.get_weapon_type(), sussy_vase_6.give_health(), sussy_vase_6.give_x(), sussy_vase_6.give_y());
                }
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

                nmap.draw_extras(projection_matrix * *player.get_trans_matrix());

                ui.draw_text(projection_matrix * *player.get_trans_matrix());

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

            if (pause == true) /*variable to test is paused*/
            {
                glDisable(GL_DEPTH_TEST);
                va.Bind();
                s.Bind();
                s.SetUniform1i("u_Texture", 8);
                s.SetUniform1f("u_alpha", 0.5f);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glEnable(GL_DEPTH_TEST);
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        /*
        for (Creature* boi : bad_bois) {
            delete boi;
        }
        */
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