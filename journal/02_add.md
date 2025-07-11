# Step 02 — `gud add`

## 📌 Goal

Stage files by hashing their contents and saving to `.gud/index`.

## 🛠 Process

- Calculate SHA-1 hash of file content.
- Write blob object to `.gud/objects/<hash>`.
- Save entry `<hash> <path>` to `.gud/index`.

## 🧠 Decisions

- Use header-only SHA-1 implementation for portability.
- Store blobs without file extensions (`.blob` removed).
- Use `IndexMap` (path → hash) for staging area.

## 🐛 Issues Encountered

- Initially, blobs for staged files were saved with `.blob` extensions inconsistently.
- The index failed to track multiple files with identical content but different paths.
- The original code prevented adding a second file with the same content hash to the index.

## ✅ Fixes

- Removed `.blob` extensions to unify object storage like Git.
- Changed `updateIndex()` to map file paths to hashes, allowing multiple paths for the same hash.
- Modified `handleAdd()` to always update the index even if the blob object already exists.
- Added clear logging when a file is already known to the object store.

## ✅ Status

`gud add` now supports staging multiple files with identical contents but different paths correctly, while storing blobs efficiently without duplication.
