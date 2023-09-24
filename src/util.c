#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *loadFile(char *shaderFile) {
  FILE *filePtr = fopen(shaderFile, "rb");
  fseek(filePtr, 0, SEEK_END);

  int fileSize = ftell(filePtr);
  rewind(filePtr);

  char *buffer = (char *)malloc(sizeof(char) * (fileSize + 1));
  memset(buffer, 0, fileSize + 1);
  fread(buffer, 1, fileSize, filePtr);

  return buffer;
}

unsigned char *readBMP(char *filename) {
  int i;
  FILE *f = fopen(filename, "rb");
  unsigned char info[54];

  fread(info, sizeof(unsigned char), 54, f);

  int width = *(int *)&info[18];
  int height = *(int *)&info[22];

  int size = 3 * width * height;
  unsigned char *data = malloc(size);

  fread(data, sizeof(unsigned char), size, f);
  fclose(f);

  for (i = 0; i < size; i += 3) {
    unsigned char tmp = data[i];
    data[i] = data[i + 2];
    data[i + 2] = tmp;
  }

  return data;
}
