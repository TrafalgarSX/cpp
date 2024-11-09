#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <SDL_opengl.h>

#include <unordered_map>

static std::unordered_map<std::string, std::tuple<GLuint, int, int>> g_texture_map;

// Simple helper function to load an image into a OpenGL texture with common
// settings
bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                         int *out_width, int *out_height) {

  if(g_texture_map.find(filename) != g_texture_map.end()) {
    *out_texture = std::get<0>(g_texture_map[filename]);
    *out_width = std::get<1>(g_texture_map[filename]);
    *out_height = std::get<2>(g_texture_map[filename]);
    return true;
  }
  // Load from file
  int image_width = 0;
  int image_height = 0;
  unsigned char *image_data =
      stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;

  // Create a OpenGL texture identifier
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_CLAMP_TO_EDGE); // This is required on WebGL for non
                                     // power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;
  g_texture_map.emplace(filename, std::make_tuple(image_texture, image_width, image_height));
  return true;
}