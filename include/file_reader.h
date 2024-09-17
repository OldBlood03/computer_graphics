#include <glew.h>
#include<include/renderer.h>
#ifndef FILEREADER
#define FILEREADER
typedef enum ShaderMode {
  NONE = 0,
  FRAGMENT = 1,
  VERTEX = 2
}ShaderMode;

int CheckLineForMode (const char *line, ShaderMode *mode);
void ReadShaderFile (const char *path, char *vertex_str, char *fragment_str);
void ReadOBJFile (const char *path, 
    GLfloat **vertices,
    unsigned long *nvertices,
    GLfloat **normals, 
    unsigned long *nnormals,
    GLfloat **textures,
    unsigned long *ntext,
    GLuint **indices,
    unsigned long *nfaces,
    GLuint **texture_indices,
    GLuint **normal_indices);
#endif
