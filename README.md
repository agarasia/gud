# gud

`gud` is a simple version control system written in C++, inspired by Git — and the classic Dark Souls joke “git gud”.

This project is designed for learning how Git works under the hood, including hashing, object storage, staging, and eventually committing, branching, and merging.

---

## 💡 Project Goal

To build a lightweight, fully functional version control system from scratch in C++, with an architecture inspired by Git. The name "gud" is a pun on Git and the phrase "git gud" from the Soulsborne community.

---

## ✅ Features Implemented (So Far)

### ✔️ `gud create`

Initializes a new empty `gud` repository by creating the following structure:

```
.gud/
├── HEAD # Points to current branch (refs/heads/master)
├── objects/ # Stores content-addressed blobs
└── refs/
└── heads/ # Branch references
```

```bash
./gud create
```

### ✔️ `gud add` <file>

Stages a file for commit by:

1. Hashing the file content using SHA-1.

2. Storing the content in .gud/objects/<hash>.blob.

3. Tracking the file path and hash in .gud/index (staging area).

```
./gud add hello.txt
```

Blob storage path:

```
.gud/objects/ab/cdef1234567890...blob
```

## 🏗️ Project Structure

```
gud/
├── .gud/ # Created at runtime – repository metadata
├── include/ # Header files
│ ├── hash.hpp # SHA-1 hashing
│ └── index.hpp # Index (staging area) handling
├── src/ # Implementation files
│ ├── add.cpp # gud add
│ ├── commit.cpp # gud commit (stub)
│ ├── init.cpp # gud init
│ ├── hash.cpp # Implements hashing
│ └── index.cpp # Index I/O
├── main.cpp # CLI entry point and dispatcher
├── Makefile # Build configuration
└── README.md
```

## 🛠️ Build Instructions

- Requirements
- C++17 or later

GNU Make

Build

```
make
```

Run

```
./gud create
./gud add file.txt
cat .gud/index
```

## 🧠 Design Decisions

- SHA-1 is used for content-addressed storage like Git.

- Blob objects are named by their hash and stored in subdirectories for performance.

- The index is a flat file tracking <hash> <filepath>, enough for a minimal staging system.

- We use a header-only SHA-1 implementation for portability (no OpenSSL).

## 🚧 Upcoming Features

- gud commit: Create commits referencing blobs in the index.

- gud log: View commit history.

- gud checkout: Restore working directory from a specific commit.

- Branching and merging.

- Diff and status.

## 📜 License

MIT License — Free to use, learn, and improve upon.

## 🧙 Author

Aum Garasia — Software Engineering Grad @ ASU, Soulsborne fan, and Dark Souls lore enjoyer. This project is part of an effort to deepen understanding of version control systems and low-level storage.

“A file may be lost, but the hash remains eternal.”
