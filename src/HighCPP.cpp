#include "HighCPP.h"

// ============================================================
// Internal helpers
// ============================================================

namespace {

enum class NumType { Int, Double, None };

struct NumVal {
    NumType type = NumType::None;
    int i = 0;
    double d = 0.0;
};

NumVal toNumeric(const var& v) {
    if (v.isInt()) return { NumType::Int, v.getInt(), 0.0 };
    if (v.isDouble()) return { NumType::Double, 0, v.getDouble() };
    if (v.isBool()) return { NumType::Int, v.getBool() ? 1 : 0, 0.0 };
    return { NumType::None, 0, 0.0 };
}

std::string varToString(const var& v) {
    if (v.isString()) return v.getString();
    if (v.isInt()) return std::to_string(v.getInt());
    if (v.isDouble()) {
        std::ostringstream oss;
        oss << v.getDouble();
        return oss.str();
    }
    if (v.isBool()) return v.getBool() ? "true" : "false";
    if (v.isNull()) return "null";
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

} // anonymous namespace

// ============================================================
// Constructors
// ============================================================

var::var() : value(std::monostate{}) {}
var::var(bool v) : value(v) {}
var::var(int v) : value(v) {}
var::var(double v) : value(v) {}
var::var(const std::string& v) : value(v) {}
var::var(std::string&& v) : value(std::move(v)) {}
var::var(const char* v) : value(std::string(v)) {}
var::var(const Array& v) : value(v) {}
var::var(Array&& v) : value(std::move(v)) {}
var::var(const Table& v) : value(v) {}
var::var(Table&& v) : value(std::move(v)) {}
var::var(const Function& v) : value(v) {}
var::var(Function&& v) : value(std::move(v)) {}
var::var(const std::any& v) : value(v) {}
var::var(std::any&& v) : value(std::move(v)) {}

// ============================================================
// Copy Constructor (deep copy)
// ============================================================

var::var(const var& other) {
    switch (other.value.index()) {
    case 0: value = std::monostate{}; break;
    case 1: value = std::get<bool>(other.value); break;
    case 2: value = std::get<int>(other.value); break;
    case 3: value = std::get<double>(other.value); break;
    case 4: value = std::get<std::string>(other.value); break;
    case 5: value = std::get<Array>(other.value); break;
    case 6: value = std::get<Table>(other.value); break;
    case 7: value = std::get<Function>(other.value); break;
    case 8: value = std::get<std::any>(other.value); break;
    default:
        throw std::runtime_error("Unknown var type during copy construction.");
    }
}

// ============================================================
// Copy Assignment (deep copy)
// ============================================================

var& var::operator=(const var& other) {
    if (this == &other) return *this;
    value = std::monostate{};

    switch (other.value.index()) {
    case 0: value = std::monostate{}; break;
    case 1: value = std::get<bool>(other.value); break;
    case 2: value = std::get<int>(other.value); break;
    case 3: value = std::get<double>(other.value); break;
    case 4: value = std::get<std::string>(other.value); break;
    case 5: value = std::get<Array>(other.value); break;
    case 6: value = std::get<Table>(other.value); break;
    case 7: value = std::get<Function>(other.value); break;
    case 8: value = std::get<std::any>(other.value); break;
    default:
        throw std::runtime_error("Unknown var type during copy assignment.");
    }
    return *this;
}

// ============================================================
// Assignment Operators
// ============================================================

var& var::operator=(bool v)               { value = v; return *this; }
var& var::operator=(int v)                { value = v; return *this; }
var& var::operator=(double v)             { value = v; return *this; }
var& var::operator=(const std::string& v) { value = v; return *this; }
var& var::operator=(std::string&& v)      { value = std::move(v); return *this; }
var& var::operator=(const char* v)        { value = std::string(v); return *this; }
var& var::operator=(const Array& v)       { value = v; return *this; }
var& var::operator=(Array&& v)            { value = std::move(v); return *this; }
var& var::operator=(const Table& v)       { value = v; return *this; }
var& var::operator=(Table&& v)            { value = std::move(v); return *this; }
var& var::operator=(const Function& v)    { value = v; return *this; }
var& var::operator=(Function&& v)         { value = std::move(v); return *this; }
var& var::operator=(const std::any& v)    { value = v; return *this; }
var& var::operator=(std::any&& v)         { value = std::move(v); return *this; }

// ============================================================
// Type Checking
// ============================================================

bool var::isNull() const     { return std::holds_alternative<std::monostate>(value); }
bool var::isBool() const     { return std::holds_alternative<bool>(value); }
bool var::isInt() const      { return std::holds_alternative<int>(value); }
bool var::isDouble() const   { return std::holds_alternative<double>(value); }
bool var::isNumber() const   { return isInt() || isDouble(); }
bool var::isString() const   { return std::holds_alternative<std::string>(value); }
bool var::isArray() const    { return std::holds_alternative<Array>(value); }
bool var::isTable() const    { return std::holds_alternative<Table>(value); }
bool var::isFunction() const { return std::holds_alternative<Function>(value); }
bool var::isObject() const   { return std::holds_alternative<std::any>(value); }

// ============================================================
// Getters
// ============================================================

bool var::getBool() const {
    if (!isBool()) throw std::bad_variant_access();
    return std::get<bool>(value);
}

int var::getInt() const {
    if (!isInt()) throw std::bad_variant_access();
    return std::get<int>(value);
}

double var::getDouble() const {
    if (!isDouble()) throw std::bad_variant_access();
    return std::get<double>(value);
}

double var::toNumber() const {
    if (isInt()) return static_cast<double>(getInt());
    if (isDouble()) return getDouble();
    throw std::runtime_error("Cannot convert " + typeOf() + " to number");
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

const Function& var::getFunction() const {
    if (!isFunction()) throw std::bad_variant_access();
    return std::get<Function>(value);
}

Function& var::getFunction() {
    if (!isFunction()) throw std::bad_variant_access();
    return std::get<Function>(value);
}

const std::any& var::getObject() const {
    if (!isObject()) throw std::bad_variant_access();
    return std::get<std::any>(value);
}

std::any& var::getObject() {
    if (!isObject()) throw std::bad_variant_access();
    return std::get<std::any>(value);
}

// ============================================================
// typeOf()
// ============================================================

std::string var::typeOf() const {
    if (isNull())     return "null";
    if (isBool())     return "bool";
    if (isInt())      return "int";
    if (isDouble())   return "double";
    if (isString())   return "string";
    if (isArray())    return "array";
    if (isTable())    return "table";
    if (isFunction()) return "function";
    if (isObject())   return "object";
    return "null";
}

// ============================================================
// Truthiness: operator bool()
// ============================================================

var::operator bool() const {
    if (isNull())     return false;
    if (isBool())     return getBool();
    if (isInt())      return getInt() != 0;
    if (isDouble())   return getDouble() != 0.0;
    if (isString())   return !getString().empty();
    if (isArray())    return !getArray().empty();
    if (isTable())    return !getTable().empty();
    if (isFunction()) return true;
    if (isObject())   return getObject().has_value();
    return false;
}

// ============================================================
// VarProxy
// ============================================================

var::VarProxy::VarProxy(var& owner, int index) : owner(owner), key(index) {}
var::VarProxy::VarProxy(var& owner, const std::string& k) : owner(owner), key(k) {}
var::VarProxy::VarProxy(var& owner, const char* k) : owner(owner), key(std::string(k)) {}

namespace {
    // Resolve an int key to a valid array index (with negative indexing)
    int resolveIndex(int idx, size_t size) {
        if (idx < 0) idx += static_cast<int>(size);
        return idx;
    }

    // Get a mutable reference to the nested var that a proxy's key points to
    var& resolveRef(var& owner, const std::variant<int, std::string>& key) {
        if (std::holds_alternative<int>(key)) {
            if (!owner.isArray()) throw std::runtime_error("Cannot index non-array with int");
            Array& arr = owner.getArray();
            int idx = resolveIndex(std::get<int>(key), arr.size());
            if (idx < 0 || idx >= static_cast<int>(arr.size()))
                throw std::out_of_range("Array index out of range");
            return arr[idx];
        }
        // string key
        if (!owner.isTable()) throw std::runtime_error("Cannot index non-table with string");
        return owner.getTable()[std::get<std::string>(key)];
    }
}

var::VarProxy::operator var() const {
    if (std::holds_alternative<int>(key)) {
        if (!owner.isArray()) throw std::runtime_error("Cannot index non-array with int");
        const Array& arr = owner.getArray();
        int idx = resolveIndex(std::get<int>(key), arr.size());
        if (idx < 0 || idx >= static_cast<int>(arr.size()))
            throw std::out_of_range("Array index out of range");
        return arr[idx];
    }
    // string key
    if (!owner.isTable()) throw std::runtime_error("Cannot index non-table with string");
    const Table& tbl = owner.getTable();
    auto it = tbl.find(std::get<std::string>(key));
    if (it == tbl.end()) return var();
    return it->second;
}

var::VarProxy& var::VarProxy::operator=(const var& val) {
    if (std::holds_alternative<int>(key)) {
        if (!owner.isArray()) throw std::runtime_error("Cannot index non-array with int");
        Array& arr = owner.getArray();
        int idx = resolveIndex(std::get<int>(key), arr.size());
        if (idx < 0) throw std::out_of_range("Negative index out of range");
        if (idx >= static_cast<int>(arr.size())) arr.resize(idx + 1, var());
        arr[idx] = val;
    } else {
        if (!owner.isTable()) throw std::runtime_error("Cannot index non-table with string");
        owner.getTable()[std::get<std::string>(key)] = val;
    }
    return *this;
}

var::VarProxy& var::VarProxy::operator=(const VarProxy& other) {
    var resolved = static_cast<var>(other);
    return operator=(resolved);
}

var::VarProxy& var::VarProxy::operator+=(const var& rhs) {
    var current = static_cast<var>(*this);
    return operator=(current + rhs);
}

var::VarProxy& var::VarProxy::operator-=(const var& rhs) {
    var current = static_cast<var>(*this);
    return operator=(current - rhs);
}

var::VarProxy& var::VarProxy::operator*=(const var& rhs) {
    var current = static_cast<var>(*this);
    return operator=(current * rhs);
}

var::VarProxy& var::VarProxy::operator/=(const var& rhs) {
    var current = static_cast<var>(*this);
    return operator=(current / rhs);
}

var::VarProxy& var::VarProxy::operator%=(const var& rhs) {
    var current = static_cast<var>(*this);
    return operator=(current % rhs);
}

var::VarProxy var::VarProxy::operator[](int index) {
    return VarProxy(resolveRef(owner, key), index);
}

var::VarProxy var::VarProxy::operator[](const std::string& k) {
    return VarProxy(resolveRef(owner, key), k);
}

var::VarProxy var::VarProxy::operator[](const char* k) {
    return operator[](std::string(k));
}

// ============================================================
// operator[] on var
// ============================================================

var::VarProxy var::operator[](int index) { return VarProxy(*this, index); }
var::VarProxy var::operator[](const std::string& key) { return VarProxy(*this, key); }
var::VarProxy var::operator[](const char* key) { return VarProxy(*this, key); }

var var::operator[](int index) const {
    if (!isArray()) throw std::runtime_error("Cannot index non-array with int");
    const Array& arr = getArray();
    int idx = index;
    if (idx < 0) idx += static_cast<int>(arr.size());
    if (idx < 0 || idx >= static_cast<int>(arr.size()))
        throw std::out_of_range("Array index out of range");
    return arr[idx];
}

var var::operator[](const std::string& key) const {
    if (!isTable()) throw std::runtime_error("Cannot index non-table with string");
    const Table& tbl = getTable();
    auto it = tbl.find(key);
    if (it == tbl.end()) return var();
    return it->second;
}

var var::operator[](const char* key) const {
    return operator[](std::string(key));
}

// ============================================================
// Arithmetic Operators
// ============================================================

var operator+(const var& lhs, const var& rhs) {
    // String concatenation if either side is a string
    if (lhs.isString() || rhs.isString()) {
        return var(varToString(lhs) + varToString(rhs));
    }
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type == NumType::None || r.type == NumType::None)
        throw std::runtime_error("Cannot add " + lhs.typeOf() + " and " + rhs.typeOf());
    if (l.type == NumType::Double || r.type == NumType::Double) {
        double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
        double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
        return var(lv + rv);
    }
    return var(l.i + r.i);
}

var operator-(const var& lhs, const var& rhs) {
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type == NumType::None || r.type == NumType::None)
        throw std::runtime_error("Cannot subtract " + lhs.typeOf() + " and " + rhs.typeOf());
    if (l.type == NumType::Double || r.type == NumType::Double) {
        double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
        double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
        return var(lv - rv);
    }
    return var(l.i - r.i);
}

