#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../common/common.h"

#define INIT_STR_SIZE 5000
#define ARG_ARR_SIZE 125
#define FILE_NAME_ARR_SIZE 100
#define PATTERNS_ARR_SIZE 10000
#define MAX_MATCH_COUNT 10000

typedef struct Patterns {
  char **arr;
  int size;
  int capacity;
} Patterns;

typedef struct Substring {
  int start_;
  int post_end_;
} Substring;

void GetFileNames(const int argc, const char **const argv, int *const files,
                  const int *const args);

void GetArguments(const int argc, const char **const argv, int *const args);

void DestroyPatterns(Patterns *const pat);

void AddPattern(Patterns *const to, const char *const complex_pattern,
                int pattern_from_file);

Patterns GetPatterns(const int argc, const char **const argv,
                     const int *const args);

regex_t *CompilePatterns(const Patterns *const patterns, const int *const args,
                         int *const error);

int SubstringComparator(const void *const lhs, const void *const rhs);

void ProccesMatchesArray(Substring *const matches, int *const matches_arr_len);

int StringDirectMatch(const regex_t *const regex, const int regex_arr_size,
                      const char *const str, Substring *const matches,
                      int *const match_arr_size);

int StringMatch(const regex_t *const regex, const int regex_arr_size,
                const char *const str, Substring *const matches,
                const int *const args, int *const match_arr_size);

void PrintMatchingString(const char *const string, const char *const file,
                         const int string_counter, const int *const args,
                         const Substring *const exact_matches, const int o_flag,
                         int match_arr_size);

void PrintShort(const char *const file_name, int matching_strings,
                const int *const args);

void ProcessInput(int argc, const char **const argv, const int *const args,
                  const int *const files, const Patterns *const patterns);

int CountFiles(const int argc, const int *const files);

int main(const int argc, const char **const argv) {
  int args[ARG_ARR_SIZE];
  int files[FILE_NAME_ARR_SIZE];
  GetArguments(argc, argv, args);
  GetFileNames(argc, argv, files, args);
  if (CountFiles(argc, files) == 1) {
    args['h'] = 1;
  }
  Patterns patterns = GetPatterns(argc, argv, args);
  ProcessInput(argc, argv, args, files, &patterns);
  DestroyPatterns(&patterns);
  return 0;
}

void GetFileNames(const int argc, const char **const argv, int *const files,
                  const int *const args) {
  for (int i = 0; i < argc; ++i) {
    files[i] = 0;
  }

  int i = (args['e'] || args['f']) ? 1 : 2;
  for (; i < argc; ++i) {
    if ((strcmp(argv[i - 1], "-f") != 0) && (strcmp(argv[i - 1], "-e") != 0) &&
        argv[i][0] != '-') {
      files[i] = 1;
    }
  }
}

void GetArguments(const int argc, const char **const argv, int *const args) {
  for (int i = 0; i < ARG_ARR_SIZE; ++i) {
    args[i] = 0;
  }
  for (int i = 1; i < argc; ++i) {
    int len = strlen(argv[i]);
    if (len > 1 && argv[i][0] == '-') {
      for (int j = 1; j < len; ++j) {
        args[(int)argv[i][j]] = 1;
      }
    }
  }
}

void DestroyPatterns(Patterns *const pat) {
  for (int i = 0; i < pat->size; ++i) {
    free(pat->arr[i]);
  }
  free(pat->arr);
}

void AddPattern(Patterns *const to, const char *const complex_pattern,
                int pattern_from_file) {
  int len = strlen(complex_pattern);
  int i = 0;
  if (!pattern_from_file &&
      (complex_pattern[0] == 39 || complex_pattern[0] == 34)) {
    i = 1;
    --len;
  }
  to->arr[to->size] = (char *)malloc(len + 1);
  char *ptr = to->arr[to->size];
  for (; i < len; ++i) {
    *ptr++ = complex_pattern[i];
  }
  *ptr = '\0';
  ++(to->size);
}

