#pragma once

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <any>
#include <type_traits>

// Forward declaration for nested structures
struct var;

// Define Array and Table using vectors and unordered_maps of var
using Array = std::vector<var>;
using Table = std::unordered_map<std::string, var>;

// Enum for var types
enum class varType {
    Null,
    Int,
    Double,
    String,
    Array,
    Table,
    Pointer,        // Pointer to var (shared_ptr<var>)
    SharedPointer,  // std::shared_ptr<void>
    WeakPointer,    // std::weak_ptr<void>
    Object          // Renamed from Custom for consistency
};

// Type trait to check if T is a std::weak_ptr
template <typename T>
struct is_weak_ptr : std::false_type {};

template <typename T>
struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};

// Type trait to check if T is explicitly supported by var constructors
template <typename T>
struct is_var_supported : std::disjunction<
    std::is_same<T, int>,
    std::is_same<T, double>,
    std::is_same<T, std::string>,
    std::is_same<T, const char*>,
    std::is_same<T, char*>,
    std::is_same<T, Array>,
    std::is_same<T, Table>,
    std::is_same<T, std::shared_ptr<var>>,
    std::is_same<T, std::any>
> {};

// Define the var structure
struct var {
    // Define the variant to hold different types
    using Pointer = std::shared_ptr<var>;

    std::variant<
        std::monostate,                 // Represents 'null' or 'undefined'
        int,                            // Integer
        double,                         // Double
        std::string,                    // String
        Array,                          // Dynamic Array
        Table,                          // Dynamic Table (Dictionary)
        Pointer,                        // Pointer to var for nested structures
        std::any,                       // Custom type for user-defined classes and pointers
        std::shared_ptr<void>,          // Shared Pointer (also used for typed pointer ownership)
        std::weak_ptr<void>             // Weak Pointer
    > value;

    // Constructors
    var();
    var(int v);
    var(double v);
    var(const std::string& v);
    var(std::string&& v);
    var(const char* v);
    var(const Array& v);
    var(Array&& v);
    var(const Table& v);
    var(Table&& v);
    var(const Pointer& v);
    var(Pointer&& v);
    var(const std::any& v);
    var(std::any&& v);

    // Template constructors
    template <typename T, typename = std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, var> &&
        !std::is_pointer_v<std::decay_t<T>> &&
        !is_weak_ptr<std::decay_t<T>>::value &&
        !is_var_supported<std::decay_t<T>>::value
        >>
        var(T&& v) : value(std::any(std::forward<T>(v))) {}

    // Typed pointer constructor: takes ownership via shared_ptr with typed deleter
    template <typename T, typename = std::enable_if_t<
        std::is_pointer_v<std::decay_t<T>> &&
        !std::is_same_v<std::decay_t<T>, void*> &&
        !is_weak_ptr<std::decay_t<T>>::value
        >>
        var(T ptr) {
            using PointeeType = std::remove_pointer_t<std::decay_t<T>>;
            if (ptr) {
                value = std::shared_ptr<void>(
                    const_cast<void*>(static_cast<const void*>(ptr)),
                    [](void* p) { delete static_cast<PointeeType*>(p); }
                );
            } else {
                value = std::shared_ptr<void>(nullptr);
            }
        }

    template <typename T, typename = std::enable_if_t<is_weak_ptr<std::decay_t<T>>::value>>
    var(const T& wp) {
        std::weak_ptr<void> wp_void;
        if (auto sp = wp.lock()) {
            wp_void = std::static_pointer_cast<void>(sp);
        }
        value = wp_void;
    }

    // Copy and Move Constructors
    var(const var& other);
    var(var&& other) noexcept = default;

    // Copy and Move Assignment Operators
    var& operator=(const var& other);
    var& operator=(var&& other) noexcept = default;

    // Assignment Operators for different types
    var& operator=(int v);
    var& operator=(double v);
    var& operator=(const std::string& v);
    var& operator=(std::string&& v);
    var& operator=(const char* v);
    var& operator=(const Array& v);
    var& operator=(Array&& v);
    var& operator=(const Table& v);
    var& operator=(Table&& v);
    var& operator=(const Pointer& v);
    var& operator=(Pointer&& v);
    var& operator=(const std::any& v);
    var& operator=(std::any&& v);

