#include "framework.h"

#if OS_WINDOWS
#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

struct OS_Window {
    HWND hwnd;
};
#endif

#if OS_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <time.h>

struct OS_Window {
    Window window;
};
#endif

#define GL_IMPLEMENTATION
#include "GL.h"

// Globals.
bool should_quit = false;
int back_buffer_width;
int back_buffer_height;

Key_State key_left;
Key_State key_right;
Key_State key_space;
Key_State key_esc;

// Internal.
static GLuint vbo;
static GLuint last_bound_texture_id;
static GLuint shader_program;
static GLuint projection_loc;

struct Vertex {
    Vector3 position;
    Vector4 color;
    Vector2 uv;
};

const int MAX_VERTICES = 1024;
static Vertex vertices[MAX_VERTICES];
int vertex_count = 0;

int current_vertex_per_primitive = 3;

const char *vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 in_uv;

out vec4 color;
out vec2 uv;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(in_position.x, in_position.y, in_position.z, 1.0f);
    color       = in_color;
    uv          = in_uv;
}
)";

const char *fragment_shader_source = R"(
#version 330 core
out vec4 final_color;

in vec4 color;
in vec2 uv;

uniform sampler2D texture_map;

void main() {
    final_color = texture(texture_map, uv) * color;
}
)";

void init_framework(void) {
    const GLubyte *gl_version  = glGetString(GL_VERSION);
    const GLubyte *gl_vendor   = glGetString(GL_VENDOR);
    const GLubyte *gl_renderer = glGetString(GL_RENDERER);
    const GLubyte *gl_shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    print("OpenGL Version:  %s\n", gl_version);
    print("OpenGL Vendor:   %s\n", gl_vendor);
    print("OpenGL Renderer: %s\n", gl_renderer);
    print("OpenGL Shading Language Version: %s\n", gl_shader_version);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, null);
    glCompileShader(vertex_shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, size_of(infoLog), NULL, infoLog);
        print("Failed to compile vertex shader:\n%s\n", infoLog);
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, null);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, size_of(infoLog), NULL, infoLog);
        print("Failed to compile fragment shader:\n%s\n", infoLog);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * size_of(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * size_of(float), (void *)12);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * size_of(float), (void *)28);
    glEnableVertexAttribArray(2);

    glUseProgram(shader_program);

    projection_loc = glGetUniformLocation(shader_program, "projection");


    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void frame_begin(int vertex_per_primitive) {
    if (vertex_per_primitive != current_vertex_per_primitive) {
        frame_flush();
    }

    current_vertex_per_primitive = vertex_per_primitive;
}

void frame_flush(void) {
    if (!vertex_count) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size_of(vertices[0]) * vertex_count, vertices, GL_STREAM_DRAW);

    if (current_vertex_per_primitive == 3) {
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    } else {
        glDrawArrays(GL_LINES, 0, vertex_count);
    }

    vertex_count = 0;
}

