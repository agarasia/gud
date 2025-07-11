# Step 01 — `gud create`

## 📌 Goal

Initialize an empty `.gud/` directory structure to represent a bare repository.

## 📁 Files Created

- `.gud/HEAD` — initialized with `ref: refs/heads/master`
- `.gud/refs/heads/master` — created as an empty file
- `.gud/objects/` — created as a blob store

## 🧠 Decisions

- Used plain text HEAD like Git.
- Default branch is `master`.

## 🐛 Issues Encountered

- Forgot to create `.gud/refs/heads/master` → caused commit to fail.
- Fixed by creating the file during `init`.

## ✅ Status

Fully working.
