*This activity has been created as part of the 42 curriculum by syaseen*

# get_next_line

## Description

**get_next_line** is a C function that returns one line at a time from a file descriptor. Each call to `get_next_line(fd)` returns the next line read from `fd`, including the trailing `\n` when there is one. When there is nothing left to read, or an error occurs, it returns `NULL`.

```c
char *get_next_line(int fd);
```

The goal of the project is to learn about:

- **Static variables**: keeping state between function calls.
- **File descriptors and `read()`**: reading data in fixed-size chunks.
- **Dynamic memory management**: allocating, joining and freeing buffers without leaks.
- **Buffer handling**: the function must behave correctly for any `BUFFER_SIZE` (from `1` to very large values), and the buffer size is set at compile time.

### Project structure

| File | Role |
|---|---|
| `get_next_line.c` | Core logic: `get_next_line`, `read_to_stash`, `extract_line`, `trim_stash` |
| `get_next_line.h` | Prototypes and the default `BUFFER_SIZE` |
| `get_next_line_utils.c` | Helper functions: `ft_strlen`, `ft_strchr`, `ft_memcpy`, `ft_strjoin`|

## Instructions

### Compilation

The buffer size is defined at compile time with the `-D BUFFER_SIZE=n` flag. If it is not provided, it defaults to `42`.

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

### Execution

```bash
./gnl some_file.txt
```

It can also read from standard input (`fd = 0`).

### Testing with different buffer sizes

```bash
for n in 1 2 42 9999 10000000; do
    cc -Wall -Wextra -Werror -D BUFFER_SIZE=$n get_next_line.c get_next_line_utils.c main.c -o gnl
    ./gnl some_file.txt | diff - some_file.txt && echo "BUFFER_SIZE=$n OK"
done
```

For memory checks, use `valgrind --leak-check=full ./gnl some_file.txt` or compile with `-fsanitize=address`.

### Notes

- The caller owns every returned line and must `free()` it.
- The function returns `NULL` if `fd < 0`, if `BUFFER_SIZE <= 0`, if `read()` fails, or when the end of the file is reached.
- The last line of a file may not end with `\n`; it is returned as is.

## Algorithm

### Chosen approach: a persistent *stash*

The difficulty is that `read()` returns raw chunks of `BUFFER_SIZE` bytes, which do not line up with line boundaries. A single read might contain half a line, or several lines. Any bytes read past the end of the current line must be remembered for the next call.

The solution is a `static char *stash` that persists between calls and holds data already read from the file but not yet returned. Each call to `get_next_line` goes through three steps:

1. **`read_to_stash`**: allocate a temporary buffer of `BUFFER_SIZE + 1` bytes. While the stash does not contain a `\n` and `read()` has not returned `0` (EOF), read a chunk, null-terminate it, and append it to the stash with `ft_strjoin`. Stop as soon as a newline is in the stash or EOF is reached. If `read()` returns `-1`, the buffer is freed and `NULL` is returned.
2. **`extract_line`**: copy characters from the start of the stash up to and including the first `\n` (or up to the end of the string if there is none) into a newly allocated string. If the stash is empty, there is nothing left and `NULL` is returned.
3. **`trim_stash`**: build a new stash containing only what comes after the returned line, and free the old one. If no `\n` was found, the whole stash was consumed, so it is freed and set to `NULL`.

### Why this design

- **Correct for any `BUFFER_SIZE`**: with `BUFFER_SIZE = 1` the loop simply reads byte by byte; with a huge value, one `read()` may swallow the whole file and the stash serves the following calls without touching the file descriptor again.
- **Simple and readable**: each function has one responsibility (read, extract, trim), which also keeps functions short enough for the 42 Norm.
- **No data loss**: leftover bytes after a newline stay in the stash and are never re-read or skipped.
- **One static variable only**, as the subject allows.

### Complexity

Let *L* be the length of a line and *B* the buffer size.

- Each call performs about *L / B* reads. Because `ft_strjoin` allocates a new string and copies the whole stash each time, building a line costs roughly O(*L²/B*) in the worst case. This is fine for typical text files and keeps the code simple. Appending into a growing buffer with doubling capacity would reduce this to O(*L*) at the cost of extra bookkeeping.
- `extract_line` and `trim_stash` are linear in the size of the stash.
- Memory use is O(*L* + *B*) per call, plus whatever is held in the stash.

### Trade-offs and limitations

- The mandatory part tracks a **single** static stash, so it handles one file descriptor at a time. Alternating calls on different file descriptors would mix their data. The bonus version solves this with one stash per file descriptor (for example, a `static char *stash[OPEN_MAX]` array).
- If `read()` fails, the stash should also be freed to avoid leaving allocated data behind.

## Resources

### References

- [`read(2)` manual page](https://man7.org/linux/man-pages/man2/read.2.html)
- [`malloc(3)` / `free(3)` manual page](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [Static variables in C (GeeksforGeeks)](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptors (Wikipedia)](https://en.wikipedia.org/wiki/File_descriptor)
- [Valgrind documentation](https://valgrind.org/docs/manual/manual.html)
- [AddressSanitizer documentation](https://clang.llvm.org/docs/AddressSanitizer.html)
- 42 Norm and the project subject (get_next_line)

### Use of AI

AI was used for explaining the project & writing the README.md 
