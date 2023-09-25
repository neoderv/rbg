
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "controls.h"
#include "cubes.h"
#include "linearAlg.h"
#include "model.h"
#include "util.h"

int loadShader(GLenum shaderType, char *shaderFile) {
  char *shaderContent = loadFile(shaderFile);
  printf("%s\n", shaderContent);
  int outShader = glCreateShader(shaderType);
  glShaderSource(outShader, 1, &shaderContent, NULL);
  glCompileShader(outShader);

  return outShader;
}

void logErrors(GLuint program) {
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    GLchar *infoLog = malloc(sizeof(GLchar) * maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    printf("\n%s\n\n", infoLog);
  }
}

int main(void) {
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  window = glfwCreateWindow(640, 480, "CGame", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  gladLoadGLLoader(glfwGetProcAddress);
  // glewInit();
  glfwSwapInterval(1);

  // glEnable              ( GL_DEBUG_OUTPUT );
  glEnable(GL_DEPTH_TEST);
  // glDebugMessageCallback( MessageCallback, 0 );

  GLuint vertexShader, fragmentShader;
  vertexShader = loadShader(GL_VERTEX_SHADER, "../res/gl/main.vert");
  fragmentShader = loadShader(GL_FRAGMENT_SHADER, "../res/gl/main.frag");

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glUseProgram(program);

  logErrors(program);

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);

  GLuint uv_buffer;
  glGenBuffers(1, &uv_buffer);

  unsigned char *blockTexture = readBMP("../res/img/main.bmp");

  int pointCount = genCubes(vertex_buffer, uv_buffer);

  GLuint text = 0;
  glBindTexture(GL_TEXTURE_2D, text);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE,
               blockTexture);

  free(blockTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glClearColor(170 / 256.0, 220 / 256.0, 230 / 256.0, 1.0);

  controls controls = {.x = 0, .y = 0, .z = 0, .xv = 0, .yv = 0, .zv = 0};

  glfwSetKeyCallback(window,keyCallback);

  while (!glfwWindowShouldClose(window)) {
    evalKeys(&controls);

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix model = IDENTITY_MATRIX;

    Matrix view = IDENTITY_MATRIX;
    translate(&view, -controls.x, -controls.y - 3, controls.z);

    Matrix proj = perspective(120.0, ratio, 0.1f, 1000.0f);

    glUniformMatrix4fv(3, 1, GL_FALSE, &model.m[0]);
    glUniformMatrix4fv(4, 1, GL_FALSE, &view.m[0]);
    glUniformMatrix4fv(5, 1, GL_FALSE, &proj.m[0]);

    GLuint vpos_location = 1;
    glEnableVertexAttribArray(vpos_location);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    GLuint uv_location = 2;
    glEnableVertexAttribArray(uv_location);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawArrays(GL_TRIANGLES, 0, pointCount);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