Patterns GetPatterns(const int argc, const char **const argv,
                     const int *const args) {
  StringBuffer str = InitString(INIT_STR_SIZE);
  Patterns pat;
  pat.arr = (char **)malloc(sizeof(char *) * PATTERNS_ARR_SIZE);
  pat.size = 0;
  pat.capacity = PATTERNS_ARR_SIZE;
  if (!args['e'] && !args['f']) {
    int found = 0;
    for (int i = 1; i < argc && !found; ++i) {
      if (argv[i][0] != '-') {
        AddPattern(&pat, argv[1], 0);
        found = 1;
      }
    }
  }
  for (int arg_idx = 1; arg_idx < argc; ++arg_idx) {
    if (strcmp(argv[arg_idx], "-e") == 0 && arg_idx + 1 < argc) {
      AddPattern(&pat, argv[arg_idx + 1], 0);
    }
    if (strcmp(argv[arg_idx], "-f") == 0 && arg_idx + 1 < argc) {
      int file_size = 0;
      if (CheckFileForValdity(argv[arg_idx + 1], args, GREP) &&
          (file_size = GetFileSize(argv[arg_idx + 1]))) {
        FILE *f = fopen(argv[arg_idx + 1], "r");
        while (file_size > 0) {
          ReadString(f, &str, &file_size);
          str.array_[str.size_] = '\0';
          if (str.array_[str.size_ - 1] == 10 && strlen(str.array_) > 1) {
            str.array_[str.size_ - 1] = '\0';
            --str.size_;
          }
          AddPattern(&pat, str.array_, 1);
        }
        fclose(f);
      }
    }
  }
  DestroyStringBuffer(&str);
  return pat;
}

regex_t *CompilePatterns(const Patterns *const patterns, const int *const args,
                         int *const error) {
  regex_t *reg_array = (regex_t *)malloc(patterns->size * sizeof(regex_t));
  regex_t *ptr = reg_array;
  for (int i = 0; i < patterns->size && *error == 0; ++i) {
    *error = regcomp(ptr++, patterns->arr[i],
                     (args['i'] ? REG_ICASE : 0) | REG_NEWLINE) != 0;
  }
  return reg_array;
}

int SubstringComparator(const void *const lhs, const void *const rhs) {
  int result_of_comparison = 0;
  const Substring *const lhs_ = (Substring *)lhs;
  const Substring *const rhs_ = (Substring *)rhs;
  if (lhs_->start_ > rhs_->start_ ||
      (lhs_->start_ == rhs_->start_ &&
       lhs_->post_end_ - lhs_->start_ < rhs_->post_end_ - rhs_->start_)) {
    result_of_comparison = 1;
  }
  return result_of_comparison;
}

void ProccesMatchesArray(Substring *const matches, int *const matches_arr_len) {
  Substring *new_array =
      (Substring *)malloc(sizeof(Substring) * *matches_arr_len);
  int new_size = 0;
  qsort(matches, *matches_arr_len, sizeof(Substring), SubstringComparator);
  for (int i = 0; i < *matches_arr_len; ++i) {
    if (i == 0 || matches[i].start_ >= new_array[new_size - 1].post_end_) {
      new_array[new_size++] = matches[i];
    }
  }
  for (int i = 0; i < new_size; ++i) {
    matches[i] = new_array[i];
  }
  *matches_arr_len = new_size;
  free(new_array);
}

int StringDirectMatch(const regex_t *const regex, const int regex_arr_size,
                      const char *const str, Substring *const matches,
                      int *const match_arr_size) {
  int counter = 0;
  regmatch_t reg_match[100];
  int j = 0;
  for (int i = 0; i < regex_arr_size; ++i) {
    int offset = 0;
    int exec_res = regexec(regex + i, str, 100, reg_match, 0) == 0 &&
                   reg_match->rm_so < (int)strlen(str);
    int len = strlen(str);

    if (exec_res) {
      matches[j].start_ = reg_match->rm_so + offset;
      matches[j].post_end_ = reg_match->rm_eo + offset;
      ++j;
      ++(*match_arr_size);
      counter += exec_res;
      offset += reg_match->rm_so + 1;
    }
    while (offset < len && exec_res) {
      if ((exec_res = (0 == regexec(regex + i, str + offset, 100, reg_match,
                                    REG_NOTBOL)))) {
        matches[j].start_ = reg_match->rm_so + offset;
        matches[j].post_end_ = reg_match->rm_eo + offset;
        ++j;
        ++(*match_arr_size);
        counter += exec_res;
        offset += reg_match->rm_so + 1;
      }
    }
  }
  ProccesMatchesArray(matches, match_arr_size);
  return counter != 0;
}