void render_update_texture(Texture *texture, unsigned char *data) {
    if (!texture->id) {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texture->id);
    }

    if (!texture->id) return;

    // https://learn.microsoft.com/en-us/windows/win32/opengl/glteximage2d
    GLenum gl_source_format = GL_RGBA8; // Texture format.
    GLenum gl_dest_format   = GL_RGBA;  // The format of the pixel data.
    if (texture->channels == 3) {
        gl_source_format = GL_RGB8;
        gl_dest_format   = GL_RGB;
    }

    // @Todo: Add more formats.
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glTexImage2D(GL_TEXTURE_2D, 0, gl_source_format, 
                 texture->width, texture->height, 
                 0, gl_dest_format, GL_UNSIGNED_BYTE, 
                 data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture texture_load_from_file(const char *file_path) {
    Texture result = {};

    stbi_uc *data = stbi_load(file_path, &result.width, &result.height, &result.channels, 0);
    if (data) {
        render_update_texture(&result, data);

        stbi_image_free(data);
    } else {
        print("Failed to load: %s\n", file_path);
    }

    return result;
}

Texture texture_load_from_memory(void *memory, s32 w, s32 h, s32 channels) {
    assert(memory != null);

    Texture result = {};
    result.width    = w;
    result.height   = h;
    result.channels = channels;

    render_update_texture(&result, (u8 *)memory);

    return result;
}

void set_texture(Texture *texture) {
if (last_bound_texture_id != texture->id) {
        frame_flush();
        GLint texture_handle = glGetUniformLocation(shader_program, "texture_map");
        glUniform1i(texture_handle, 0);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    last_bound_texture_id = texture->id;
}

void rendering_2d(int w, int h, float x, float y) {
    float proj[16] = {
        2.0f/w,  0,       0,   0,
        0,       2.0f/h,  0,   0,
        0,       0,       1,   0,
        x-1,       y-1,       0,   1
    };
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, proj);
}

void draw_vertex(Vector2 p, Vector4 c) {
    if (vertex_count > (MAX_VERTICES-1)) frame_flush();

    Vertex *v = vertices + vertex_count;
    v->position.x = p.x;
    v->position.y = p.y;
    v->position.z = 0;
    v->uv.x       = 0;
    v->uv.y       = 0;
    v->color      = c;

    vertex_count += 1;
}

void draw_quad(float x0, float y0, float x1, float y1, Vector4 c) {
    if (vertex_count > (MAX_VERTICES - 6)) frame_flush();

    Vertex *v = vertices + vertex_count;
    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 0;
    v->uv.y       = 1;
    v += 1;

    v->position.x = x1;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 1;
    v->uv.y       = 1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 1;
    v->uv.y       = 0;
    v += 1;

    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 0;
    v->uv.y       = 1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 1;
    v->uv.y       = 0;
    v += 1;

    v->position.x = x0;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = 0;
    v->uv.y       = 0;
    v += 1;

    vertex_count += 6;
}

void draw_quad(float x0, float y0, float x1, float y1, 
               float u0, float v0, float u1, float v1,
               Vector4 c) {
    if (vertex_count > (MAX_VERTICES - 6)) frame_flush();

    Vertex *v = vertices + vertex_count;
    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u0;
    v->uv.y       = v1;
    v += 1;

    v->position.x = x1;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u1;
    v->uv.y       = v1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u1;
    v->uv.y       = v0;
    v += 1;

    v->position.x = x0;
    v->position.y = y0;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u0;
    v->uv.y       = v1;
    v += 1;

    v->position.x = x1;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u1;
    v->uv.y       = v0;
    v += 1;

    v->position.x = x0;
    v->position.y = y1;
    v->position.z = 0;
    v->color      = c;
    v->uv.x       = u0;
    v->uv.y       = v0;
    v += 1;

    vertex_count += 6;
}

void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {
    if (vertex_count > (MAX_VERTICES - 6)) frame_flush();

    Vertex *v = vertices + vertex_count;
    v->position = p0;
    v->color    = c0;
    v->uv.x     = 0;
    v->uv.y     = 1;
    v += 1;

    v->position = p1;
    v->color    = c1;
    v->uv.x     = 1;
    v->uv.y     = 1;
    v += 1;

    v->position = p2;
    v->color    = c2;
    v->uv.x     = 1;
    v->uv.y     = 0;
    v += 1;

    v->position = p0;
    v->color    = c0;
    v->uv.x     = 0;
    v->uv.y     = 1;
    v += 1;

    v->position = p2;
    v->color    = c2;
    v->uv.x     = 1;
    v->uv.y     = 0;
    v += 1;

    v->position = p3;
    v->color    = c3;
    v->uv.x     = 0;
    v->uv.y     = 0;
    v += 1;

    vertex_count += 6;
}

void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector2 uv3,
               Vector4 color) {
    if (vertex_count > (MAX_VERTICES - 6)) frame_flush();

    Vertex *v = vertices + vertex_count;
    v->position = p0;
    v->color    = color;
    v->uv.x     = uv0.x;
    v->uv.y     = uv3.y;
    v += 1;

    v->position = p1;
    v->color    = color;
    v->uv.x     = uv1.x;
    v->uv.y     = uv2.y;
    v += 1;

    v->position = p2;
    v->color    = color;
    v->uv.x     = uv2.x;
    v->uv.y     = uv1.y;
    v += 1;

    v->position = p0;
    v->color    = color;
    v->uv.x     = uv0.x;
    v->uv.y     = uv3.y;
    v += 1;

    v->position = p2;
    v->color    = color;
    v->uv.x     = uv2.x;
    v->uv.y     = uv1.y;
    v += 1;

    v->position = p3;
    v->color    = color;
    v->uv.x     = uv3.x;
    v->uv.y     = uv0.y;
    v += 1;

    vertex_count += 6;
}

