#ifdef NDEBUG
#undef NDEBUG
#endif

#include "assert.h"
#include <stdint.h>
#include <stdio.h>

#include "../src/include/vector.h"

DECLARE_AND_IMPLEMENT_VECTOR(i32, int32_t)
DECLARE_AND_IMPLEMENT_VECTOR(intptr, int32_t *)
DECLARE_AND_IMPLEMENT_VECTOR(of_vectors, struct vector_i32)

static void build_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

    assert(vector_i32_is_empty(&v));
    assert(vector_i32_len(&v) == 0);
    assert(vector_i32_cap(&v) == 0);

    vector_i32_free(&v);
}

static void build_make_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);

    assert(vector_i32_is_empty(&v));
    assert(vector_i32_len(&v) == 0);
    assert(vector_i32_cap(&v) == 0);

    vector_i32_free(&v);
}

static void capacity_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

    size_t const n = 10;
    vector_i32_reserve(&v, n);

    assert(vector_i32_cap(&v) == n);

    vector_i32_free(&v);
}

// NOLINTNEXTLINE
static void push_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);

    int32_t const x = 3;
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 1);
    assert(vector_i32_cap(&v) == 1);
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 2);
    assert(vector_i32_cap(&v) == 2);
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 3);
    assert(vector_i32_cap(&v) == 4);
    vector_i32_push(&v, &x);
    assert(vector_i32_len(&v) == 4);
    assert(vector_i32_cap(&v) == 4);

    vector_i32_free(&v);
}

static void push_by_value_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

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
    vector_i32_init(&v, NULL, NULL);

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
    vector_i32_init(&v, NULL, NULL);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);

    struct vector_i32 w;
    vector_i32_init(&w, NULL, NULL);

    assert(vector_i32_clone(&w, &v));

    assert(*vector_i32_at(&v, 0) == *vector_i32_at(&w, 0));
    assert(*vector_i32_at(&v, 1) == *vector_i32_at(&w, 1));

    vector_i32_free(&v);
    vector_i32_free(&w);
}

static bool comp_i32(int32_t const *a, int32_t const *b) {
    return *a == *b;  // NOLINT
}

static void equal_test(void) {
    {
        struct vector_i32 v;
        vector_i32_init(&v, NULL, NULL);

        struct vector_i32 w;
        vector_i32_init(&w, NULL, NULL);

        vector_i32_push_by_value(&v, 1);
        vector_i32_push_by_value(&v, 2);

        assert(vector_i32_clone(&w, &v));

        assert(vector_i32_equal(&v, &w, &comp_i32));

        vector_i32_free(&v);
        vector_i32_free(&w);
    }
    {
        struct vector_i32 v;
        vector_i32_init(&v, NULL, NULL);

        struct vector_i32 w;
        vector_i32_init(&w, NULL, NULL);

        vector_i32_push_by_value(&v, 1);
        vector_i32_push_by_value(&v, 2);

        vector_i32_push_by_value(&v, -1);
        vector_i32_push_by_value(&v, -1);

        assert(!vector_i32_equal(&v, &w, &comp_i32));

        vector_i32_free(&v);
        vector_i32_free(&w);
    }
    {
        struct vector_i32 v;
        vector_i32_init(&v, NULL, NULL);

        struct vector_i32 w;
        vector_i32_init(&w, NULL, NULL);

        vector_i32_push_by_value(&v, 1);
        vector_i32_push_by_value(&v, 2);

        vector_i32_push_by_value(&v, -1);

        assert(!vector_i32_equal(&v, &w, &comp_i32));

        vector_i32_free(&v);
        vector_i32_free(&w);
    }
}

static void last_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);

    assert(*vector_i32_last(&v) == 2);

    vector_i32_free(&v);
}

static void pop_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);

    {
        int32_t const x = vector_i32_pop(&v);
        assert(x == 4);
    }
    {
        int32_t const x = vector_i32_pop(&v);
        assert(x == 3);
    }
    {
        int32_t const x = vector_i32_pop(&v);
        assert(x == 2);
    }
    {
        int32_t const x = vector_i32_pop(&v);
        assert(x == 1);
    }

    vector_i32_free(&v);
}

static void for_each_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

    int32_t const a = 1;
    int32_t const b = 2;
    int32_t const c = 3;
    int32_t const d = 4;
    vector_i32_push_by_value(&v, a);
    vector_i32_push_by_value(&v, b);
    vector_i32_push_by_value(&v, c);
    vector_i32_push_by_value(&v, d);

    int32_t sum = 0;
    for (int32_t const *item = vector_i32_begin(&v);
         item != vector_i32_end(&v);
         ++item) {
        sum += *item;
    }
    assert(sum == a + b + c + d);

    vector_i32_free(&v);
}

static void swap_test(void) {
    struct vector_i32 v;
    vector_i32_init(&v, NULL, NULL);

    struct vector_i32 v_clone;
    vector_i32_init(&v_clone, NULL, NULL);

    struct vector_i32 w;
    vector_i32_init(&w, NULL, NULL);

    struct vector_i32 w_clone;
    vector_i32_init(&w_clone, NULL, NULL);

    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);

    vector_i32_clone(&v_clone, &v);

    vector_i32_push_by_value(&w, 10);  // NOLINT
    vector_i32_push_by_value(&w, 20);  // NOLINT
    vector_i32_push_by_value(&w, 30);  // NOLINT
    vector_i32_push_by_value(&w, 40);  // NOLINT

    vector_i32_clone(&w_clone, &w);

    vector_i32_swap(&v, &w);

    assert(vector_i32_equal(&v, &w_clone, &comp_i32));
    assert(vector_i32_equal(&w, &v_clone, &comp_i32));

    vector_i32_free(&v);
    vector_i32_free(&v_clone);
    vector_i32_free(&w);
    vector_i32_free(&w_clone);
}

