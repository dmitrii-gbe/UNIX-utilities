#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../common/common.h"

#define MAX_LINES 10
#define LINE_WIDTH 100
#define INIT_STR_SIZE 1000
#define SUBSTR_FROM_STR 3

void PrintLineFeed(int count) {
  for (int i = 0; i < count; ++i) {
    printf("%c", 10);
  }
}

void PrintLine(int count) {
  for (int i = 0; i < count; ++i) {
    printf("%c", rand() % 256);
  }
}

int CheckForParentiezes(char* str) {
  int op_r = 0;
  int cl_r = 0;
  int op_f = 0;
  int cl_f = 0;
  int op_q = 0;
  int cl_q = 0;
  int len = strlen(str);
  for (int i = 0; i < len; ++i) {
    op_r += str[i] == '(';
    cl_r += str[i] == ')';
    op_f += str[i] == '{';
    cl_f += str[i] == '}';
    op_q += str[i] == '[';
    cl_q += str[i] == ']';
  }
  return (op_r + cl_r + op_f + cl_f + op_q + cl_q) == 0;
}

char* GenerateRandomSubstr(StringBuffer* str) {
  char* substr = (char*)malloc(INIT_STR_SIZE);
  char* cursor = substr;
  int len = strlen(str->array_);
  int start = rand() % len;
  int end = rand() % len;
  if (start > end) {
    int tmp = end;
    end = start;
    start = tmp;
  }

  if (end - start > INIT_STR_SIZE) {
    end = start + INIT_STR_SIZE;
  }
  for (int i = start; i < end; ++i) {
    *cursor++ = str->array_[i];
  }
  *cursor = '\0';
  return substr;
}

void ToUpper(char* str, int size) {
  int idx = 0;
  for (; idx < size; ++idx) {
    if (str[idx] > 96 && str[idx] < 123) {
      str[idx] = str[idx] - 32;
    }
  }
}

int main(int argc, char** argv) {
  srand(time(NULL));
  StringBuffer str = InitString(INIT_STR_SIZE);
  FILE* f = fopen(argv[1], "r");
  int file_size = CheckIfFileIsEmpty(f);
  while (file_size > 0) {
    ReadString(f, &str, &file_size);
    str.array_[str.size_] = '\0';
    for (int counter = 0; counter < SUBSTR_FROM_STR; ++counter) {
      char* substr = GenerateRandomSubstr(&str);
      if (CheckForParentiezes(substr) && strlen(substr) > 5) {
        printf("%s", substr);
        if (substr[strlen(substr) - 1] != 10) {
          printf("%c", 10);
        }
        ToUpper(substr, strlen(substr));
        printf("%s", substr);
        if (substr[strlen(substr) - 1] != 10) {
          printf("%c", 10);
        }
      }
      free(substr);
    }
  }
  DestroyStringBuffer(&str);
  return 0;
}
