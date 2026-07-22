## Codebase Notes
0. `struct_free()` frees internal members, `struct_delete()` also frees the struct itself
1. returns `0` for success, non-zero or `NULL` for error case
2. supports `init-free` or `new-delete` pattern:
- `init-free`: stack allocate the `struct` then call `init`, later call `free`
- `new-delete`: single `new` returns a heap initialized container, `delete` frees
3. `const` used when element pointed-to will not be modified by function
4. `a*b > SIZE_MAX` means overflow, rearrange `a > SIZE_MAX/b`
5. value in `data` pointer is copied into the container's `data` field
6. set pointer members to `NULL` after calling `container_free()` to prevent double free

## Be Careful
- Does shallow copy run into problems?

## Implementation Plan
- [x] Dynamic Array
- [ ] Singly Linked List
- [ ] String Builder
- [ ] Hash Map
- [ ] Binary Search Tree
- [ ] Binary Heap
- [ ] AVL Tree