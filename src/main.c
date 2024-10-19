#include<stdio.h>
#include<string.h>

#include<include/shader_compiler.h>
#include<include/file_reader.h>
#include<include/renderer.h>

#include<GL/glew.h>
#include<GL/freeglut.h>
#include<math.h>
#include<assert.h>
#define asset_path "assets/meshes/torus.obj"


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

  Vertex *vertices;
  GLuint *indices;
  unsigned long nvertices, nindices;

  //NOTE: ReadOBJ assumes that there are exactly 3 values per vertex and per face
  ReadOBJFile(asset_path, &vertices, &nvertices, &indices, &nindices);

  CreateProgram();
  CreateMesh(vertices, 3*nvertices, indices, 3*nindices);

  //glut callbacks
  glutMotionFunc(MotionFunc);
  glutIdleFunc(Draw);
  glutMainLoop();

  free(vertices);
  free(indices);
  return 0;
}
