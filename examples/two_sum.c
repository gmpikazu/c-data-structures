#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../containers/map.h"

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    assert(numsSize >= 2);
    *returnSize = 2;
    int* result = malloc(2 * sizeof(int));
    if (result == NULL) { return NULL; }
    
    // default comparator is memcmp, works for int
    map_t* complementVSindex = map_new(7, 0.5, NULL, NULL);
    if (complementVSindex == NULL) {
        free(result);
        return NULL;
    }
    size_t k_sz = sizeof(int), v_sz = sizeof(int);
    
    for (int i = 0; i < numsSize; i++){
        int complement = target - nums[i];
        int* res = map_get(complementVSindex, &complement, k_sz);
        if (res != NULL) {
            int j = *res;
            result[0] = i;
            result[1] = j;
            map_delete(complementVSindex);
            return result;
        }
        map_set(complementVSindex, &nums[i], k_sz, &i, v_sz);
    }
    map_delete(complementVSindex);
    free(result);
    return NULL;
}

int main() {
    int nums[] = {2, 7, 11, 15};
    int target = 9;
    int returnSize = 0;
    int* ret = twoSum(nums, sizeof(nums)/sizeof(int), target, &returnSize);
    printf("nums[%d]=%d + nums[%d]=%d = %d\n", 
           ret[0], nums[ret[0]], ret[1], nums[ret[1]], 
           nums[ret[0]] + nums[ret[1]]);
    free(ret);
}