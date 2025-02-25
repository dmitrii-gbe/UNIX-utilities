#include "common.h"

#include <stdio.h>
#include <stdlib.h>

void ReallocStringBuffer(StringBuffer *const buf, int limit) {
  if (limit > buf->capacity_) {
    int new_capacity = limit * 2 + 1;
    buf->array_ = realloc(buf->array_, new_capacity);
    buf->capacity_ = new_capacity;
  }
}

void DestroyStringBuffer(StringBuffer *buf) { free(buf->array_); }

StringBuffer InitString(int capacity) {
  StringBuffer buf;
  buf.array_ = (char *)malloc(capacity);
  buf.size_ = 0;
  buf.capacity_ = capacity;
  return buf;
}

int CheckIfFileIsEmpty(FILE *f) {
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);
  return size;
}

void ReadString(FILE *f, StringBuffer *input_buf, int *const file_size) {
  char *tmp = input_buf->array_;
  char c = 0;
  input_buf->size_ = 0;
  while (*file_size > 0 && c != '\n') {
    ReallocStringBuffer(input_buf, input_buf->size_ + 1);
    tmp = input_buf->array_ + input_buf->size_;
    input_buf->size_ += fread(tmp, 1, 1, f);
    --(*file_size);
    c = *tmp++;
  }
}

int IsEmpty(const StringBuffer *const str) {
  return *(uint8_t *)str->array_ == 10;
}

int CheckIfDirectory(const char *const fileName) {
  struct stat path;
  return stat(fileName, &path) == 0 && S_ISDIR(path.st_mode);
}

int CheckFileForValdity(const char *const file, const int *const args,
                        int cat_or_grep) {
  int result = 0;
  int is_directory = CheckIfDirectory(file);
  if (is_directory) {
    if (cat_or_grep == CAT) {
      printf("cat: %s: Is a directory\n", file);
    } else {
      printf("grep: %s: Is a directory\n", file);
    }
  } else {
    FILE *f = fopen(file, "r");
    if (!f) {
      if (cat_or_grep == CAT) {
        printf("cat: %s: No such file or directory\n", file);
      } else if (cat_or_grep == GREP && !args['s']) {
        printf("grep: %s: No such file or directory\n", file);
      }
    } else {
      result = 1;
      fclose(f);
    }
  }
  return result;
}

int GetFileSize(const char *const file) {
  int size = 0;
  FILE *f = fopen(file, "r");
  size = CheckIfFileIsEmpty(f);
  fclose(f);
  return size;
}
