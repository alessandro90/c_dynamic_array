#ifndef VECTOR_H
#define VECTOR_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DECLARE_VECTOR(type_name, type)                                                           \
    struct vector_##type_name {                                                                   \
        type *data;                                                                               \
        size_t size;                                                                              \
        size_t capacity;                                                                          \
        void (*free_item)(type *);                                                                \
    };                                                                                            \
                                                                                                  \
    struct vector_##type_name vector_##type_name##_make(void (*free_item)(type *));               \
                                                                                                  \
    void vector_##type_name##_init(struct vector_##type_name *vector, void (*free_item)(type *)); \
                                                                                                  \
    bool vector_##type_name##_is_empty(struct vector_##type_name const *vector);                  \
                                                                                                  \
    size_t vector_##type_name##_len(struct vector_##type_name const *vector);                     \
                                                                                                  \
    size_t vector_##type_name##_cap(struct vector_##type_name const *vector);                     \
                                                                                                  \
    type *vector_##type_name##_at(struct vector_##type_name const *vector, size_t n);             \
                                                                                                  \
    void vector_##type_name##_free(struct vector_##type_name *vector);                            \
                                                                                                  \
    bool vector_##type_name##_reserve(struct vector_##type_name *vector, size_t n);               \
                                                                                                  \
    bool vector_##type_name##_clone(                                                              \
        struct vector_##type_name *dst,                                                           \
        struct vector_##type_name const *src);                                                    \
                                                                                                  \
    bool vector_##type_name##_equal(struct vector_##type_name const *left,                        \
                                    struct vector_##type_name const *right);                      \
                                                                                                  \
    bool vector_##type_name##_push(struct vector_##type_name *vector, type const *v);             \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *vector, type v);           \
                                                                                                  \
    type *vector_##type_name##_pop(struct vector_##type_name *vector);                            \
                                                                                                  \
    type *vector_##type_name##_last(struct vector_##type_name *vector);                           \
    type *vector_##type_name##_data(struct vector_##type_name *vector);                           \
    void vector_##type_name##_swap(struct vector_##type_name *a,                                  \
                                   struct vector_##type_name *b);                                 \
    bool vector_##type_name##_resize(struct vector_##type_name *vector, size_t s);                \
    type *vector_##type_name##_begin(struct vector_##type_name *vector);                          \
    type *vector_##type_name##_end(struct vector_##type_name *vector);                            \
    void vector_##type_name##_erase_at(struct vector_##type_name *vector, size_t n);              \
    bool vector_##type_name##_shrink(struct vector_##type_name *vector);


