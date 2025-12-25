#ifndef GL_INCLUDE_H
#define GL_INCLUDE_H

#ifndef APIENTRY
#   if OS_WINDOWS
#       define APIENTRY __stdcall
#   else
#       define APIENTRY
#   endif
#endif

#ifndef GLAPI
#define GLAPI extern
#endif

#ifdef GL_USE_LEGACY_PROCS
#undef GL_USE_LEGACY_PROCS
#define GL_USE_LEGACY_PROCS 1
#else
#define GL_USE_LEGACY_PROCS 0
#endif

#define GL_PROC(ident) CONCAT(ident,PROC)

#if OS_WINDOWS
#define GL_LOAD_PROC(ident) ident = (GL_PROC(ident) *)W32_wglGetProcAddress(#ident)
#elif OS_LINUX
#define GL_LOAD_PROC(ident) ident = (GL_PROC(ident) *)glXGetProcAddress((const GLubyte *)#ident)
#else
#error Undefined GL_LOAD_PROC
#endif


typedef unsigned int  GLenum;
typedef unsigned char GLboolean;
typedef unsigned int  GLbitfield;
typedef int           GLint;
typedef int           GLsizei;
typedef unsigned char GLubyte;
typedef unsigned int  GLuint;
typedef float         GLfloat;
typedef float         GLclampf;
typedef double        GLclampd;
typedef void          GLvoid;
typedef intptr_t      GLsizeiptr;
typedef char          GLchar;


#define GL_BLEND                   0x0BE2
#define GL_CULL_FACE               0x0B44
#define GL_DEPTH_TEST              0x0B71
#define GL_LINE_SMOOTH             0x0B20
#define GL_LINE_WIDTH              0x0B21
#define GL_LINE_WIDTH_RANGE        0x0B22
#define GL_LINE_WIDTH_GRANULARITY  0x0B23
#define GL_LINE_STIPPLE            0x0B24
#define GL_LINE_STIPPLE_PATTERN    0x0B25
#define GL_LINE_STIPPLE_REPEAT     0x0B26
#define GL_UNPACK_SWAP_BYTES       0x0CF0
#define GL_UNPACK_LSB_FIRST        0x0CF1
#define GL_UNPACK_ROW_LENGTH       0x0CF2
#define GL_UNPACK_SKIP_ROWS        0x0CF3
#define GL_UNPACK_SKIP_PIXELS      0x0CF4
#define GL_UNPACK_ALIGNMENT        0x0CF5
#define GL_PACK_SWAP_BYTES         0x0D00
#define GL_PACK_LSB_FIRST          0x0D01
#define GL_PACK_ROW_LENGTH         0x0D02
#define GL_PACK_SKIP_ROWS          0x0D03
#define GL_PACK_SKIP_PIXELS        0x0D04
#define GL_PACK_ALIGNMENT          0x0D05
#define GL_TEXTURE_1D              0x0DE0
#define GL_TEXTURE_2D              0x0DE1
#define GL_SCISSOR_BOX             0x0C10
#define GL_SCISSOR_TEST            0x0C11
#define GL_STENCIL_TEST            0x0B90
#define GL_STENCIL_CLEAR_VALUE     0x0B91
#define GL_STENCIL_FUNC            0x0B92
#define GL_STENCIL_VALUE_MASK      0x0B93
#define GL_STENCIL_FAIL            0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF             0x0B97
#define GL_STENCIL_WRITEMASK       0x0B98
#define GL_STENCIL_BITS            0x0D57


/* AlphaFunction */
#define GL_NEVER    0x0200
#define GL_LESS     0x0201
#define GL_EQUAL    0x0202
#define GL_LEQUAL   0x0203
#define GL_GREATER  0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL   0x0206
#define GL_ALWAYS   0x0207

/* BeginMode */
#define GL_POINTS         0x0000
#define GL_LINES          0x0001
#define GL_LINE_LOOP      0x0002
#define GL_LINE_STRIP     0x0003
#define GL_TRIANGLES      0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN   0x0006
#define GL_QUADS          0x0007
#define GL_QUAD_STRIP     0x0008
#define GL_POLYGON        0x0009

/* BlendingFactorDest */
#define GL_ZERO                0
#define GL_ONE                 1
#define GL_SRC_COLOR           0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA           0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA           0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305

/* BlendingFactorSrc */
#define GL_DST_COLOR           0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE  0x0308

/* Boolean */
#define GL_TRUE  1
#define GL_FALSE 0

