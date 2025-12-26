#include "general.h"
#include <math.h>

#include "stb_image.h"

#include "framework.cpp"

int main(void) {
    auto window = init_window_and_opengl("Space Invaders", 800, 600);

    Texture spaceships = texture_load_from_file("data/spaceships.png");

    float u0 = 800.0f / spaceships.width;
    float v0 = 0.0f;
    float u1 = u0 + (112.0f / spaceships.width);
    float v1 = v0 + (100.0f / spaceships.height);

    Vector2 ship_size = {112.0f,100.0f};
    Vector2 ship_position  = {214, 10};

    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float64 last_counter = 0;

    while (!should_quit) {
        float64 wall_counter = get_current_time();
        float current_dt = (float)(wall_counter - last_counter);
        if (current_dt > 0.16f) current_dt = 0.16f;
        last_counter = wall_counter;

        update_window_events();

        if (key_esc.is_down) {
            should_quit = true;
        }

        float dx = 240.0f * current_dt;

        if (key_left.is_down)  ship_position.x -= dx;
        if (key_right.is_down) ship_position.x += dx;


        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height);

        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        draw_quad(ship_position.x, ship_position.y, 
                  ship_position.x + ship_size.x, 
                  ship_position.y + ship_size.y, 
                  u0, v0, u1, v1, 
                  Vector4{1,1,1,1});

        frame_flush();
        swap_buffers(window);

        Sleep(1);
    }

    free_window_and_opengl(window);

    return 0;
}






















#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"