#include "general.h"
#include <math.h>

#include "stb_image.h"

#include "framework.cpp"

float ship_radius = 114.0f / 2.0f;
Vector2 ship_position = {400, 60};
int my_health = 3;

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool is_hostile;
};

Bullet bullets[256];
int bullet_count = 0;
float bullet_countdown = 0;

struct Invader {
    Vector2 position;
    Vector2 velocity;
};
float invader_bullet_countdown;

Invader invaders[256];
int invaders_count = 0;

inline int random_get(int min_value, int max_value) {
    int result = rand() % (max_value - min_value + 1) + min_value;
    return result;
}

inline float random_get_float(float x) {
    float result = ((float)rand() / (float)RAND_MAX) * x;
    return result;
}

inline void spawn_invaders(void) {
    invaders_count = random_get(6, 10);

    float x0 = 0.1f * back_buffer_width;
    float x1 = back_buffer_width - x0;
    float y0 = 0.55f * back_buffer_height;
    float y1 = 0.9f  * back_buffer_height;

    float min_speed = 80.0f;
    float max_speed = 180.0f;

    for (int index = 0; index < invaders_count; index++) {
        Invader *invader = &invaders[index];
        invader->position.x = x0 + random_get_float(x1-x0);
        invader->position.y = y0 + random_get_float(y1-y0);

        invader->velocity.x = min_speed + random_get_float(max_speed-min_speed);
        invader->velocity.y = 0;
    }
}

inline Bullet *fire_bullet(Vector2 position) {
    if (bullet_countdown > 0) return null;

    Bullet bullet;
    bullet.is_hostile = false;
    bullet.position.x = position.x - 45.0f/2.0f;
    bullet.position.y = position.y;
    bullet.velocity.x = 0;
    bullet.velocity.y = 280;

    bullet_countdown = 0.22f;

    if (bullet_count < array_count(bullets)) {
        Bullet *result = &bullets[bullet_count];
        bullet_count += 1;

        *result = bullet;
        return result;
    }

    return null;
}

inline Bullet *invader_fire_bullet(Invader *invader) {
    if (invader_bullet_countdown > 0) return null;

    Bullet bullet;
    bullet.is_hostile = true;
    bullet.position.x = invader->position.x - 45.0f/2.0f;
    bullet.position.y = invader->position.y - ship_radius;;
    bullet.velocity.x = 0;
    bullet.velocity.y = -240;
    if (random_get(0, 2)) {
        bullet.velocity.y = -320;
    }

    invader_bullet_countdown = 0.96f;

    if (bullet_count < array_count(bullets)) {
        Bullet *result = &bullets[bullet_count];
        bullet_count += 1;

        *result = bullet;
        return result;
    }

    return null;
}

float distance(Vector2 p0, Vector2 p1) {
    float result = (p1.x-p0.x)*(p1.x-p0.x) + (p1.y-p0.y)*(p1.y-p0.y);
    return sqrtf(result);
}

inline bool check_invaders_collision(Bullet *bullet) {
    if (bullet->is_hostile) {
        if (distance(ship_position, bullet->position) < (ship_radius*0.75f)) {
            my_health -= 1;
            return true;
        }
        return false;
    }

    for (int index = 0; index < invaders_count; index++) {
        Invader *invader = &invaders[index];

        if (distance(invader->position, bullet->position) < ship_radius) {
            invaders[index] = invaders[invaders_count-1];
            invaders_count -= 1;
            return true;
        }
    }

    return false;
}

