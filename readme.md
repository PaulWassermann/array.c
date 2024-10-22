# array.c

This project is about building a scientific computing library, mimicking 
[numpy](https://numpy.org/), only with minimal functionality ! Just enough to 
ultimately power [deep_learner](https://github.com/PaulWassermann/deep-learner).

## Core concept

The package revolves around a single construct : the `Array` type. It 
basically is a fat pointer, storing a contiguous array of data, as well as 
characteristics such as the shape, the strides, and all the info required to
correctly "interpret" the array.

```c
typedef struct Array {
    void *data;
    size_t *shape;
    size_t *strides;
    size_t ndim;
    size_t nelems;
    DType dtype;
} Array;
```

An array holds its `dtype` as a member of the struct, allowing for some sort of
""""generic programming"""" (notice the quotes): there is only one type of 
`Array` (as opposed to having types like `ArrayFloat32`, `ArrayInt8`, etc) but 
at the price of having to cast the `data` pointer each time its pointed memory 
must be accessed.

To access a particular element using numpy-like <u>array indexing</u>, the 
`array_index_to_memory` function can be used; its companion function, 
`memory_index_to_array`, on the other hand, allows to reconstruct the numpy-like
array index from the bytes offset. A macro will soon be added to facilitate 
grabbing an element from an array.

## Testing

Unit tests are written in a way made to be reminiscent of 
[pytest](https://docs.pytest.org/en/stable/); however, the testing framework is
still nonexistent at this point, only a few utilities have been developped.
The testing part is planned to be improved in the near future.