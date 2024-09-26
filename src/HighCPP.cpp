#include "HighCpp.h"

// Constructors
var::var() : value(std::monostate{}) {}
var::var(int v) : value(v) {}
var::var(double v) : value(v) {}
var::var(const std::string& v) : value(v) {}
var::var(std::string&& v) : value(std::move(v)) {}
var::var(const char* v) : value(std::string(v)) {}
var::var(const Array& v) : value(v) {}
var::var(Array&& v) : value(std::move(v)) {}
var::var(const Table& v) : value(v) {}
var::var(Table&& v) : value(std::move(v)) {}
var::var(const Pointer& v) : value(v) {}
var::var(Pointer&& v) : value(std::move(v)) {}
var::var(const std::any& v) : value(v) {}
var::var(std::any&& v) : value(std::move(v)) {}
var::var(void* v) : value(v) {} // Constructor for void*

// Template constructors
template <typename T, typename>
var::var(T&& v) : value(std::any(std::forward<T>(v))) {}

template <typename T, typename>
var::var(T ptr) : value(ptr) {}

template <typename T, typename>
var::var(const T& wp) {
    std::weak_ptr<void> wp_void;
    if (auto sp = wp.lock()) {
        wp_void = std::static_pointer_cast<void>(sp);
    }
    value = wp_void;
}

// Copy Constructor for Deep Copy
var::var(const var& other) {
    // Handle each type accordingly
    switch (other.value.index()) {
    case 0: // std::monostate
        value = std::monostate{};
        break;
    case 1: // int
        value = std::get<int>(other.value);
        break;
    case 2: // double
        value = std::get<double>(other.value);
        break;
    case 3: // std::string
        value = std::get<std::string>(other.value);
        break;
    case 4: // Array
        value = std::get<Array>(other.value);
        break;
    case 5: // Table
        value = std::get<Table>(other.value);
        break;
    case 6: // Pointer (shared_ptr<var>)
    {
        auto originalPtr = std::get<Pointer>(other.value);
        if (originalPtr) {
            // Perform a deep copy
            value = std::make_shared<var>(*originalPtr);
        }
        else {
            value = Pointer(nullptr);
        }
    }
    break;
    case 7: // std::any
    {
        const std::any& originalAny = std::get<std::any>(other.value);
        if (originalAny.has_value()) {
            // Attempt to copy based on the stored type
            if (originalAny.type() == typeid(std::shared_ptr<var>)) {
                auto originalSharedPtr = std::any_cast<std::shared_ptr<var>>(originalAny);
                if (originalSharedPtr) {
                    value = std::make_shared<var>(*originalSharedPtr);
                }
                else {
                    value = std::shared_ptr<void>(nullptr);
                }
            }
            else {
                // For other types, perform a copy
                value = originalAny;
            }
        }
        else {
            value = std::any{};
        }
    }
    break;
    case 8: // void* (Raw Pointer)
        value = std::get<void*>(other.value);
        break;
    case 9: // std::shared_ptr<void>
        value = std::get<std::shared_ptr<void>>(other.value);
        break;
    case 10: // std::unique_ptr<void, std::default_delete<void>>
    {
        // Cannot copy unique_ptr; set to nullptr or handle appropriately
        // Here, we'll set it to nullptr
        value = std::unique_ptr<void, std::default_delete<void>>(nullptr);
    }
    break;
    case 11: // std::weak_ptr<void>
        value = std::get<std::weak_ptr<void>>(other.value);
        break;
    default:
        throw std::runtime_error("Unknown var type during copy construction.");
    }
}