Vector2 rotate_z(Vector2 v, Vector2 c, float theta) {
    Vector2 result;

    float ct = cosf(theta);
    float st = sinf(theta);

    v.x -= c.x;
    v.y -= c.y;

    result.x = v.x*ct - v.y*st;
    result.y = v.x*st + v.y*ct;

    result.x += c.x;
    result.y += c.y;

    return result;
}




#if OS_WINDOWS

#if COMPILER_CL
#pragma comment(lib, "Gdi32.lib")
#endif


HGLRC gl_context;

bool opengl_init(HWND window) {
    // Create dummy window.
    HINSTANCE hInstance = GetModuleHandleW(null);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = size_of(WNDCLASSEXW);
    wcex.hInstance     = hInstance;
    wcex.lpszClassName = L"Dummy Window";
    wcex.lpfnWndProc   = DefWindowProcW;

    // Register the window class.
    if (RegisterClassExW(&wcex) == 0) {
        write_string("RegisterClassExW returned 0.\n", true);
        return false;
    }

    HWND hwnd = CreateWindowExW(0,
                                L"Dummy Window",
                                L"Dummy Window",
                                0,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                null,
                                null,
                                hInstance,
                                null);

    if (!hwnd) {
        write_string("CreateWindowExW returned 0.\n", true);
        return false;
    }

    HDC dummy_dc = GetDC(hwnd);

    {
        PIXELFORMATDESCRIPTOR pixel_format = {};
        pixel_format.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format.nVersion     = 1;
        pixel_format.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format.iPixelType   = PFD_TYPE_RGBA;
        pixel_format.cColorBits   = 32;
        pixel_format.cDepthBits   = 24;
        pixel_format.cStencilBits = 8;
        pixel_format.iLayerType   = PFD_MAIN_PLANE;

        int pixel_format_index = ChoosePixelFormat(dummy_dc, &pixel_format);
        if (!pixel_format_index) {
            write_string("Failed to ChoosePixelFormat.\n", true);
            return false;
        }

        BOOL success = SetPixelFormat(dummy_dc, pixel_format_index, &pixel_format);
        if (success == FALSE) {
            write_string("Failed to SetPixelFormat.\n", true);
            return false;
        }
    }

    if (!gl_load()) return false;

    // Get dummy GL context.
    HGLRC hglrc = W32_wglCreateContext(dummy_dc);
    if (!hglrc) {
        write_string("Failed to wglCreateContext.\n", true);
        return false;
    }

    if (!W32_wglMakeCurrent(dummy_dc, hglrc)) {
        write_string("Failed to wglMakeCurrent.\n", true);
        return false;
    }

    // Load WGL functions using wglGetProcAddress.
    GL_LOAD_PROC(wglCreateContextAttribsARB);
    GL_LOAD_PROC(wglChoosePixelFormatARB);

    W32_wglDeleteContext(hglrc);
    ReleaseDC(hwnd, dummy_dc);
    DestroyWindow(hwnd);
    UnregisterClassW(L"Dummy Window", hInstance);


    // Create modern GL context.
    int pixel_attrib_list[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,

        // WGL_COLOR_BITS_ARB,     32,
        // WGL_DEPTH_BITS_ARB,     24,
        // WGL_STENCIL_BITS_ARB,   8,
        // WGL_AUX_BUFFERS_ARB,    1,

        0,  // Terminator.
    };

    HDC hdc = GetDC(window);

    int pixel_format_index;
    UINT extended_pick;
    BOOL success = wglChoosePixelFormatARB(hdc, pixel_attrib_list, null, 1, 
        &pixel_format_index, &extended_pick);
    if ((success == FALSE) || (extended_pick == 0)) {
        write_string("Failed to wglChoosePixelFormatARB.\n", true);
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    if (!DescribePixelFormat(hdc, pixel_format_index, 
                             size_of(pfd), &pfd)) {
        write_string("Failed to DescribePixelFormat.\n", true);
        return false;
    }

    if (!SetPixelFormat(hdc, pixel_format_index, &pfd)) {
        write_string("Failed to SetPixelFormat.\n", true);
        return false;
    }

    int gl_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
#if GL_USE_LEGACY_PROCS
        // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
#else
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#endif
        0,
    };

    gl_context = wglCreateContextAttribsARB(hdc, null, gl_attribs);
    if (!gl_context) {
        write_string("Failed to wglCreateContextAttribsARB.\n", true);
        return false;
    }

    if (!W32_wglMakeCurrent(hdc, gl_context)) {
        write_string("Failed to wglMakeCurrent.\n", true);
        return false;
    }

    // Load modern GL functions...
    gl_load_extensions();
    
    return true;
}

