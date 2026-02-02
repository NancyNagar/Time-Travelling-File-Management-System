# Time-Travelling File System

## Overview
This project is a simplified, in-memory version control system (like a mini Git) built in C++. It manages versioned files, branching, snapshots, and history using custom data structures:

- **Tree** (`tree.hpp`) – stores version history of a file.
- **HashMap** (`hashmap.hpp`) – maps version IDs to versions (implemented via linear probing).
- **Heap** (`heap.hpp`) – used for system-wide analytics (recent files, biggest trees).
- **FileSystem** (`filesystem.hpp`) – main API combining all operations.
- **Main** (`main.cpp`) – command-line interface.

## Compilation
Use the provided shell script:
```bash
chmod +x compile.sh
./compile.sh
```
This will build an executable named `fs.out`.

## Running
Start the program and type commands interactively:
```bash
./fs.out
```

## Commands
- `CREATE <filename>` – create new file (root version 0, initial snapshot).
- `READ <filename>` – print content of active version.
- `INSERT <filename> <content>` – append content to active version (or fork new version if snapshot).
- `UPDATE <filename> <content>` – replace content of active version.
- `SNAPSHOT <filename> <message>` – snapshot active version with message.
- `ROLLBACK <filename> [versionID]` – rollback to parent or specific version.
- `HISTORY <filename>` – list snapshots on path from active to root.
- `RECENT FILES [num]` – list most recently modified files (default 10).
- `BIGGEST TREES [num]` – list files with most versions (default 10).
- `EXIT` – exit program.

## Example Session
```
CREATE notes
INSERT notes Hello
SNAPSHOT notes First commit
INSERT notes , world!
READ notes
HISTORY notes
CREATE diary
INSERT diary Day1: Good morning
SNAPSHOT diary Morning log
INSERT diary Day1: Good night
SNAPSHOT diary Night log
READ diary
HISTORY diary
RECENT FILES 5
BIGGEST TREES 5
EXIT

```

## Error Handling
- Attempts to access non-existent files or invalid version IDs will print an error message.
- Rollback at root with no parent prints "Rollback failed".

## Notes
- All data structures are implemented manually without STL map/set/priority_queue.
- This is an in-memory system; files and history disappear once the program exits.
