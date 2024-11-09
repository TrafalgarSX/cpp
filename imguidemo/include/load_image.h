#ifndef _LOAD_IMAGE_H_
#define _LOAD_IMAGE_H_

#define STB_IMAGE_IMPLEMENTATION
#include <SDL_opengl.h>

bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                         int *out_width, int *out_height);

#endif