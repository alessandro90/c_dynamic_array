#include "include/vector.h"

#include <stdio.h>

DECLARE_VECTOR(int, int)

IMPLEMENT_VECTOR(int, int)

int main() {
    struct vector_int v;
    vector_int_init(&v);

    vector_int_reserve(&v, 5);  // NOLINT

    int const x = 7;
    vector_int_push(&v, &x);

    vector_int_push_by_value(&v, 4);

    int s = 0;
    FOR_EACH_VECTOR_ITEM(int, int, v, item) {
        s += *item;
    }

    printf("The sum is: %d\n", s);
    vector_int_free(&v);
    return 0;
}