static LRESULT CALLBACK win32_main_window_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_ERASEBKGND:
            return 1;

        case WM_CLOSE:
            should_quit = true;
            break;

        case WM_SIZE:
            back_buffer_width  = LOWORD(lparam);
            back_buffer_height = HIWORD(lparam);
            break;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            // bool was_down = ((s32)lparam & 0x40000000) != 0;
            bool is_down  = true;
            
            if (wparam == VK_LEFT) {
                key_left.is_down  = is_down;
            } else if (wparam == VK_RIGHT) {
                key_right.is_down  = is_down;
            } else if (wparam == VK_SPACE) {
                key_space.is_down  = is_down;
            } else if (wparam == VK_ESCAPE) {
                key_esc.is_down  = is_down;
            }
        } break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            bool is_down  = false;
            
            if (wparam == VK_LEFT) {
                key_left.is_down = is_down;
            } else if (wparam == VK_RIGHT) {
                key_right.is_down = is_down;
            } else if (wparam == VK_SPACE) {
                key_space.is_down = is_down;
            } else if (wparam == VK_ESCAPE) {
                key_esc.is_down = is_down;
            }
        } break;

        default:
            return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    return 0;
}

static float64 one_over_frequency = 1.0;

OS_Window *init_window(const char *title, int w, int h) {
    HINSTANCE hInstance = GetModuleHandleW(null);

    WNDCLASSEXW wc = {};
    wc.cbSize = size_of(WNDCLASSEXW);
    wc.style                = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.cbClsExtra           = 0;
    wc.cbWndExtra           = 0;
    wc.hInstance            = hInstance;
    wc.hCursor              = LoadCursorW(null, IDC_ARROW);
    // wc.hbrBackground        = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName         = null;
    wc.lpszClassName        = L"WindowClassName";

    wc.lpfnWndProc          = win32_main_window_callback;


    // Register the window class.
    if (RegisterClassExW(&wc) == 0) {
        write_string("RegisterClassExW returned 0.\n", true);
        return 0;
    }

    RECT rect = {};
    rect.right = w;
    rect.bottom = h;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);

    WCHAR *w32_utf8_to_wide(const char *s, Allocator allocator);
    WCHAR *title_wide = w32_utf8_to_wide(title, temporary_allocator);

    HWND hwnd = CreateWindowExW(0,
                                L"WindowClassName",
                                title_wide,
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                rect.right-rect.left, rect.bottom-rect.left,
                                null,
                                null,
                                hInstance,
                                null);

    if (hwnd == null) {
        write_string("CreateWindowExW returned 0.\n", true);
        return 0;
    }

    OS_Window *result = New(OS_Window);
    result->hwnd = hwnd;

    opengl_init(hwnd);
    
    init_framework();

    // Display the window.
    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    LARGE_INTEGER large_frequency;
    if (QueryPerformanceFrequency(&large_frequency)) {
        one_over_frequency = 1.0 / (float64)large_frequency.QuadPart;
    }

    back_buffer_width = w;
    back_buffer_height = h;

    return result;
}

float64 get_current_time(void) {
    LARGE_INTEGER wall_counter;
    QueryPerformanceCounter(&wall_counter);
    return wall_counter.QuadPart * one_over_frequency;
}

void free_window_and_opengl(OS_Window *w) {
    UNUSED(w->hwnd);
    W32_wglMakeCurrent(null, null);
    W32_wglDeleteContext(gl_context);
    MemFree(w);
}

