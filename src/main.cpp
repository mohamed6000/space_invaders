#include "general.h"
#include <math.h>

#include "stb_image.h"

#include "framework.cpp"

float ship_radius     = 114.0f / 2.0f;
float pickup_radius   = 114.0f / 2.0f;
float bullet_radius   = 45.0f / 2.0f;
Vector2 ship_position = {400, 60};

Vector2 global_scroll;
Vector2 screen_shake;
float   shake_angle;
float   shake_radius = 1.0f;

bool has_force_shield = false;
float force_shield_countdown = 0;

int my_health  = 3;
int max_health = 5;
bool is_alive = true;
s64 my_score = 0;

enum Shot_Type {
    SHOT_SINGLE = 0,
    SHOT_DOUBLE = 1,
    SHOT_TRIPLE = 2,
};
int shot_type = SHOT_SINGLE;

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool is_hostile;
    int type;
};

Bullet bullets[256];
int bullet_count = 0;
float bullet_countdown = 0;

struct Invader {
    Vector2 position;
    Vector2 velocity;
    Vector2 destination;
    float sleep_cooldown;
    int texture_index;
};
float invader_bullet_countdown;

Invader invaders[256];
int invaders_count = 0;
bool all_introduced_invaders = false;

enum Pickup_Type {
    PICKUP_ONE   = 0,
    PICKUP_TWO   = 1,
    PICKUP_THREE = 2,
    PICKUP_HP    = 3,
};

struct Pickup {
    Vector2 position;
    Vector2 velocity;
    int type;
};

Pickup pickups[256];
int pickup_count = 0;

enum Level_State {
    LEVEL_ENEMY_INTRO,
    LEVEL_GAMEPLAY,
    LEVEL_BOSS_FIGHT,
};

Level_State level_state = LEVEL_ENEMY_INTRO;
int level_index = 1;

inline int random_get(int min_value, int max_value) {
    int result = rand() % (max_value - min_value + 1) + min_value;
    return result;
}

inline float random_get_float(float x) {
    float result = ((float)rand() / (float)RAND_MAX) * x;
    return result;
}

inline void spawn_invaders(int level) {
    invaders_count = random_get(6, 10 + 1 * level);

    int x0 = (int)(0.1f * back_buffer_width);
    int x1 = (int)(back_buffer_width - x0);
    int y0 = (int)(0.6f * back_buffer_height);
    int y1 = (int)(0.9f  * back_buffer_height);

    int min_speed = 80;
    int max_speed = 180;

    for (int index = 0; index < invaders_count; index++) {
        Invader *invader = &invaders[index];

        invader->destination.x = (float)random_get(x0, x1);
        invader->destination.y = (float)random_get(y0, y1);

        invader->position.x = invader->destination.x;
        invader->position.y = y1 + invader->destination.y;

        if (random_get(0, 2)) {
            invader->velocity.x = -(float)random_get(min_speed, max_speed);
        } else {
            invader->velocity.x =  (float)random_get(min_speed, max_speed);
        }

        invader->velocity.y = 0;

        invader->texture_index = random_get(0, 3);
    }
}

inline Bullet *fire_bullet(Vector2 position) {
    Bullet bullet;
    bullet.is_hostile = false;
    bullet.position.x = position.x;
    bullet.position.y = position.y;
    bullet.velocity.x = 0;
    bullet.velocity.y = 280;
    bullet.type = SHOT_SINGLE;

    bullet_countdown = 0.42f;

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
    bullet.position.x = invader->position.x;
    bullet.position.y = invader->position.y - ship_radius;
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
    if (!is_alive) return false;

    if (bullet->is_hostile) {
        if (distance(ship_position, bullet->position) < (ship_radius*0.75f)) {
            if (!has_force_shield) {
                my_health -= 1;
                if (my_health <= 0) {
                    is_alive = false;
                    print("Game Over: You have %lld points\n", my_score);
                }
                shake_radius = 2.0f;
            }
            return true;
        }
        return false;
    }

    for (int index = 0; index < invaders_count; index++) {
        Invader *invader = &invaders[index];

        if (distance(invader->position, bullet->position) < (ship_radius*0.9f)) {
            int roll = random_get(0, 100);
            if (roll < 60) {
                // Drop items.
                if (pickup_count < array_count(pickups)) {
                    Pickup *p = &pickups[pickup_count];
                    pickup_count += 1;

                    p->position.x = invader->position.x;
                    p->position.y = invader->position.y;
                    p->velocity.x = 0;
                    p->velocity.y = -230.0f;
                    p->type = random_get(0, 3);
                }
            }

            invaders[index] = invaders[invaders_count-1];
            invaders_count -= 1;

            my_score += 10;

            return true;
        }
    }

    return false;
}

