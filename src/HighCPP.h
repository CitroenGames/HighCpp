#pragma once

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <any>
#include <type_traits>
#include <functional>
#include <sstream>
#include <cmath>

// Forward declaration
struct var;

// Type aliases
using Array = std::vector<var>;
using Table = std::unordered_map<std::string, var>;
using Function = std::function<var(Array)>;

// Type trait to check if T is explicitly supported by var constructors
template <typename T>
struct is_var_supported : std::disjunction<
    std::is_same<T, bool>,
    std::is_same<T, int>,
    std::is_same<T, double>,
    std::is_same<T, std::string>,
    std::is_same<T, const char*>,
    std::is_same<T, char*>,
    std::is_same<T, Array>,
    std::is_same<T, Table>,
    std::is_same<T, Function>,
    std::is_same<T, std::any>
> {};

struct var {
    std::variant<
        std::monostate,   // 0 - null
        bool,             // 1 - boolean
        int,              // 2 - integer
        double,           // 3 - double
        std::string,      // 4 - string
        Array,            // 5 - array
        Table,            // 6 - table
        Function,         // 7 - callable
        std::any          // 8 - custom object
    > value;

    // ---- VarProxy for operator[] read/write ----
    struct VarProxy {
        var& owner;
        std::variant<int, std::string> key;

        VarProxy(var& owner, int index);
        VarProxy(var& owner, const std::string& k);
        VarProxy(var& owner, const char* k);

        // Read
        operator var() const;

        // Write
        VarProxy& operator=(const var& val);

        // Chained access
        VarProxy operator[](int index);
        VarProxy operator[](const std::string& key);
        VarProxy operator[](const char* key);

        // Allow printing a proxy
        friend std::ostream& operator<<(std::ostream& os, const VarProxy& proxy) {
            os << static_cast<var>(proxy);
            return os;
        }
    };

    // ---- Constructors ----
    var();
    var(bool v);
    var(int v);
    var(double v);
    var(const std::string& v);
    var(std::string&& v);
    var(const char* v);
    var(const Array& v);
    var(Array&& v);
    var(const Table& v);
    var(Table&& v);
    var(const Function& v);
    var(Function&& v);
    var(const std::any& v);
    var(std::any&& v);

    // Catch-all: wrap unknown types in std::any
    template <typename T, typename = std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, var> &&
        !std::is_same_v<std::decay_t<T>, VarProxy> &&
        !is_var_supported<std::decay_t<T>>::value
    >>
    var(T&& v) : value(std::any(std::forward<T>(v))) {}

    // Copy and Move
    var(const var& other);
    var(var&& other) noexcept = default;

    // ---- Assignment Operators ----
    var& operator=(const var& other);
    var& operator=(var&& other) noexcept = default;

    var& operator=(bool v);
    var& operator=(int v);
    var& operator=(double v);
    var& operator=(const std::string& v);
    var& operator=(std::string&& v);
    var& operator=(const char* v);
    var& operator=(const Array& v);
    var& operator=(Array&& v);
    var& operator=(const Table& v);
    var& operator=(Table&& v);
    var& operator=(const Function& v);
    var& operator=(Function&& v);
    var& operator=(const std::any& v);
    var& operator=(std::any&& v);

    // Catch-all assignment
    template <typename T, typename = std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, var> &&
        !std::is_same_v<std::decay_t<T>, VarProxy> &&
        !is_var_supported<std::decay_t<T>>::value
    >>
    var& operator=(T&& v) {
        value = std::any(std::forward<T>(v));
        return *this;
    }

    // ---- Type Checking ----
    bool isNull() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isTable() const;
    bool isFunction() const;
    bool isObject() const;

    // ---- Getters ----
    bool getBool() const;
    int getInt() const;
    double getDouble() const;
    double toNumber() const;
    const std::string& getString() const;
    std::string& getString();
    const Array& getArray() const;
    Array& getArray();
    const Table& getTable() const;
    Table& getTable();
    const Function& getFunction() const;
    Function& getFunction();
    const std::any& getObject() const;
    std::any& getObject();