void update_window_events(void) {
    MSG msg;
    while (PeekMessageW(&msg, null, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void swap_buffers(OS_Window *w) {
    HDC hdc = GetDC(w->hwnd);
    BOOL ok = SwapBuffers(hdc);
    if (ok == FALSE) {
        write_string("Failed to SwapBuffers.\n", true);
    }
}

void os_sleep(u32 ms) {
    Sleep((DWORD)ms);
}

#endif  // OS_WINDOWS



#if OS_LINUX

GLXFBConfig glx_choose_best_fb_config(Display *display, int screen_id, int *visual_attributes) {
    // Getting matching framebuffer configs.
    int fb_count;
    GLXFBConfig *fb_configs = glXChooseFBConfig(display, screen_id, visual_attributes, &fb_count);
    if (!fb_configs) {
        write_string("Failed to glXChooseFBConfig.\n", true);
        return 0;
    }

    print("Found %d matching FB configs.\n", fb_count);

    int best_fbc_index  = -1;
    int worst_fbc_index = -1;
    int best_sample_count  = -1;
    int worst_sample_count = 999;

    for (int index = 0; index < fb_count; index++) {
        XVisualInfo *vi = glXGetVisualFromFBConfig(display, fb_configs[index]);
        if (vi) {
            int sample_buffer, sample_count;
            glXGetFBConfigAttrib(display, fb_configs[index], GLX_SAMPLE_BUFFERS, &sample_buffer);
            glXGetFBConfigAttrib(display, fb_configs[index], GLX_SAMPLES,        &sample_count);

            if ((best_fbc_index < 0) || (sample_buffer && (sample_count > best_sample_count))) {
                best_fbc_index = index;
                best_sample_count = sample_count;
            }

            if ((worst_fbc_index < 0) || (!sample_buffer && (sample_count < worst_sample_count))) {
                worst_fbc_index = index;
                worst_sample_count = sample_count;
            }
            XFree(vi);
        }
    }

    GLXFBConfig best_config = fb_configs[best_fbc_index];
    XFree(fb_configs);

    return best_config;
}

static Display *display;
static Colormap colormap;
static Atom wm_delete_window;
static GLXContext gl_context;

static KeyCode xkey_left;
static KeyCode xkey_right;
static KeyCode xkey_space;
static KeyCode xkey_escape;

OS_Window *init_window(const char *title, int w, int h) {
    display = XOpenDisplay(null);
    if (!display) {
        write_string("Failed to XOpenDisplay.\n", true);
        return 0;
    }

    if (!glx_load()) return 0;

    // Get a matching FB config.
    int visual_attributes[] = {
        GLX_X_RENDERABLE,   True,
        GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,    GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_ALPHA_SIZE,     8,
        GLX_DEPTH_SIZE,     24,
        GLX_STENCIL_SIZE,   8,
        GLX_DOUBLEBUFFER,   True,
        // GLX_SAMPLE_BUFFERS, 1,
        // GLX_SAMPLES,        4,
        None,
    };

    // GLX version.
    GLint glx_major, glx_minor;
    glXQueryVersion(display, &glx_major, &glx_minor);
    if (((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1)) {
        write_string("Invalid GLX version: You need at least GLX 1.3.\n", true);
        return 0;
    }

    print("GLX Version: %d.%d\n", glx_major, glx_minor);

    GL_LOAD_PROC(glXCreateContextAttribsARB);
    GL_LOAD_PROC(glXChooseFBConfig);
    GL_LOAD_PROC(glXGetFBConfigAttrib);
    GL_LOAD_PROC(glXGetVisualFromFBConfig);
    GL_LOAD_PROC(glXQueryExtensionsString);

    int screen_id   = XDefaultScreen(display);
    u64 black_color = BlackPixel(display, screen_id);
    u64 white_color = WhitePixel(display, screen_id);

    GLXFBConfig best_config = glx_choose_best_fb_config(display, screen_id, visual_attributes);

    // Get visual.
    XVisualInfo *visual_info = glXGetVisualFromFBConfig(display, best_config);

    Window parent_window = RootWindow(display, screen_id);

    colormap = XCreateColormap(display, parent_window, 
                                        visual_info->visual, 
                                        AllocNone);

    XSetWindowAttributes window_attributes = {};
    window_attributes.border_pixel      = black_color;
    window_attributes.background_pixel  = white_color;
    window_attributes.override_redirect = True;
    window_attributes.colormap   = colormap;
    window_attributes.event_mask = ExposureMask|KeyPressMask|KeyReleaseMask|StructureNotifyMask;

    Window window = XCreateWindow(display, parent_window, 
                                  10, 10,
                                  800, 600,
                                  0, 
                                  visual_info->depth, 
                                  InputOutput,
                                  visual_info->visual,
                                  CWBackPixel|CWColormap|CWBorderPixel|CWEventMask,
                                  &window_attributes);
    if (!window) {
        write_string("Failed to create window.\n", true);
        return 0;
    }

    XFree(visual_info);

    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING",  False);

    XChangeProperty(display, window, net_wm_name, utf8_string, 
                    8, PropModeReplace, 
                    (const u8 *)title, string_length(title));

    XStoreName(display, window, title);  // Legacy fallback.

    // Notify when the window manager attempts to delete the window.
    wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(display, window, &wm_delete_window, 1)) {
        write_string("Failed to XSetWMProtocols.\n", true);
    }

    const char *glx_extensions = glXQueryExtensionsString(display, screen_id);
    if (!gl_is_extension_supported(glx_extensions, "GLX_ARB_create_context")) {
        write_string("glXCreateContextAttribsARB() not found.\n", true);
        return 0;
    }

    int context_attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        // GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#if GL_USE_LEGACY_PROCS
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
#else
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
#endif
        None,
    };

    gl_context = glXCreateContextAttribsARB(display, 
                                                       best_config,
                                                       null,
                                                       True,
                                                       context_attributes);
    if (!gl_context) {
        write_string("Failed to glXCreateContextAttribsARB.\n", true);
        return 0;
    }

    if (!glXMakeCurrent(display, window, gl_context)) {
        write_string("Failed to glXMakeCurrent.\n", true);
        return 0;
    }

    if (!gl_load()) return 0;

    // Load modern GL functions...
    gl_load_extensions();

    XClearWindow(display, window);
    XMapRaised(display, window);

    XWindowAttributes size_attributes;
    XGetWindowAttributes(display, window, &size_attributes);
    
    init_framework();

    back_buffer_width = w;
    back_buffer_height = h;


    xkey_left   = XKeysymToKeycode(display, XK_Left);
    xkey_right  = XKeysymToKeycode(display, XK_Right);
    xkey_space  = XKeysymToKeycode(display, XK_space);
    xkey_escape = XKeysymToKeycode(display, XK_Escape);


    OS_Window *result = New(OS_Window);
    result->window = window;

    return result;
}

const float64 ONE_OVER_NANO_SECOND = 0.000000001;

float64 get_current_time(void) {
    struct timespec tspec;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tspec);
    return (float64)tspec.tv_sec + tspec.tv_nsec * ONE_OVER_NANO_SECOND;
}

void free_window_and_opengl(OS_Window *w) {
    glXDestroyContext(display, gl_context);

    XFreeColormap(display, colormap);
    XDestroyWindow(display, w->window);
    XCloseDisplay(display);

    MemFree(w);
}

void update_window_events(void) {
    // Non-blocking event loop.
    while (XPending(display) > 0) {
        XEvent event;
        XNextEvent(display, &event);

        switch (event.type) {
            case ClientMessage:
                if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
                    should_quit = true;
                }
                break;

            case DestroyNotify:
                should_quit = true;
                break;

            case Expose:
                // Repaint.
                break;

            case ConfigureNotify:
                // Handle resize here.
                back_buffer_width  = event.xconfigure.width;
                back_buffer_height = event.xconfigure.height;
                break;

            case KeyPress:
            {
                bool is_down = true;
                KeyCode key_code = event.xkey.keycode;

                if (key_code == xkey_left) {
                    key_left.is_down = is_down;
                } else if (key_code == xkey_right) {
                    key_right.is_down = is_down;
                } else if (key_code == xkey_space) {
                    key_space.is_down = is_down;
                } else if (key_code == xkey_escape) {
                    key_esc.is_down = is_down;
                }
            } break;

            case KeyRelease:
            {
                bool is_down = false;
                KeyCode key_code = event.xkey.keycode;

                if (key_code == xkey_left) {
                    key_left.is_down = is_down;
                } else if (key_code == xkey_right) {
                    key_right.is_down = is_down;
                } else if (key_code == xkey_space) {
                    key_space.is_down = is_down;
                } else if (key_code == xkey_escape) {
                    key_esc.is_down = is_down;
                }
            } break;
        }
    }
}

void swap_buffers(OS_Window *w) {
    glXSwapBuffers(display, w->window);
}

void os_sleep(u32 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec   = ms / 1000;
    ts.tv_nsec  = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, 0);
#else
    if (ms >= 1000) {
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

#endif  // OS_LINUX
