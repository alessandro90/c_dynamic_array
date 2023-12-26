#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "maybe.h"


#define DECLARE_VECTOR(type_name, type)                                                                               \
    struct vector_##type_name {                                                                                       \
        size_t size;                                                                                                  \
        size_t capacity;                                                                                              \
        void (*free_item)(type *);                                                                                    \
        void *(*allocator)(void *, size_t);                                                                           \
        type *data;                                                                                                   \
    };                                                                                                                \
                                                                                                                      \
    DEFINE_MAYBE(vector_##type_name, struct vector_##type_name);                                                      \
                                                                                                                      \
    [[nodiscard]] struct vector_##type_name vector_##type_name##_make(void (*free_item)(type *),                      \
                                                                      void *(*allocator)(void *, size_t));            \
                                                                                                                      \
    void vector_##type_name##_init(struct vector_##type_name *vector,                                                 \
                                   void (*free_item)(type *),                                                         \
                                   void *(*allocator)(void *, size_t));                                               \
                                                                                                                      \
    [[nodiscard]] bool vector_##type_name##_is_empty(struct vector_##type_name const *vector);                        \
                                                                                                                      \
    [[nodiscard]] size_t vector_##type_name##_len(struct vector_##type_name const *vector);                           \
                                                                                                                      \
    [[nodiscard]] size_t vector_##type_name##_cap(struct vector_##type_name const *vector);                           \
                                                                                                                      \
    [[nodiscard]] type *vector_##type_name##_at(struct vector_##type_name const *vector, size_t n);                   \
                                                                                                                      \
    void vector_##type_name##_free(struct vector_##type_name *vector);                                                \
                                                                                                                      \
    bool vector_##type_name##_reserve(struct vector_##type_name *vector, size_t n);                                   \
                                                                                                                      \
    bool vector_##type_name##_clone(struct vector_##type_name *dst,                                                   \
                                    struct vector_##type_name const *src);                                            \
                                                                                                                      \
    bool vector_##type_name##_clone_deep(struct vector_##type_name *dst,                                              \
                                         struct vector_##type_name const *src,                                        \
                                         type (*copy_item)(type const *));                                            \
                                                                                                                      \
    [[nodiscard]] bool vector_##type_name##_equal(struct vector_##type_name const *left,                              \
                                                  struct vector_##type_name const *right,                             \
                                                  bool (*comp)(type const *, type const *));                          \
                                                                                                                      \
    bool vector_##type_name##_push(struct vector_##type_name *vector, type const *v);                                 \
                                                                                                                      \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *vector, type v);                               \
                                                                                                                      \
    [[nodiscard]] type vector_##type_name##_pop(struct vector_##type_name *vector);                                   \
                                                                                                                      \
    [[nodiscard]] type *vector_##type_name##_last(struct vector_##type_name *vector);                                 \
                                                                                                                      \
    [[nodiscard]] type *vector_##type_name##_data(struct vector_##type_name *vector);                                 \
                                                                                                                      \
    void vector_##type_name##_swap(struct vector_##type_name *a, struct vector_##type_name *b);                       \
                                                                                                                      \
    bool vector_##type_name##_resize(struct vector_##type_name *vector, size_t s);                                    \
                                                                                                                      \
    [[nodiscard]] type *vector_##type_name##_begin(struct vector_##type_name *vector);                                \
                                                                                                                      \
    [[nodiscard]] type *vector_##type_name##_end(struct vector_##type_name *vector);                                  \
                                                                                                                      \
    void vector_##type_name##_erase_at(struct vector_##type_name *vector, size_t n);                                  \
                                                                                                                      \
    bool vector_##type_name##_shrink(struct vector_##type_name *vector);                                              \
                                                                                                                      \
    [[nodiscard]] struct Maybe_vector_##type_name vector_##type_name##_from(struct vector_##type_name const *vector,  \
                                                                            type (*copy_item)(type const *));         \
                                                                                                                      \
    [[nodiscard]] type *vector_##type_name##_find(struct vector_##type_name *vector, bool (*is_match)(type const *)); \
                                                                                                                      \
    [[nodiscard]] struct Maybe_index vector_##type_name##_find_index(struct vector_##type_name *vector,               \
                                                                     bool (*is_match)(type const *));