// Copy Assignment Operator for Deep Copy
var& var::operator=(const var& other) {
    if (this == &other) return *this; // Self-assignment check

    // Clear current value
    value = std::monostate{};

    // Handle each type accordingly
    switch (other.value.index()) {
    case 0: // std::monostate
        value = std::monostate{};
        break;
    case 1: // int
        value = std::get<int>(other.value);
        break;
    case 2: // double
        value = std::get<double>(other.value);
        break;
    case 3: // std::string
        value = std::get<std::string>(other.value);
        break;
    case 4: // Array
        value = std::get<Array>(other.value);
        break;
    case 5: // Table
        value = std::get<Table>(other.value);
        break;
    case 6: // Pointer (shared_ptr<var>)
    {
        auto originalPtr = std::get<Pointer>(other.value);
        if (originalPtr) {
            // Perform a deep copy
            value = std::make_shared<var>(*originalPtr);
        }
        else {
            value = Pointer(nullptr);
        }
    }
    break;
    case 7: // std::any
    {
        const std::any& originalAny = std::get<std::any>(other.value);
        if (originalAny.has_value()) {
            // Attempt to copy based on the stored type
            if (originalAny.type() == typeid(std::shared_ptr<var>)) {
                auto originalSharedPtr = std::any_cast<std::shared_ptr<var>>(originalAny);
                if (originalSharedPtr) {
                    value = std::make_shared<var>(*originalSharedPtr);
                }
                else {
                    value = std::shared_ptr<void>(nullptr);
                }
            }
            else {
                // For other types, perform a copy
                value = originalAny;
            }
        }
        else {
            value = std::any{};
        }
    }
    break;
    case 8: // void* (Raw Pointer)
        value = std::get<void*>(other.value);
        break;
    case 9: // std::shared_ptr<void>
        value = std::get<std::shared_ptr<void>>(other.value);
        break;
    case 10: // std::unique_ptr<void, std::default_delete<void>>
    {
        // Cannot copy unique_ptr; set to nullptr or handle appropriately
        // Here, we'll set it to nullptr
        value = std::unique_ptr<void, std::default_delete<void>>(nullptr);
    }
    break;
    case 11: // std::weak_ptr<void>
        value = std::get<std::weak_ptr<void>>(other.value);
        break;
    default:
        throw std::runtime_error("Unknown var type during copy assignment.");
    }

    return *this;
}

// Template assignment operators
template <typename T, typename>
var& var::operator=(T&& v) {
    value = std::any(std::forward<T>(v));
    return *this;
}

template <typename T, typename>
var& var::operator=(T ptr) {
    value = ptr;
    return *this;
}

template <typename T, typename>
var& var::operator=(const T& wp) {
    std::weak_ptr<void> wp_void;
    if (auto sp = wp.lock()) {
        wp_void = std::static_pointer_cast<void>(sp);
    }
    value = wp_void;
    return *this;
}

// Type checking
bool var::isInt() const { return std::holds_alternative<int>(value); }
bool var::isDouble() const { return std::holds_alternative<double>(value); }
bool var::isString() const { return std::holds_alternative<std::string>(value); }
bool var::isArray() const { return std::holds_alternative<Array>(value); }
bool var::isTable() const { return std::holds_alternative<Table>(value); }
bool var::isPointer() const { return std::holds_alternative<Pointer>(value); }
bool var::isRawPointer() const { return std::holds_alternative<void*>(value); }
bool var::isSharedPointer() const { return std::holds_alternative<std::shared_ptr<void>>(value); }
bool var::isUniquePointer() const { return std::holds_alternative<std::unique_ptr<void, std::default_delete<void>>>(value); }
bool var::isWeakPointer() const { return std::holds_alternative<std::weak_ptr<void>>(value); }
bool var::IsObject() const { return std::holds_alternative<std::any>(value); } // Renamed from isCustom()
bool var::isNull() const { return std::holds_alternative<std::monostate>(value); }

// Getters with type safety
int var::getInt() const {
    if (!isInt()) throw std::bad_variant_access();
    return std::get<int>(value);
}

double var::getDouble() const {
    if (!isDouble()) throw std::bad_variant_access();
    return std::get<double>(value);
}

const std::string& var::getString() const {
    if (!isString()) throw std::bad_variant_access();
    return std::get<std::string>(value);
}

std::string& var::getString() {
    if (!isString()) throw std::bad_variant_access();
    return std::get<std::string>(value);
}

const Array& var::getArray() const {
    if (!isArray()) throw std::bad_variant_access();
    return std::get<Array>(value);
}

Array& var::getArray() {
    if (!isArray()) throw std::bad_variant_access();
    return std::get<Array>(value);
}

const Table& var::getTable() const {
    if (!isTable()) throw std::bad_variant_access();
    return std::get<Table>(value);
}

Table& var::getTable() {
    if (!isTable()) throw std::bad_variant_access();
    return std::get<Table>(value);
}

const var::Pointer& var::getPointer() const {
    if (!isPointer()) throw std::bad_variant_access();
    return std::get<Pointer>(value);
}

var::Pointer& var::getPointer() {
    if (!isPointer()) throw std::bad_variant_access();
    return std::get<Pointer>(value);
}

const std::any& var::getObject() const { // Renamed from getCustom()
    if (!IsObject()) throw std::bad_variant_access();
    return std::get<std::any>(value);
}

std::any& var::getObject() { // Renamed from getCustom()
    if (!IsObject()) throw std::bad_variant_access();
    return std::get<std::any>(value);
}

