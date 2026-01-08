#pragma once
#include <GL/gl.h>


GLuint LoadTexture2D(const char* path, bool flipY = true);        // للـ JPG/PNG
GLuint LoadHDRToLDRTexture2D(const char* path, float exposure = 0.2f, bool flipY = true); // للـ HDR
