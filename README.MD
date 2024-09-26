# HighCPP - A Dynamic Variant Type for C++

## Introduction

**HighCPP** is a versatile and dynamic variant type for C++ that enables developers to store and manipulate a wide range of data types seamlessly. Leveraging modern C++ features such as `std::variant`, `std::any`, and smart pointers, `HighCPP` provides a flexible solution for scenarios requiring dynamic type handling, similar to scripting languages like Python or JavaScript.

Whether you're building a scripting engine, a dynamic data structure, or simply need a more flexible type system in your C++ projects, `HighCPP` offers the tools you need to handle diverse data effortlessly.

## Features

- **Primitive Types:** Store integers, doubles, and strings.
- **Dynamic Arrays:** Manage lists of `HighCPP` objects with dynamic resizing and element manipulation.
- **Tables (Dictionaries):** Utilize key-value pairs for associative data storage.
- **Smart Pointers:** Handle `std::shared_ptr`, `std::unique_ptr`, `std::weak_ptr`, and raw pointers.
- **Custom Objects:** Store user-defined types using `std::any`.
- **Utility Functions:** Create ranges, slices, and retrieve lengths of arrays and tables.
- **Deep Copy Support:** Ensure independent copies of `HighCPP` objects where applicable.
- **Exception Safety:** Robust error handling with informative exceptions.
- **Extensible Design:** Easily extendable to accommodate additional types and functionalities.
