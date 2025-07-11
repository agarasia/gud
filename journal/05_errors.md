# Common Errors & Resolutions

---

## ❌ `std::out_of_range: basic_string` during commit

**Cause:**  
Used `ref.substr(5)` on `.gud/HEAD` without checking if it starts with `"ref: "`.

**Fix:**  
Added a prefix check:

```cpp
if (ref.rfind("ref: ", 0) != 0) { error... }
```

## ❌ Failed to update branch ref: .gud/refs/heads/master

**Cause:**  
`refs/heads/master` didn’t exist during `gud commit`.

**Fix:**  
Ensure file is created during gud create using:

```cpp
std::ofstream(".gud/refs/heads/master")
```

---

## 🐛 Index Handling: Missing duplicate content entries with different paths

**Symptom:**  
Adding multiple files with the same content hash resulted in only one entry appearing in `.gud/index`.

**Cause:**  
The update logic only checked if a hash already existed in the index, ignoring that multiple files can share the same hash.

**Fix:**  
Changed `updateIndex()` to use a map from file path to hash, ensuring unique file paths are tracked even if hashes duplicate.

Also ensured `handleAdd()` always updates the index for every file, regardless of whether the blob already exists.

---

## 🐛 Blob File Extension Inconsistency

**Symptom:**  
Some blob objects were stored with `.blob` extensions, others without.

**Cause:**  
Inconsistent file naming in different parts of the code.

**Fix:**  
Removed `.blob` extensions entirely to match Git-style storage, where the hash alone determines the object filename.

---
