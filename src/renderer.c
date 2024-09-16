#include<stdio.h>
#include<math.h>

#include<include/renderer.h>
#include<include/file_reader.h>
#include<include/shader_compiler.h>

static GLuint vao;
static GLsizei nelements;
static GLint uniform;

void glClearError(){
  while(glGetError()){}
}
void glCheckError(const char *function_name, const char *file, int line){
  GLenum flag;

  while((flag = glGetError())){
    fprintf (stderr, "ERROR LINE %u: triggered by function %s\nfile:%s\ncode: %u", line, function_name, file, flag);
  }
}


void CreateBuffers(){
  CHECK(glGenVertexArrays(1,&vao));
  CHECK(glBindVertexArray(vao));

  GLuint vbo;
  CHECK(glGenBuffers(1, &vbo));
  CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

  GLuint ibo;
  CHECK(glGenBuffers(1, &ibo));
  CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

  CHECK(glEnableVertexAttribArray(0));
  //params: (index of attrib, how many components in attribute, type of components, normalize?, stride
  //offset of vertex[for some reason a pointer but insert an int])
  CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0));
}

void CreateMesh(GLfloat* vertices, unsigned int nverts, GLuint *indices, unsigned int n){
  nelements = n;
  CreateBuffers();
  CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nverts, vertices, GL_STATIC_DRAW));
  CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * n, indices, GL_STATIC_DRAW));

  CHECK(glBindVertexArray(0));
  CHECK(glBindBuffer(GL_ARRAY_BUFFER,0));
  CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
}

void CreateProgram (){
  //janky solution, max line len 399 because of null char
  const size_t max_capacity = 400;
  char *vertex_str = (char *)malloc(sizeof(char) * max_capacity);
  char *fragment_str =(char *)malloc(sizeof(char) * max_capacity);
  const char *shader_file_path = "shaders/shader";

  ReadShaderFile(shader_file_path, vertex_str, fragment_str);
  GLuint program_id = CreateShader(vertex_str, fragment_str);
  glUseProgram(program_id);
  uniform = glGetUniformLocation(program_id, "time");
}

void Draw() {
  static float time = 0.0;
  float increment = 0.005;
  time += increment;
  glUniform1f(uniform, time);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glClear(GL_COLOR_BUFFER_BIT);
  CHECK(glBindVertexArray(vao));
  CHECK(glDrawElements(GL_TRIANGLES, nelements,GL_UNSIGNED_INT, NULL));
  glFlush();
}