    // ---- Type Info ----
    std::string typeOf() const;

    // ---- Truthiness ----
    explicit operator bool() const;

    // ---- operator[] ----
    VarProxy operator[](int index);
    VarProxy operator[](const std::string& key);
    VarProxy operator[](const char* key);
    var operator[](int index) const;
    var operator[](const std::string& key) const;
    var operator[](const char* key) const;

    // ---- operator() for callables ----
    template <typename... Args>
    var operator()(Args&&... args) const {
        if (!isFunction()) throw std::runtime_error("var is not callable");
        Array argv{var(std::forward<Args>(args))...};
        return getFunction()(argv);
    }

    var operator()() const {
        if (!isFunction()) throw std::runtime_error("var is not callable");
        return getFunction()(Array{});
    }

    // ---- Arithmetic Operators ----
    friend var operator+(const var& lhs, const var& rhs);
    friend var operator-(const var& lhs, const var& rhs);
    friend var operator*(const var& lhs, const var& rhs);
    friend var operator/(const var& lhs, const var& rhs);
    friend var operator%(const var& lhs, const var& rhs);

    var& operator+=(const var& rhs);
    var& operator-=(const var& rhs);
    var& operator*=(const var& rhs);
    var& operator/=(const var& rhs);
    var& operator%=(const var& rhs);

    var operator-() const;
    var operator+() const;

    // ---- Comparison Operators ----
    friend bool operator==(const var& lhs, const var& rhs);
    friend bool operator!=(const var& lhs, const var& rhs);
    friend bool operator<(const var& lhs, const var& rhs);
    friend bool operator>(const var& lhs, const var& rhs);
    friend bool operator<=(const var& lhs, const var& rhs);
    friend bool operator>=(const var& lhs, const var& rhs);

    // ---- Method-Style API ----
    void push(const var& val);
    var pop();
    var length() const;
    bool contains(const var& val) const;
    var keys() const;
    var values() const;
    bool has(const std::string& key) const;

    // ---- Iteration (arrays only) ----
    Array::iterator begin();
    Array::iterator end();
    Array::const_iterator begin() const;
    Array::const_iterator end() const;

    // ---- Output ----
    friend std::ostream& operator<<(std::ostream& os, const var& v);

    // ---- Static Utilities ----
    static size_t len(const var& v);
    static var range(int start, int end, int step = 1);
    static var range(int end);
    static var slice(const var& v, int start, int end, int step = 1);
};

// ---- Free function utilities ----
inline size_t len(const var& v) { return var::len(v); }
inline var range(int start, int end, int step = 1) { return var::range(start, end, step); }
inline var range(int end) { return var::range(end); }
inline var slice(const var& v, int start, int end, int step = 1) { return var::slice(v, start, end, step); }

// ---- print() ----
namespace detail {
    inline void print_var(std::ostream& os, const var& v) {
        if (v.isString()) {
            os << v.getString();
        } else {
            os << v;
        }
    }

    template <typename T>
    void print_val(std::ostream& os, const T& v) {
        os << v;
    }

    template <>
    inline void print_val<var>(std::ostream& os, const var& v) {
        print_var(os, v);
    }

    template <>
    inline void print_val<var::VarProxy>(std::ostream& os, const var::VarProxy& v) {
        print_var(os, static_cast<var>(v));
    }

    template <>
    inline void print_val<bool>(std::ostream& os, const bool& v) {
        os << (v ? "true" : "false");
    }
}

inline void print() {
    std::cout << std::endl;
}

template <typename T>
void print(const T& first) {
    detail::print_val(std::cout, first);
    std::cout << std::endl;
}

template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    detail::print_val(std::cout, first);
    std::cout << " ";
    print(rest...);
}
