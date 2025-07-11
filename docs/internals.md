# Internal Design of gud

## Object Types

- **Blob**: Stores file contents. Named by SHA-1 hash.
- **Tree**: Snapshot of staged files (`blob <hash> <path>`).
- **Commit**: Contains tree reference, parent commit, author, and message.

## Directory Structure

```
.gud/
├── objects/ # Stores blobs, trees, commits
├── index # Staging area
├── HEAD # Points to current branch
└── refs/heads/ # Branch commit pointers
```
