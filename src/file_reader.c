#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<include/file_reader.h>

#ifndef FILE_READER
#define FILE_READER

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

  unsigned int line_size = 255;
  char line[line_size];

  while (fgets(line, sizeof(line), fptr)){

    if (CheckLineForMode(line, &mode)){
      continue;
    }

    if (mode == VERTEX) {
      strncat(vertex_str, line, line_size);
      continue;
    }

    if (mode == FRAGMENT) {
      strncat(fragment_str, line, line_size);
      continue;
    }
    assert(!"ERROR: no #shader specifier in shader file \n");
  }

  fclose(fptr); 
}

static void AllocateSpaceForOBJ(float **vertices, float **indices){
  while ((fgets(line, sizeof(line), fptr), strstr(line, "#"))){
    if(strstr(line, "vertex")){
      char *p = line;
      char *digits = NULL;

      //while not null term char
      while(*p){
        if (isdigit(*p)) digits = p;
        else *p = '\0';
        p++;
      }

      if (!digits){
        fprintf(stderr, "OBJ ERROR: vertex line did not contain digits");
        continue;
      }
      int num_vertices = atoi(digits);

      if (num_vertices == 0){
        fprintf(stderr, "atoi returned bad value when reading obj file");
      }

      *indices = (float *) (malloc(sizeof(float)*num_vertices));
    }

    if(strstr(line, "face")){
      char *p = line;
      char *digits = NULL;

      //while not null term char
      while(*p){
        if (isdigit(*p)) digits = p;
        else *p = '\0';
        p++;
      }

      if (!digits){
        fprintf(stderr, "OBJ ERROR: vertex line did not contain digits");
        continue;
      }

      int num_vertices = atoi(digits);

      if (num_vertices == 0){
        fprintf(stderr, "atoi returned bad value when reading obj file");
      }

      *indices = (float *) (malloc(sizeof(float)*num_vertices));
    }
  }
}
//caller responsible for freeing memory
void ReadOBJFile (const char *path, float **vertices, float **indices){

  FILE *fptr = fopen(path, "r");
  assert (fptr && "ERROR: could not open OBJ file\n");

  unsigned int line_size = 255;
  char line[line_size]; 
  AllocateSpaceForOBJ(vertices, indices);

  while (fgets(line, sizeof(line), fptr)){
    if (line[0] == 'v'){
      char *token;
      while(strtok(token, "\n v")){

      }
    }
    assert(!"ERROR: no #shader specifier in shader file \n");
  }

  fclose(fptr); 
}
#endif
