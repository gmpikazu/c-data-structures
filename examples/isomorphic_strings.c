#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "../containers/map.h"

// can also use char array instead of map
bool isIsomorphic(char *s, char *t) {
    map_t* forward = map_new(7, 0.5, NULL, NULL);
    map_t* reverse = map_new(7, 0.5, NULL, NULL);
    size_t k_sz = sizeof(char), v_sz = sizeof(char);
    size_t len_s = strlen(s), len_t = strlen(t);
    if (len_s != len_t) { return false; }
    for (size_t i = 0; i < len_s; i++) {
        char* f_val = map_get(forward, &s[i], k_sz);
        char* r_val = map_get(reverse, &t[i], k_sz);
        if (f_val == NULL && r_val == NULL) {
            map_set(forward, &s[i], k_sz, &t[i], v_sz);
            map_set(reverse, &t[i], k_sz, &s[i], v_sz);
        }
        else {
            bool failed = (r_val != NULL && s[i] != *r_val) || (f_val != NULL && *f_val != t[i]);
            if (failed) {
                map_delete(forward);
                map_delete(reverse);
                return false;
            }
        }
    }
    map_delete(forward);
    map_delete(reverse);
    return true;
}

int main() {
    char *s = "egg", *t = "add";
    bool ret = isIsomorphic(s, t);
    char* ans = ret ? "true" : "false";
    printf("%s\n", ans);
}