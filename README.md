# UNIX Utilities

This repository contains C implementations of two fundamental UNIX utilities: `grep` and `cat`.

## Overview

- **`grep`**: Searches for patterns within files and outputs the matching lines.
- **`cat`**: Concatenates and displays the contents of files.

## Features

- **Pattern Matching**: Supports basic regular expressions for flexible searching.
- **File Handling**: Manages multiple files and standard input seamlessly.
- **Command-Line Options**: Includes standard flags to modify utility behavior.

## Installation

To compile the utilities, ensure you have a C compiler installed, then run:

```sh
make
```


This will generate executable binaries for `grep` and `cat`.

## Usage

**grep**

```sh
./grep [OPTIONS] PATTERN [FILE...]
```


- `PATTERN`: The regular expression to search for.
- `FILE`: One or more files to search. If no files are specified, standard input is used.

### **Supported Flags:**
1. **`-e`** → Define a search pattern (explicitly specified in the command line).
2. **`-f`** → Read patterns from a file.
3. **`-i`** → Case-insensitive search.
4. **`-v`** → Invert match (select lines that do not match).
5. **`-c`** → Count matching lines instead of printing them.
6. **`-n`** → Prefix output lines with their line numbers.
7. **`-h`** → Suppress filenames in output (used when multiple files are provided).
8. **`-l`** → Only print filenames that contain matches.
9. **`-o`** → Print only the matched parts of lines.

**cat**

```sh
./cat [OPTIONS] [FILE...]
```


- `FILE`: One or more files to display. If no files are specified, standard input is used.

## Examples

- **grep**: Search for the word "example" in `file.txt`:

  ```sh
  ./grep "example" file.txt
  ```


- **cat**: Display the contents of `file.txt`:

  ```sh
  ./cat file.txt
  ```


## License

This project is licensed under the MIT License. 
