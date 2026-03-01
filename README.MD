# HighCPP - A Dynamic Variant Type for C++

## Introduction

**HighCPP** is a versatile and dynamic variant type for C++ that enables developers to store and manipulate a wide range of data types seamlessly. Leveraging modern C++20 features such as `std::variant`, `std::any`, and smart pointers, `HighCPP` provides a flexible solution for scenarios requiring dynamic type handling, similar to scripting languages like Python or JavaScript.

Whether you're building a scripting engine, a dynamic data structure, or simply need a more flexible type system in your C++ projects, `HighCPP` offers the tools you need to handle diverse data effortlessly.

## Features

- **Primitive Types:** Store integers, doubles, and strings.
- **Dynamic Arrays:** Manage lists of `HighCPP` objects with dynamic resizing and element manipulation.
- **Tables (Dictionaries):** Utilize key-value pairs for associative data storage.
- **Smart Pointers:** Native support for managing `std::shared_ptr`, `std::unique_ptr` (with proper memory management and type erasure), `std::weak_ptr`, and raw pointers.
- **Custom Objects:** Store any user-defined types or classes safely using `std::any`.
- **Utility Functions:** Create ranges (`range`), slice arrays (`slice`), and dynamically retrieve lengths of arrays and tables (`len`).
- **Deep Copy Support:** Ensure independent copies of nested structures and variant objects where applicable.
- **Robust Type Inference:** SFINAE constraints ensure standard types aren't accidentally wrapped in generic type containers.
- **Exception Safety:** Robust error handling with informative exceptions (`std::bad_variant_access`, `std::out_of_range`).

## Building the Project

HighCPP uses CMake for building.

```bash
# Generate build files
cmake -S . -B build

# Build the library and example executable
cmake --build build
```

## Example Usage

```cpp
#include "HighCpp.h"
#include <iostream>

int main() {
    // Basic types
    var intVar = 42;
    var doubleVar = 3.14159;
    var stringVar = "Hello, World!";

    // Arrays
    var arrayVar = makeArray(Array{ makeInt(1), makeInt(2), makeInt(3) });
    appendElement(arrayVar, makeInt(4));
    std::cout << "Array: " << arrayVar << std::endl;

    // Tables (Dictionaries)
    var tableVar = makeTable(Table{
        { "key1", makeString("value1") },
        { "key2", makeDouble(2.718) }
    });
    setElement(tableVar, "key3", makeInt(100));
    std::cout << "Table: " << tableVar << std::endl;

    // Smart Pointers
    var uniquePtrVar = var::makeSmartPointer(std::make_unique<var>(makeInt(999)));
    std::cout << "Unique Pointer: " << uniquePtrVar << std::endl;

    // Utility functions
    var rangeVar = range(0, 10, 2);
    std::cout << "Range (0 to 10 step 2): " << rangeVar << std::endl;

    return 0;
}
```

## Requirements

- C++20 compatible compiler (e.g., GCC 10+, Clang 10+, MSVC 19.29+)
- CMake 3.10 or higher