    // Template assignment operators
    template <typename T, typename = std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, var> &&
        !std::is_pointer_v<std::decay_t<T>> &&
        !is_weak_ptr<std::decay_t<T>>::value &&
        !is_var_supported<std::decay_t<T>>::value
        >>
        var & operator=(T&& v) {
            value = std::any(std::forward<T>(v));
            return *this;
        }

    // Typed pointer assignment: takes ownership via shared_ptr with typed deleter
    template <typename T, typename = std::enable_if_t<
        std::is_pointer_v<std::decay_t<T>> &&
        !std::is_same_v<std::decay_t<T>, void*> &&
        !is_weak_ptr<std::decay_t<T>>::value
        >>
        var & operator=(T ptr) {
            using PointeeType = std::remove_pointer_t<std::decay_t<T>>;
            if (ptr) {
                value = std::shared_ptr<void>(
                    const_cast<void*>(static_cast<const void*>(ptr)),
                    [](void* p) { delete static_cast<PointeeType*>(p); }
                );
            } else {
                value = std::shared_ptr<void>(nullptr);
            }
            return *this;
        }

    template <typename T, typename = std::enable_if_t<is_weak_ptr<std::decay_t<T>>::value>>
    var& operator=(const T& wp) {
        std::weak_ptr<void> wp_void;
        if (auto sp = wp.lock()) {
            wp_void = std::static_pointer_cast<void>(sp);
        }
        value = wp_void;
        return *this;
    }

    // Type checking functions
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isTable() const;
    bool isPointer() const;
    bool isSharedPointer() const;
    bool isWeakPointer() const;
    bool IsObject() const; // Renamed from isCustom()
    bool isNull() const;

    // Getters with type safety
    int getInt() const;
    double getDouble() const;
    const std::string& getString() const;
    std::string& getString();
    const Array& getArray() const;
    Array& getArray();
    const Table& getTable() const;
    Table& getTable();
    const Pointer& getPointer() const;
    Pointer& getPointer();
    const std::any& getObject() const; // Renamed from getCustom()
    std::any& getObject(); // Renamed from getCustom()
    std::shared_ptr<void> getSharedPointer() const;
    std::weak_ptr<void> getWeakPointer() const;

    // Helper to get type as string
    std::string typeOf() const;

    // Overload the output operator for var
    friend std::ostream& operator<<(std::ostream& os, const var& varObj);

    // Function to retrieve varType
    friend varType getVarType(const var& varObj);

    // ------------------------ Helper Functions Declarations ------------------------

    // Boxing functions
    static var makeInt(int x);
    static var makeDouble(double x);
    static var makeString(const std::string& x);
    static var makeString(std::string&& x);
    static var makeArray(const Array& arr);
    static var makeArray(Array&& arr);
    static var makeTable(const Table& tbl);
    static var makeTable(Table&& tbl);
    static var makePointer(const var& varObj);
    static var makePointer(var&& varObj);
    static var makeCustom(const std::any& customObj);
    static var makeCustom(std::any&& customObj);

    // Array functions
    static var newArray(const Array& arr);
    static var newArray(Array&& arr);
    static var getElement(const var& arrayVar, size_t index);
    static void setElement(var& arrayVar, size_t index, const var& value);
    static void appendElement(var& arrayVar, const var& value);

    // Table functions
    static var newTable(const Table& tbl);
    static var newTable(Table&& tbl);
    static var getElement(const var& tableVar, const std::string& key);
    static void setElement(var& tableVar, const std::string& key, const var& value);

    // Utility functions
    static size_t len(const var& varObj);
    static var range(int start, int end, int step = 1);
    static var range(int end);
    static var slice(const var& arrayVar, int start, int end, int step = 1);

    // Utility functions for smart pointers
    template <typename T>
    static var makeSmartPointer(const std::shared_ptr<T>& ptr);

    template <typename T>
    static var makeSmartPointer(std::shared_ptr<T>&& ptr);

    template <typename T>
    static var makeSmartPointer(std::unique_ptr<T>&& ptr);

