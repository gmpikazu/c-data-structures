## Codebase Notes
1. returns `0` for success, non-zero or `NULL` for error case
2. supports `init-free` or `new-delete` pattern:
- `init-free`: stack allocate the `struct` then call `init`, later call `free`
- `new-delete`: single `new` returns a heap initialized container, `delete` frees
3. `const` used when element pointed-to will not be modified by function
4. `a*b > SIZE_MAX` means overflow, rearrange `a > SIZE_MAX/b`

## Implementation Plan
- [x] Dynamic Array
- [ ] Linked List
- [ ] String Builder
- [ ] Hash Map
- [ ] Binary Search Tree
- [ ] Binary Heap
- [ ] AVL Tree