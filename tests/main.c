#include "assert.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../src/include/vector.h"

DECLARE_VECTOR(i32, int32_t)

IMPLEMENT_VECTOR(i32, int32_t)

static void build_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    assert(vector_i32_is_empty(&v));
    assert(vector_i32_is_empty(&v));
    assert(vector_i32_len(&v) == 0);
    assert(vector_i32_cap(&v) == 0);

    vector_i32_free(&v);
}

static void push_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    int32_t const x = 3;
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 1);
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 2);
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 3);
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 4);

    vector_i32_free(&v);
}

static void push_by_value_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);

    {
        int32_t const *x = vector_i32_at(&v, 0);
        assert(*x == 1);
    }
    {
        int32_t const *x = vector_i32_at(&v, 1);
        assert(*x == 2);
    }
    {
        int32_t const *x = vector_i32_at(&v, 2);
        assert(*x == 3);
    }

    vector_i32_free(&v);
}

static void at_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    {
        int32_t const x = 1;
        vector_i32_push(&v, &x);
    }
    {
        int32_t const x = 2;
        vector_i32_push(&v, &x);
    }
    {
        int32_t const x = 3;
        vector_i32_push(&v, &x);
    }
    {
        int32_t const *const x = vector_i32_at(&v, 0);
        assert(*x == 1);
    }
    {
        int32_t const *const x = vector_i32_at(&v, 1);
        assert(*x == 2);
    }
    {
        int32_t const *const x = vector_i32_at(&v, 2);
        assert(*x == 3);
    }

    vector_i32_free(&v);
}

static void clone_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);

    struct vector_i32 w;
    vector_i32_init(&w);

    assert(vector_i32_clone(&w, &v));

    assert(*vector_i32_at(&v, 0) == *vector_i32_at(&w, 0));
    assert(*vector_i32_at(&v, 1) == *vector_i32_at(&w, 1));

    vector_i32_free(&v);
    vector_i32_free(&w);
}

static void equal_test(void) {
    {
        struct vector_i32 v;
        vector_i32_init(&v);

        struct vector_i32 w;
        vector_i32_init(&w);

        vector_i32_push_by_value(&v, 1);
        vector_i32_push_by_value(&v, 2);

        assert(vector_i32_clone(&w, &v));

        assert(vector_i32_equal(&v, &w));

        vector_i32_free(&v);
        vector_i32_free(&w);
    }
    {
        struct vector_i32 v;
        vector_i32_init(&v);

        struct vector_i32 w;
        vector_i32_init(&w);

        vector_i32_push_by_value(&v, 1);
        vector_i32_push_by_value(&v, 2);

        vector_i32_push_by_value(&v, -1);
        vector_i32_push_by_value(&v, -1);

        assert(!vector_i32_equal(&v, &w));

        vector_i32_free(&v);
        vector_i32_free(&w);
    }
    {
        struct vector_i32 v;
        vector_i32_init(&v);

        struct vector_i32 w;
        vector_i32_init(&w);

        vector_i32_push_by_value(&v, 1);
        vector_i32_push_by_value(&v, 2);

        vector_i32_push_by_value(&v, -1);

        assert(!vector_i32_equal(&v, &w));

        vector_i32_free(&v);
        vector_i32_free(&w);
    }
}

static void last_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);

    assert(*vector_i32_last(&v) == 2);

    vector_i32_free(&v);
}

static void pop_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);

    {
        int32_t const *x = vector_i32_pop(&v);
        assert(*x == 4);
    }
    {
        int32_t const *x = vector_i32_pop(&v);
        assert(*x == 3);
    }
    {
        int32_t const *x = vector_i32_pop(&v);
        assert(*x == 2);
    }
    {
        int32_t const *x = vector_i32_pop(&v);
        assert(*x == 1);
    }

    vector_i32_free(&v);
}

static void for_each_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v);

    int32_t const a = 1;
    int32_t const b = 2;
    int32_t const c = 3;
    int32_t const d = 4;
    vector_i32_push_by_value(&v, a);
    vector_i32_push_by_value(&v, b);
    vector_i32_push_by_value(&v, c);
    vector_i32_push_by_value(&v, d);

    int32_t sum = 0;
    FOR_EACH_VECTOR_ITEM(i32, int32_t const, v, item) {
        sum += *item;
    }
    assert(sum == a + b + c + d);

    vector_i32_free(&v);
}

int main(void) {
    build_test();
    push_test();
    at_test();
    push_by_value_test();
    clone_test();
    equal_test();
    last_test();
    pop_test();
    for_each_test();
    return 0;
}