#include<stdio.h>
#include<string.h>

#include<include/shader_compiler.h>
#include<include/file_reader.h>
#include<include/renderer.h>

#include<GL/glew.h>
#include<GL/freeglut.h>
#include<math.h>
#include<assert.h>


void WindowInit(int argc, char *argv[]){
  glutInit (&argc, argv);
  glutInitContextVersion(4,5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(200,30);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("main");
}

int main (int argc, char *argv[]){
  WindowInit(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK){
    fprintf(stderr, "Error: something went wrong when initializing GLEW. CODE: %u", err);
  }

  glClearColor(0,0,0,1);

  //temp data
  GLfloat vertices [] = {
    -1,-1,0,
    1,-1,0,
    0,1,0,
    0,0,1
  };

  GLuint indices [] = {
    0,1,2,
    0,1,3,
    1,2,3,
    0,2,3
  };

  CreateProgram();
  CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));

  glutIdleFunc(Draw);
  glutMainLoop();
  return 0;
}

