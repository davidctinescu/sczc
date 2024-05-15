# SCZC (SIMPLE C ZRES COMPILER)
---
meant to replace zres (written in rust)

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