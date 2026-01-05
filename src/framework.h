#ifndef FRAMEWORK_INCLUDE_H
#define FRAMEWORK_INCLUDE_H

#include "general.h"
#include "GL.h"

struct OS_Window;

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};

struct Texture {
    GLuint id;
    int width;
    int height;
    int channels;
};

struct Key_State {
    bool is_down;
    bool was_down;
};

extern Key_State key_left;
extern Key_State key_right;
extern Key_State key_space;
extern Key_State key_esc;

extern bool should_quit;
extern int back_buffer_width;
extern int back_buffer_height;

OS_Window *init_window(const char *title, int w, int h);
float64 get_current_time(void);
void free_window_and_opengl(OS_Window *w);
void update_window_events(void);
void swap_buffers(OS_Window *w);
void os_sleep(u32 ms);

void init_framework(void);
void frame_begin(int vertex_per_primitive = 3);
void frame_flush(void);
void render_update_texture(Texture *texture, unsigned char *data);
Texture texture_load_from_file(const char *file_path);
void set_texture(Texture *texture);
void rendering_2d(int w, int h, float x = 0, float y = 0);

void draw_vertex(Vector2 p, Vector4 c);

void draw_quad(float x0, float y0, float x1, float y1, Vector4 c);
void draw_quad(float x0, float y0, float x1, float y1, 
               float u0, float v0, float u1, float v1,
               Vector4 c);
void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3);
void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector2 uv3,
               Vector4 color);

Vector2 rotate_z(Vector2 v, Vector2 c, float theta);

#endif  // FRAMEWORK_INCLUDE_H