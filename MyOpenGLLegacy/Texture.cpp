#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/gl.h>
#include <cstdio>
#include <vector>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static float clamp01(float v) { return (v < 0.f) ? 0.f : (v > 1.f ? 1.f : v); }

GLuint LoadTexture2D(const char* path, bool flipY)
{
    stbi_set_flip_vertically_on_load(flipY);

    int w = 0, h = 0, comp = 0;
    unsigned char* data = stbi_load(path, &w, &h, &comp, 0);
    if (!data)
    {
        printf("stbi_load FAILED: %s\n", path);
        return 0;
    }

    GLenum fmt = (comp == 4) ? GL_RGBA : GL_RGB;

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return tex;
}

GLuint LoadHDRToLDRTexture2D(const char* path, float exposure, bool flipY)
{
    stbi_set_flip_vertically_on_load(flipY);

    int w = 0, h = 0, comp = 0;
    float* hdr = stbi_loadf(path, &w, &h, &comp, 3); // forced RGB
    if (!hdr)
    {
        printf("stbi_loadf FAILED: %s\n", path);
        return 0;
    }

    // Tone mapping (Reinhard) + Gamma
    std::vector<unsigned char> ldr(w * h * 3);

    const float invGamma = 1.0f / 2.2f;
    for (int i = 0; i < w * h; ++i)
    {
        float r = hdr[i * 3 + 0] * exposure;
        float g = hdr[i * 3 + 1] * exposure;
        float b = hdr[i * 3 + 2] * exposure;

        // Reinhard: x / (1+x)
        r = r / (1.0f + r);
        g = g / (1.0f + g);
        b = b / (1.0f + b);

        // Gamma
        r = powf(clamp01(r), invGamma);
        g = powf(clamp01(g), invGamma);
        b = powf(clamp01(b), invGamma);

        ldr[i * 3 + 0] = (unsigned char)(r * 255.0f + 0.5f);
        ldr[i * 3 + 1] = (unsigned char)(g * 255.0f + 0.5f);
        ldr[i * 3 + 2] = (unsigned char)(b * 255.0f + 0.5f);
    }

    stbi_image_free(hdr);

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, ldr.data());
    return tex;
}
