# Dynamic  array in C

Implementation of a dynamic array in the C programming language.
C23 standard is required.

## API

It is just one single macro (2 macros actually, one for a declaration and one for a definition) of a C++ std::vector-like object. Each macro call defines a new vector type.

### Exmaples

To declare a new vector type containing doubles:

```c
DECLARE_VECTOR(f64, double)
```

Define the same vector, that is, get all the function implementations:

```c
IMPLEMENT_VECTOR(f64, double)
```

There is macro to do both:

```c
DECLARE_AND_IMPLEMENT_VECTOR(f64, double)
```

The result is a new `struct` called `vector_f64`. All the functions for this type begin with the type name. For example

```c
struct vector_f64 v = vector_f64_make(NULL); // Argument is a function to free the vector elements or NULL if there is nothing to free

// Reserve space for 10 doubles
vector_f64_reserve(&v, 10);

// Push items (by pointer to avoid copies or directly by value)
double const x = 1.;
vector_f64_push(&v, &x);
vector_f64_push_by_value(&v, x);

// Pop an item. Note that popping from an empty vector is UB
double const j = vector_f64_pop(&v);

// Iterator-like for loop
for (double *it = vector_f64_begin(&v); it != vector_f64_end(&v); ++it) {
    // ...
}

// Element accessor (get a pointer, vector still owns the memory)
double *w = vector_f64_at(0);

// other functions
// equal compares all the elements
// comp is a function of the type bool(*)(type const*, type const*) use to compare the elements
bool vector_f64_equal(&v1, &v2, &comp);

// Create a deep copy from a vector (the maybe type is declared alongside the vector)
// The second argument is a pointer to function for the deep copy of the items.
// If NULL the items are copied with memcpy
Maybe_vector_f64 const mv = vector_f64_from(&v, NULL);
if (ms.has_value) {
    // do stuff with
    mv.value; // == vector_f64
}

// Find a pointer to a given element satisfying a predicate or get NULL
double const *p = vector_f64_find(&v, &greater_then_3); // greater_than_3 :: double const* -> bool

// Find the index of a given element satisfying a predicate
Maybe_index const mi = vector_f64_find_index(&v, &greater_then_3); // greater_than_3 :: double const* -> bool

// Erase the n-th element and rearrange the items accordingly
vector_f64_erase_at(&v, 2);

// Free the memory
vector_64_free(&v);
```

## Tests build

With CMake (version >=3.21 required). Tests are built with simple _asserts_ without any dependency.

```shell
mkdir build && cd build
make test && ctest --rerun-failed --output-on-failure
```