/* DataType */
#define GL_BYTE           0x1400
#define GL_UNSIGNED_BYTE  0x1401
#define GL_SHORT          0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT            0x1404
#define GL_UNSIGNED_INT   0x1405
#define GL_FLOAT          0x1406
#define GL_2_BYTES        0x1407
#define GL_3_BYTES        0x1408
#define GL_4_BYTES        0x1409
#define GL_DOUBLE         0x140A

/* DrawBufferMode */
#define GL_NONE           0
#define GL_FRONT_LEFT     0x0400
#define GL_FRONT_RIGHT    0x0401
#define GL_BACK_LEFT      0x0402
#define GL_BACK_RIGHT     0x0403
#define GL_FRONT          0x0404
#define GL_BACK           0x0405
#define GL_LEFT           0x0406
#define GL_RIGHT          0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0           0x0409
#define GL_AUX1           0x040A
#define GL_AUX2           0x040B
#define GL_AUX3           0x040C

/* FrontFaceDirection */
#define GL_CW  0x0900
#define GL_CCW 0x0901

/* LogicOp */
#define GL_CLEAR         0x1500
#define GL_AND           0x1501
#define GL_AND_REVERSE   0x1502
#define GL_COPY          0x1503
#define GL_AND_INVERTED  0x1504
#define GL_NOOP          0x1505
#define GL_XOR           0x1506
#define GL_OR            0x1507
#define GL_NOR           0x1508
#define GL_EQUIV         0x1509
#define GL_INVERT        0x150A
#define GL_OR_REVERSE    0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED   0x150D
#define GL_NAND          0x150E
#define GL_SET           0x150F

/* MatrixMode */
#define GL_MODELVIEW  0x1700
#define GL_PROJECTION 0x1701

/* PixelFormat */
#define GL_COLOR_INDEX     0x1900
#define GL_STENCIL_INDEX   0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED             0x1903
#define GL_GREEN           0x1904
#define GL_BLUE            0x1905
#define GL_ALPHA           0x1906
#define GL_RGB             0x1907
#define GL_RGBA            0x1908
#define GL_LUMINANCE       0x1909
#define GL_LUMINANCE_ALPHA 0x190A

/* polygon_offset */
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_UNITS  0x2A00
#define GL_POLYGON_OFFSET_POINT  0x2A01
#define GL_POLYGON_OFFSET_LINE   0x2A02
#define GL_POLYGON_OFFSET_FILL   0x8037

/* texture */
#define GL_RGB8  0x8051
#define GL_RGBA8 0x8058

/* TextureEnvMode */
#define GL_MODULATE 0x2100
#define GL_DECAL    0x2101

/* TextureEnvParameter */
#define GL_TEXTURE_ENV_MODE  0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201

/* TextureEnvTarget */
#define GL_TEXTURE_ENV 0x2300

/* TextureMagFilter */
#define GL_NEAREST 0x2600
#define GL_LINEAR  0x2601

/* TextureMinFilter */
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST  0x2701
#define GL_NEAREST_MIPMAP_LINEAR  0x2702
#define GL_LINEAR_MIPMAP_LINEAR   0x2703

/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S     0x2802
#define GL_TEXTURE_WRAP_T     0x2803

/* TextureWrapMode */
#define GL_CLAMP  0x2900
#define GL_REPEAT 0x2901

/* StencilOp */
#define GL_KEEP    0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR    0x1E02
#define GL_DECR    0x1E03

#define GL_VENDOR     0x1F00
#define GL_RENDERER   0x1F01
#define GL_VERSION    0x1F02
#define GL_EXTENSIONS 0x1F03


#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000

// GLCOREARB.
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_ARRAY_BUFFER    0x8892
#define GL_STREAM_DRAW     0x88E0
#define GL_STATIC_DRAW     0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER   0x8B31
#define GL_COMPILE_STATUS  0x8B81
#define GL_LINK_STATUS     0x8B82
#define GL_TEXTURE0        0x84C0

