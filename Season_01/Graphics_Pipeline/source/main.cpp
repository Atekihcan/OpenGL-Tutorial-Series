/******************************************************************************
 * Copyright (c) 2020 Atekihcan <contact@atekihcan.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Exploring Graphics Pipeline with OpenGL
 *
 ******************************************************************************/

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "linmath.h"
#include "utilities.h"

// clang-format off
static const struct {
    float x, y;
    float r, g, b;
} vertices[3] = {
    {-1.f, 0.f, 1.f, 0.f, 0.f},
    { 1.f, 0.f, 0.f, 1.f, 0.f},
    { 0.f, 1.f, 0.f, 0.f, 1.f}
};
// clang-format on

static const char* vert_shader_str =
    "#version 110\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "    gl_Position = projection * view * model * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char* frag_shader_str =
    "#version 110\n"
    "varying vec3 color;\n"
    "uniform vec3 u_color;"
    "void main() {\n"
    "    gl_FragColor = vec4(u_color, 1.0);\n"
    "}\n";

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLFWwindow* initWindow(int width, int height) {
    GLFWwindow* window = nullptr;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return window;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(width, height, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return window;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);
    return window;
}

GLuint initGL() {
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    // NOTE: OpenGL error checks have been omitted for brevity

    // Generate and fill up vertex buffer
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Compile shaders and program object
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_shader_str, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_shader_str, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // Query and set vertex attributes
    GLint vpos_location = glGetAttribLocation(program, "vPos");
    GLint vcol_location = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(sizeof(float) * 2));
    return program;
}

int main(void) {
    auto window = initWindow(600, 400);
    if (!window) {
        exit(EXIT_FAILURE);
    }

    auto program = initGL();

    bool repeated = false;
    float ratio;
    int width, height;
    mat4x4 m, v, p, mvp;
    GLfloat color_r[] = {1.f, 0.f, 0.f};
    GLfloat color_g[] = {0.f, 1.f, 0.f};
    GLfloat color_b[] = {0.f, 0.f, 1.f};

    while (!glfwWindowShouldClose(window)) {
        // Take screenshot if "S" key is pressed
        int down = glfwGetKey(window, GLFW_KEY_S);
        if (down && !repeated) {
            saveScreenshot("00_geometry.png");
            repeated = true;
        } else if (!down) {
            repeated = false;
        }

        glfwGetFramebufferSize(window, &width, &height);
        ratio = (float)width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        glUseProgram(program);

        mat4x4_identity(v);
        mat4x4_translate(v, 0.f, 0.f, -3.f);
        mat4x4_perspective(p, 39.6f * (M_PI / 180.f), ratio, 1.f, 5.f);
        // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 5.f, -5.f);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const GLfloat*)v);
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const GLfloat*)p);

        // Draw green triangle
        // Identity model matrix
        mat4x4_identity(m);
        mat4x4_translate(m, 1.5f, 0.f, -4.f);
        m[0][0] = 2.f;
        m[1][1] = 2.f;
        m[2][2] = 2.f;
        glUniform3fv(glGetUniformLocation(program, "u_color"), 1, (const GLfloat*)color_g);
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat*)m);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw blue triangle
        // (1.0, 0.0, -0.5) translation, +PI/6 rotation on X
        mat4x4_identity(m);
        mat4x4_translate(m, 1.5f, 0.f, -0.5f);
        mat4x4_rotate_X(m, m, -M_PI / 6.f);

        glUniform3fv(glGetUniformLocation(program, "u_color"), 1, (const GLfloat*)color_b);
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat*)m);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw red triangle
        // Identity model matrix
        mat4x4_identity(m);
        glUniform3fv(glGetUniformLocation(program, "u_color"), 1, (const GLfloat*)color_r);
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat*)m);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
