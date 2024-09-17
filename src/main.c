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

static void TimeUniformUpdate(GLint id){
  static float time = 0.0;
  float increment = 0.005;
  time += increment;
  glUniform1f(id, time);
}

int main (int argc, char *argv[]){
  WindowInit(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK){
    fprintf(stderr, "Error: something went wrong when initializing GLEW. CODE: %u", err);
  }

  glClearColor(0,0,0,1);

  GLfloat *vertices, *normals, *textures;
  GLuint *indices , *texture_indices, *normal_indices;
  unsigned long nvertices, nnormals, ntextures, nindices;
  const char * asset_path = "assets/meshes/torus.obj";

  //NOTE: ReadOBJ assumes that there are exactly 3 values per vertex and per face
  ReadOBJFile(asset_path,
      &vertices,
      &nvertices,
      &normals,
      &nnormals,
      &textures,
      &ntextures,
      &indices,
      &nindices,
      &texture_indices,
      &normal_indices);

  CreateProgram();
  PushUniform("time", TimeUniformUpdate);
  CreateMesh(vertices, 3*nvertices, indices, 3*nindices);

  glutIdleFunc(Draw);
  glutMainLoop();

  free(vertices);
  free(indices);
  while (PopUniform()){}
  return 0;
}