// GL Core ARB Procs.
typedef void APIENTRY GL_PROC(glGenVertexArrays) (GLsizei n, GLuint *arrays);
typedef void APIENTRY GL_PROC(glBindVertexArray) (GLuint array);
typedef void APIENTRY GL_PROC(glGenBuffers) (GLsizei n, GLuint *buffers);
typedef void APIENTRY GL_PROC(glBindBuffer) (GLenum target, GLuint buffer);
typedef void APIENTRY GL_PROC(glBufferData) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void APIENTRY GL_PROC(glCompileShader) (GLuint shader);
typedef GLuint APIENTRY GL_PROC(glCreateProgram) (void);
typedef GLuint APIENTRY GL_PROC(glCreateShader) (GLenum type);
typedef void APIENTRY GL_PROC(glDeleteProgram) (GLuint program);
typedef void APIENTRY GL_PROC(glDeleteShader) (GLuint shader);
typedef void APIENTRY GL_PROC(glDetachShader) (GLuint program, GLuint shader);
typedef void APIENTRY GL_PROC(glDisableVertexAttribArray) (GLuint index);
typedef void APIENTRY GL_PROC(glEnableVertexAttribArray) (GLuint index);
typedef void APIENTRY GL_PROC(glLinkProgram) (GLuint program);
typedef void APIENTRY GL_PROC(glShaderSource) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void APIENTRY GL_PROC(glUseProgram) (GLuint program);
typedef void APIENTRY GL_PROC(glAttachShader) (GLuint program, GLuint shader);
typedef void APIENTRY GL_PROC(glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void APIENTRY GL_PROC(glGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
typedef void APIENTRY GL_PROC(glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY GL_PROC(glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef GLint APIENTRY GL_PROC(glGetUniformLocation) (GLuint program, const GLchar *name);
typedef void APIENTRY GL_PROC(glActiveTexture) (GLenum texture);
typedef void APIENTRY GL_PROC(glUniform1i) (GLint location, GLint v0);

GL_PROC(glGenVertexArrays)          *glGenVertexArrays;
GL_PROC(glBindVertexArray)          *glBindVertexArray;
GL_PROC(glGenBuffers)               *glGenBuffers;
GL_PROC(glBindBuffer)               *glBindBuffer;
GL_PROC(glBufferData)               *glBufferData;
GL_PROC(glCompileShader)            *glCompileShader;
GL_PROC(glCreateProgram)            *glCreateProgram;
GL_PROC(glCreateShader)             *glCreateShader;
GL_PROC(glDeleteProgram)            *glDeleteProgram;
GL_PROC(glDeleteShader)             *glDeleteShader;
GL_PROC(glDetachShader)             *glDetachShader;
GL_PROC(glDisableVertexAttribArray) *glDisableVertexAttribArray;
GL_PROC(glEnableVertexAttribArray)  *glEnableVertexAttribArray;
GL_PROC(glLinkProgram)              *glLinkProgram;
GL_PROC(glShaderSource)             *glShaderSource;
GL_PROC(glUseProgram)               *glUseProgram;
GL_PROC(glAttachShader)             *glAttachShader;
GL_PROC(glVertexAttribPointer)      *glVertexAttribPointer;
GL_PROC(glGetShaderiv)              *glGetShaderiv;
GL_PROC(glGetShaderInfoLog)         *glGetShaderInfoLog;
GL_PROC(glUniformMatrix4fv)         *glUniformMatrix4fv;
GL_PROC(glGetUniformLocation)       *glGetUniformLocation;
GL_PROC(glActiveTexture)            *glActiveTexture;
GL_PROC(glUniform1i)                *glUniform1i;


// GL 1 procs.
typedef const GLubyte * APIENTRY GL_PROC(glGetString) (GLenum name);
typedef void APIENTRY GL_PROC(glClear) (GLbitfield mask);
typedef void APIENTRY GL_PROC(glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void APIENTRY GL_PROC(glBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void APIENTRY GL_PROC(glDisable) (GLenum cap);
typedef void APIENTRY GL_PROC(glEnable) (GLenum cap);
typedef void APIENTRY GL_PROC(glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void APIENTRY GL_PROC(glGenTextures) (GLsizei n, GLuint *textures);
typedef void APIENTRY GL_PROC(glBindTexture) (GLenum target, GLuint texture);
typedef void APIENTRY GL_PROC(glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void APIENTRY GL_PROC(glTexParameteri) (GLenum target, GLenum pname, GLint param);
typedef void APIENTRY GL_PROC(glDepthFunc) (GLenum func);
typedef void APIENTRY GL_PROC(glClearDepth) (GLclampd depth);
typedef void APIENTRY GL_PROC(glPolygonMode) (GLenum face, GLenum mode);
typedef void APIENTRY GL_PROC(glPolygonOffset) (GLfloat factor, GLfloat units);
typedef void APIENTRY GL_PROC(glDepthMask) (GLboolean flag);
typedef void APIENTRY GL_PROC(glDepthRange) (GLclampd zNear, GLclampd zFar);
typedef void APIENTRY GL_PROC(glCullFace) (GLenum mode);
typedef void APIENTRY GL_PROC(glLineWidth) (GLfloat width);
typedef void APIENTRY GL_PROC(glFrontFace) (GLenum mode);
typedef void APIENTRY GL_PROC(glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void APIENTRY GL_PROC(glStencilFunc) (GLenum func, GLint ref, GLuint mask);
typedef void APIENTRY GL_PROC(glStencilMask) (GLuint mask);
typedef void APIENTRY GL_PROC(glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void APIENTRY GL_PROC(glDrawBuffer) (GLenum mode);
typedef void APIENTRY GL_PROC(glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void APIENTRY GL_PROC(glDrawArrays) (GLenum mode, GLint first, GLsizei count);
typedef void APIENTRY GL_PROC(glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void APIENTRY GL_PROC(glDeleteTextures) (GLsizei n, const GLuint *textures);
typedef void APIENTRY GL_PROC(glClearStencil) (GLint s);
typedef void APIENTRY GL_PROC(glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void APIENTRY GL_PROC(glPixelStoref) (GLenum pname, GLfloat param);
typedef void APIENTRY GL_PROC(glPixelStorei) (GLenum pname, GLint param);
typedef void APIENTRY GL_PROC(glReadBuffer) (GLenum mode);

GL_PROC(glGetString)     *glGetString;
GL_PROC(glClear)         *glClear;
GL_PROC(glClearColor)    *glClearColor;
GL_PROC(glBlendFunc)     *glBlendFunc;
GL_PROC(glDisable)       *glDisable;
GL_PROC(glEnable)        *glEnable;
GL_PROC(glViewport)      *glViewport;
GL_PROC(glGenTextures)   *glGenTextures;
GL_PROC(glBindTexture)   *glBindTexture;
GL_PROC(glTexImage2D)    *glTexImage2D;
GL_PROC(glTexParameteri) *glTexParameteri;
GL_PROC(glDepthFunc)     *glDepthFunc;
GL_PROC(glClearDepth)    *glClearDepth;
GL_PROC(glPolygonMode)   *glPolygonMode;
GL_PROC(glPolygonOffset) *glPolygonOffset;
GL_PROC(glDepthMask)     *glDepthMask;
GL_PROC(glDepthRange)    *glDepthRange;
GL_PROC(glCullFace)      *glCullFace;
GL_PROC(glLineWidth)     *glLineWidth;
GL_PROC(glFrontFace)     *glFrontFace;
GL_PROC(glScissor)       *glScissor;
GL_PROC(glStencilFunc)   *glStencilFunc;
GL_PROC(glStencilMask)   *glStencilMask;
GL_PROC(glStencilOp)     *glStencilOp;
GL_PROC(glDrawBuffer)    *glDrawBuffer;
GL_PROC(glColorMask)     *glColorMask;
GL_PROC(glDrawArrays)    *glDrawArrays;
GL_PROC(glDrawElements)  *glDrawElements;
GL_PROC(glDeleteTextures)*glDeleteTextures;
GL_PROC(glClearStencil)  *glClearStencil;
GL_PROC(glReadPixels)    *glReadPixels;
GL_PROC(glPixelStoref)   *glPixelStoref;
GL_PROC(glPixelStorei)   *glPixelStorei;
GL_PROC(glReadBuffer)    *glReadBuffer;


#if GL_USE_LEGACY_PROCS
/***************** Legacy functions *********************/
typedef void APIENTRY GL_PROC(glBegin) (GLenum mode);
typedef void APIENTRY GL_PROC(glEnd) (void);
typedef void APIENTRY GL_PROC(glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void APIENTRY GL_PROC(glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
typedef void APIENTRY GL_PROC(glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void APIENTRY GL_PROC(glMatrixMode) (GLenum mode);
typedef void APIENTRY GL_PROC(glLoadMatrixf) (const GLfloat *m);
typedef void APIENTRY GL_PROC(glTexEnvf) (GLenum target, GLenum pname, GLfloat param);
typedef void APIENTRY GL_PROC(glTexCoord2f) (GLfloat s, GLfloat t);
typedef void APIENTRY GL_PROC(glLoadIdentity) (void);

GL_PROC(glBegin)       *glBegin;
GL_PROC(glEnd)         *glEnd;
GL_PROC(glVertex3f)    *glVertex3f;
GL_PROC(glColor3f)     *glColor3f;
GL_PROC(glColor4f)     *glColor4f;
GL_PROC(glMatrixMode)  *glMatrixMode;
GL_PROC(glLoadMatrixf) *glLoadMatrixf;
GL_PROC(glTexEnvf)     *glTexEnvf;
GL_PROC(glTexCoord2f)  *glTexCoord2f;
GL_PROC(glLoadIdentity)*glLoadIdentity;
/***************** Legacy functions *********************/
#endif


bool gl_load(void);
bool gl_load_extensions(void);

void gl_unload(void);

bool gl_is_extension_supported(const char *extList, const char *extension);

#endif  // GL_INCLUDE_H



#ifdef GL_IMPLEMENTATION

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
bool gl_is_extension_supported(const char *extList, const char *extension) {
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
    return false;

    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    for (start=extList;;) {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if ( where == start || *(where - 1) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' )
                return true;

        start = terminator;
    }

    return false;
}

#if OS_WINDOWS

// Windows implementation requires including Windows header file
// #include <Windows.h>

// WGL.
typedef HGLRC WINAPI GL_PROC(wglCreateContext)(HDC);
typedef BOOL  WINAPI GL_PROC(wglDeleteContext)(HGLRC);
typedef BOOL  WINAPI GL_PROC(wglMakeCurrent)(HDC, HGLRC);
typedef PROC  WINAPI GL_PROC(wglGetProcAddress)(LPCSTR);

static GL_PROC(wglCreateContext)  *W32_wglCreateContext;
static GL_PROC(wglDeleteContext)  *W32_wglDeleteContext;
static GL_PROC(wglMakeCurrent)    *W32_wglMakeCurrent;
static GL_PROC(wglGetProcAddress) *W32_wglGetProcAddress;


// WGLEXT.
#define WGL_DRAW_TO_WINDOW_ARB    0x2001
#define WGL_ACCELERATION_ARB      0x2003
#define WGL_SUPPORT_OPENGL_ARB    0x2010
#define WGL_DOUBLE_BUFFER_ARB     0x2011
#define WGL_PIXEL_TYPE_ARB        0x2013
#define WGL_COLOR_BITS_ARB        0x2014
#define WGL_RED_BITS_ARB          0x2015
#define WGL_GREEN_BITS_ARB        0x2017
#define WGL_BLUE_BITS_ARB         0x2019
#define WGL_ALPHA_BITS_ARB        0x201B
#define WGL_DEPTH_BITS_ARB        0x2022
#define WGL_STENCIL_BITS_ARB      0x2023
#define WGL_AUX_BUFFERS_ARB       0x2024
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB         0x202B

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef HGLRC WINAPI GL_PROC(wglCreateContextAttribsARB) (HDC hDC, HGLRC hShareContext, 
                                                          const int *attribList);
typedef BOOL WINAPI GL_PROC(wglChoosePixelFormatARB) (HDC hdc,
                                                      const int *piAttribIList, 
                                                      const FLOAT *pfAttribFList, 
                                                      UINT nMaxFormats, 
                                                      int *piFormats, 
                                                      UINT *nNumFormats);

static GL_PROC(wglCreateContextAttribsARB) *wglCreateContextAttribsARB;
static GL_PROC(wglChoosePixelFormatARB)    *wglChoosePixelFormatARB;


#define W32_LOAD_GL_1_1_PROC(ident) ident = (GL_PROC(ident) *)GetProcAddress(gl_module, #ident)
#define W32_LOAD_WGL_PROC(ident) CONCAT(W32_,ident) = (GL_PROC(ident) *)GetProcAddress(gl_module, #ident)


static HMODULE gl_module;

bool gl_load(void) {
    gl_module = LoadLibraryA("OpenGL32.dll");
    if (!gl_module) {
        write_string("Failed to load Opengl32.\n", true);
        return false;
    }

    W32_LOAD_WGL_PROC(wglCreateContext);
    W32_LOAD_WGL_PROC(wglDeleteContext);
    W32_LOAD_WGL_PROC(wglMakeCurrent);
    W32_LOAD_WGL_PROC(wglGetProcAddress);


    W32_LOAD_GL_1_1_PROC(glGetString);
    W32_LOAD_GL_1_1_PROC(glClear);
    W32_LOAD_GL_1_1_PROC(glClearColor);
    W32_LOAD_GL_1_1_PROC(glBlendFunc);
    W32_LOAD_GL_1_1_PROC(glDisable);
    W32_LOAD_GL_1_1_PROC(glEnable);
    W32_LOAD_GL_1_1_PROC(glViewport);
    W32_LOAD_GL_1_1_PROC(glGenTextures);
    W32_LOAD_GL_1_1_PROC(glBindTexture);
    W32_LOAD_GL_1_1_PROC(glTexImage2D);
    W32_LOAD_GL_1_1_PROC(glTexParameteri);
    W32_LOAD_GL_1_1_PROC(glDepthFunc);
    W32_LOAD_GL_1_1_PROC(glClearDepth);
    W32_LOAD_GL_1_1_PROC(glPolygonMode);
    W32_LOAD_GL_1_1_PROC(glPolygonOffset);
    W32_LOAD_GL_1_1_PROC(glDepthMask);
    W32_LOAD_GL_1_1_PROC(glDepthRange);
    W32_LOAD_GL_1_1_PROC(glCullFace);
    W32_LOAD_GL_1_1_PROC(glLineWidth);
    W32_LOAD_GL_1_1_PROC(glFrontFace);
    W32_LOAD_GL_1_1_PROC(glScissor);
    W32_LOAD_GL_1_1_PROC(glStencilFunc);
    W32_LOAD_GL_1_1_PROC(glStencilMask);
    W32_LOAD_GL_1_1_PROC(glStencilOp);
    W32_LOAD_GL_1_1_PROC(glDrawBuffer);
    W32_LOAD_GL_1_1_PROC(glColorMask);
    W32_LOAD_GL_1_1_PROC(glDrawElements);
    W32_LOAD_GL_1_1_PROC(glDeleteTextures);
    W32_LOAD_GL_1_1_PROC(glClearStencil);
    W32_LOAD_GL_1_1_PROC(glReadPixels);
    W32_LOAD_GL_1_1_PROC(glPixelStoref);
    W32_LOAD_GL_1_1_PROC(glPixelStorei);
    W32_LOAD_GL_1_1_PROC(glReadBuffer);
    W32_LOAD_GL_1_1_PROC(glDrawArrays);


#if GL_USE_LEGACY_PROCS
    W32_LOAD_GL_1_1_PROC(glBegin);
    W32_LOAD_GL_1_1_PROC(glEnd);
    W32_LOAD_GL_1_1_PROC(glVertex3f);
    W32_LOAD_GL_1_1_PROC(glColor3f);
    W32_LOAD_GL_1_1_PROC(glColor4f);
    W32_LOAD_GL_1_1_PROC(glMatrixMode);
    W32_LOAD_GL_1_1_PROC(glLoadMatrixf);
    W32_LOAD_GL_1_1_PROC(glTexEnvf);
    W32_LOAD_GL_1_1_PROC(glTexCoord2f);
    W32_LOAD_GL_1_1_PROC(glLoadIdentity);
#endif

    return true;
}

bool gl_load_extensions(void) {
    GL_LOAD_PROC(glGenVertexArrays);
    GL_LOAD_PROC(glBindVertexArray);
    GL_LOAD_PROC(glGenBuffers);
    GL_LOAD_PROC(glBindBuffer);
    GL_LOAD_PROC(glBufferData);
    GL_LOAD_PROC(glCompileShader);
    GL_LOAD_PROC(glCreateProgram);
    GL_LOAD_PROC(glCreateShader);
    GL_LOAD_PROC(glDeleteProgram);
    GL_LOAD_PROC(glDeleteShader);
    GL_LOAD_PROC(glDetachShader);
    GL_LOAD_PROC(glDisableVertexAttribArray);
    GL_LOAD_PROC(glEnableVertexAttribArray);
    GL_LOAD_PROC(glLinkProgram);
    GL_LOAD_PROC(glShaderSource);
    GL_LOAD_PROC(glUseProgram);
    GL_LOAD_PROC(glAttachShader);
    GL_LOAD_PROC(glVertexAttribPointer);
    GL_LOAD_PROC(glGetShaderiv);
    GL_LOAD_PROC(glGetShaderInfoLog);
    GL_LOAD_PROC(glUniformMatrix4fv);
    GL_LOAD_PROC(glGetUniformLocation);
    GL_LOAD_PROC(glActiveTexture);
    GL_LOAD_PROC(glUniform1i);

    return true;
}

void gl_unload(void) {
    if (gl_module) {
        FreeLibrary(gl_module);
        gl_module = null;
    }
}

#endif  // OS_WINDOWS



#if OS_LINUX

// GLX.
typedef struct __GLXcontextRec *GLXContext;
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXDrawable;


#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
#define GLX_CONTEXT_FLAGS_ARB         0x2094
#define GLX_CONTEXT_PROFILE_MASK_ARB  0x9126

#define GLX_CONTEXT_DEBUG_BIT_ARB              0x0001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002


/*
 * Tokens for glXChooseVisual and glXGetConfig:
 */
#define GLX_USE_GL           1
#define GLX_BUFFER_SIZE      2
#define GLX_LEVEL            3
#define GLX_RGBA             4
#define GLX_DOUBLEBUFFER     5
#define GLX_STEREO           6
#define GLX_AUX_BUFFERS      7
#define GLX_RED_SIZE         8
#define GLX_GREEN_SIZE       9
#define GLX_BLUE_SIZE        10
#define GLX_ALPHA_SIZE       11
#define GLX_DEPTH_SIZE       12
#define GLX_STENCIL_SIZE     13
#define GLX_ACCUM_RED_SIZE   14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE  16
#define GLX_ACCUM_ALPHA_SIZE 17

#define GLX_X_VISUAL_TYPE 0x22
#define GLX_WINDOW_BIT    0x00000001
#define GLX_RGBA_BIT      0x00000001
#define GLX_TRUE_COLOR    0x8002
#define GLX_DRAWABLE_TYPE 0x8010
#define GLX_RENDER_TYPE   0x8011
#define GLX_X_RENDERABLE  0x8012
#define GLX_RGBA_TYPE     0x8014

/*
 * GLX 1.4 and later:
 */
#define GLX_SAMPLE_BUFFERS 0x186a0 /*100000*/
#define GLX_SAMPLES        0x186a1 /*100001*/


typedef Bool GL_PROC(glXQueryVersion)( Display *dpy, int *maj, int *min );
typedef XVisualInfo* GL_PROC(glXChooseVisual)( Display *dpy, int screen, int *attribList );
typedef GLXContext GL_PROC(glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
typedef void GL_PROC(glXDestroyContext)( Display *dpy, GLXContext ctx );
typedef Bool GL_PROC(glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx);
typedef void GL_PROC(glXSwapBuffers)( Display *dpy, GLXDrawable drawable );

typedef void (*__GLXextFuncPtr)(void);
typedef __GLXextFuncPtr GL_PROC(glXGetProcAddress) (const GLubyte *);

typedef GLXContext GL_PROC(glXCreateContextAttribsARB)(Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);
typedef GLXFBConfig *GL_PROC(glXChooseFBConfig)(Display *dpy, int screen, const int *attribList, int *nitems);
typedef int GL_PROC(glXGetFBConfigAttrib)(Display *dpy, GLXFBConfig config, int attribute, int *value);
typedef XVisualInfo *GL_PROC(glXGetVisualFromFBConfig)(Display *dpy, GLXFBConfig config);
typedef const char *GL_PROC(glXQueryExtensionsString)(Display *dpy, int screen);

static GL_PROC(glXQueryVersion)            *glXQueryVersion;
static GL_PROC(glXChooseVisual)            *glXChooseVisual;
static GL_PROC(glXCreateContext)           *glXCreateContext;
static GL_PROC(glXDestroyContext)          *glXDestroyContext;
static GL_PROC(glXMakeCurrent)             *glXMakeCurrent;
static GL_PROC(glXSwapBuffers)             *glXSwapBuffers;
static GL_PROC(glXGetProcAddress)          *glXGetProcAddress;
static GL_PROC(glXCreateContextAttribsARB) *glXCreateContextAttribsARB;
static GL_PROC(glXChooseFBConfig)          *glXChooseFBConfig;
static GL_PROC(glXGetFBConfigAttrib)       *glXGetFBConfigAttrib;
static GL_PROC(glXGetVisualFromFBConfig)   *glXGetVisualFromFBConfig;
static GL_PROC(glXQueryExtensionsString)   *glXQueryExtensionsString;

#define GLX_LOAD_PROC(ident) ident = (GL_PROC(ident) *)dlsym(glx_module, #ident)

static bool glx_load(void) {
    void *glx_module = dlopen("libGLX.so.0", RTLD_NOW | RTLD_GLOBAL);
    if (!glx_module) {
        glx_module = dlopen("libGLX.so", RTLD_NOW | RTLD_GLOBAL);
    }

    if (!glx_module) {
        write_string("Failed to load GLX.\n", true);
        return false;
    }

    GLX_LOAD_PROC(glXQueryVersion);
    GLX_LOAD_PROC(glXChooseVisual);
    GLX_LOAD_PROC(glXCreateContext);
    GLX_LOAD_PROC(glXDestroyContext);
    GLX_LOAD_PROC(glXMakeCurrent);
    GLX_LOAD_PROC(glXSwapBuffers);

    glXGetProcAddress = (glXGetProcAddressPROC *)dlsym(glx_module, "glXGetProcAddressARB");
    if (!glXGetProcAddress) {
        GLX_LOAD_PROC(glXGetProcAddress);
    }

    return true;
}


#define GL1_LOAD_PROC(ident) ident = (GL_PROC(ident) *)dlsym(gl_module, #ident)

static void *gl_module;

bool gl_load(void) {
    gl_module = dlopen("libGL.so.1", RTLD_NOW | RTLD_GLOBAL);
    if (!gl_module) {
        gl_module = dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);
    }

    if (!gl_module) {
        write_string("Failed to load GL module.\n", true);
        return false;
    }

    // @Todo: Fallback to dlsym when glXGetProcAddress Fails?

    GL1_LOAD_PROC(glGetString);
    GL1_LOAD_PROC(glClear);
    GL1_LOAD_PROC(glClearColor);
    GL1_LOAD_PROC(glBlendFunc);
    GL1_LOAD_PROC(glDisable);
    GL1_LOAD_PROC(glEnable);
    GL1_LOAD_PROC(glViewport);
    GL1_LOAD_PROC(glGenTextures);
    GL1_LOAD_PROC(glBindTexture);
    GL1_LOAD_PROC(glTexImage2D);
    GL1_LOAD_PROC(glTexParameteri);
    GL1_LOAD_PROC(glDepthFunc);
    GL1_LOAD_PROC(glClearDepth);
    GL1_LOAD_PROC(glPolygonMode);
    GL1_LOAD_PROC(glPolygonOffset);
    GL1_LOAD_PROC(glDepthMask);
    GL1_LOAD_PROC(glDepthRange);
    GL1_LOAD_PROC(glCullFace);
    GL1_LOAD_PROC(glLineWidth);
    GL1_LOAD_PROC(glFrontFace);
    GL1_LOAD_PROC(glScissor);
    GL1_LOAD_PROC(glStencilFunc);
    GL1_LOAD_PROC(glStencilMask);
    GL1_LOAD_PROC(glStencilOp);
    GL1_LOAD_PROC(glDrawBuffer);
    GL1_LOAD_PROC(glColorMask);
    GL1_LOAD_PROC(glDrawElements);
    GL1_LOAD_PROC(glDeleteTextures);
    GL1_LOAD_PROC(glClearStencil);
    GL1_LOAD_PROC(glReadPixels);
    GL1_LOAD_PROC(glPixelStoref);
    GL1_LOAD_PROC(glPixelStorei);
    GL1_LOAD_PROC(glReadBuffer);
    GL1_LOAD_PROC(glDrawArrays);


#if GL_USE_LEGACY_PROCS
    GL1_LOAD_PROC(glBegin);
    GL1_LOAD_PROC(glEnd);
    GL1_LOAD_PROC(glVertex3f);
    GL1_LOAD_PROC(glColor3f);
    GL1_LOAD_PROC(glColor4f);
    GL1_LOAD_PROC(glMatrixMode);
    GL1_LOAD_PROC(glLoadMatrixf);
    GL1_LOAD_PROC(glTexEnvf);
    GL1_LOAD_PROC(glTexCoord2f);
    GL1_LOAD_PROC(glLoadIdentity);
#endif

    return true;
}

bool gl_load_extensions(void) {
    GL_LOAD_PROC(glGenVertexArrays);
    GL_LOAD_PROC(glBindVertexArray);
    GL_LOAD_PROC(glGenBuffers);
    GL_LOAD_PROC(glBindBuffer);
    GL_LOAD_PROC(glBufferData);
    GL_LOAD_PROC(glCompileShader);
    GL_LOAD_PROC(glCreateProgram);
    GL_LOAD_PROC(glCreateShader);
    GL_LOAD_PROC(glDeleteProgram);
    GL_LOAD_PROC(glDeleteShader);
    GL_LOAD_PROC(glDetachShader);
    GL_LOAD_PROC(glDisableVertexAttribArray);
    GL_LOAD_PROC(glEnableVertexAttribArray);
    GL_LOAD_PROC(glLinkProgram);
    GL_LOAD_PROC(glShaderSource);
    GL_LOAD_PROC(glUseProgram);
    GL_LOAD_PROC(glAttachShader);
    GL_LOAD_PROC(glVertexAttribPointer);
    GL_LOAD_PROC(glGetShaderiv);
    GL_LOAD_PROC(glGetShaderInfoLog);
    GL_LOAD_PROC(glUniformMatrix4fv);
    GL_LOAD_PROC(glGetUniformLocation);
    GL_LOAD_PROC(glActiveTexture);
    GL_LOAD_PROC(glUniform1i);

    return true;
}

void gl_unload(void) {
    if (gl_module) {
        dlclose(gl_module);
        gl_module = null;
    }
}

#endif  // OS_LINUX

#endif  // GL_IMPLEMENTATION