    template <typename T>
    static var makeSmartPointer(const std::weak_ptr<T>& ptr);

    // Non-owning reference to an external object (user manages lifetime)
    template <typename T>
    static var ref(T* ptr) {
        var v;
        if (ptr) {
            v.value = std::shared_ptr<void>(
                const_cast<void*>(static_cast<const void*>(ptr)),
                [](void*) { /* no-op deleter: does not own the object */ }
            );
        } else {
            v.value = std::shared_ptr<void>(nullptr);
        }
        return v;
    }
};

// Free functions
inline varType getVarType(const var& varObj) {
    if (varObj.isInt()) return varType::Int;
    if (varObj.isDouble()) return varType::Double;
    if (varObj.isString()) return varType::String;
    if (varObj.isArray()) return varType::Array;
    if (varObj.isTable()) return varType::Table;
    if (varObj.isPointer()) return varType::Pointer;
    if (varObj.isSharedPointer()) return varType::SharedPointer;
    if (varObj.isWeakPointer()) return varType::WeakPointer;
    if (varObj.IsObject()) return varType::Object;
    return varType::Null;
}

// Boxing functions
inline var makeInt(int x) { return var::makeInt(x); }
inline var makeDouble(double x) { return var::makeDouble(x); }
inline var makeString(const std::string& x) { return var::makeString(x); }
inline var makeString(std::string&& x) { return var::makeString(std::move(x)); }
inline var makeArray(const Array& arr) { return var::makeArray(arr); }
inline var makeArray(Array&& arr) { return var::makeArray(std::move(arr)); }
inline var makeTable(const Table& tbl) { return var::makeTable(tbl); }
inline var makeTable(Table&& tbl) { return var::makeTable(std::move(tbl)); }
inline var makePointer(const var& varObj) { return var::makePointer(varObj); }
inline var makePointer(var&& varObj) { return var::makePointer(std::move(varObj)); }
inline var makeCustom(const std::any& customObj) { return var::makeCustom(customObj); }
inline var makeCustom(std::any&& customObj) { return var::makeCustom(std::move(customObj)); }

// Array functions
inline var newArray(const Array& arr) { return var::newArray(arr); }
inline var newArray(Array&& arr) { return var::newArray(std::move(arr)); }
inline var getElement(const var& arrayVar, size_t index) { return var::getElement(arrayVar, index); }
inline void setElement(var& arrayVar, size_t index, const var& value) { var::setElement(arrayVar, index, value); }
inline void appendElement(var& arrayVar, const var& value) { var::appendElement(arrayVar, value); }

// Table functions
inline var newTable(const Table& tbl) { return var::newTable(tbl); }
inline var newTable(Table&& tbl) { return var::newTable(std::move(tbl)); }
inline var getElement(const var& tableVar, const std::string& key) { return var::getElement(tableVar, key); }
inline void setElement(var& tableVar, const std::string& key, const var& value) { var::setElement(tableVar, key, value); }

// Utility functions
inline size_t len(const var& varObj) { return var::len(varObj); }
inline var range(int start, int end, int step = 1) { return var::range(start, end, step); }
inline var range(int end) { return var::range(end); }
inline var slice(const var& arrayVar, int start, int end, int step = 1) { return var::slice(arrayVar, start, end, step); }

// Utility functions for smart pointers
template <typename T>
var var::makeSmartPointer(const std::shared_ptr<T>& ptr) {
    var v;
    v.value = std::static_pointer_cast<void>(ptr);
    return v;
}

template <typename T>
var var::makeSmartPointer(std::shared_ptr<T>&& ptr) {
    var v;
    v.value = std::static_pointer_cast<void>(std::move(ptr));
    return v;
}

template <typename T>
var var::makeSmartPointer(std::unique_ptr<T>&& ptr) {
    var v;
    v.value = std::shared_ptr<void>(std::move(ptr));
    return v;
}

template <typename T>
var var::makeSmartPointer(const std::weak_ptr<T>& ptr) {
    std::weak_ptr<void> wp_void;
    if (auto sp = ptr.lock()) {
        wp_void = std::static_pointer_cast<void>(sp);
    }
    return var(wp_void);
}
