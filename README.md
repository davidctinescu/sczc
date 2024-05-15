# SCZC (SIMPLE C ZRES COMPILER)
---
THIS FOLLOWS GENERAL DEFINITIONS OF A COMPILER!

meant to replace zres (written in rust)
compiles to intermediate represantation (C code) and uses GCC to produce the file executable.

please make sure to have `make` installed and run, to compile:
```sh
make clean all
```
dependencies:
`gcc make`

---
current directory structure:
```
main_dir/
│
├── Makefile
├── out/
└── src/
    ├── main.c
    └── libs/
        └── reader.h
```
run using `make all` then use `./out/sczc <input_file>`

---
currently introduced file reading and basic tokenization