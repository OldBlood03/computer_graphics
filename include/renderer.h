#include<GL/glew.h>
#include<stdlib.h>
#ifndef RENDERER
#define RENDERER

#define CHECK(x) glClearError();x;glCheckError(#x, __FILE__, __LINE__)

void glClearError();

void glCheckError(const char *function_name, const char *file, int line);

typedef struct UniformStack{
struct UniformStack *previous;
GLint id;
void (*callback)(GLint);
}UniformStack;

typedef struct Vertex{
GLfloat position[3];
GLfloat normal[3];
} Vertex;

void KeyboardFunc(unsigned char key, int x, int y);
int CheckBufferBinding ();
void UnbindAll ();
void CreateBuffers();
void CreateMesh(Vertex* vertices, unsigned int nverts, GLuint *indices, unsigned int nelements);
void Draw();
void CreateProgram();
int PopUniform();
void MotionFunc(int,int);
#endif
