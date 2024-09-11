#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <include/shader_compiler.h>

GLuint CompileShader(GLenum type, const char *str){
  GLuint id;
  id = glCreateShader(type);
  glShaderSource(id, 1, &str, NULL);
  glCompileShader(id);

  //error handling
  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);

  if (!status){
    GLint length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    char *message = (char *)(alloca(sizeof(char) * length));
    glGetShaderInfoLog(id, length, &length, message);

    fprintf (stderr, "SHADER ERROR: %s\n", message);
    exit(1);
  }

  return id;
}

GLuint CreateShader(const char* vertex_shader,const char* fragment_shader){
  GLuint program = glCreateProgram ();
  GLuint vertex_id = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  GLuint fragment_id = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program, vertex_id);
  glAttachShader(program, fragment_id);

  glLinkProgram(program);

  GLint status;
  glValidateProgram(program);

  //error handling
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  assert(status && "PROGRAM DID NOT LINK PROPERLY");
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

  if (!status){
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    char *message = (char *)(alloca(sizeof(char) * length));
    glGetProgramInfoLog(program, length, &length, message);

    fprintf (stderr, "PROGRAM ERROR: %s\n", message);
    exit(1);
  }

  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);
  return program;
}
