#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "opengl.h"

void opengl_init()
{
  if (gl_clear)
    return;

  HMODULE opengldll = 
    LoadLibraryA("opengl32.dll");
  
  // win32 upgrade to OpenGL 3.0+ context
  wgl_create_context_attribs_arb = 
    (PNFWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

  // OpenGL 1.1
  gl_clear =            (PFNGLCLEARPROC)GetProcAddress(opengldll, "glClear");
  gl_clear_color =      (PFNGLCLEARCOLORPROC)GetProcAddress(opengldll, "glClearColor");
  gl_get_error =        (PFNGLGETERRORPROC)GetProcAddress(opengldll, "glGetError");
  gl_tex_parameter_i =  (PFNGLTEXPARAMETERI)GetProcAddress(opengldll, "glTexParameteri");
  gl_tex_image_2D =     (PFNGLTEXIMAGE2D)GetProcAddress(opengldll, "glTexImage2D");
  gl_bind_texture =     (PFNGLBINDTEXTURE)GetProcAddress(opengldll, "glBindTexture");

  // Modern extensions
  gl_create_shader =                (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
  gl_shader_source =                (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
  gl_compile_shader =               (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
  gl_get_shader_iv =                (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
  gl_get_shader_info_log =          (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
  gl_create_program =               (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
  gl_attach_shader =                (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
  gl_link_program =                 (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
  gl_get_program_iv =               (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
  gl_get_program_info_log =         (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
  gl_delete_shader =                (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
  gl_gen_vertex_arrays =            (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
  gl_gen_buffers =                  (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
  gl_bind_vertex_array =            (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
  gl_bind_buffer =                  (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
  gl_buffer_data =                  (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
  gl_vertex_attrib_pointer =        (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
  gl_enable_vertex_attrib_array =   (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
  gl_use_program =                  (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
  gl_bind_vertex_array =            (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
  gl_draw_elements =                (PFNGLDRAWELEMENTSPROC)wglGetProcAddress("glDrawElements");
  gl_delete_vertex_arrays =         (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
  gl_delete_buffers =               (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
  gl_delete_program =               (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
  gl_uniform_matrix_4fv =           (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
  gl_get_uniform_location =         (PFNGLGETUNIFORMLOCATION)wglGetProcAddress("glGetUniformLocation");
  gl_gen_textures =                 (PFNGLGENTEXTURES)wglGetProcAddress("glGenTextures");
  gl_generate_mipmap =              (PFNGLGENERATEMIPMAP)wglGetProcAddress("glGenerateMipmap");
  gl_active_texture =               (PFNGLACTIVETEXTURE)wglGetProcAddress("glActiveTexture");

  //FreeLibrary(opengldll);
}