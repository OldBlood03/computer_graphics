#include<GL/glew.h>
#include<stdlib.h>
#ifndef RENDERER
#define RENDERER

#define CHECK(x) glClearError();x;glCheckError(#x, __FILE__, __LINE__)

void glClearError();

void glCheckError(const char *function_name, const char *file, int line);

typedef struct DataBlock{
  GLuint vao, ibo;
  unsigned int nelements;
} DataBlock;

int CheckBufferBinding ();
void UnbindAll ();
void CreateBuffers();
void CreateMesh(GLfloat* vertices, unsigned int nverts, GLuint *indices, unsigned int nelements);
void Draw();
void CreateProgram();
#endif