void* var::getRawPointer() const {
    if (!isRawPointer()) throw std::bad_variant_access();
    return std::get<void*>(value);
}

std::shared_ptr<void> var::getSharedPointer() const {
    if (!isSharedPointer()) throw std::bad_variant_access();
    return std::get<std::shared_ptr<void>>(value);
}

std::unique_ptr<void, std::default_delete<void>>& var::getUniquePointer() {
    if (!isUniquePointer()) throw std::bad_variant_access();
    return std::get<std::unique_ptr<void, std::default_delete<void>>>(value);
}

const std::unique_ptr<void, std::default_delete<void>>& var::getUniquePointer() const {
    if (!isUniquePointer()) throw std::bad_variant_access();
    return std::get<std::unique_ptr<void, std::default_delete<void>>>(value);
}

std::weak_ptr<void> var::getWeakPointer() const {
    if (!isWeakPointer()) throw std::bad_variant_access();
    return std::get<std::weak_ptr<void>>(value);
}

// Helper to get type as string
std::string var::typeOf() const {
    if (isInt()) return "Int";
    if (isDouble()) return "Double";
    if (isString()) return "String";
    if (isArray()) return "Array";
    if (isTable()) return "Table";
    if (isPointer()) return "Pointer";
    if (isRawPointer()) return "RawPointer";
    if (isSharedPointer()) return "SharedPointer";
    if (isUniquePointer()) return "UniquePointer";
    if (isWeakPointer()) return "WeakPointer";
    if (IsObject()) return "Object"; // Changed from "Custom" to "Object"
    return "Null";
}

// Overload the output operator for var
std::ostream& operator<<(std::ostream& os, const var& varObj) {
    if (varObj.isInt()) {
        os << varObj.getInt();
    }
    else if (varObj.isDouble()) {
        os << varObj.getDouble();
    }
    else if (varObj.isString()) {
        os << '"' << varObj.getString() << '"';
    }
    else if (varObj.isArray()) {
        os << "[ ";
        const Array& arr = varObj.getArray();
        for (const auto& item : arr) {
            os << item << " ";
        }
        os << "]";
    }
    else if (varObj.isTable()) {
        os << "{ ";
        const Table& tbl = varObj.getTable();
        for (const auto& [key, value] : tbl) {
            os << '"' << key << "\": " << value << " ";
        }
        os << "}";
    }
    else if (varObj.isPointer()) {
        os << "Pointer(";
        if (varObj.getPointer()) {
            os << *(varObj.getPointer());
        }
        else {
            os << "nullptr";
        }
        os << ")";
    }
    else if (varObj.isRawPointer()) {
        os << "RawPointer(" << varObj.getRawPointer() << ")";
    }
    else if (varObj.isSharedPointer()) {
        os << "SharedPointer(";
        auto ptr = varObj.getSharedPointer();
        if (ptr) {
            // Attempt to print the pointed-to var
            // Since stored as shared_ptr<void>, we need to cast
            // Here, we'll assume it's pointing to var
            auto pointedVar = std::static_pointer_cast<var>(ptr);
            os << *pointedVar;
        }
        else {
            os << "nullptr";
        }
        os << ")";
    }
    else if (varObj.isUniquePointer()) {
        os << "UniquePointer(";
        const auto& ptr = varObj.getUniquePointer(); // Corrected line

        if (ptr) {
            // Print the raw address without dereferencing
            os << ptr.get();
        }
        else {
            os << "nullptr";
        }
        os << ")";
    }
    else if (varObj.isWeakPointer()) {
        os << "WeakPointer(";
        auto ptr = varObj.getWeakPointer().lock();
        if (ptr) {
            // Attempt to print the pointed-to var
            auto pointedVar = std::static_pointer_cast<var>(ptr);
            os << *pointedVar;
        }
        else {
            os << "expired";
        }
        os << ")";
    }
    else if (varObj.IsObject()) {
        os << "Object(";
        try {
            const std::any& customObj = varObj.getObject();
            if (customObj.has_value()) {
                // Generic handling for std::any
                os << "/* Custom Type: " << customObj.type().name() << " */";
            }
            else {
                os << "/* Empty std::any */";
            }
        }
        catch (...) {
            os << "/* Unprintable Custom Type */";
        }
        os << ")";
    }
    else {
        os << "Null";
    }
    return os;
}

