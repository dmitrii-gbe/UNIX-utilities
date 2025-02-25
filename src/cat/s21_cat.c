#include <stdio.h>

#include "../common/common.h"

#define INIT_STR_SIZE 1000
#define ARG_ARR_SIZE 119
#define FILE_NAME_ARR_SIZE 100

int GetFileNames(const int argc, const char **const argv, int *const files);

void GetArguments(const int argc, const char **const argv,
                  int *const arguments);

void ReplaceCharacters(StringBuffer *input_buf, StringBuffer *output_buf,
                       const int *args);

void PrintRaw(const StringBuffer *const str);

void ProcessString(StringBuffer *const input_buf,
                   StringBuffer *const output_buf, const int *args,
                   int *const counter, int *eof);

void ProcessInput(const int argc, const char **const argv,
                  const int *const args);

int main(const int argc, const char **const argv) {
  int args[ARG_ARR_SIZE];
  GetArguments(argc, argv, args);
  ProcessInput(argc, argv, args);
  return 0;
}

int GetFileNames(const int argc, const char **const argv, int *const files) {
  int counter = 0;
  for (int i = 1; i < argc; ++i) {
    if ((strlen(argv[i]) == 1 && argv[i][0] == '-') || argv[i][0] != '-') {
      files[counter++] = i;
    }
  }
  return counter;
}

void GetArguments(const int argc, const char **const argv,
                  int *const arguments) {
  for (int i = 0; i < ARG_ARR_SIZE; ++i) {
    arguments[i] = 0;
  }
  for (int i = 1; i < argc; ++i) {
    int len = strlen(argv[i]);
    if (len > 1 && argv[i][0] == '-' && argv[i][1] != '-') {
      for (int j = 1; j < len; ++j) {
        arguments[(int)argv[i][j]] = 1;
        if (argv[i][j] == 'e' || argv[i][j] == 't') {
          arguments['v'] = 1;
        } else if (argv[i][j] == 'A') {
          arguments['v'] = 1;
          arguments['E'] = 1;
          arguments['T'] = 1;
        }
      }
    }
    if (strcmp(argv[i], "--number") == 0) {
      arguments['n'] = 1;
    }
    if (strcmp(argv[i], "--squeeze-blank") == 0) {
      arguments['s'] = 1;
    }
    if (strcmp(argv[i], "--number-nonblank") == 0) {
      arguments['b'] = 1;
    }
  }
  if (arguments['b']) {
    arguments['n'] = 0;
  }
  if (arguments['E']) {
    arguments['e'] = 1;
  }
  if (arguments['T']) {
    arguments['t'] = 1;
  }
}

void ReplaceCharacters(StringBuffer *input_buf, StringBuffer *output_buf,
                       const int *args) {
  ReallocStringBuffer(output_buf, input_buf->size_ * 4 + 1);
  char *output = output_buf->array_;
  char *input_ptr = input_buf->array_;
  while (input_ptr != input_buf->array_ + input_buf->size_) {
    if (args['t'] && *input_ptr == '\t') {
      *output++ = '^';
      *output++ = 'I';
    } else if (args['e'] && *input_ptr == '\n') {
      *output++ = '$';
      *output++ = '\n';
    } else if (args['v'] && *input_ptr >= 0 && *input_ptr <= 8) {
      *output++ = '^';
      *output++ = *input_ptr + 64;
    } else if (args['v'] && *input_ptr >= 11 && *input_ptr <= 31) {
      *output++ = '^';
      *output++ = *input_ptr + 64;
    } else if (args['v'] && *(uint8_t *)input_ptr == 127) {
      *output++ = '^';
      *output++ = '?';
    } else if (args['v'] && *(uint8_t *)input_ptr >= 128 &&
               *(uint8_t *)input_ptr <= 159) {
      *output++ = 'M';
      *output++ = '-';
      *output++ = '^';
      *output++ = *(uint8_t *)input_ptr - 64;
    } else if (args['v'] && *(uint8_t *)input_ptr >= 160 &&
               *(uint8_t *)input_ptr < 255) {
      *output++ = 'M';
      *output++ = '-';
      *output++ = *(uint8_t *)input_ptr - 128;
    } else if (args['v'] && *(uint8_t *)input_ptr == 255) {
      *output++ = 'M';
      *output++ = '-';
      *output++ = '^';
      *output++ = '?';
    } else {
      *output++ = *input_ptr;
    }
    ++input_ptr;
  }
  *output = '\0';
  output_buf->size_ = output - output_buf->array_;
}

void PrintRaw(const StringBuffer *const str) {
  for (int i = 0; i < str->size_; ++i) {
    putchar(str->array_[i]);
  }
}

void ProcessString(StringBuffer *const input_buf,
                   StringBuffer *const output_buf, const int *args,
                   int *const counter, int *eof) {
  int empty = IsEmpty(input_buf);
  ReplaceCharacters(input_buf, output_buf, args);
  if (args['b']) {
    if (!*eof && !empty) {
      printf("%6d\t", *counter);
      ++(*counter);
      PrintRaw(output_buf);
    } else {
      PrintRaw(output_buf);
    }
  } else if (args['n']) {
    if (!*eof) {
      printf("%6d\t", *counter);
      ++(*counter);
      PrintRaw(output_buf);
    } else {
      PrintRaw(output_buf);
    }
  } else {
    PrintRaw(output_buf);
  }
  *eof = 0;
}

void ProcessInput(const int argc, const char **const argv,
                  const int *const args) {
  int files[FILE_NAME_ARR_SIZE];
  int files_count = GetFileNames(argc, argv, files);
  StringBuffer input_buf = InitString(INIT_STR_SIZE);
  StringBuffer out_buf = InitString(INIT_STR_SIZE);
  char *s = (char *)malloc(INIT_STR_SIZE);
  int counter = 1, eof = 0, prev_empty = 0, empty, string_counter = 0,
      file_size;
  for (int i = 0; i < files_count; ++i) {
    int prev_eof = eof;
    if (argv[files[i]][0] == '-') {
      if (scanf("%5000s", s)) {
        printf("%s\n", s);
      }
    } else if (CheckFileForValdity(argv[files[i]], args, CAT) &&
               (file_size = GetFileSize(argv[files[i]]))) {
      FILE *f = fopen(argv[files[i]], "r");
      while (file_size > 0) {
        ReadString(f, &input_buf, &file_size);
        empty = input_buf.size_ == 0 || input_buf.array_[0] == '\n';
        ++string_counter;
        if (!args['s'] || !empty || !prev_empty) {
          ProcessString(&input_buf, &out_buf, args, &counter, &eof);
        }
        prev_empty = (prev_eof && string_counter == 1) ? 0 : empty;
      }
      string_counter = 0;
      eof = input_buf.array_[input_buf.size_ - 1] != '\n';
      fclose(f);
    }
  }
  free(s);
  DestroyStringBuffer(&input_buf);
  DestroyStringBuffer(&out_buf);
}