int StringMatch(const regex_t *const regex, const int regex_arr_size,
                const char *const str, Substring *const matches,
                const int *const args, int *const match_arr_size) {
  int direct_match =
      StringDirectMatch(regex, regex_arr_size, str, matches, match_arr_size);
  return (!args['v'] && direct_match) || (args['v'] && !direct_match);
}

void PrintMatchingString(const char *const string, const char *const file,
                         const int string_counter, const int *const args,
                         const Substring *const exact_matches, const int o_flag,
                         int match_arr_size) {
  if (o_flag) {
    for (int i = 0; i < match_arr_size; ++i) {
      int len = exact_matches[i].post_end_ - exact_matches[i].start_;
      char *substr = (char *)malloc(len + 1);
      int idx = 0;
      for (int j = exact_matches[i].start_; j != exact_matches[i].post_end_;
           ++j) {
        substr[idx++] = string[j];
      }
      substr[idx] = '\0';
      if (strlen(substr) >= 1 && substr[strlen(substr) - 1] != 10) {
        PrintMatchingString(substr, file, string_counter, args, exact_matches,
                            0, 0);
      }
      free(substr);
    }
  } else {
    if (!args['h']) {
      printf("%s:", file);
    }
    if (args['n']) {
      printf("%d:", string_counter);
    }
    printf("%s", string);
    if (string[strlen(string) - 1] != 10) {
      printf("%c", 10);
    }
  }
}

void PrintShort(const char *const file_name, int matching_strings,
                const int *const args) {
  if (args['l']) {
    printf("%s\n", file_name);
  } else if (args['c']) {
    if (!args['h']) {
      printf("%s:%d\n", file_name, matching_strings);
    } else {
      printf("%d\n", matching_strings);
    }
  }
}

void ProcessInput(int argc, const char **const argv, const int *const args,
                  const int *const files, const Patterns *const patterns) {
  int reg_compilation_error = 0;
  regex_t *reg_array = CompilePatterns(patterns, args, &reg_compilation_error);
  if (reg_compilation_error != 0) {
    printf("grep: Invalid regular expression\n");
  }
  StringBuffer str = InitString(INIT_STR_SIZE);
  for (int i = 0; i < argc && reg_compilation_error == 0; ++i) {
    int file_size = 0;
    if (files[i] && CheckFileForValdity(argv[i], args, GREP) &&
        (file_size = GetFileSize(argv[i]))) {
      FILE *f = fopen(argv[i], "r");
      int string_counter = 1;
      int matching_string_counter = 0;
      while (file_size > 0) {
        ReadString(f, &str, &file_size);
        str.array_[str.size_] = '\0';
        Substring *matching_subst = calloc(MAX_MATCH_COUNT, sizeof(Substring));
        int match_arr_size = 0;
        if (StringMatch(reg_array, patterns->size, str.array_, matching_subst,
                        args, &match_arr_size)) {
          ++matching_string_counter;
          if (!args['l'] && !args['c']) {
            PrintMatchingString(str.array_, argv[i], string_counter, args,
                                matching_subst, args['o'], match_arr_size);
          }
        }
        ++string_counter;
        free(matching_subst);
      }
      if ((args['l'] || args['c'])) {
        PrintShort(argv[i], matching_string_counter, args);
      }
      fclose(f);
    }
  }

  for (int i = 0; i < patterns->size; ++i) {
    regfree(&reg_array[i]);
  }
  free(reg_array);
  DestroyStringBuffer(&str);
}

int CountFiles(const int argc, const int *const files) {
  int counter = 0;
  for (int i = 0; i < argc; ++i) {
    counter += files[i];
  }
  return counter;
}
