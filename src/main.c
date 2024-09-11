#include<stdio.h>
#include<string.h>

#include<include/shader_compiler.h>
#include<include/file_reader.h>

#include<GL/glew.h>
#include<GL/freeglut.h>
#include<GL/glu.h>
#include<math.h>
#include<assert.h>

#define CHECK(x) glClearError();x;glCheckError(#x, __FILE__, __LINE__)

static void glClearError(){
  while(glGetError()){}
}

static void glCheckError(const char *function_name, const char *file, int line){
  GLenum flag;

  while((flag = glGetError())){
    fprintf (stderr, "ERROR LINE %u: triggered by function %s\nfile:%s\ncode: %u", line, function_name, file, flag);
  }
}

typedef struct DataBlock{
  GLuint vbo, vao, ibo;
  size_t nelements;
  size_t vbo_size;
  int initialized;
} DataBlock;

DataBlock CreateBuffers(DataBlock *ids, GLfloat* vertices, unsigned int nverts, GLuint *indices, unsigned int nelements){

  ids->nelements = sizeof(indices);

  CHECK(glGenVertexArrays(1,&(ids->vao)));
  CHECK(glBindVertexArray(ids->vao));

  CHECK(glGenBuffers(1, &(ids->vbo)));
  CHECK(glBindBuffer(GL_ARRAY_BUFFER, ids->vbo));
  CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

  CHECK(glGenBuffers(1, &(ids->ibo)));
  CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids->ibo));
  CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, ids->nelements, indices, GL_STATIC_DRAW));
  ids->initialized=1;

  CHECK(glEnableVertexAttribArray(0));
  //params: (index of attrib, how many components in attribute, type of components, normalize?, stride
  //offset of vertex[for some reason a pointer but insert an int])
  CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0));
}

static DataBlock ids = { .vbo=0, .vao=0, .ibo=0, .initialized=0};

void Draw() {
  GLfloat vertices [8] = {
    -1,0,
    1,0,
    0,1,
    0,-1,
  };

  GLuint indices [6] = {
    0,1,2,
    0,1,3
  };
  if (!ids.initialized){
    CreateBuffers(&ids, vertices, 8, indices, 6);
  }

  glClear(GL_COLOR_BUFFER_BIT);

  CHECK(glBindVertexArray(ids.vao));

  CHECK(glDrawElements(GL_TRIANGLES, ids.nelements,GL_UNSIGNED_INT, NULL));

  glFlush();
}

int main (int argc, char *argv[]){
  glutInit (&argc, argv);
  glutInitContextVersion(4,5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(200,30);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("main");


  //without glewinit, genBuffer seg faults
  GLenum err = glewInit();
  if (err != GLEW_OK){
    fprintf(stderr, "Error: something went wrong when initializing GLEW. CODE: %u", err);
  }

  glClearColor(0,0,0,1);

  //janky solution, max line len 399 because of null char
  const size_t max_capacity = 400;
  char *vertex_str = (char *)malloc(sizeof(char) * max_capacity);
  char *fragment_str =(char *)malloc(sizeof(char) * max_capacity);
  const char *shader_file_path = "shaders/shader";

  ReadShaderFile(shader_file_path, vertex_str, fragment_str);
  GLuint program_id = CreateShader(vertex_str, fragment_str);

  glUseProgram(program_id);
  glutIdleFunc(Draw);
  glutMainLoop();
  glDeleteProgram(program_id);

  return 0;
}

