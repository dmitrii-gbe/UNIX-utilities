#include <stdio.h>

int main() {
  for (int i = 0; i < 256; ++i) {
    if (i == 250) {
      printf("%c\n", 255);
    }
    printf("%c\n", i);
  }
  return 0;
}
