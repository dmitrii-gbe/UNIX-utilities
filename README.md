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
This C program is an implementation of a **cat**-like utility with additional features controlled by various flags. Below is a breakdown of the **supported flags** based on how they are processed in `GetArguments()` and used throughout the program.

---

## **Supported Flags and Their Behavior**

| **Flag** | **Long Option**         | **Description** |
|---------|--------------------------|----------------|
| **`-b`** | `--number-nonblank`  | Number only non-blank lines, overriding `-n`. |
| **`-e`** | *(No long equivalent)* | Display `$` at the end of each line and enable `-v` (non-printable character visibility). |
| **`-E`** | *(No long equivalent)* | Equivalent to `-e`. |
| **`-n`** | `--number` | Number all lines, including empty ones. |
| **`-s`** | `--squeeze-blank` | Suppress repeated empty lines (only one empty line is shown). |
| **`-t`** | *(No long equivalent)* | Show tab characters as `^I` and enable `-v`. |
| **`-T`** | *(No long equivalent)* | Equivalent to `-t`. |
| **`-v`** | *(No long equivalent)* | Show non-printable characters using caret notation (e.g., `^A` for ASCII 1). |

---

## **Implementation Details**
- **Argument Parsing**
  - Iterates through `argv[]`, checking for flags.
  - Flags are stored in an `arguments[]` array indexed by their ASCII values.
  - Some flags automatically enable others:
    - `-e` and `-t` enable `-v`.
    - `-A` enables `-v`, `-E`, and `-T`.
    - `-b` disables `-n`.
    - `-E` is treated as `-e`, and `-T` is treated as `-t`.

- **Processing Input**
  - Reads input from files or `stdin` if `-` is used as a filename.
  - Uses `squeeze-blank` mode (`-s`) to filter consecutive blank lines.
  - Calls `ProcessString()` to apply transformations and print formatted output.

- **Handling Special Characters**
  - If `-t` is enabled: Converts tabs (`\t`) into `^I`.
  - If `-e` is enabled: Appends `$` at the end of lines.
  - If `-v` is enabled: Converts non-printable ASCII characters (0-31, 127-255) to caret notation (`^A`, `M-^?`, etc.).

- **Line Numbering**
  - If `-b` is enabled, it numbers only non-empty lines.
  - If `-n` is enabled, it numbers all lines.
  - The numbering format is right-aligned with a tab separator.

---

## **Example Usage**
```sh
./cat -n file.txt       # Number all lines
./cat -b file.txt       # Number non-blank lines
./cat -e file.txt       # Show end-of-line markers ($) and non-printable chars
./cat -s file.txt       # Squeeze multiple empty lines
./cat -t file.txt       # Show tabs as ^I
```

Would you like a README file or man-page-style documentation for this? 🚀

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