// Function to retrieve varType
varType getVarType(const var& varObj) {
    if (varObj.isInt()) return varType::Int;
    if (varObj.isDouble()) return varType::Double;
    if (varObj.isString()) return varType::String;
    if (varObj.isArray()) return varType::Array;
    if (varObj.isTable()) return varType::Table;
    if (varObj.isPointer()) return varType::Pointer;
    if (varObj.isRawPointer()) return varType::RawPointer;
    if (varObj.isSharedPointer()) return varType::SharedPointer;
    if (varObj.isUniquePointer()) return varType::UniquePointer;
    if (varObj.isWeakPointer()) return varType::WeakPointer;
    if (varObj.IsObject()) return varType::Object; // Changed from Custom
    return varType::Null;
}

// ------------------------ Helper Functions Implementations ------------------------

// Boxing functions
var var::makeInt(int x) { return var(x); }
var var::makeDouble(double x) { return var(x); }
var var::makeString(const std::string& x) { return var(x); }
var var::makeString(std::string&& x) { return var(std::move(x)); }
var var::makeArray(const Array& arr) { return var(arr); }
var var::makeArray(Array&& arr) { return var(std::move(arr)); }
var var::makeTable(const Table& tbl) { return var(tbl); }
var var::makeTable(Table&& tbl) { return var(std::move(tbl)); }
var var::makePointer(const var& varObj) { return var(std::make_shared<var>(varObj)); }
var var::makePointer(var&& varObj) { return var(std::make_shared<var>(std::move(varObj))); }
var var::makeCustom(const std::any& customObj) { return var(customObj); }
var var::makeCustom(std::any&& customObj) { return var(std::move(customObj)); }

// Array functions
var var::newArray(const Array& arr) { return var(arr); }
var var::newArray(Array&& arr) { return var(std::move(arr)); }
var var::getElement(const var& arrayVar, size_t index) {
    if (!arrayVar.isArray()) throw std::runtime_error("var is not an Array");
    const Array& arr = arrayVar.getArray();
    if (index >= arr.size()) throw std::out_of_range("Index out of range");
    return arr[index];
}
void var::setElement(var& arrayVar, size_t index, const var& value) {
    if (!arrayVar.isArray()) throw std::runtime_error("var is not an Array");
    Array& arr = arrayVar.getArray();
    if (index >= arr.size()) arr.resize(index + 1, var());
    arr[index] = value;
}
void var::appendElement(var& arrayVar, const var& value) {
    if (!arrayVar.isArray()) throw std::runtime_error("var is not an Array");
    arrayVar.getArray().emplace_back(value);
}

// Table functions
var var::newTable(const Table& tbl) { return var(tbl); }
var var::newTable(Table&& tbl) { return var(std::move(tbl)); }
var var::getElement(const var& tableVar, const std::string& key) {
    if (!tableVar.isTable()) throw std::runtime_error("var is not a Table");
    const Table& tbl = tableVar.getTable();
    auto it = tbl.find(key);
    if (it == tbl.end()) throw std::out_of_range("Key not found");
    return it->second;
}
void var::setElement(var& tableVar, const std::string& key, const var& value) {
    if (!tableVar.isTable()) throw std::runtime_error("var is not a Table");
    tableVar.getTable()[key] = value;
}

// Utility functions
size_t var::len(const var& varObj) {
    if (varObj.isArray()) return varObj.getArray().size();
    if (varObj.isTable()) return varObj.getTable().size();
    throw std::runtime_error("var is neither Array nor Table");
}

var var::range(int start, int end, int step) {
    if (step == 0) throw std::invalid_argument("Step cannot be zero");
    Array arr;
    if (step > 0) {
        for (int i = start; i < end; i += step) {
            arr.emplace_back(makeInt(i));
        }
    }
    else {
        for (int i = start; i > end; i += step) {
            arr.emplace_back(makeInt(i));
        }
    }
    return var(arr);
}

var var::range(int end) {
    return range(0, end, 1);
}

var var::slice(const var& arrayVar, int start, int end, int step) {
    if (!arrayVar.isArray()) throw std::runtime_error("var is not an Array");
    const Array& arr = arrayVar.getArray();
    Array slicedArr;

    // Handle negative indices
    auto normalize = [&](int index) -> int {
        if (index < 0) return static_cast<int>(arr.size()) + index;
        return index;
        };

    start = normalize(start);
    end = normalize(end);

    if (step == 0) throw std::invalid_argument("Step cannot be zero");

    if (step > 0) {
        for (int i = start; i < end && i < static_cast<int>(arr.size()); i += step) {
            slicedArr.emplace_back(arr[i]);
        }
    }
    else {
        for (int i = start; i > end && i >= 0; i += step) {
            slicedArr.emplace_back(arr[i]);
        }
    }

    return var(slicedArr);
}
