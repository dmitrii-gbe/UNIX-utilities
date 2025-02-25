#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINES 10
#define LINE_WIDTH 100

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

int main(int argc, char** argv) {
  srand(time(NULL));
  if (strcmp(argv[1], "-lf") == 0) {
    int num = rand() % MAX_LINES;
    for (int i = 0; i < num + argc; ++i) {
      printf("\n");
    }
  } else {
    for (int i = 0; i < MAX_LINES + argc; ++i) {
      PrintLine(rand() % 50);
      PrintLineFeed(rand() % 4);
    }
  }
  return 0;
}