// NOLINTNEXTLINE
static void resize_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);
    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);

    assert(vector_i32_resize(&v, 4));

    assert(vector_i32_len(&v) == 4);
    assert(*vector_i32_at(&v, 0) == 1);
    assert(*vector_i32_at(&v, 1) == 2);
    assert(*vector_i32_at(&v, 2) == 0);
    assert(*vector_i32_at(&v, 3) == 0);

    assert(vector_i32_resize(&v, 1));
    assert(vector_i32_len(&v) == 1);
    assert(*vector_i32_at(&v, 0) == 1);

    vector_i32_free(&v);
}

// NOLINTNEXTLINE
static void shrink_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);

    assert(vector_i32_resize(&v, 10));

    assert(vector_i32_len(&v) == 10);
    assert(vector_i32_cap(&v) == 10);

    (void)vector_i32_pop(&v);
    (void)vector_i32_pop(&v);
    (void)vector_i32_pop(&v);
    (void)vector_i32_pop(&v);

    assert(vector_i32_len(&v) == 6);
    assert(vector_i32_cap(&v) == 10);

    assert(vector_i32_shrink(&v));

    assert(vector_i32_cap(&v) == 6);

    vector_i32_free(&v);
}

static void erase_at_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);
    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);

    vector_i32_erase_at(&v, 2);

    assert(vector_i32_len(&v) == 3);

    assert(*vector_i32_at(&v, 0) == 1);
    assert(*vector_i32_at(&v, 1) == 2);
    assert(*vector_i32_at(&v, 2 == 4));

    vector_i32_free(&v);
}

static void from_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);
    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);

    struct Maybe_vector_i32 maybew = vector_i32_from(&v, NULL);

    assert(maybew.has_value);
    assert(vector_i32_equal(&v, &maybew.value, &comp_i32));

    vector_i32_free(&v);
    vector_i32_free(&maybew.value);
}

static bool get_3(int32_t const *item) {
    return *item == 3;  // NOLINT
}

static bool get_100(int32_t const *item) {
    return *item == 100;  // NOLINT
}

static void find_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);
    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);

    assert(*vector_i32_find(&v, &get_3) == 3);

    assert(vector_i32_find(&v, &get_100) == NULL);
    vector_i32_free(&v);
}

static void find_index_test(void) {
    struct vector_i32 v = vector_i32_make(NULL, NULL);
    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);
    vector_i32_push_by_value(&v, 4);
    {
        struct Maybe_index const idx = vector_i32_find_index(&v, &get_3);
        assert(idx.has_value);
        assert(idx.value == 2);
    }

    {
        struct Maybe_index const idx = vector_i32_find_index(&v, &get_100);
        assert(!idx.has_value);
    }
    vector_i32_free(&v);
}


static int32_t *make_intptr(int32_t *const *i) {
    if (i == NULL || *i == NULL) { return NULL; }
    int32_t *j = malloc(sizeof(*i));
    *j = **i;
    return j;
}

static void free_i32(int32_t **i) { free(*i); }

static void clone_deep_test(void) {
    struct vector_intptr v = vector_intptr_make(&free_i32, NULL);
    {
        int32_t *i = malloc(sizeof(int32_t));
        *i = 1;
        vector_intptr_push_by_value(&v, i);
    }
    {
        int32_t *i = malloc(sizeof(int32_t));
        *i = 2;
        vector_intptr_push_by_value(&v, i);
    }

    struct vector_intptr w = vector_intptr_make(&free_i32, NULL);

    assert(vector_intptr_clone_deep(&w, &v, &make_intptr));

    assert(**vector_intptr_at(&v, 0) == **vector_intptr_at(&w, 0));
    assert(**vector_intptr_at(&v, 1) == **vector_intptr_at(&w, 1));

    vector_intptr_free(&v);
    vector_intptr_free(&w);
}

static void matrix_test(void) {
    struct vector_of_vectors m = vector_of_vectors_make(&vector_i32_free, NULL);

    struct vector_i32 v = vector_i32_make(NULL, NULL);
    vector_i32_push_by_value(&v, 1);
    vector_i32_push_by_value(&v, 2);
    vector_i32_push_by_value(&v, 3);

    vector_of_vectors_push_by_value(&m, vector_i32_make(NULL, NULL));
    // Ownership goes to vac of vec because push just make a
    // shallow copy of v
    vector_of_vectors_push_by_value(&m, v);
    vector_of_vectors_push_by_value(&m, vector_i32_make(NULL, NULL));

    assert(vector_i32_equal(vector_of_vectors_at(&m, 1), &v, &comp_i32));

    vector_of_vectors_free(&m);
}

int main(void) {
    build_test();
    build_make_test();
    push_test();
    capacity_test();
    at_test();
    push_by_value_test();
    clone_test();
    equal_test();
    last_test();
    pop_test();
    swap_test();
    for_each_test();
    resize_test();
    shrink_test();
    erase_at_test();
    from_test();
    find_test();
    find_index_test();
    clone_deep_test();
    matrix_test();
    return 0;
}
