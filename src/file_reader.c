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

//obj files only include the following data
//v,vn,vt 
//f(face)/texture coords/normal
// you can join lines with line continuation character 
//you can NOT use negative indices even though the specification allows for them I will not
//groups are ignored

static void PreprocessOBJFile(FILE *fptr,unsigned long *nvertices, unsigned long *nnormals, unsigned long *ntext, unsigned long *nfaces){
  char line[MAX_LINE_SIZE]; 
  while (fgets(line, sizeof(line), fptr)){
    if (!strncmp (line, "vt", 2)){
      (*ntext)++;
    }
    else if (!strncmp (line, "vn", 2)){
      (*nnormals)++;
    }
    else if (*line == 'v'){
      (*nvertices)++;
    }
    else if (*line == 'f'){
      (*nfaces)++;
    }
  }
  rewind(fptr);
}

static unsigned int vi = 0, vni = 0, vti = 0, fi = 0;
static void ProcessLine( char * line,
    GLfloat **vertices,
    GLfloat **normals,
    GLfloat **textures,
    GLuint **indices,
    GLuint **texture_indices,
    GLuint **normal_indices ){

  if (!strncmp (line, "vt", 2)){
    char *token;
    token = strtok(line, "\n vt");
    (*normals)[vti] = atof(token);
    vti++;

    while((token = strtok(NULL, "\n vt"))){
      (*vertices)[vti] = atof(token);
      vti++;
    }
  }

  else if (!strncmp (line, "vn", 2)){
    char *token;

    token = strtok(line, "\n vn");
    (*normals)[vni] = atof(token);
    vni++;

    while((token = strtok(NULL, "\n vn"))){
      (*vertices)[vni] = atof(token);
      vni++;
    }
  }

  else if (*line == 'v'){
    char *token;

    token = strtok(line, "\n v");
    (*vertices)[vi] = atof(token);
    vi++;

    while((token = strtok(NULL, "\n v"))){
      (*vertices)[vi] = atof(token);
      vi++;
    }
  }

  else if (*line == 'f'){
    char *token;
    char *delim;
    token = strtok(line, "\n f");
    (*indices)[fi] = strtoul(token, &delim, 10) -1;

    if (*delim == '/' && delim[1] != '/'){
      (*texture_indices)[fi] = strtoul(delim +1, &delim, 10)-1;
    }
    if (*delim == '/'){
      (*normal_indices)[fi] = strtoul(delim +1, &delim, 10)-1;
    }

    fi++;
    while((token = strtok(NULL, "\n f"))){
      (*indices)[fi] = strtoul(token, &delim, 10) - 1;

      if (*delim == '/' && delim[1] != '/'){
        (*texture_indices)[fi] = strtoul(delim +1, &delim, 10) - 1;
      }
      if (*delim == '/'){
        (*normal_indices)[fi] = strtoul(delim +1, &delim, 10) - 1;
      }
      fi++;
    }
  }
}

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
    GLuint **normal_indices){

  *nvertices = 0;
  *nnormals = 0;
  *ntext = 0;
  *nfaces = 0;

  FILE *fptr = fopen(path, "r");
  assert (fptr && "ERROR: could not open OBJ file\n");

  char line[MAX_LINE_SIZE]; 

  PreprocessOBJFile(fptr, nvertices, nnormals, ntext, nfaces);

  (*vertices) = (GLfloat*)(malloc(sizeof(GLfloat) * (*nvertices)*3));

  (*indices) = (GLuint*)(malloc(sizeof(GLuint) * (*nfaces)*3));
  (*texture_indices) = (GLuint*)(malloc(sizeof(GLuint) * (*nfaces)*3));
  (*normal_indices) = (GLuint*)(malloc(sizeof(GLuint) * (*nfaces)*3));

  (*normals) = (GLfloat*)(malloc(sizeof(GLfloat) * (*nnormals)*3));
  (*textures) = (GLfloat*)(malloc(sizeof(GLfloat) * (*ntext)*2));

  while (fgets(line, MAX_LINE_SIZE, fptr)){
    ProcessLine(line, vertices, normals, textures, indices, texture_indices, normal_indices);
  }

  fclose(fptr); 
}
