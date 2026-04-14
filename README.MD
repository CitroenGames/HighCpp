# HighCPP - Scripting-Language Semantics in C++20

**HighCPP** gives you a single `var` type that behaves like a variable in Python, JavaScript, or Lua. Write dynamically-typed code in C++ with operators, truthiness, first-class functions, and iteration -- no interpreter required.

```cpp
#include "HighCpp.h"

int main() {
    var x = 42;
    var arr = Array{1, 2, 3};
    arr.push(4);
    print("sum:", arr[0] + arr[1]);  // sum: 3

    var greet = Function([](Array args) -> var {
        return var("Hello, ") + args[0] + var("!");
    });
    print(greet("World"));  // Hello, World!
}
```

## Features

- **Dynamic typing** -- `var` holds null, bool, int, double, string, array, table, function, or any custom object
- **Operators** -- arithmetic (`+`, `-`, `*`, `/`, `%`), comparison (`==`, `!=`, `<`, `>`, `<=`, `>=`), indexing (`[]`)
- **String concatenation** -- `var("hello ") + name` with automatic number-to-string conversion
- **Truthiness** -- `if (myVar)` works like Python/JS (null, false, 0, 0.0, "", empty collections are falsy)
- **First-class functions** -- store callables in `var`, invoke with `f(args...)`
- **Method-style API** -- `.push()`, `.pop()`, `.length()`, `.keys()`, `.values()`, `.has()`, `.contains()`
- **Iteration** -- range-based `for` loops over arrays
- **Python-style utilities** -- `range()`, `slice()`, `len()`, `print()`
- **Deep copy semantics** -- assigning arrays/tables creates independent copies
- **Custom objects** -- store any C++ type via `std::any`

## Building

```bash
cmake -S . -B build
cmake --build build
```

## API Reference

### Construction

```cpp
var x;                      // null
var b = true;               // bool
var i = 42;                 // int
var d = 3.14;               // double
var s = "hello";            // string
var arr = Array{1, 2, 3};   // array
var tbl = Table{{"a", 1}, {"b", 2}};  // table
var f = Function([](Array args) -> var { return args[0] + args[1]; });  // function
```

### Type Checking

```cpp
x.isNull()      x.isBool()     x.isInt()      x.isDouble()
x.isNumber()    x.isString()   x.isArray()    x.isTable()
x.isFunction()  x.isObject()   x.typeOf()     // returns "int", "string", etc.
```

### Indexing

```cpp
// Arrays (supports negative indexing)
var arr = Array{10, 20, 30};
var first = arr[0];     // 10
var last = arr[-1];     // 30
arr[0] = 99;            // write

// Tables
var tbl = Table{{"name", "Alice"}};
var name = tbl["name"]; // "Alice"
tbl["age"] = 30;        // insert

// Nested access
var data = Table{{"items", Array{1, 2, 3}}};
var x = data["items"][0]; // 1
```

### Arithmetic

```cpp
var a = 10, b = 3;
a + b    // 13 (int)
a - b    // 7
a * b    // 30
a / b    // 3 (integer division)
a % b    // 1

var c = 2.5;
a + c    // 12.5 (promotes to double)

// String concatenation
var("Hello, ") + var("World")     // "Hello, World"
var("Score: ") + var(100)         // "Score: 100"

// Compound assignment
a += 5;  a -= 2;  a *= 3;  a /= 4;  a %= 2;

// Unary
var neg = -a;
```

### Comparison

```cpp
var(10) == var(10)      // true
var(10) == var(10.0)    // true (int/double coercion)
var(10) != var(3)       // true
var(10) > var(3)        // true
var("abc") < var("def") // true (lexicographic)
```

### Truthiness

```cpp
if (myVar) { /* truthy */ }
```

| Value | Truthy? |
|-------|---------|
| `null` | falsy |
| `false` | falsy |
| `0`, `0.0` | falsy |
| `""` (empty string) | falsy |
| `[]` (empty array) | falsy |
| `{}` (empty table) | falsy |
| Everything else | truthy |

### Functions

```cpp
var add = Function([](Array args) -> var {
    return args[0] + args[1];
});
var result = add(3, 4);  // 7

// Functions are first-class values
var ops = Table{{"add", add}};
print(ops["add"](2, 3));  // 5
```

### Methods

```cpp
// Arrays
arr.push(val);           // append element
var last = arr.pop();    // remove and return last
arr.length();            // array size (as var)
arr.contains(val);       // linear search

// Tables
tbl.keys();              // array of key strings
tbl.values();            // array of values
tbl.has("key");          // key existence check
tbl.length();            // number of entries

// Strings
str.length();            // string length
```

### Iteration

```cpp
var nums = Array{10, 20, 30};
for (auto& item : nums) {
    print(item);
}
```

### Utility Functions

```cpp
// Print (variadic, space-separated, with newline)
print("hello", 42, true);  // hello 42 true

// Range
range(5);           // [0, 1, 2, 3, 4]
range(0, 10, 2);    // [0, 2, 4, 6, 8]

// Slice (supports negative indices)
slice(arr, 1, 4);       // elements [1..4)
slice(arr, 0, -1);      // all but last
slice(arr, 0, 6, 2);    // every other element

// Length
len(arr);    // array/table/string size
```

### Custom Objects

For advanced use, any C++ type can be stored via `std::any`:

```cpp
struct Point { double x, y; };
var pt = std::any(Point{3.0, 4.0});
auto p = std::any_cast<Point>(pt.getObject());
```

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 19.29+)
- CMake 3.10 or higher

## License

MIT -- see [LICENSE.txt](LICENSE.txt)
