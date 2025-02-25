#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define CAT 0
#define GREP 1

typedef struct StringBuffer {
  char *array_;
  int size_;
  int capacity_;
} StringBuffer;

void DestroyStringBuffer(StringBuffer *buf);

void ReallocStringBuffer(StringBuffer *const buf, int new_capacity);

int CheckIfFileIsEmpty(FILE *f);

StringBuffer InitString(const int capacity);

void ReadString(FILE *f, struct StringBuffer *input_buf, int *const file_size);

int IsEmpty(const struct StringBuffer *const str);

int CheckIfDirectory(const char *const fileName);

int CheckFileForValdity(const char *const file, const int *const args,
                        int cat_or_grep);

int GetFileSize(const char *const file);
