#ifndef FILEREADER
#define FILEREADER
typedef enum ShaderMode {
  NONE = 0,
  FRAGMENT = 1,
  VERTEX = 2
}ShaderMode;

int CheckLineForMode (const char *line, ShaderMode *mode);
void ReadShaderFile (const char *path, char *vertex_str, char *fragment_str);
#endif
