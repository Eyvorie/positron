#include <math.h>
#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "platform.h"
#include "opengl.h"
#include "util.h"
#include "alloc.h"

#pragma pack(push, 1)
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BitmapFileHeader;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BitmapInfoHeader;
#pragma pack(pop)

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";
const char *fragment_shader_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord);\n"
    "}\0";

int interpret(int argc, char *argv[])
{

  assert(argc > 1,
    "fatal error: please specify a file to compile");

  FILE *fp = fopen(argv[1], "rb");
  assert(fp != NULL, "fatal error: could not read file @ '%s'", argv[1]);
  
  fseek(fp, 0L, SEEK_END);
  // assert fseek
  size_t size = ftell(fp);

  char *buffer = array_of(char, size + 1);
  fseek(fp, 0L, SEEK_SET);
  // assert fseek
  fread(buffer, sizeof(char), size, fp);
  assert(!ferror(fp), "fatal error: could not read file");
  buffer[size] = '\0';
  fclose(fp);

  Window *window = create_window("Positron app", 640, 480);
  Lexer lexer;
  lexer_init(&lexer, buffer, size);
  AstNode *ast = parse(&lexer);

  // opengl setup
  ///////////////
  unsigned int vertex_shader = gl_create_shader(GL_VERTEX_SHADER);
  gl_shader_source(vertex_shader, 1, &vertex_shader_source, NULL);
  gl_compile_shader(vertex_shader);

  // check for shader compile errors
  int success;
  char info_log[512];
  gl_get_shader_iv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    gl_get_shader_info_log(vertex_shader, 512, NULL, info_log);
    printf("%s\n", info_log);
  }

  // fragment shader
  unsigned int fragment_shader = gl_create_shader(GL_FRAGMENT_SHADER);
  gl_shader_source(fragment_shader, 1, &fragment_shader_source, NULL);
  gl_compile_shader(fragment_shader);

  // check for shader compile errors
  gl_get_shader_iv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    gl_get_shader_info_log(fragment_shader, 512, NULL, info_log);
    printf("%s\n", info_log);
  }

  // link shaders
  unsigned int shader_program = gl_create_program();
  gl_attach_shader(shader_program, vertex_shader);
  gl_attach_shader(shader_program, fragment_shader);
  gl_link_program(shader_program);

  // check for linking errors
  gl_get_program_iv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    gl_get_program_info_log(shader_program, 512, NULL, info_log);
    printf("%s\n", info_log);
  }

  gl_delete_shader(vertex_shader);
  gl_delete_shader(fragment_shader);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
    // positions          // colors           // texture coords
      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
  };
  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
  };

  unsigned int vbo, vao, ebo;
  gl_gen_vertex_arrays(1, &vao);
  gl_gen_buffers(1, &vbo);
  gl_gen_buffers(1, &ebo);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  gl_bind_vertex_array(vao);

  gl_bind_buffer(GL_ARRAY_BUFFER, vbo);
  gl_buffer_data(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  gl_bind_buffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  gl_buffer_data(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  gl_vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  gl_enable_vertex_attrib_array(0);
  // color attribute
  gl_vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  gl_enable_vertex_attrib_array(1);
  // texture coord attribute
  gl_vertex_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  gl_enable_vertex_attrib_array(2);

  unsigned int texture;
  
  // texture 1
  // ---------
  gl_gen_textures(1, &texture);
  gl_bind_texture(GL_TEXTURE_2D, texture); 
    // set the texture wrapping parameters
  gl_tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  gl_tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  gl_tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl_tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /// LOAD IMAGE
  DocumentElement *document = (DocumentElement *)ast->expression;
  char *path = NULL;
  for (int i = 0; i < document->attributes; i++) {
    if (strcmp(document->attribute_keys[i], "src") == 0) {
      path = document->attribute_values[i];
      break;
    }
  }
  
  FILE *file_ptr = fopen(path, "rb");
  if (file_ptr == NULL) {
      printf("Error: Unable to open file %s\n", path);
      return NULL;
  }

  // Get the file size
  fseek(file_ptr, 0, SEEK_END);
  long file_size = ftell(file_ptr);
  rewind(file_ptr);

  // Allocate memory to load the entire file
  unsigned char *file_buffer = (unsigned char*)malloc(file_size);
  if (!file_buffer) {
      printf("Error: Unable to allocate memory for file buffer\n");
      fclose(file_ptr);
      return 1;
  }

  // Read the entire file into the buffer
  size_t bytes_read = fread(file_buffer, 1, file_size, file_ptr);
  if (bytes_read != file_size) {
      printf("Error: Expected to read %ld bytes but got %zu bytes\n", file_size, bytes_read);
      free(file_buffer);
      fclose(file_ptr);
      return 1;
  }

  fclose(file_ptr);

  // Extract the BMP file header and info header from the buffer
  BitmapFileHeader* file_header = (BitmapFileHeader*)file_buffer;
  BitmapInfoHeader* info_header = (BitmapInfoHeader*)(file_buffer + sizeof(BitmapFileHeader));

  // Ensure biSizeImage is correctly set
  int row_size = ((info_header->biBitCount * info_header->biWidth + 31) / 32) * 4;
  int data_size = row_size * abs(info_header->biHeight);
  if (info_header->biSizeImage == 0) {
      info_header->biSizeImage = data_size;
  }

  // Calculate the pointer to the start of the pixel data
  unsigned char* bitmap_image = file_buffer + file_header->bfOffBits;

  // Print the first few bytes of the pixel data for debugging
  printf("First few bytes of bitmap data:\n");
  for (int i = 0; i < 10; i++) {
      printf("%02X ", bitmap_image[i]);
  }
  printf("\n");

  // Normally, you would process or display the image data here

  // Free the allocated memory
  //free(file_buffer);

  /// END LOAD IMAGE
  // load image, create texture and generate mipmaps
  //unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
  unsigned char *data = bitmap_image;
  if (data) {
    gl_tex_image_2D(GL_TEXTURE_2D, 0, GL_RGB, info_header->biWidth, info_header->biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap_image);
    gl_generate_mipmap(GL_TEXTURE_2D);
  }

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  gl_bind_buffer(GL_ARRAY_BUFFER, 0); 

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  gl_bind_vertex_array(0); 

  while (app_running(window)) {

    gl_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl_clear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    gl_active_texture(GL_TEXTURE0);
    gl_bind_texture(GL_TEXTURE_2D, texture);

    gl_use_program(shader_program);
    gl_bind_vertex_array(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    gl_draw_elements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    swap_buffers(window);
  }

  gl_delete_vertex_arrays(1, &vao);
  gl_delete_buffers(1, &vbo);
  gl_delete_buffers(1, &ebo);
  gl_delete_program(shader_program);
  destroy_window(window);

  free(buffer);
}