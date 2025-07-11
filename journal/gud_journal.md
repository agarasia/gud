# gud Journal

This folder contains a chronological breakdown of the design, implementation, debugging, and decisions made while building the `gud` version control system.

## 📜 Journal Index

- [01 — `gud create`](#01--gud-create)
- [02 — `gud add`](#02--gud-add)
- [03 — `gud commit`](#03--gud-commit)
- [04 — `gud log`](#04--gud-log)
- [05 — Build System](#05--build-system)
- [06 — Error Logs & Fixes](#06--error-logs--fixes)

## 01 — `gud create`

### 📌 Goal

Initialize an empty `.gud/` directory structure to represent a bare repository.

### 📁 Files Created

- `.gud/HEAD` — initialized with `ref: refs/heads/master`
- `.gud/refs/heads/master` — created as an empty file
- `.gud/objects/` — created as a blob store

### 🧠 Decisions

- Use plain text HEAD like Git
- Default branch is `master`
- Follow Git-style `refs/heads/master` layout

### 🐛 Issues Encountered

- Forgot to create `.gud/refs/heads/master` → caused commit to crash
- Fixed by creating the file during `init`

## 02 — `gud add`

### 📌 Goal

Stage files by hashing their contents and saving to `.gud/index`

### 🛠 Process

- Calculate SHA-1 hash of file content
- Write blob object to `.gud/objects/<first-two-hex>/<rest>`
- Save entry `<hash> <path>` to `.gud/index`

### 🧠 Decisions

- Use header-only SHA-1 implementation for portability
- Simple line-by-line index format
- Store blobs only if not already stored

### 🐛 Issues Encountered

- SHA-1 header needed to be included in multiple files manually
- Fixed by verifying all `.cpp` files include `sha1.hpp` where necessary
- Provided stylized user messages using thematic, poetic output

## 03 — `gud commit`

### 📌 Goal

Create a snapshot of staged files and store as commit object

### 🛠 Process

- Read staged entries from `.gud/index`
- Write a tree object from file hashes
- Create commit object with:
  - Tree hash
  - Optional parent commit
  - Author, timestamp, message
- Store commit in `.gud/objects/`
- Update branch reference
- Clear index after committing

### 🧠 Decisions

- Commit format includes:
  ```
  tree <hash>
  parent <hash>
  Author: <name>, Time: <timestamp>
  Commit: <message>
  ```
- Timestamp generated using `chrono` + `ctime`
- HEAD reference resolves current branch path for update

### 🐛 Issues Encountered

- Crash due to unsafe `substr(5)` on malformed `.gud/HEAD`
- Branch ref file didn’t exist — fixed by ensuring it's created during `create`
- Commit hash printed with stylized output to enhance UX

## 04 — `gud log`

### 📌 Goal

Traverse commit history and display structured log output

### 🛠 Process

- Read current commit hash from `.gud/HEAD` → `refs/heads/master`
- Traverse each commit via the `parent` field
- Parse each commit into a map of fields (tree, parent, author, time, commit)
- Display each commit using colorized/stylized terminal output

### 🧠 Decisions

- Use ANSI escape codes for color formatting (green hash, yellow author/date, cyan parent/message)
- Ensure consistent commit format across `commit` and `log`
- Normalize commit keys during parsing to allow flexible input
- End log output with poetic closure

### 🐛 Issues Encountered

- Infinite loop caused by missing update to currentHash in loop
- Fix: assign `currentHash = fields["parent"]` in traversal
- Parsing inconsistency from early commit formatting — fixed by enforcing key normalization and expected structure

## 05 — Build System

### 📌 Goal

Setup a Makefile to build all components and support testing

### 🛠 Process

- Use `-Iinclude` to support header-only SHA-1
- Build binaries for main and all test suites
- Targets:
  - `make` to build `gud`
  - `make test` to run test suite
  - `make clean` to remove binaries, test files, and `.gud`

### 🧠 Decisions

- Compile each `.cpp` to `.o` then link into binary
- Add separate test binaries: `test_add`, `test_commit`, `test_commit_multiple`, `test_log`
- Output user-friendly results with ✅ / ❌

### 🐛 Issues Encountered

- Missing separator in Makefile from indentation issues → use TABs only
- Duplicate symbol error for functions defined in multiple places — fixed with proper header usage

## 06 — Error Logs & Fixes

### ❌ `std::out_of_range` from `ref.substr(5)`

- **Cause:** Reading `.gud/HEAD` without validating format
- **Fix:** Check with `ref.rfind("ref: ", 0) == 0` before substr

### ❌ `Failed to update branch ref`

- **Cause:** `.gud/refs/heads/master` file didn’t exist
- **Fix:** Create the file during `gud init`

### ❌ Undefined `SHA1` identifier

- **Cause:** Forgot to include `sha1.hpp`
- **Fix:** Add `#include "sha1.hpp"` in every `.cpp` that uses SHA1

### ❌ `log` command showed nothing despite valid commit

- **Cause:** Commit parser missed normalized keys / infinite loop in history walk
- **Fix:** Normalize commit keys to lowercase, update current hash during traversal
