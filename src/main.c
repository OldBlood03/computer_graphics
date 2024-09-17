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

static int lastx = 0,lasty = 0;
static int dx = 0, dy = 0;

//camera position vector, heading vector always -position vector
static int cx=0,cy=0,cz=1;

void MotionFunc (int x, int y){
  int dx = x-lastx;
  int dy = y-lasty;
}

static void TimeUniformUpdate(GLint id){
  static float time = 0.0;
  float increment = 0.005;
  time += increment;
  glUniform1f(id, time);
}

static void CameraUniformUpdate(GLint id){
  //do some camera wizardry
  cx = cos(cx + dx) - sin(cx + dx);
  cy = 0;
  cz = 0;
  glUniform3f(id, cx, cy, cz);
}

int main (int argc, char *argv[]){
  WindowInit(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK){
    fprintf(stderr, "Error: something went wrong when initializing GLEW. CODE: %u", err);
  }

  glClearColor(0,0,0,1);

  GLfloat *vertices;
  GLuint *indices;
  unsigned long nvertices, nindices;
  const char * asset_path = "assets/cone.obj";

  //NOTE: ReadOBJ assumes that there are exactly 3 values per vertex and per face
  ReadOBJFile(asset_path, &vertices, &indices, &nvertices, &nindices);

  CreateProgram();
  PushUniform("time", TimeUniformUpdate);
  CreateMesh(vertices, 3*nvertices, indices, 3*nindices);

  glutMotionFunc(MotionFunc);
  glutIdleFunc(Draw);
  glutMainLoop();

  free(vertices);
  free(indices);
  while (PopUniform()){}
  return 0;
}

