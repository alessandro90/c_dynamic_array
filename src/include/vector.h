#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>  // NOLINT
#include <stddef.h>  // NOLINT
#include <stdlib.h>  // NOLINT
#include <string.h>  // NOLINT

#define DECLARE_VECTOR(type_name, type)                                                       \
    struct vector_##type_name {                                                               \
        type *data;                                                                           \
        size_t size;                                                                          \
        size_t capacity;                                                                      \
    };                                                                                        \
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
    type *vector_##type_name##_last(struct vector_##type_name *const vector);

#define FOR_EACH_VECTOR_ITEM(type_name, type, vector, item_name)     \
    for (type *item_name = vector_##type_name##_at(&vector, 0);      \
         item_name != vector_##type_name##_at(&vector, vector.size); \
         ++item_name)


#define IMPLEMENT_VECTOR(type_name, type)                                                      \
                                                                                               \
    void vector_##type_name##_init(struct vector_##type_name *const vector) {                  \
        vector->data = NULL;                                                                   \
        vector->size = 0;                                                                      \
        vector->capacity = 0;                                                                  \
    }                                                                                          \
                                                                                               \
    bool vector_##type_name##_is_empty(struct vector_##type_name const *const vector) {        \
        return vector->size == 0;                                                              \
    }                                                                                          \
                                                                                               \
    type *vector_##type_name##_at(struct vector_##type_name const *const vector, size_t n) {   \
        return vector->data + n;                                                               \
    }                                                                                          \
    void vector_##type_name##_free(struct vector_##type_name *const vector) {                  \
        free(vector->data);                                                                    \
        vector->data = NULL;                                                                   \
        vector->size = 0;                                                                      \
        vector->capacity = 0;                                                                  \
    }                                                                                          \
    bool vector_##type_name##_reserve(struct vector_##type_name *const vector, size_t n) {     \
        void *new_ptr = realloc(vector->data, sizeof(type) * n);                               \
        if (new_ptr == NULL) {                                                                 \
            return false;                                                                      \
        }                                                                                      \
        vector->data = new_ptr;                                                                \
        vector->capacity = n;                                                                  \
        return true;                                                                           \
    }                                                                                          \
    bool vector_##type_name##_clone(                                                           \
        struct vector_##type_name *const dst,                                                  \
        struct vector_##type_name const *const src) {                                          \
        bool const ok = vector_##type_name##_reserve(dst, src->size);                          \
        if (!ok) { return false; }                                                             \
        memcpy(dst->data, src->data, sizeof(type) * src->size); /* NOLINT */                   \
        dst->size = src->size;                                                                 \
        return true;                                                                           \
    }                                                                                          \
    bool vector_##type_name##_equal(struct vector_##type_name const *const left,               \
                                    struct vector_##type_name const *const right) {            \
        return left->size == right->size                                                       \
               && memcmp(left->data, right->data, left->size) == 0;                            \
    }                                                                                          \
    bool vector_##type_name##_push(struct vector_##type_name *const vector, type const *v) {   \
        if (vector->size < vector->capacity) {                                                 \
            vector->data[vector->size] = *v;                                                   \
            ++vector->size;                                                                    \
            return true;                                                                       \
        }                                                                                      \
        size_t const incr = vector->capacity == 0 ? 1U : 2 * vector->capacity;                 \
        void *new_ptr = realloc(vector->data, sizeof(type) * incr);                            \
        if (new_ptr == NULL) {                                                                 \
            return false;                                                                      \
        }                                                                                      \
        vector->data = new_ptr;                                                                \
        vector->data[vector->size] = *v;                                                       \
        ++vector->size;                                                                        \
        return true;                                                                           \
    }                                                                                          \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *const vector, type v) { \
        return vector_##type_name##_push(vector, &v);                                          \
    }                                                                                          \
    type *vector_##type_name##_pop(struct vector_##type_name *const vector) {                  \
        if (vector->size == 0) {                                                               \
            return NULL;                                                                       \
        }                                                                                      \
        --vector->size;                                                                        \
        return vector->data + vector->size;                                                    \
    }                                                                                          \
    size_t vector_##type_name##_len(struct vector_##type_name const *const vector) {           \
        return vector->size;                                                                   \
    }                                                                                          \
                                                                                               \
    size_t vector_##type_name##_cap(struct vector_##type_name const *const vector) {           \
        return vector->capacity;                                                               \
    }                                                                                          \
    type *vector_##type_name##_last(struct vector_##type_name *const vector) {                 \
        return vector->data + vector->size - 1U;                                               \
    }


#endif