#define IMPLEMENT_VECTOR(type_name, type)                                                                   \
                                                                                                            \
    void vector_##type_name##_init(struct vector_##type_name *vector,                                       \
                                   void (*free_item)(type *),                                               \
                                   void *(*allocator)(void *, size_t)) {                                    \
        vector->data = NULL;                                                                                \
        vector->size = 0;                                                                                   \
        vector->capacity = 0;                                                                               \
        vector->free_item = free_item;                                                                      \
        vector->allocator = allocator != NULL ? allocator : &realloc;                                       \
    }                                                                                                       \
                                                                                                            \
    struct vector_##type_name vector_##type_name##_make(void (*free_item)(type *),                          \
                                                        void *(*allocator)(void *, size_t)) {               \
        struct vector_##type_name v;                                                                        \
        vector_##type_name##_init(&v, free_item, allocator);                                                \
        return v;                                                                                           \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_is_empty(struct vector_##type_name const *vector) {                           \
        return vector->size == 0;                                                                           \
    }                                                                                                       \
                                                                                                            \
    [[nodiscard]] type *vector_##type_name##_at(struct vector_##type_name const *vector, size_t n) {        \
        return vector->data + n;                                                                            \
    }                                                                                                       \
                                                                                                            \
    void vector_##type_name##_free(struct vector_##type_name *vector) {                                     \
        if (vector->free_item != NULL) {                                                                    \
            for (type *item = vector_##type_name##_begin(vector);                                           \
                 item != vector_##type_name##_end(vector);                                                  \
                 ++item) {                                                                                  \
                vector->free_item(item);                                                                    \
            }                                                                                               \
        }                                                                                                   \
        free(vector->data);                                                                                 \
        vector_##type_name##_init(vector, vector->free_item, vector->allocator);                            \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_reserve(struct vector_##type_name *vector, size_t n) {                        \
        if (n == vector->size) { return true; }                                                             \
        if (n < vector->size) {                                                                             \
            if (vector->free_item != NULL) {                                                                \
                for (type *item = vector_##type_name##_begin(vector) + n;                                   \
                     item != vector_##type_name##_end(vector);                                              \
                     ++item) {                                                                              \
                    vector->free_item(item);                                                                \
                }                                                                                           \
            }                                                                                               \
        }                                                                                                   \
        void *const new_ptr = vector->allocator(vector->data, sizeof(type) * n);                            \
        if (new_ptr == NULL) {                                                                              \
            return false;                                                                                   \
        }                                                                                                   \
        vector->data = new_ptr;                                                                             \
        vector->capacity = n;                                                                               \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_clone(struct vector_##type_name *dst, struct vector_##type_name const *src) { \
        bool const ok = vector_##type_name##_reserve(dst, src->size);                                       \
        if (!ok) { return false; }                                                                          \
        memcpy(dst->data, src->data, sizeof(type) * src->size); /* NOLINT */                                \
        dst->size = src->size;                                                                              \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_clone_deep(struct vector_##type_name *dst,                                    \
                                         struct vector_##type_name const *src,                              \
                                         type (*copy_item)(type const *)) {                                 \
        if (copy_item == NULL) {                                                                            \
            return vector_##type_name##_clone(dst, src);                                                    \
        }                                                                                                   \
        bool const ok = vector_##type_name##_reserve(dst, src->size);                                       \
        if (!ok) { return false; }                                                                          \
        for (type const *item = src->data; item != src->data + src->size; ++item) {                         \
            vector_##type_name##_push_by_value(dst, copy_item(item));                                       \
        }                                                                                                   \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_equal(struct vector_##type_name const *left,                                  \
                                    struct vector_##type_name const *right,                                 \
                                    bool (*comp)(type const *a, type const *b)) {                           \
        if (left->size != right->size) { return false; }                                                    \
        for (size_t i = 0; i < left->size; ++i) {                                                           \
            if (!comp(&left->data[i], &right->data[i])) { return false; }                                   \
        }                                                                                                   \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_push(struct vector_##type_name *vector, type const *v) {                      \
        if (vector->size < vector->capacity) {                                                              \
            vector->data[vector->size] = *v;                                                                \
            ++vector->size;                                                                                 \
            return true;                                                                                    \
        }                                                                                                   \
        size_t const incr = vector->capacity == 0 ? 1U : (2 * vector->capacity);                            \
        void *const new_ptr = vector->allocator(vector->data, sizeof(type) * incr);                         \
        if (new_ptr == NULL) {                                                                              \
            return false;                                                                                   \
        }                                                                                                   \
        vector->data = new_ptr;                                                                             \
        vector->capacity = incr;                                                                            \
        vector->data[vector->size] = *v;                                                                    \
        ++vector->size;                                                                                     \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_push_by_value(struct vector_##type_name *vector, type v) {                    \
        return vector_##type_name##_push(vector, &v);                                                       \
    }                                                                                                       \
                                                                                                            \
    type vector_##type_name##_pop(struct vector_##type_name *vector) {                                      \
        --vector->size;                                                                                     \
        return vector->data[vector->size];                                                                  \
    }                                                                                                       \
                                                                                                            \
    size_t vector_##type_name##_len(struct vector_##type_name const *vector) {                              \
        return vector->size;                                                                                \
    }                                                                                                       \
                                                                                                            \
    size_t vector_##type_name##_cap(struct vector_##type_name const *vector) {                              \
        return vector->capacity;                                                                            \
    }                                                                                                       \
                                                                                                            \
    type *vector_##type_name##_last(struct vector_##type_name *vector) {                                    \
        return vector->data + vector->size - 1U;                                                            \
    }                                                                                                       \
                                                                                                            \
    type *vector_##type_name##_data(struct vector_##type_name *vector) {                                    \
        return vector->data;                                                                                \
    }                                                                                                       \
                                                                                                            \
    void vector_##type_name##_swap(struct vector_##type_name *a, struct vector_##type_name *b) {            \
        {                                                                                                   \
            type *temp = a->data;                                                                           \
            a->data = b->data;                                                                              \
            b->data = temp;                                                                                 \
        }                                                                                                   \
        {                                                                                                   \
            size_t temp = a->size;                                                                          \
            a->size = b->size;                                                                              \
            b->size = temp;                                                                                 \
        }                                                                                                   \
        {                                                                                                   \
            size_t temp = a->capacity;                                                                      \
            a->capacity = b->capacity;                                                                      \
            b->capacity = temp;                                                                             \
        }                                                                                                   \
        {                                                                                                   \
            void (*temp)(type *) = a->free_item;                                                            \
            a->free_item = b->free_item;                                                                    \
            b->free_item = temp;                                                                            \
        }                                                                                                   \
        {                                                                                                   \
            void *(*temp)(void *, size_t) = a->allocator;                                                   \
            a->allocator = b->allocator;                                                                    \
            b->allocator = temp;                                                                            \
        }                                                                                                   \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_resize(struct vector_##type_name *vector, size_t s) {                         \
        if (!vector_##type_name##_reserve(vector, s)) {                                                     \
            return false;                                                                                   \
        };                                                                                                  \
        if (s > vector->size) {                                                                             \
            memset(vector_##type_name##_end(vector), 0, sizeof(type) * (s - vector->size));                 \
        }                                                                                                   \
        vector->size = s;                                                                                   \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    type *vector_##type_name##_begin(struct vector_##type_name *vector) {                                   \
        return vector->data;                                                                                \
    }                                                                                                       \
                                                                                                            \
    type *vector_##type_name##_end(struct vector_##type_name *vector) {                                     \
        return vector->data + vector->size;                                                                 \
    }                                                                                                       \
                                                                                                            \
    void vector_##type_name##_erase_at(struct vector_##type_name *vector, size_t i) {                       \
        if (vector->size <= i) { return; }                                                                  \
        if (vector->free_item != NULL) { vector->free_item(vector->data + i); }                             \
        memmove(vector->data + i, vector->data + i + 1U, sizeof(type) * (vector->size - i - 1U));           \
        --vector->size;                                                                                     \
    }                                                                                                       \
                                                                                                            \
    bool vector_##type_name##_shrink(struct vector_##type_name *vector) {                                   \
        if (vector->size == vector->capacity) { return true; }                                              \
        void *const new_ptr = vector->allocator(vector->data, sizeof(type) * vector->size);                 \
        if (new_ptr == NULL) {                                                                              \
            return false;                                                                                   \
        }                                                                                                   \
        vector->data = new_ptr;                                                                             \
        vector->capacity = vector->size;                                                                    \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    struct Maybe_vector_##type_name vector_##type_name##_from(struct vector_##type_name const *vector,      \
                                                              type (*copy_item)(type const *)) {            \
        struct vector_##type_name w = vector_##type_name##_make(vector->free_item, vector->allocator);      \
        if (!vector_##type_name##_clone_deep(&w, vector, copy_item)) {                                      \
            return (struct Maybe_vector_##type_name){.has_value = false};                                   \
        }                                                                                                   \
        return (struct Maybe_vector_##type_name){.has_value = true, .value = w};                            \
    }                                                                                                       \
                                                                                                            \
    type *vector_##type_name##_find(struct vector_##type_name *vector, bool (*is_match)(type const *)) {    \
        for (type *item = vector_##type_name##_begin(vector);                                               \
             item != vector_##type_name##_end(vector);                                                      \
             ++item) {                                                                                      \
            if (is_match(item)) { return item; }                                                            \
        }                                                                                                   \
        return NULL;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    struct Maybe_index vector_##type_name##_find_index(struct vector_##type_name *vector,                   \
                                                       bool (*is_match)(type const *)) {                    \
        for (size_t i = 0; i < vector->size; ++i) {                                                         \
            if (is_match(&vector->data[i])) {                                                               \
                return (struct Maybe_index){.has_value = true, .value = i};                                 \
            }                                                                                               \
        }                                                                                                   \
        return (struct Maybe_index){.has_value = false};                                                    \
    }


#define DECLARE_AND_IMPLEMENT_VECTOR(type_name, type) \
    DECLARE_VECTOR(type_name, type)                   \
    IMPLEMENT_VECTOR(type_name, type)
#endif