var operator*(const var& lhs, const var& rhs) {
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type == NumType::None || r.type == NumType::None)
        throw std::runtime_error("Cannot multiply " + lhs.typeOf() + " and " + rhs.typeOf());
    if (l.type == NumType::Double || r.type == NumType::Double) {
        double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
        double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
        return var(lv * rv);
    }
    return var(l.i * r.i);
}

var operator/(const var& lhs, const var& rhs) {
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type == NumType::None || r.type == NumType::None)
        throw std::runtime_error("Cannot divide " + lhs.typeOf() + " and " + rhs.typeOf());
    if (l.type == NumType::Double || r.type == NumType::Double) {
        double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
        double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
        if (rv == 0.0) throw std::runtime_error("Division by zero");
        return var(lv / rv);
    }
    if (r.i == 0) throw std::runtime_error("Division by zero");
    return var(l.i / r.i);
}

var operator%(const var& lhs, const var& rhs) {
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type == NumType::None || r.type == NumType::None)
        throw std::runtime_error("Cannot modulo " + lhs.typeOf() + " and " + rhs.typeOf());
    if (l.type == NumType::Double || r.type == NumType::Double) {
        double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
        double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
        if (rv == 0.0) throw std::runtime_error("Modulo by zero");
        return var(std::fmod(lv, rv));
    }
    if (r.i == 0) throw std::runtime_error("Modulo by zero");
    return var(l.i % r.i);
}

