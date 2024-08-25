#ifndef OPENGL_H
#define OPENGL_H
#include <stdint.h>

void opengl_init();

// GL constants and enum values
// some of these constants are found in this reference file
// https://registry.khronos.org/OpenGL/api/GL/glext.h
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_TRIANGLES                      0x0004
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_COLOR_BUFFER_BIT               0x00004
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE_2D                     0xDE1
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_REPEAT                         0x2901
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_LINEAR                         0x2601
#define GL_RGB                            0x1907

// GL type enums
// https://github.com/KhronosGroup/OpenGL-Registry
#define GL_TRUE             1
#define GL_FALSE            0
#define GL_UNSIGNED_BYTE    0x1401
#define GL_INT              0x1404
#define GL_UNSIGNED_INT     0x1405
#define GL_FLOAT            0x1406

// GL types 
// https://www.khronos.org/opengl/wiki/OpenGL_Type
typedef char        GLchar;
typedef uint8_t     GLboolean;
typedef int8_t      GLbyte;
typedef uint8_t     GLubyte;
typedef int16_t     GLshort;
typedef uint16_t    GLushort;
typedef int32_t     GLint;
typedef uint32_t    GLuint;
typedef int64_t     GLint64;
typedef uint64_t    GLuint64;
typedef float       GLfloat;
typedef double      GLdouble;
typedef uint32_t    GLbitfield;
typedef uint32_t    GLsizei;
typedef uint32_t    GLenum;

// ptr size for 32bit vs 64bit
#if INTPTR_MAX == INT64_MAX
typedef int64_t     GLintptr;
typedef uint64_t    GLsizeiptr;
#elif INTPTR_MAX == INT32_MAX
typedef int32_t     GLintptr;
typedef uint32_t    GLsizeiptr;
#endif

#ifdef _WIN32
#include <Windows.h>
typedef HGLRC (*PNFWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);
PNFWGLCREATECONTEXTATTRIBSARBPROC wgl_create_context_attribs_arb;
#endif

// OpenGL extension definitions
// https://registry.khronos.org/OpenGL-Refpages/gl4/
typedef GLuint (*PFNGLCREATESHADERPROC)(GLenum);
typedef void (*PFNGLSHADERSOURCEPROC)(GLuint, GLsizei, const GLchar **, const GLint *);
typedef void (*PFNGLCOMPILESHADERPROC)(GLuint);
typedef void (*PFNGLGETSHADERIVPROC)(GLuint, GLenum, GLint *);
typedef void (*PFNGLGETSHADERINFOLOGPROC)(GLuint, GLsizei, GLsizei, GLchar *);
typedef GLuint (*PFNGLCREATEPROGRAMPROC)();
typedef void (*PFNGLATTACHSHADERPROC)(GLuint, GLuint);
typedef void (*PFNGLLINKPROGRAMPROC)(GLuint);
typedef void (*PFNGLGETPROGRAMIVPROC)(GLuint, GLenum, GLint *);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (*PFNGLDELETESHADERPROC)(GLuint);
typedef void (*PFNGLGENVERTEXARRAYSPROC)(GLsizei, GLuint *);
typedef void (*PFNGLGENBUFFERSPROC)(GLsizei, GLuint *);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void (*PFNGLBINDBUFFERPROC)(GLenum, GLuint);
typedef void (*PFNGLBUFFERDATAPROC)(GLenum, GLsizeiptr, const void *, GLenum);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void *);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void (*PFNGLCLEARCOLORPROC)(GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (*PFNGLCLEARPROC)(GLbitfield);
typedef void (*PFNGLUSEPROGRAMPROC)(GLuint);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void (*PFNGLDRAWELEMENTSPROC)(GLenum, GLsizei, GLenum, const void *);
typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(GLsizei, const GLuint *);
typedef void (*PFNGLDELETEBUFFERSPROC)(GLsizei, const GLuint *);
typedef void (*PFNGLDELETEPROGRAMPROC)(GLuint);
typedef GLenum (*PFNGLGETERRORPROC)();
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)(GLint, GLsizei, GLboolean, const GLfloat *);
typedef GLint (*PFNGLGETUNIFORMLOCATION)(GLuint, const GLchar *);
typedef void (*PFNGLGENTEXTURES)(GLsizei, GLuint *);
typedef void (*PFNGLTEXPARAMETERI)(GLenum, GLenum, GLint);
typedef void (*PFNGLTEXIMAGE2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *);
typedef void (*PFNGLGENERATEMIPMAP)(GLenum);
typedef void (*PFNGLACTIVETEXTURE)(GLenum);
typedef void (*PFNGLBINDTEXTURE)(GLenum, GLuint);

// function varibles
PFNGLCREATESHADERPROC                   gl_create_shader;
PFNGLSHADERSOURCEPROC                   gl_shader_source;
PFNGLCOMPILESHADERPROC                  gl_compile_shader;
PFNGLGETSHADERIVPROC                    gl_get_shader_iv;
PFNGLGETSHADERINFOLOGPROC               gl_get_shader_info_log;
PFNGLCREATEPROGRAMPROC                  gl_create_program;
PFNGLATTACHSHADERPROC                   gl_attach_shader;
PFNGLLINKPROGRAMPROC                    gl_link_program;
PFNGLGETPROGRAMIVPROC                   gl_get_program_iv;
PFNGLGETPROGRAMINFOLOGPROC              gl_get_program_info_log;
PFNGLDELETESHADERPROC                   gl_delete_shader;
PFNGLGENVERTEXARRAYSPROC                gl_gen_vertex_arrays;
PFNGLGENBUFFERSPROC                     gl_gen_buffers;
PFNGLBINDVERTEXARRAYPROC                gl_bind_vertex_array;
PFNGLBINDBUFFERPROC                     gl_bind_buffer;
PFNGLBUFFERDATAPROC                     gl_buffer_data;
PFNGLVERTEXATTRIBPOINTERPROC            gl_vertex_attrib_pointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC        gl_enable_vertex_attrib_array;
PFNGLCLEARCOLORPROC                     gl_clear_color;
PFNGLCLEARPROC                          gl_clear;
PFNGLUSEPROGRAMPROC                     gl_use_program;
PFNGLBINDVERTEXARRAYPROC                gl_bind_vertex_array;
PFNGLDRAWELEMENTSPROC                   gl_draw_elements;
PFNGLDELETEVERTEXARRAYSPROC             gl_delete_vertex_arrays;
PFNGLDELETEBUFFERSPROC                  gl_delete_buffers;
PFNGLDELETEPROGRAMPROC                  gl_delete_program;
PFNGLGETERRORPROC                       gl_get_error;
PFNGLUNIFORMMATRIX4FVPROC               gl_uniform_matrix_4fv;
PFNGLGETUNIFORMLOCATION                 gl_get_uniform_location;
PFNGLGENTEXTURES                        gl_gen_textures;
PFNGLTEXPARAMETERI                      gl_tex_parameter_i;
PFNGLTEXIMAGE2D                         gl_tex_image_2D;
PFNGLGENERATEMIPMAP                     gl_generate_mipmap;
PFNGLACTIVETEXTURE                      gl_active_texture;
PFNGLBINDTEXTURE                        gl_bind_texture;
#endif