#define IMPLEMENT_VECTOR(type_name, type)                                                          \
                                                                                                   \
    void vector_##type_name##_init(struct vector_##type_name *vector, void (*free_item)(type *)) { \
        vector->data = NULL;                                                                       \
        vector->size = 0;                                                                          \
        vector->capacity = 0;                                                                      \
        vector->free_item = free_item;                                                             \
    }                                                                                              \
    struct vector_##type_name vector_##type_name##_make(void (*free_item)(type *)) {               \
        struct vector_##type_name v;                                                               \
        vector_##type_name##_init(&v, free_item);                                                  \
        return v;                                                                                  \
    }                                                                                              \
    bool vector_##type_name##_is_empty(struct vector_##type_name const *vector) {                  \
        return vector->size == 0;                                                                  \
    }                                                                                              \
    type *vector_##type_name##_at(struct vector_##type_name const *vector, size_t n) {             \
        return vector->data + n;                                                                   \
    }                                                                                              \
    void vector_##type_name##_free(struct vector_##type_name *vector) {                            \
        if (vector->free_item != NULL) {                                                           \
            for (type *item = vector_##type_name##_begin(vector);                                  \
                 item != vector_##type_name##_end(vector);                                         \
                 ++item) {                                                                         \
                vector->free_item(item);                                                           \
            }                                                                                      \
        }                                                                                          \
        free(vector->data);                                                                        \
        vector->data = NULL;                                                                       \
        vector->size = 0;                                                                          \
        vector->capacity = 0;                                                                      \
    }                                                                                              \
    bool vector_##type_name##_reserve(struct vector_##type_name *vector, size_t n) {               \
        if (n == vector->size) { return true; }                                                    \
        if (n < vector->size) {                                                                    \
            if (vector->free_item != NULL) {                                                       \
                for (type *item = vector_##type_name##_begin(vector) + n;                          \
                     item != vector_##type_name##_end(vector);                                     \
                     ++item) {                                                                     \
                    vector->free_item(item);                                                       \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        void *const new_ptr = realloc(vector->data, sizeof(type) * n);                             \
        if (new_ptr == NULL) {                                                                     \
            return false;                                                                          \
        }                                                                                          \
        vector->data = new_ptr;                                                                    \
        vector->capacity = n;                                                                      \
        return true;                                                                               \
    }                                                                                              \
    bool vector_##type_name##_clone(                                                               \
        struct vector_##type_name *dst,                                                            \
        struct vector_##type_name const *src) {                                                    \
        bool const ok = vector_##type_name##_reserve(dst, src->size);                              \
        if (!ok) { return false; }                                                                 \
        memcpy(dst->data, src->data, sizeof(type) * src->size); /* NOLINT */                       \
        dst->size = src->size;                                                                     \
        return true;                                                                               \
    }                                                                                              \
    bool vector_##type_name##_equal(struct vector_##type_name const *left,                         \
                                    struct vector_##type_name const *right) {                      \
        return left->size == right->size                                                           \
               && memcmp(left->data, right->data, left->size) == 0;                                \
    }                                                                                              \
    bool vector_##type_name##_push(struct vector_##type_name *vector, type const *v) {             \
        if (vector->size < vector->capacity) {                                                     \
            vector->data[vector->size] = *v;                                                       \
            ++vector->size;                                                                        \
            return true;                                                                           \
        }                                                                                          \
        size_t const incr = vector->capacity == 0 ? 1U : (2 * vector->capacity);                   \
        void *const new_ptr = realloc(vector->data, sizeof(type) * incr);                          \
        if (new_ptr == NULL) {                                                                     \
            return false;                                                                          \
        }                                                                                          \
        vector->data = new_ptr;                                                                    \
        vector->capacity = incr;                                                                   \
        vector->data[vector->size] = *v;                                                           \
        ++vector->size;                                                                            \
        return true;                                                                               \
    }                                                                                              \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *vector, type v) {           \
        return vector_##type_name##_push(vector, &v);                                              \
    }                                                                                              \
    type *vector_##type_name##_pop(struct vector_##type_name *vector) {                            \
        if (vector->size == 0) {                                                                   \
            return NULL;                                                                           \
        }                                                                                          \
        --vector->size;                                                                            \
        return vector->data + vector->size;                                                        \
    }                                                                                              \
    size_t vector_##type_name##_len(struct vector_##type_name const *vector) {                     \
        return vector->size;                                                                       \
    }                                                                                              \
    size_t vector_##type_name##_cap(struct vector_##type_name const *vector) {                     \
        return vector->capacity;                                                                   \
    }                                                                                              \
    type *vector_##type_name##_last(struct vector_##type_name *vector) {                           \
        return vector->data + vector->size - 1U;                                                   \
    }                                                                                              \
    type *vector_##type_name##_data(struct vector_##type_name *vector) {                           \
        return vector->data;                                                                       \
    }                                                                                              \
    void vector_##type_name##_swap(struct vector_##type_name *a,                                   \
                                   struct vector_##type_name *b) {                                 \
        {                                                                                          \
            type *temp = a->data;                                                                  \
            a->data = b->data;                                                                     \
            b->data = temp;                                                                        \
        }                                                                                          \
        {                                                                                          \
            size_t temp = a->size;                                                                 \
            a->size = b->size;                                                                     \
            b->size = temp;                                                                        \
        }                                                                                          \
        {                                                                                          \
            size_t temp = a->capacity;                                                             \
            a->capacity = b->capacity;                                                             \
            b->capacity = temp;                                                                    \
        }                                                                                          \
    }                                                                                              \
    bool vector_##type_name##_resize(struct vector_##type_name *vector, size_t s) {                \
        if (!vector_##type_name##_reserve(vector, s)) {                                            \
            return false;                                                                          \
        };                                                                                         \
        if (s > vector->size) {                                                                    \
            memset(vector_##type_name##_end(vector), 0, sizeof(type) * (s - vector->size));        \
            vector->size = s;                                                                      \
        }                                                                                          \
        return true;                                                                               \
    }                                                                                              \
    type *vector_##type_name##_begin(struct vector_##type_name *vector) {                          \
        return vector->data;                                                                       \
    }                                                                                              \
    type *vector_##type_name##_end(struct vector_##type_name *vector) {                            \
        return vector->data + vector->size;                                                        \
    }                                                                                              \
    void vector_##type_name##_erase_at(struct vector_##type_name *vector, size_t i) {              \
        if (vector->free_item != NULL) { vector->free_item(vector->data + i); }                    \
        memmove(vector->data + i, vector->data + i + 1U, sizeof(type) * (vector->size - i - 1U));  \
        --vector->size;                                                                            \
    }                                                                                              \
    bool vector_##type_name##_shrink(struct vector_##type_name *vector) {                          \
        return vector_##type_name##_reserve(vector, vector->size);                                 \
    }

#endif
