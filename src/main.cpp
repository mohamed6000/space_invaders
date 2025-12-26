#include "general.h"
#include <math.h>

#include "stb_image.h"

#include "framework.cpp"

Vector2 ship_size = {114.0f,114.0f};
Vector2 ship_position  = {400, 60};

struct Bullet {
    Vector2 position;
    Vector2 velocity;
};

Bullet bullets[256];
int bullet_count = 0;
float bullet_countdown = 0;

inline void fire_bullets(void) {
    if (bullet_countdown > 0) return;

    Bullet bullet;
    bullet.position.x = ship_position.x - 45.0f/2.0f;
    bullet.position.y = ship_position.y;
    bullet.velocity.x = 0;
    bullet.velocity.y = 280.0f;

    bullet_countdown = 0.22f;

    if (bullet_count < array_count(bullets)) {
        bullets[bullet_count] = bullet;
        bullet_count += 1;
    }
}

int main(void) {
    auto window = init_window_and_opengl("Space Invaders", 800, 600);

    Texture spaceship = texture_load_from_file("data/spaceship.png");
    Texture bullet    = texture_load_from_file("data/bullet.png");

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

        if (key_fire.is_down) fire_bullets();

        float dx = 240.0f * current_dt;

        if (key_left.is_down)  ship_position.x -= dx;
        if (key_right.is_down) ship_position.x += dx;

        float x0 = 0.4f * ship_size.x;
        float x1 = back_buffer_width - x0;

        if (ship_position.x < x0) ship_position.x = x0;
        if (ship_position.x > x1) ship_position.x = x1;



        bullet_countdown -= current_dt;
        if (bullet_countdown <= 0) bullet_countdown = 0;

        for (int index = 0; index < bullet_count; index++) {
            Bullet *b = &bullets[index];
            b->position.x += b->velocity.x * current_dt;
            b->position.y += b->velocity.y * current_dt;

            if (b->position.y > back_buffer_height * 0.9f) {
                // Remove bullet outside of the view.
                bullets[index] = bullets[bullet_count-1];
                bullet_count -= 1;
            }
        }


        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height);

        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        set_texture(&spaceship);

        Vector2 p0;
        p0.x = ship_position.x - ship_size.x/2;
        p0.y = ship_position.y - ship_size.y/2;

        Vector2 p1;
        p1.x = ship_position.x + ship_size.x/2;
        p1.y = ship_position.y + ship_size.y/2;

        draw_quad(p0.x, p0.y, 
                  p1.x, p1.y, 
                  Vector4{1,1,1,1});

        set_texture(&bullet);

        for (int index = 0; index < bullet_count; index++) {
            Bullet *b = &bullets[index];
            draw_quad(b->position.x,
                      b->position.y,
                      b->position.x + 45.0f,
                      b->position.y + 45.0f,
                      Vector4{1,1,1,1});
        }

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