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

### ✔️ `gud commit -m "message"`

Creates a new commit by:

1. Reading all staged files from .gud/index.

2. Writing a tree object representing the snapshot.

3. Creating a commit object containing:

   - Tree reference

   - Parent commit (if any)

   - Author, timestamp

   - Commit message

4. Updating the current branch (refs/heads/master) to point to the new commit.

```bash
./gud commit -m "initial commit"
```

### ✔️ `gud log`

Displays the commit history in a colorized and stylized fashion by:

1. Reading the current branch commit hash from `.gud/HEAD`.

2. Traversing commit objects backward through their `parent` field.

3. Parsing commit metadata (tree, author, time, message, parent).

4. Printing commit details with colors for easy readability:

   - Commit hash in **green**

   - Author and date in **yellow**

   - Message and parent commit in **cyan**

5. Ending with poetic closure lines to enhance User Experience.

```bash
./gud log
```

Sample output excerpt:

```
Commit: e5acaa816217a2184f58c70177994aa3452204ef
Author: Aum, Time: 2025-07-11 10:50:41
Message: Initial commit
Parent: None
----------------------------------------

```

## 🏗️ Project Structure

```
gud/
├── .gud/                 # Created at runtime – repository metadata
├── include/              # Header files
│   ├── hash.hpp          # SHA-1 hashing logic
│   ├── index.hpp         # Staging area (index) handling
│   ├── commit.hpp        # Commit logic
│   └── sha1.hpp          # Header-only SHA-1 implementation
├── src/                  # Implementation files
│   ├── create.cpp        # gud create
│   ├── add.cpp           # gud add
│   ├── commit.cpp        # gud commit
│   ├── hash.cpp          # Hashing wrappers
│   ├── index.cpp         # Index parsing/writing
│   └── log.cpp           # Commit log traversal and display
├── tests/                # Test suites
│   ├── test_add.cpp
│   ├── test_commit.cpp
│   ├── test_commit_multiple.cpp
│   └── test_log.cpp
├── main.cpp              # CLI entry point
├── Makefile              # Build and test configuration
└── README.md             # This file
```

## 🛠️ Build Instructions

- Requirements
- C++17 or later

GNU Make

Build

```
make
```

Run the program:

```bash
./gud create
./gud add file.txt
./gud commit -m "Initial commit"
./gud log
```

Run tests:

```bash
make test
```

You will see stylized pass/fail message for each test suite, for instance:

```bash
🧪 Running gud add tests...
✅ gud add passed
🧪 Running gud commit tests...
✅ gud commit passed
...

```

## 🧠 Design Decisions

- Use SHA-1 hashing for all content addressing, similar to Git.

- Store blobs under .gud/objects/ in subdirectories by first two hash characters for performance.

- The index is a simple flat file mapping `<hash> <filepath>` representing the staging area.

- Commit objects include tree hash, optional parent hash, author, timestamp, and message.

- `gud log` uses ANSI escape codes for vibrant terminal colorization and includes poetic theming for engagement.

- Commit traversal in `log` follows the parent links to show full history.

- Code organized modularly: separate headers and source files for clarity and reusability.

## 🚧 Upcoming Features

- `gud status`: See staged, modified, and untracked files

- `gud checkout` <hash>: Restore files from a specific commit

- `gud branch`, `gud merge`: Basic branching and merging support

- `gud diff`: Show file differences

## 📜 License

MIT License — Free to use, learn, and improve upon.

## 🧙 Author

Aum Garasia — Software Engineering Grad @ ASU, Soulsborne fan, and Dark Souls lore enjoyer. This project is part of an effort to deepen understanding of version control systems and low-level storage.

“A file may be lost, but the hash remains eternal.”