inline void imm_draw_circle(Vector2 center, float radius, Vector4 color, int segments = 32) {
    for (int index = 0; index < segments; index++) {
        float theta0 = (float)index / segments * (float)TAU;
        float theta1 = (float)(index + 1) / segments * (float)TAU;

        float ct0 = cosf(theta0);
        float st0 = sinf(theta0);
        float ct1 = cosf(theta1);
        float st1 = sinf(theta1);

        float x0 = ct0 * radius;
        float y0 = st0 * radius;
        float x1 = ct1 * radius;
        float y1 = st1 * radius;

        Vector2 p0 = {center.x + x0, center.y + y0};
        Vector2 p1 = {center.x + x1, center.y + y1};

        draw_vertex(p0, color);
        draw_vertex(p1, color);
    }
}

void simulate_gameplay(float dt) {
    if (key_fire.is_down && (bullet_countdown <= 0) && is_alive) {
        if ((shot_type == SHOT_SINGLE) || (shot_type == SHOT_TRIPLE)) {
            fire_bullet(ship_position);
        }
        if ((shot_type == SHOT_DOUBLE) || (shot_type == SHOT_TRIPLE)) {
            Bullet *b1 = fire_bullet(ship_position);
            Bullet *b2 = fire_bullet(ship_position);

            b1->position.x = ship_position.x - (ship_radius * 0.75f);
            b1->velocity.y = 220.0f;
            b1->type = SHOT_DOUBLE;

            b2->position.x = ship_position.x + (ship_radius * 0.75f);
            b2->velocity.y = 220.0f;
            b2->type = SHOT_DOUBLE;
        }
    }

    float dx = 240.0f * dt;

    if (is_alive) {
        if (key_left.is_down) { 
            ship_position.x -= dx;
            global_scroll.x -= 0.1f * dt;
        }
        if (key_right.is_down) {
            ship_position.x += dx;
            global_scroll.x += 0.1f * dt;
        }
    }

    float x0 = 0.8f * ship_radius;
    float x1 = back_buffer_width - x0;

    if (ship_position.x < x0) ship_position.x = x0;
    if (ship_position.x > x1) ship_position.x = x1;

    force_shield_countdown -= dt;
    if (force_shield_countdown <= 0) has_force_shield = false;



    for (int index = 0; index < invaders_count; index++) {
        Invader *it = &invaders[index];
        it->position.x += it->velocity.x * dt;
        it->position.y += it->velocity.y * dt;

        if ((it->position.x > x1) || (it->position.x < x0)) {
            it->velocity.x = -it->velocity.x;
            it->position.x += it->velocity.x * dt;
        }

        invader_bullet_countdown -= dt;
        it->sleep_cooldown       -= dt;

        if (it->sleep_cooldown <= 0) {
            int roll = random_get(0, 100);
            if (roll < 50) {
                invader_fire_bullet(it);
            }

            it->sleep_cooldown = 0.55f;
        }
    }

    bullet_countdown -= dt;
    if (bullet_countdown <= 0) bullet_countdown = 0;

    for (int index = 0; index < bullet_count; index++) {
        Bullet *b = &bullets[index];
        b->position.x += b->velocity.x * dt;
        b->position.y += b->velocity.y * dt;

        if (check_invaders_collision(b) || 
            (b->position.y > back_buffer_height) ||
            (b->position.y < 0)) {
            // Remove bullet outside of the view.
            bullets[index] = bullets[bullet_count-1];
            bullet_count -= 1;
        }
    }


    for (int index = 0; index < pickup_count; index++) {
        Pickup *it = &pickups[index];
        it->position.x += it->velocity.x * dt;
        it->position.y += it->velocity.y * dt;

        if (it->position.y < 0) {
            // Remove the pickup.
            pickups[index] = pickups[pickup_count-1];
            pickup_count -= 1;
        }

        if (distance(ship_position, it->position) < ship_radius) {
            if (it->type == PICKUP_HP) {
                my_health += 1;
                if (my_health > max_health) my_health = max_health;
            } else if (it->type == PICKUP_TWO) {
                shot_type = SHOT_DOUBLE;
            } else if (it->type == PICKUP_THREE) {
                shot_type = SHOT_TRIPLE;
            } else if (it->type == PICKUP_ONE) {
                has_force_shield = true;
                force_shield_countdown = 5.0f;
            }

            // Remove the pickup.
            pickups[index] = pickups[pickup_count-1];
            pickup_count -= 1;

            my_score += 10;
        }
    }

    if (invaders_count == 0) {
        bullet_countdown = 0.1f;

        if (!bullet_count && !pickup_count) {
            // Give the player a bit of time before respawning.

            level_index += 1;
            if ((level_index % 10) == 0) max_health += 1;

            print("Level: %d (max_health = %d)\n", level_index, max_health);

            spawn_invaders(level_index);
            level_state = LEVEL_ENEMY_INTRO;
        }
    }

    screen_shake = {0, 0};
    if (shake_radius > 1) {
        screen_shake.x = sinf(shake_angle) * shake_radius * dt;
        screen_shake.y = cosf(shake_angle) * shake_radius * dt;
        shake_radius -= 5.0f * dt;
        shake_angle += 0.5f + random_get_float(1.5f - 0.5f);
    }
}

