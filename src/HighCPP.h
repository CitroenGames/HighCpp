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
    RawPointer,     // Raw Pointer (void*)
    SharedPointer,  // std::shared_ptr<void>
    UniquePointer,  // std::unique_ptr<void, std::default_delete<void>>
    WeakPointer,    // std::weak_ptr<void>
    Object          // Renamed from Custom for consistency
};

// Type trait to check if T is a std::weak_ptr
template <typename T>
struct is_weak_ptr : std::false_type {};

template <typename T>
struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};

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
        void*,                          // Raw Pointer
        std::shared_ptr<void>,          // Shared Pointer
        std::unique_ptr<void, std::default_delete<void>>, // Unique Pointer
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
    var(void* v);

    // Template constructors
    template <typename T, typename = std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, var> &&
        !std::is_pointer_v<std::decay_t<T>> &&
        !is_weak_ptr<std::decay_t<T>>::value
        >>
        var(T&& v);

    template <typename T, typename = std::enable_if_t<
        std::is_pointer_v<std::decay_t<T>> &&
        !std::is_same_v<std::decay_t<T>, void*> &&
        !is_weak_ptr<std::decay_t<T>>::value
        >>
        var(T ptr);

    template <typename T, typename = std::enable_if_t<is_weak_ptr<std::decay_t<T>>::value>>
    var(const T& wp);

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
        !is_weak_ptr<std::decay_t<T>>::value
        >>
        var & operator=(T&& v);

    template <typename T, typename = std::enable_if_t<
        std::is_pointer_v<std::decay_t<T>> &&
        !std::is_same_v<std::decay_t<T>, void*> &&
        !is_weak_ptr<std::decay_t<T>>::value
        >>
        var & operator=(T ptr);

    template <typename T, typename = std::enable_if_t<is_weak_ptr<std::decay_t<T>>::value>>
    var& operator=(const T& wp);

    // Type checking functions
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isTable() const;
    bool isPointer() const;
    bool isRawPointer() const;
    bool isSharedPointer() const;
    bool isUniquePointer() const;
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
    void* getRawPointer() const;
    std::shared_ptr<void> getSharedPointer() const;
    std::unique_ptr<void, std::default_delete<void>>& getUniquePointer();
    const std::unique_ptr<void, std::default_delete<void>>& getUniquePointer() const;
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
};

// Free functions
varType getVarType(const var& varObj);

// Boxing functions
var makeInt(int x);
var makeDouble(double x);
var makeString(const std::string& x);
var makeString(std::string&& x);
var makeArray(const Array& arr);
var makeArray(Array&& arr);
var makeTable(const Table& tbl);
var makeTable(Table&& tbl);
var makePointer(const var& varObj);
var makePointer(var&& varObj);
var makeCustom(const std::any& customObj);
var makeCustom(std::any&& customObj);

// Array functions
var newArray(const Array& arr);
var newArray(Array&& arr);
var getElement(const var& arrayVar, size_t index);
void setElement(var& arrayVar, size_t index, const var& value);
void appendElement(var& arrayVar, const var& value);

// Table functions
var newTable(const Table& tbl);
var newTable(Table&& tbl);
var getElement(const var& tableVar, const std::string& key);
void setElement(var& tableVar, const std::string& key, const var& value);

// Utility functions
size_t len(const var& varObj);
var range(int start, int end, int step);
var range(int end);
var slice(const var& arrayVar, int start, int end, int step);

// Utility functions for smart pointers
template <typename T>
var var::makeSmartPointer(const std::shared_ptr<T>& ptr) {
    return var(std::static_pointer_cast<void>(ptr));
}

template <typename T>
var var::makeSmartPointer(std::shared_ptr<T>&& ptr) {
    return var(std::static_pointer_cast<void>(std::move(ptr)));
}

template <typename T>
var var::makeSmartPointer(std::unique_ptr<T>&& ptr) {
    return var(std::unique_ptr<void, std::default_delete<void>>(ptr.release()));
}

template <typename T>
var var::makeSmartPointer(const std::weak_ptr<T>& ptr) {
    std::weak_ptr<void> wp_void;
    if (auto sp = ptr.lock()) {
        wp_void = std::static_pointer_cast<void>(sp);
    }
    return var(wp_void);
}
