#ifndef VECTOR_H
#define VECTOR_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DECLARE_VECTOR(type_name, type)                                                       \
    struct vector_##type_name {                                                               \
        type *data;                                                                           \
        size_t size;                                                                          \
        size_t capacity;                                                                      \
    };                                                                                        \
                                                                                              \
    struct vector_##type_name vector_##type_name##_make(void);                                \
                                                                                              \
    void vector_##type_name##_init(struct vector_##type_name *const vector);                  \
                                                                                              \
    bool vector_##type_name##_is_empty(struct vector_##type_name const *const vector);        \
                                                                                              \
    size_t vector_##type_name##_len(struct vector_##type_name const *const vector);           \
                                                                                              \
    size_t vector_##type_name##_cap(struct vector_##type_name const *const vector);           \
                                                                                              \
    type *vector_##type_name##_at(struct vector_##type_name const *const vector, size_t n);   \
                                                                                              \
    void vector_##type_name##_free(struct vector_##type_name *const vector);                  \
                                                                                              \
    bool vector_##type_name##_reserve(struct vector_##type_name *const vector, size_t n);     \
                                                                                              \
    bool vector_##type_name##_clone(                                                          \
        struct vector_##type_name *const dst,                                                 \
        struct vector_##type_name const *const src);                                          \
                                                                                              \
    bool vector_##type_name##_equal(struct vector_##type_name const *const left,              \
                                    struct vector_##type_name const *const right);            \
                                                                                              \
    bool vector_##type_name##_push(struct vector_##type_name *const vector, type const *v);   \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *const vector, type v); \
                                                                                              \
    type *vector_##type_name##_pop(struct vector_##type_name *const vector);                  \
                                                                                              \
    type *vector_##type_name##_last(struct vector_##type_name *const vector);                 \
    type *vector_##type_name##_data(struct vector_##type_name *const vector);                 \
    void vector_##type_name##_swap(struct vector_##type_name *const a,                        \
                                   struct vector_##type_name *const b);                       \
    bool vector_##type_name##_resize(struct vector_##type_name *const vector, size_t s);      \
    type *vector_##type_name##_begin(struct vector_##type_name *const vector);                \
    type *vector_##type_name##_end(struct vector_##type_name *const vector);                  \
    void vector_##type_name##_erase_at(struct vector_##type_name *const vector, size_t n);    \
    bool vector_##type_name##_shrink(struct vector_##type_name *const vector);


