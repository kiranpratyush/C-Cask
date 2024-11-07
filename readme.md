# CaskDB - Bitcask-style Key-Value Store [v0.1]

A Bitcask-style key-value storage engine implemented in C. This project is a learning exercise to explore file operations, memory management, and concurrency in C.

## Current State (v0.1)
This is a basic implementation built for learning C programming and low-level system concepts. It currently supports basic read-write and compaction as outlined in the [Bitcask paper](https://riak.com/assets/bitcask-intro.pdf).

## Learning Goals
- Understand core C programming concepts.
- Learn file handling and memory management techniques.
- Implement concurrency with pthreads.
- Gain experience with network programming.

## What I’ve Learned So Far
- Handling header files and using conditional macros.
- File operations (`fopen`, `fread`, `fwrite`).
- Pointer arithmetic and memory management.
  
## Planned Improvements
- **Concurrency**: Add `pthread` support for concurrent access.
- **HTTP Server**: Implement a basic HTTP interface for client-server usage.
- **Memory Management**: Resolve memory leaks by systematically freeing allocated memory.
- **Error Handling**: Replace `assert` with robust error-handling patterns.

## Known Limitations
- Lack of proper error handling.
- Possible memory leaks (not all allocated memory is freed).
- No concurrent read/write support.

## How to Build and Run
Compile the project using GCC:
```sh
gcc -g -o cask main.c compact.c utils.c
./cask
