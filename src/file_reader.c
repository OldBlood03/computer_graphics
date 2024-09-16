#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<include/file_reader.h>

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

static void AllocateSpaceForOBJ(FILE* fptr, GLfloat **vertices, GLuint **indices, unsigned long *nvertices, unsigned long *nindices){
  unsigned int line_size = 255;
  char line[line_size]; 

  while (fgets(line, sizeof(line), fptr) && strstr(line, "#")){
    if(strstr(line, "vertex")){
      char *p = line;
      char *digits = NULL;

      //while not null term char
      while(*p){
        if (isdigit(*p) && !digits){
          digits = p;
        }
        else if (!isdigit(*p)) *p = '\0';
        p++;
      }

      if (!digits){
        fprintf(stderr, "OBJ ERROR: vertex line did not contain digits");
        continue;
      }

      int num_vertices = strtoul(digits, NULL, 10);

      if (num_vertices == 0){
        fprintf(stderr, "atoi returned bad value when reading obj file");
      }

      *nvertices = num_vertices;
      *vertices = (GLfloat *) (malloc(3*sizeof(GLfloat)*num_vertices));
    }

    if(strstr(line, "face")){
      char *p = line;
      char *digits = NULL;

      //while not null term char
      while(*p){
        if (isdigit(*p) && !digits){
          digits = p;
        }
        else if (!isdigit(*p)) *p = '\0';
        p++;
      }

      if (!digits){
        fprintf(stderr, "OBJ ERROR: face line did not contain digits");
        continue;
      }

      int num_indices = strtoul(digits,NULL, 10);

      if (num_indices == 0){
        fprintf(stderr, "atoi returned bad value when reading obj file");
      }

      *nindices = num_indices;
      *indices = (GLuint *) (malloc(3*sizeof(GLuint)*num_indices));
    }
  }
  //have to set file pointer back to the start of the line without # after fgets moved the fp
  size_t length = strlen(line);
  fseek(fptr, -length, SEEK_CUR);
}

//caller responsible for freeing memory
//NOTE: ReadOBJ is very janky and assumes the file has exactly 3 coordinates per vertex and 3 indices per face
void ReadOBJFile (const char *path, GLfloat **vertices, GLuint **indices, unsigned long *nvertices,unsigned long *nindices){
  FILE *fptr = fopen(path, "r");
  assert (fptr && "ERROR: could not open OBJ file\n");

  unsigned int line_size = 255;
  char line[line_size]; 

  AllocateSpaceForOBJ(fptr, vertices, indices, nvertices, nindices);

  for (int i = 0; i < *nvertices; i++){
    char *token;
    fgets(line, sizeof(line), fptr);

    token = strtok(line, "\n v");
    (*vertices)[i*3] = atof(token);

    for(int index = 1; (token = strtok(NULL, "\n v")); index++){
      (*vertices)[i*3 + index] = atof(token);
    }
  }

  for (int i = 0; i < *nindices; i++){
    char *token;
    fgets(line, sizeof(line), fptr);

    token = strtok(line, "\n f");
    (*indices)[i*3] = strtoul(token, NULL, 10);

    for(int index = 1; (token = strtok(NULL, "\n f")); index++){
      (*indices)[3*i + index] = strtoul(token, NULL, 10);
    }
  }
  fclose(fptr); 
}