var& var::operator+=(const var& rhs) { *this = *this + rhs; return *this; }
var& var::operator-=(const var& rhs) { *this = *this - rhs; return *this; }
var& var::operator*=(const var& rhs) { *this = *this * rhs; return *this; }
var& var::operator/=(const var& rhs) { *this = *this / rhs; return *this; }
var& var::operator%=(const var& rhs) { *this = *this % rhs; return *this; }

var var::operator-() const {
    if (isInt()) return var(-getInt());
    if (isDouble()) return var(-getDouble());
    throw std::runtime_error("Cannot negate " + typeOf());
}

var var::operator+() const {
    if (isInt() || isDouble()) return *this;
    throw std::runtime_error("Unary + not supported for " + typeOf());
}

// ============================================================
// Comparison Operators
// ============================================================

bool operator==(const var& lhs, const var& rhs) {
    if (lhs.isNull() && rhs.isNull()) return true;
    if (lhs.isNull() || rhs.isNull()) return false;

    if (lhs.isBool() && rhs.isBool()) return lhs.getBool() == rhs.getBool();

    // Numeric comparison with coercion
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type != NumType::None && r.type != NumType::None) {
        if (l.type == NumType::Double || r.type == NumType::Double) {
            double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
            double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
            return lv == rv;
        }
        return l.i == r.i;
    }

    if (lhs.isString() && rhs.isString()) return lhs.getString() == rhs.getString();

    if (lhs.isArray() && rhs.isArray()) {
        const Array& a = lhs.getArray();
        const Array& b = rhs.getArray();
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (!(a[i] == b[i])) return false;
        }
        return true;
    }

    return false;
}