int main(void) {
    auto window = init_window_and_opengl("Space Invaders", 800, 600);

    Texture spaceship  = texture_load_from_file("data/spaceship.png");
    Texture spaceship_upgrade = texture_load_from_file("data/spaceship_upgrade.png");
    Texture spaceship2 = texture_load_from_file("data/spaceship2.png");
    Texture spaceship3 = texture_load_from_file("data/spaceship3.png");
    Texture spaceship4 = texture_load_from_file("data/spaceship4.png");
    Texture spaceship5 = texture_load_from_file("data/spaceship5.png");
    Texture bullet     = texture_load_from_file("data/bullet.png");
    Texture bullet2    = texture_load_from_file("data/bullet2.png");
    Texture missile    = texture_load_from_file("data/missile.png");
    Texture white      = texture_load_from_file("data/white_pixel.png");
    Texture force_shield = texture_load_from_file("data/force_shield.png");
    Texture background = texture_load_from_file("data/background.png");
    Texture mid        = texture_load_from_file("data/mid.png");

    // Pickups.
    Texture pickup1   = texture_load_from_file("data/pickup1.png");
    Texture pickup2   = texture_load_from_file("data/pickup2.png");
    Texture pickup3   = texture_load_from_file("data/pickup3.png");
    Texture pickup_hp = texture_load_from_file("data/pickup4.png");

    Texture *texture_pickups[4] = {&pickup1, &pickup2, &pickup3, &pickup_hp};
    Texture *invader_ships[4] = {&spaceship2, &spaceship3, &spaceship4, &spaceship5};

    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spawn_invaders(level_index);

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

        if (level_state == LEVEL_GAMEPLAY) {
            global_scroll.y -= 0.22f * current_dt;

            simulate_gameplay(current_dt);
        } else if (level_state == LEVEL_ENEMY_INTRO) {
            all_introduced_invaders = true;
            global_scroll.y -= 0.88f * current_dt;

            for (int index = 0; index < invaders_count; index++) {
                Invader *it = &invaders[index];

                if (it->position.y > it->destination.y) {
                    it->position.y -= 180.0f * current_dt;
                    all_introduced_invaders = false;
                }
            }

            if (all_introduced_invaders) {
                level_state = LEVEL_GAMEPLAY;
            }
        }

        if (global_scroll.y >= 1) global_scroll.y = 0;


        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height, screen_shake.x, screen_shake.y);

        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        frame_begin();

        // Background
        {
            float x0 = -(back_buffer_width * 0.25f);
            float x1 =  (back_buffer_width * 1.25f);

            float y0 = -(back_buffer_height * 0.25f);
            float y1 =  (back_buffer_height * 1.25f);

            set_texture(&background);
            draw_quad(x0, y0, x1, y1, 
                      global_scroll.x, global_scroll.y, 
                      global_scroll.x + 1, global_scroll.y + 1, 
                      Vector4{1,1,1,1});

            x0 = 0;
            x1 = (float)back_buffer_width;

            y0 = 0;
            y1 = (float)back_buffer_height;

            set_texture(&mid);
            float speed0 = 0.4f;
            draw_quad(x0, y0, x1, y1, 
                      global_scroll.x * speed0,     global_scroll.y * speed0, 
                      global_scroll.x * speed0 + 1, global_scroll.y * speed0 + 1, 
                      Vector4{1,1,1,1});
        }


        for (int index = 0; index < invaders_count; index++) {
            Invader *invader = &invaders[index];
            
            set_texture(invader_ships[invader->texture_index]);

            Vector2 p0;
            p0.x = invader->position.x - ship_radius;
            p0.y = invader->position.y - ship_radius;

            Vector2 p1;
            p1.x = invader->position.x + ship_radius;
            p1.y = invader->position.y + ship_radius;

            draw_quad(p0.x, p0.y, p1.x, p1.y, 0, 1, 1, 0, Vector4{1,1,1,1});
        }

        if (is_alive) {
            if (shot_type == SHOT_SINGLE)
                set_texture(&spaceship);
            else
                set_texture(&spaceship_upgrade);
            
            Vector2 p0;
            p0.x = ship_position.x - ship_radius;
            p0.y = ship_position.y - ship_radius;

            Vector2 p1;
            p1.x = ship_position.x + ship_radius;
            p1.y = ship_position.y + ship_radius;

            draw_quad(p0.x, p0.y, 
                      p1.x, p1.y,
                      Vector4{1,1,1,1});

            if (has_force_shield) {
                set_texture(&force_shield);
                draw_quad(p0.x, p0.y,
                          p1.x, p1.y,
                          Vector4{1,1,1,1});
            }
        }

        for (int index = 0; index < pickup_count; index++) {
            Pickup *it = &pickups[index];

            set_texture(texture_pickups[it->type]);
            
            draw_quad(it->position.x - pickup_radius, 
                      it->position.y - pickup_radius, 
                      it->position.x + pickup_radius, 
                      it->position.y + pickup_radius,
                      Vector4{1,1,1,1});
        }

        for (int index = 0; index < bullet_count; index++) {
            Bullet *b = &bullets[index];
            if (b->is_hostile) {
                set_texture(&bullet2);
            } else {
                if (b->type == SHOT_SINGLE)
                    set_texture(&bullet);
                else if (b->type == SHOT_DOUBLE)
                    set_texture(&missile);
                else assert(false);
            }

            draw_quad(b->position.x - bullet_radius,
                      b->position.y - bullet_radius,
                      b->position.x + bullet_radius,
                      b->position.y + bullet_radius,
                      Vector4{1,1,1,1});
        }


        // HUD.
        if (level_state == LEVEL_GAMEPLAY) {
            set_texture(&white);
            float yy = back_buffer_height * 0.9f;
            for (int index = 0; index < my_health; index++) {
                float x = 20.0f * index + 5.0f;
                draw_quad(x, yy, x + 15.0f, yy + 15.0f, Vector4{1,1,1,1});
            }
        }

        // Debug shapes.
#if 0
        frame_begin(2);

        for (int index = 0; index < invaders_count; index++) {
            Invader *invader = &invaders[index];

            imm_draw_circle(invader->position, ship_radius, {1,0,0,1});
        }

        imm_draw_circle(ship_position, ship_radius * 0.75f, {1,0,0,1});
#endif

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