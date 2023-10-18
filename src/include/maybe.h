#ifndef MAYBE_H
#define MAYBE_H

#include <stdbool.h>
#include <stddef.h>

#define DEFINE_MAYBE(type_name, type) \
    struct Maybe_##type_name {        \
        bool has_value;               \
        type value;                   \
    }

DEFINE_MAYBE(index, size_t);

#endif
