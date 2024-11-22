![Pipeline status](https://github.com/PaulWassermann/array.c/actions/workflows/github-actions-ci.yaml/badge.svg)

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
    Device device;
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

### GPU support

The `device` member of the `Array` struct indicates where the memory pointed to 
by the `data` pointer resides : CPU or CUDA device.

## Testing

Unit tests are written in a way made to be reminiscent of 
[pytest](https://docs.pytest.org/en/stable/). To build the tests builder 
executable, run

```bash
make test-builder
```

The tests builder is a command line tool, run `builder.exe -h` to print help :

```bash
Usage: builder.exe [tests folder] -t [template file] -o [output file]
```


It performs the following actions :

1) walk a specified `tests folder`,  looking for files with a "test_" prefix
2) read those files, looking for test functions; they can be recognized by
their "test_" prefix
    <u>Note</u>: unit tests are required to return `void` and musn't take have 
    parameters
3) Writes the adequate include lines and calls to found test functions in the
target `output file`, using the provided `template file`