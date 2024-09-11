#include<GL/glew.h>
#include<GL/glut.h>

GLuint CompileShader(GLenum type, const char *str);
GLuint CreateShader(const char* vertex_shader,const char* fragment_shader);
