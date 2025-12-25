#include "general.h"
#include <math.h>

#include "stb_image.h"

#include "framework.cpp"

int main(void) {
    auto window = init_window_and_opengl(640, 480);

    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float64 last_counter = get_current_time();

    while (!should_quit) {
        float64 wall_counter = get_current_time();
        float current_dt = (float)(wall_counter - last_counter);
        last_counter = wall_counter;

        UNUSED(current_dt);

        update_window_events();

        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height);

        glClearColor(0.2f, 0.38f, 0.72f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        

        frame_flush();
        swap_buffers(window);
    }

    free_window_and_opengl(window);

    return 0;
}






















#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"