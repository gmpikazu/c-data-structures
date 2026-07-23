#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "../containers/bst.h"

bool containsDuplicate(int *nums, int numsSize) {
    // default comparator is memcmp, works for int
    bst_t* unique = bst_new(NULL);

    for (int i = 0; i < numsSize; i++) {
        int* ptr = bst_get(unique, &nums[i], sizeof(int));
        if (ptr != NULL) {
            bst_delete(unique);
            return true;
        }
        char dummy_char = '\0'; // we don't use val in this problem
        bst_insert(unique, &nums[i], sizeof(int), &dummy_char, sizeof(char));
    }
    bst_delete(unique);
    return false;
}

int main() {
    int nums[] = {1, 2, 3, 1};
    bool ret = containsDuplicate(nums, sizeof(nums)/sizeof(int));
    char* ans = ret ? "true" : "false";
    printf("%s\n", ans);
}