bool operator!=(const var& lhs, const var& rhs) { return !(lhs == rhs); }

bool operator<(const var& lhs, const var& rhs) {
    // Numeric
    auto l = toNumeric(lhs);
    auto r = toNumeric(rhs);
    if (l.type != NumType::None && r.type != NumType::None) {
        if (l.type == NumType::Double || r.type == NumType::Double) {
            double lv = (l.type == NumType::Int) ? static_cast<double>(l.i) : l.d;
            double rv = (r.type == NumType::Int) ? static_cast<double>(r.i) : r.d;
            return lv < rv;
        }
        return l.i < r.i;
    }
    // String
    if (lhs.isString() && rhs.isString()) return lhs.getString() < rhs.getString();

    throw std::runtime_error("Cannot compare " + lhs.typeOf() + " and " + rhs.typeOf());
}

bool operator>(const var& lhs, const var& rhs)  { return rhs < lhs; }
bool operator<=(const var& lhs, const var& rhs) { return !(rhs < lhs); }
bool operator>=(const var& lhs, const var& rhs) { return !(lhs < rhs); }

// ============================================================
// Method-Style API
// ============================================================

void var::push(const var& val) {
    if (!isArray()) throw std::runtime_error("push() requires an array");
    getArray().push_back(val);
}

var var::pop() {
    if (!isArray()) throw std::runtime_error("pop() requires an array");
    Array& arr = getArray();
    if (arr.empty()) throw std::runtime_error("pop() on empty array");
    var last = std::move(arr.back());
    arr.pop_back();
    return last;
}

var var::length() const {
    if (isArray())  return var(static_cast<int>(getArray().size()));
    if (isTable())  return var(static_cast<int>(getTable().size()));
    if (isString()) return var(static_cast<int>(getString().size()));
    throw std::runtime_error("length() not supported for " + typeOf());
}

