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

## 🐛 Commit and tree objects were saved with `.blob` extensions

**Fix:**
Fixed by writing files with no extension - hash alone determines object type.

## 🐛 `.blob` extension was inconsistently added to some object files

**Fix:**
Fixed by ensuring all object files (blobs, trees, commits) are stored with just the SHA-1 hash and no extension, Git-style.