int main(void) {
    auto window = init_window_and_opengl("Space Invaders", 800, 600);

    Texture spaceship  = texture_load_from_file("data/spaceship.png");
    Texture spaceship2 = texture_load_from_file("data/spaceship2.png");
    Texture bullet     = texture_load_from_file("data/bullet.png");
    Texture bullet2    = texture_load_from_file("data/bullet2.png");
    Texture white      = texture_load_from_file("data/white_pixel.png");

    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spawn_invaders();

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

        if (key_fire.is_down) {
            fire_bullet(ship_position);
        }

        float dx = 240.0f * current_dt;

        if (key_left.is_down)  ship_position.x -= dx;
        if (key_right.is_down) ship_position.x += dx;

        float x0 = 0.8f * ship_radius;
        float x1 = back_buffer_width - x0;

        if (ship_position.x < x0) ship_position.x = x0;
        if (ship_position.x > x1) ship_position.x = x1;



        bullet_countdown -= current_dt;
        if (bullet_countdown <= 0) bullet_countdown = 0;

        for (int index = 0; index < bullet_count; index++) {
            Bullet *b = &bullets[index];
            b->position.x += b->velocity.x * current_dt;
            b->position.y += b->velocity.y * current_dt;

            if (check_invaders_collision(b) || 
                (b->position.y > back_buffer_height)) {
                // Remove bullet outside of the view.
                bullets[index] = bullets[bullet_count-1];
                bullet_count -= 1;
            }
        }


        for (int index = 0; index < invaders_count; index++) {
            Invader *it = &invaders[index];
            it->position.x += it->velocity.x * current_dt;
            it->position.y += it->velocity.y * current_dt;

            if ((it->position.x > x1) || (it->position.x < x0)) {
                it->velocity.x = -it->velocity.x;
            }

            invader_bullet_countdown -= current_dt;

            int roll = random_get(0, 100);
            if (roll < 50) {
                invader_fire_bullet(it);
            }
        }


        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height);

        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        frame_begin();

        set_texture(&spaceship2);

        for (int index = 0; index < invaders_count; index++) {
            Invader *invader = &invaders[index];

            Vector2 p0;
            p0.x = invader->position.x - ship_radius;
            p0.y = invader->position.y - ship_radius;

            Vector2 p1;
            p1.x = invader->position.x + ship_radius;
            p1.y = invader->position.y + ship_radius;

            draw_quad(p0.x, p0.y, p1.x, p1.y, Vector4{1,1,1,1});
        }

        set_texture(&spaceship);
        {
            Vector2 p0;
            p0.x = ship_position.x - ship_radius;
            p0.y = ship_position.y - ship_radius;

            Vector2 p1;
            p1.x = ship_position.x + ship_radius;
            p1.y = ship_position.y + ship_radius;

            draw_quad(p0.x, p0.y, 
                      p1.x, p1.y, 
                      Vector4{1,1,1,1});
        }

        for (int index = 0; index < bullet_count; index++) {
            Bullet *b = &bullets[index];
            if (b->is_hostile) {
                set_texture(&bullet2);
            } else {
                set_texture(&bullet);
            }

            draw_quad(b->position.x,
                      b->position.y,
                      b->position.x + 45.0f,
                      b->position.y + 45.0f,
                      Vector4{1,1,1,1});
        }


        // HUD.
        set_texture(&white);
        float yy = back_buffer_height * 0.9f;
        for (int index = 0; index < my_health; index++) {
            float x = 20.0f * index + 5.0f;
            draw_quad(x, yy, x + 15.0f, yy + 15.0f, Vector4{1,1,1,1});
        }


        frame_begin(2);

        for (int index = 0; index < invaders_count; index++) {
            Invader *invader = &invaders[index];

            Vector2 p0;
            Vector2 p1;
            Vector2 p2;
            Vector2 p3;

            Vector4 color = {1,0,0,1};

            p0.x = invader->position.x - ship_radius;
            p0.y = invader->position.y - ship_radius;

            p1.x = invader->position.x + ship_radius;
            p1.y = invader->position.y - ship_radius;

            p2.x = invader->position.x + ship_radius;
            p2.y = invader->position.y + ship_radius;

            p3.x = invader->position.x - ship_radius;
            p3.y = invader->position.y + ship_radius;

            draw_vertex(p0, color);
            draw_vertex(p1, color);

            draw_vertex(p1, color);
            draw_vertex(p2, color);

            draw_vertex(p2, color);
            draw_vertex(p3, color);

            draw_vertex(p3, color);
            draw_vertex(p0, color);
        }

        {
            Vector4 color = {1,0,0,1};

            Vector2 p0;
            Vector2 p1;
            Vector2 p2;
            Vector2 p3;

            p0.x = ship_position.x - ship_radius * 0.75f;
            p0.y = ship_position.y - ship_radius * 0.75f;

            p1.x = ship_position.x + ship_radius * 0.75f;
            p1.y = ship_position.y - ship_radius * 0.75f;

            p2.x = ship_position.x + ship_radius * 0.75f;
            p2.y = ship_position.y + ship_radius * 0.75f;

            p3.x = ship_position.x - ship_radius * 0.75f;
            p3.y = ship_position.y + ship_radius * 0.75f;

            draw_vertex(p0, color);
            draw_vertex(p1, color);

            draw_vertex(p1, color);
            draw_vertex(p2, color);

            draw_vertex(p2, color);
            draw_vertex(p3, color);

            draw_vertex(p3, color);
            draw_vertex(p0, color);
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