#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#include<include/file_reader.h>

#define MAX_LINE_SIZE 255

int CheckLineForMode (const char *line, ShaderMode *mode){
  if (strstr(line,"vertex")){
    *mode = VERTEX;
    return 1;
  }

  if (strstr(line,"fragment")){
    *mode = FRAGMENT;
    return 1;
  }

  return 0;
}

void ReadShaderFile (const char *path, char *vertex_str, char *fragment_str){
  //make sure strncat works as expected
  vertex_str[0] = 0x00;
  fragment_str[0] = 0x00;

  FILE *fptr = fopen(path, "r");

  assert (fptr && "ERROR: could not open shader file\n");

  ShaderMode mode = NONE;

  char line[MAX_LINE_SIZE];

  while (fgets(line, sizeof(line), fptr)){

    if (CheckLineForMode(line, &mode)){
      continue;
    }

    if (mode == VERTEX) {
      strncat(vertex_str, line, MAX_LINE_SIZE);
      continue;
    }

    if (mode == FRAGMENT) {
      strncat(fragment_str, line, MAX_LINE_SIZE);
      continue;
    }
    assert(!"ERROR: no #shader specifier in shader file \n");
  }

  fclose(fptr); 
}

//you can NOT use negative indices even though the specification allows for them I will not
//groups are ignored
// ignoring textures (for now)
// assuming nvertices == nnormals
static void PreprocessOBJFile(FILE *fptr,unsigned long *nvertices, unsigned long *nfaces){
  char line[MAX_LINE_SIZE]; 
  while (fgets(line, sizeof(line), fptr)){
    if (line[0] == 'v' && line[1] == ' '){
      (*nvertices)++;
    }
    else if (*line == 'f'){
      (*nfaces)++;
    }
  }
  rewind(fptr);
}

static size_t vi = 0, vni = 0, fi = 0; //vti = 0

//assumes normals an vertices are paired so that each vertex can only have or use 1 normal
//also assumes vn defined before faces

static void ProcessLine( char * line,
    Vertex **vertices,
    GLfloat *normals,
    GLuint **indices){

  if (!strncmp (line, "vt", 2)){
    //   char *token;
    //   token = strtok(line, "\n vt");
    //   (*textures)[vti] = atof(token);
    //   vti++;
    //
    //   while((token = strtok(NULL, "\n vt"))){
    //     (*textures)[vti] = atof(token);
    //     vti++;
    //   }
  }

  else if (!strncmp (line, "vn", 2)){
    char *token;

    token = strtok(line, "\n vn");
    normals[vni] = atof(token);
    vni++;

    while((token = strtok(NULL, "\n vn"))){
      normals[vni] = atof(token);
      vni++;
    }
  }

  else if (*line == 'v'){
    char *token;

    token = strtok(line, "\n v");
    (*vertices)[vi].position[0] = atof(token);

    for(int i = 1;(token = strtok(NULL, "\n v")); i++){
      (*vertices)[vi].position[i] = atof(token);
    }
    vi++;
  }

  else if (*line == 'f'){
    char *token;
    char *delim;
    token = strtok(line, "\n f");
    (*indices)[fi] = strtoul(token, &delim, 10) -1;

    if (*delim == '/' && delim[1] != '/'){
      //(*texture_indices)[fi] = strtoul(delim +1, &delim, 10)-1;
    }
    if (*delim == '/'){
      unsigned long nindex = strtoul(delim +1, &delim, 10)-1;
      for(int i = 0; i < 3; i++){
        (*vertices)[(*indices)[fi]].normal[i] = normals[nindex + i];
      }
    }

    fi++;
    while((token = strtok(NULL, "\n f"))){
      (*indices)[fi] = strtoul(token, &delim, 10) - 1;

      if (*delim == '/' && delim[1] != '/'){
        //(*texture_indices)[fi] = strtoul(delim +1, &delim, 10) - 1;
      }
      if (*delim == '/'){
        unsigned long nindex = strtoul(delim +1, &delim, 10)-1;
        for(int i = 0; i < 3; i++){
          (*vertices)[(*indices)[fi]].normal[i] = normals[nindex + i];
        }
      }
      fi++;
    }
  }
}

void ReadOBJFile (const char *path, Vertex **vertices, unsigned long *nvertices, GLuint **indices, unsigned long *nfaces){
  *nvertices = 0;
  *nfaces = 0;

  FILE *fptr = fopen(path, "r");
  assert (fptr && "ERROR: could not open OBJ file\n");

  char line[MAX_LINE_SIZE]; 

  PreprocessOBJFile(fptr, nvertices, nfaces);

  (*vertices) = (Vertex*)(malloc(sizeof(Vertex) * (*nvertices)));
  (*indices) = (GLuint*)(malloc(sizeof(GLuint) * (*nfaces)*3));

  //temp array
  GLfloat* normals = (GLfloat*)(malloc((*nvertices)*sizeof(GLfloat)*3));

  while (fgets(line, MAX_LINE_SIZE, fptr)){
    ProcessLine(line, vertices, normals, indices);
  }

  fclose(fptr); 
}
