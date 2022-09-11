
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void flipVertically(int width, int height, char* data) {
    char rgb[3];

    for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width; ++x) {
            int top    = (x + y * width) * 3;
            int bottom = (x + (height - y - 1) * width) * 3;

            memcpy(rgb, data + top, sizeof(rgb));
            memcpy(data + top, data + bottom, sizeof(rgb));
            memcpy(data + bottom, rgb, sizeof(rgb));
        }
    }
}

int saveScreenshot(const char* filename) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x      = viewport[0];
    int y      = viewport[1];
    int width  = viewport[2];
    int height = viewport[3];

    char* data = (char*)malloc((size_t)(width * height * 3));  // 3 components (R, G, B)

    if (!data)
        return 0;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    flipVertically(width, height, data);

    int saved = stbi_write_png(filename, width, height, 3, data, 0);

    free(data);

    if (saved)
        printf("Successfully Saved Image: %s\n", filename);
    else
        fprintf(stderr, "Failed Saving Image: %s\n", filename);

    return saved;
}