#define IMPLEMENT_VECTOR(type_name, type)                                                         \
                                                                                                  \
    void vector_##type_name##_init(struct vector_##type_name *const vector) {                     \
        vector->data = NULL;                                                                      \
        vector->size = 0;                                                                         \
        vector->capacity = 0;                                                                     \
    }                                                                                             \
    struct vector_##type_name vector_##type_name##_make(void) {                                   \
        struct vector_##type_name v;                                                              \
        vector_##type_name##_init(&v);                                                            \
        return v;                                                                                 \
    }                                                                                             \
    bool vector_##type_name##_is_empty(struct vector_##type_name const *const vector) {           \
        return vector->size == 0;                                                                 \
    }                                                                                             \
    type *vector_##type_name##_at(struct vector_##type_name const *const vector, size_t n) {      \
        return vector->data + n;                                                                  \
    }                                                                                             \
    void vector_##type_name##_free(struct vector_##type_name *const vector) {                     \
        free(vector->data);                                                                       \
        vector->data = NULL;                                                                      \
        vector->size = 0;                                                                         \
        vector->capacity = 0;                                                                     \
    }                                                                                             \
    bool vector_##type_name##_reserve(struct vector_##type_name *const vector, size_t n) {        \
        void *const new_ptr = realloc(vector->data, sizeof(type) * n);                            \
        if (new_ptr == NULL) {                                                                    \
            return false;                                                                         \
        }                                                                                         \
        vector->data = new_ptr;                                                                   \
        vector->capacity = n;                                                                     \
        return true;                                                                              \
    }                                                                                             \
    bool vector_##type_name##_clone(                                                              \
        struct vector_##type_name *const dst,                                                     \
        struct vector_##type_name const *const src) {                                             \
        bool const ok = vector_##type_name##_reserve(dst, src->size);                             \
        if (!ok) { return false; }                                                                \
        memcpy(dst->data, src->data, sizeof(type) * src->size); /* NOLINT */                      \
        dst->size = src->size;                                                                    \
        return true;                                                                              \
    }                                                                                             \
    bool vector_##type_name##_equal(struct vector_##type_name const *const left,                  \
                                    struct vector_##type_name const *const right) {               \
        return left->size == right->size                                                          \
               && memcmp(left->data, right->data, left->size) == 0;                               \
    }                                                                                             \
    bool vector_##type_name##_push(struct vector_##type_name *const vector, type const *v) {      \
        if (vector->size < vector->capacity) {                                                    \
            vector->data[vector->size] = *v;                                                      \
            ++vector->size;                                                                       \
            return true;                                                                          \
        }                                                                                         \
        size_t const incr = vector->capacity == 0 ? 1U : 2 * vector->capacity;                    \
        void *const new_ptr = realloc(vector->data, sizeof(type) * incr);                         \
        if (new_ptr == NULL) {                                                                    \
            return false;                                                                         \
        }                                                                                         \
        vector->data = new_ptr;                                                                   \
        vector->data[vector->size] = *v;                                                          \
        ++vector->size;                                                                           \
        return true;                                                                              \
    }                                                                                             \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *const vector, type v) {    \
        return vector_##type_name##_push(vector, &v);                                             \
    }                                                                                             \
    type *vector_##type_name##_pop(struct vector_##type_name *const vector) {                     \
        if (vector->size == 0) {                                                                  \
            return NULL;                                                                          \
        }                                                                                         \
        --vector->size;                                                                           \
        return vector->data + vector->size;                                                       \
    }                                                                                             \
    size_t vector_##type_name##_len(struct vector_##type_name const *const vector) {              \
        return vector->size;                                                                      \
    }                                                                                             \
    size_t vector_##type_name##_cap(struct vector_##type_name const *const vector) {              \
        return vector->capacity;                                                                  \
    }                                                                                             \
    type *vector_##type_name##_last(struct vector_##type_name *const vector) {                    \
        return vector->data + vector->size - 1U;                                                  \
    }                                                                                             \
    type *vector_##type_name##_data(struct vector_##type_name *const vector) {                    \
        return vector->data;                                                                      \
    }                                                                                             \
    void vector_##type_name##_swap(struct vector_##type_name *const a,                            \
                                   struct vector_##type_name *const b) {                          \
        {                                                                                         \
            type *temp = a->data;                                                                 \
            a->data = b->data;                                                                    \
            b->data = temp;                                                                       \
        }                                                                                         \
        {                                                                                         \
            size_t temp = a->size;                                                                \
            a->size = b->size;                                                                    \
            b->size = temp;                                                                       \
        }                                                                                         \
        {                                                                                         \
            size_t temp = a->capacity;                                                            \
            a->capacity = b->capacity;                                                            \
            b->capacity = temp;                                                                   \
        }                                                                                         \
    }                                                                                             \
    bool vector_##type_name##_resize(struct vector_##type_name *const vector, size_t s) {         \
        if (s == vector->size) { return true; }                                                   \
        size_t const old_size = vector->size;                                                     \
        if (!vector_##type_name##_reserve(vector, s)) {                                           \
            return false;                                                                         \
        };                                                                                        \
        if (s > vector->size) {                                                                   \
            for (size_t i = 0; i < vector->size - s; ++i) {                                       \
                if (!vector_##type_name##_push_by_value(vector, (type){0})) {                     \
                    vector_##type_name##_reserve(vector, old_size);                               \
                    return false;                                                                 \
                }                                                                                 \
            }                                                                                     \
        }                                                                                         \
        return true;                                                                              \
    }                                                                                             \
    type *vector_##type_name##_begin(struct vector_##type_name *const vector) {                   \
        return vector->data;                                                                      \
    }                                                                                             \
    type *vector_##type_name##_end(struct vector_##type_name *const vector) {                     \
        return vector->data + vector->size;                                                       \
    }                                                                                             \
    void vector_##type_name##_erase_at(struct vector_##type_name *const vector, size_t i) {       \
        memmove(vector->data + i, vector->data + i + 1U, sizeof(type) * (vector->size - i - 1U)); \
        --vector->size;                                                                           \
    }                                                                                             \
    bool vector_##type_name##_shrink(struct vector_##type_name *const vector) {                   \
        return vector_##type_name##_reserve(vector, vector->size);                                \
    }

#endif