bool var::contains(const var& val) const {
    if (!isArray()) throw std::runtime_error("contains() requires an array");
    for (const auto& item : getArray()) {
        if (item == val) return true;
    }
    return false;
}

var var::keys() const {
    if (!isTable()) throw std::runtime_error("keys() requires a table");
    Array result;
    for (const auto& [k, v] : getTable()) {
        result.push_back(var(k));
    }
    return var(std::move(result));
}

var var::values() const {
    if (!isTable()) throw std::runtime_error("values() requires a table");
    Array result;
    for (const auto& [k, v] : getTable()) {
        result.push_back(v);
    }
    return var(std::move(result));
}

bool var::has(const std::string& key) const {
    if (!isTable()) throw std::runtime_error("has() requires a table");
    return getTable().count(key) > 0;
}

// ============================================================
// Iteration
// ============================================================

Array::iterator var::begin() {
    if (!isArray()) throw std::runtime_error("Cannot iterate non-array");
    return getArray().begin();
}

Array::iterator var::end() {
    if (!isArray()) throw std::runtime_error("Cannot iterate non-array");
    return getArray().end();
}

Array::const_iterator var::begin() const {
    if (!isArray()) throw std::runtime_error("Cannot iterate non-array");
    return getArray().begin();
}

Array::const_iterator var::end() const {
    if (!isArray()) throw std::runtime_error("Cannot iterate non-array");
    return getArray().end();
}

// ============================================================
// operator<<
// ============================================================

std::ostream& operator<<(std::ostream& os, const var& v) {
    if (v.isNull()) {
        os << "null";
    } else if (v.isBool()) {
        os << (v.getBool() ? "true" : "false");
    } else if (v.isInt()) {
        os << v.getInt();
    } else if (v.isDouble()) {
        os << v.getDouble();
    } else if (v.isString()) {
        os << '"' << v.getString() << '"';
    } else if (v.isArray()) {
        os << "[";
        const Array& arr = v.getArray();
        for (size_t i = 0; i < arr.size(); ++i) {
            if (i > 0) os << ", ";
            os << arr[i];
        }
        os << "]";
    } else if (v.isTable()) {
        os << "{";
        const Table& tbl = v.getTable();
        bool first = true;
        for (const auto& [key, val] : tbl) {
            if (!first) os << ", ";
            os << '"' << key << "\": " << val;
            first = false;
        }
        os << "}";
    } else if (v.isFunction()) {
        os << "function";
    } else if (v.isObject()) {
        os << "Object(";
        try {
            const std::any& obj = v.getObject();
            if (obj.has_value()) {
                os << obj.type().name();
            } else {
                os << "empty";
            }
        } catch (...) {
            os << "unknown";
        }
        os << ")";
    }
    return os;
}

// ============================================================
// Static Utilities
// ============================================================

size_t var::len(const var& v) {
    if (v.isArray()) return v.getArray().size();
    if (v.isTable()) return v.getTable().size();
    if (v.isString()) return v.getString().size();
    throw std::runtime_error("len() not supported for " + v.typeOf());
}

var var::range(int start, int end, int step) {
    if (step == 0) throw std::invalid_argument("Step cannot be zero");
    Array arr;
    if (step > 0) {
        for (int i = start; i < end; i += step) {
            arr.emplace_back(i);
        }
    } else {
        for (int i = start; i > end; i += step) {
            arr.emplace_back(i);
        }
    }
    return var(std::move(arr));
}

var var::range(int end) {
    return range(0, end, 1);
}

var var::slice(const var& v, int start, int end, int step) {
    if (!v.isArray()) throw std::runtime_error("slice() requires an array");
    const Array& arr = v.getArray();
    Array result;

    auto normalize = [&](int index) -> int {
        if (index < 0) return static_cast<int>(arr.size()) + index;
        return index;
    };

    start = normalize(start);
    end = normalize(end);

    if (step == 0) throw std::invalid_argument("Step cannot be zero");

    if (step > 0) {
        for (int i = start; i < end && i < static_cast<int>(arr.size()); i += step) {
            if (i >= 0) result.emplace_back(arr[i]);
        }
    } else {
        for (int i = start; i > end && i >= 0; i += step) {
            if (i < static_cast<int>(arr.size())) result.emplace_back(arr[i]);
        }
    }

    return var(std::move(result));
}
