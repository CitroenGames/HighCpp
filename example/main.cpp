#include "HighCPP.h"

int main() {
    // --- 1. Simple construction ---
    var x = 42;
    var pi = 3.14;
    var name = "HighCPP";
    var flag = true;
    var nothing;

    print("=== Basic Types ===");
    print("x =", x);
    print("pi =", pi);
    print("name =", name);
    print("flag =", flag);
    print("nothing =", nothing);
    print("types:", x.typeOf(), pi.typeOf(), name.typeOf(), flag.typeOf(), nothing.typeOf());
    print();

    // --- 2. Arrays ---
    print("=== Arrays ===");
    var arr = Array{1, 2, 3, 4, 5};
    print("arr =", arr);

    arr.push(6);
    print("after push(6):", arr);

    print("arr[0] =", arr[0]);
    print("arr[-1] =", arr[-1]);

    arr[0] = 100;
    print("after arr[0] = 100:", arr);

    print("length =", arr.length());
    print("contains 3?", arr.contains(3));
    print("contains 99?", arr.contains(99));

    var last = arr.pop();
    print("popped:", last);
    print("remaining:", arr);
    print();

    // --- 3. Tables ---
    print("=== Tables ===");
    var tbl = Table{{"name", "Alice"}, {"age", 30}};
    print("tbl =", tbl);

    tbl["email"] = "alice@example.com";
    print("after adding email:", tbl);

    print("tbl[\"name\"] =", tbl["name"]);
    print("has 'name'?", tbl.has("name"));
    print("has 'phone'?", tbl.has("phone"));
    print("keys:", tbl.keys());
    print("values:", tbl.values());
    print("length =", tbl.length());
    print();

    // --- 4. Arithmetic ---
    print("=== Arithmetic ===");
    var a = 10;
    var b = 3;
    print("10 + 3 =", a + b);
    print("10 - 3 =", a - b);
    print("10 * 3 =", a * b);
    print("10 / 3 =", a / b);
    print("10 % 3 =", a % b);

    var c = 2.5;
    print("10 + 2.5 =", a + c);
    print("10 * 2.5 =", a * c);

    // String concatenation
    var greeting = var("Hello, ") + name + var("!");
    print(greeting);

    // Number to string via concat
    var msg = var("The answer is ") + x;
    print(msg);
    print();

    // --- 5. Comparisons ---
    print("=== Comparisons ===");
    print("10 == 10:", var(10) == var(10));
    print("10 == 10.0:", var(10) == var(10.0));
    print("10 != 3:", a != b);
    print("10 > 3:", a > b);
    print("10 < 3:", a < b);
    print("10 >= 10:", a >= var(10));
    print("\"abc\" < \"def\":", var("abc") < var("def"));
    print();

    // --- 6. Truthiness ---
    print("=== Truthiness ===");
    var test_values = Array{var(), var(0), var(0.0), var(false), var(""), Array{}, Table{}, var(42), var("hi"), var(true)};
    for (const auto& v : test_values) {
        if (v) {
            print(v, "-> truthy");
        } else {
            print(v, "-> falsy");
        }
    }
    print();

    // --- 7. Functions ---
    print("=== Functions ===");
    var add = Function([](Array args) -> var {
        return args[0] + args[1];
    });
    print("add(3, 4) =", add(3, 4));

    var greet = Function([](Array args) -> var {
        return var("Hello, ") + args[0] + var("!");
    });
    print(greet("World"));

    var factorial = Function([](Array args) -> var {
        int n = args[0].getInt();
        int result = 1;
        for (int i = 2; i <= n; ++i) result *= i;
        return result;
    });
    print("factorial(5) =", factorial(5));
    print("type:", add.typeOf());
    print();

    // --- 8. Iteration ---
    print("=== Iteration ===");
    var nums = Array{10, 20, 30, 40, 50};
    for (const auto& item : nums) {
        print(" -", item);
    }
    print();

    // --- 9. Range and Slice ---
    print("=== Range & Slice ===");
    var r = range(0, 10, 2);
    print("range(0, 10, 2):", r);

    var r2 = range(5);
    print("range(5):", r2);

    var s = slice(r, 1, 4);
    print("slice(r, 1, 4):", s);
    print();

    // --- 10. Deep Copy ---
    print("=== Deep Copy ===");
    var original = Array{1, 2, 3};
    var copy = original;
    copy[0] = 999;
    print("original:", original);
    print("copy:", copy);
    print();

    // --- 11. Compound Assignment ---
    print("=== Compound Assignment ===");
    var counter = 0;
    counter += 5;
    print("0 += 5 ->", counter);
    counter *= 3;
    print("*= 3 ->", counter);
    counter -= 2;
    print("-= 2 ->", counter);
    print();

    // --- 12. Nested Access ---
    print("=== Nested Structures ===");
    var data = Table{
        {"users", Array{
            Table{{"name", "Alice"}, {"score", 95}},
            Table{{"name", "Bob"}, {"score", 87}}
        }}
    };
    print("data:", data);
    print("first user:", data["users"][0]);
    print("first user name:", data["users"][0]["name"]);
    print();

    // --- 13. Len ---
    print("=== len() ===");
    print("len(arr):", len(arr));
    print("len(tbl):", len(tbl));
    print("len(\"hello\"):", len(var("hello")));

    return 0;
}
