# COP 4610: Project 3

- Group members:
  - Emmanuel Gonzalez
  - John Sanchez
  - Andrew Stringfellow

## Archive Contents

Filename: `proj_3_gonzalez_sanchez_stringfellow.tar`

```
.
├── README.md // Markdown file containing documentation of our implementation of the assignment.
├── Makefile // Compiles the source code into a binary.
├── include
│   ├── FATImage.h // Declarations of the FAT32 file system reserved region fields.
│   └── filetable.h // Declarations of the file allocation table and its helper functions.
|   └── dirparse.h // Declarations for directory parsing/editing functions.
|   └── parse.h // Declarations for string conversion functions.
|   └── utils.h // Declarations of utility functions.
└── src
    ├── Shell.c // The shell utility which acts as a terminal and handles the user's commands.
    └── filetable.c // Definitions of the file allocation table helper functions.
    └── dirparse.c // Helper functions to edit/view directories / display contents.
    └── parse.c // Converts between standard format strings and FAT32 style strings.
    └── utils.c // Functions for finding free directory entries, clusters, etc.
```

## Build Instructions

## Known Bugs and Unfinished Portions

## Division of Labor
- Emmanuel Gonzalez
- John Sanchez
- Andrew Stringfellow
