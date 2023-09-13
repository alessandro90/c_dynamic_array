#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>  // NOLINT
#include <stddef.h>  // NOLINT
#include <stdlib.h>  // NOLINT
#include <string.h>  // NOLINT

#define DECLARE_VECTOR(type)                                                        \
    struct vector_##type;                                                           \
                                                                                    \
    void vector_##type##_init(struct vector_##type *const vector);                  \
                                                                                    \
    bool vector_##type##_is_empty(struct vector_##type const *const vector);        \
                                                                                    \
    type *vector_##type##_at(struct vector_##type const *const vector, size_t n);   \
                                                                                    \
    void vector_##type##_free(struct vector_##type *const vector);                  \
                                                                                    \
    bool vector_##type##_reserve(struct vector_##type *const vector, size_t n);     \
                                                                                    \
    bool vector_##type##_clone(                                                     \
        struct vector_##type *const dst,                                            \
        struct vector_##type const *const src);                                     \
                                                                                    \
    bool vector_##type##_equal(struct vector_##type const *const left,              \
                               struct vector_##type const *const right);            \
                                                                                    \
    bool vector_##type##_push(struct vector_##type *const vector, type const *v);   \
    bool vector_##type##_push_by_value(struct vector_##type *const vector, type v); \
                                                                                    \
    type *vector_##type##_pop(struct vector_##type *const vector);                  \
                                                                                    \
    type *vector_##type##_last(struct vector_##type *const vector);

#define FOR_EACH_VECTOR_ITEM(type, vector, item_name)           \
    for (type *item_name = vector_##type##_at(&vector, 0);      \
         item_name != vector_##type##_at(&vector, vector.size); \
         ++item_name)


#define IMPLEMENT_VECTOR(type)                                                       \
    struct vector_##type {                                                           \
        type *data;                                                                  \
        size_t size;                                                                 \
        size_t capacity;                                                             \
    };                                                                               \
                                                                                     \
    void vector_##type##_init(struct vector_##type *const vector) {                  \
        vector->data = NULL;                                                         \
        vector->size = 0;                                                            \
        vector->capacity = 0;                                                        \
    }                                                                                \
                                                                                     \
    bool vector_##type##_is_empty(struct vector_##type const *const vector) {        \
        return vector->size == 0;                                                    \
    }                                                                                \
                                                                                     \
    type *vector_##type##_at(struct vector_##type const *const vector, size_t n) {   \
        return vector->data + n;                                                     \
    }                                                                                \
    void vector_##type##_free(struct vector_##type *const vector) {                  \
        free(vector->data);                                                          \
        vector->data = NULL;                                                         \
        vector->size = 0;                                                            \
        vector->capacity = 0;                                                        \
    }                                                                                \
    bool vector_##type##_reserve(struct vector_##type *const vector, size_t n) {     \
        void *new_ptr = realloc(vector->data, n);                                    \
        if (new_ptr == NULL) {                                                       \
            return false;                                                            \
        }                                                                            \
        vector->data = new_ptr;                                                      \
        vector->capacity = n;                                                        \
        return true;                                                                 \
    }                                                                                \
    bool vector_##type##_clone(                                                      \
        struct vector_##type *const dst,                                             \
        struct vector_##type const *const src) {                                     \
        bool const ok = vector_##type##_reserve(dst, src->size);                     \
        if (!ok) { return false; }                                                   \
        memcpy(dst->data, src->data, src->size);                                     \
        dst->size = src->size;                                                       \
        dst->capacity = src->size;                                                   \
        return true;                                                                 \
    }                                                                                \
    bool vector_##type##_equal(struct vector_##type const *const left,               \
                               struct vector_##type const *const right) {            \
        return left->size == right->size                                             \
               && memcmp(left->data, right->data, left->size) == 0;                  \
    }                                                                                \
    bool vector_##type##_push(struct vector_##type *const vector, type const *v) {   \
        if (vector->size < vector->capacity) {                                       \
            vector->data[vector->size] = *v;                                         \
            ++vector->size;                                                          \
            return true;                                                             \
        }                                                                            \
        size_t const incr = vector->capacity == 0 ? 1U : 2 * vector->capacity;       \
        void *new_ptr = realloc(vector->data, incr);                                 \
        if (new_ptr == NULL) {                                                       \
            return false;                                                            \
        }                                                                            \
        vector->data = new_ptr;                                                      \
        vector->data[vector->size] = *v;                                             \
        ++vector->size;                                                              \
        return true;                                                                 \
    }                                                                                \
    bool vector_##type##_push_by_value(struct vector_##type *const vector, type v) { \
        return vector_##type##_push(vector, &v);                                     \
    }                                                                                \
    type *vector_##type##_pop(struct vector_##type *const vector) {                  \
        if (vector->size == 0) {                                                     \
            return NULL;                                                             \
        }                                                                            \
        --vector->size;                                                              \
        return vector->data + vector->size;                                          \
    }

#endif
