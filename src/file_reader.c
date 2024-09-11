#include<stdio.h>
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
#endif
