#include "HighCpp.h"
#include <cassert>

// A sample custom class to store in std::any
class MyClass {
public:
    std::string name;
    int value;

    MyClass(const std::string& n, int v) : name(n), value(v) {}

    friend std::ostream& operator<<(std::ostream& os, const MyClass& obj) {
        os << "MyClass(name: " << obj.name << ", value: " << obj.value << ")";
        return os;
    }
};

int main() {
    try {
        // 1. Creating var objects with different types
        var intVar = makeInt(42);
        var doubleVar = makeDouble(3.14159);
        var stringVar = makeString("Hello, World!");

        // 2. Creating and manipulating an array
        var arrayVar = makeArray(Array{ makeInt(1), makeInt(2), makeInt(3) });
        appendElement(arrayVar, makeInt(4));
        setElement(arrayVar, 5, makeInt(6)); // This will resize the array
        std::cout << "Array Var: " << arrayVar << std::endl;

        // 3. Creating and manipulating a table
        var tableVar = makeTable(Table{
            { "key1", makeString("value1") },
            { "key2", makeDouble(2.718) },
            { "key3", makeInt(100) }
            });
        setElement(tableVar, "key4", makeString("value4"));
        std::cout << "Table Var: " << tableVar << std::endl;

        // 4. Handling shared_ptr
        var sharedPtrVar = var::makeSmartPointer(std::make_shared<var>(makeString("Shared Pointer")));
        std::cout << "Shared Pointer Var: " << sharedPtrVar << std::endl;

        // 5. Handling unique_ptr
        var uniquePtrVar = var::makeSmartPointer(std::make_unique<var>(makeInt(999)));
        std::cout << "Unique Pointer Var: " << uniquePtrVar << std::endl;

        // 6. Handling raw pointers
        int rawInt = 555;
        var rawPtrVar = var(&rawInt);
        std::cout << "Raw Pointer Var: " << rawPtrVar << std::endl;

        // 7. Handling weak_ptr
        std::shared_ptr<var> sharedForWeak = std::make_shared<var>(makeDouble(6.626));
        var weakPtrVar = var::makeSmartPointer(std::weak_ptr<var>(sharedForWeak));
        std::cout << "Weak Pointer Var: " << weakPtrVar << std::endl;

        // 8. Handling custom objects with std::any
        std::shared_ptr<MyClass> myClassPtr = std::make_shared<MyClass>("TestObject", 123);
        var customVar = makeCustom(myClassPtr);
        std::cout << "Custom Object Var: " << customVar << std::endl;

        // Retrieving the custom object
        if (customVar.IsObject()) {
            auto retrievedPtr = std::any_cast<std::shared_ptr<MyClass>>(customVar.getObject());
            if (retrievedPtr) {
                std::cout << "Retrieved Custom Object: " << *retrievedPtr << std::endl;
            }
        }

        // 9. Using range and slice functions
        var rangeVar = range(0, 10, 2);
        std::cout << "Range Var (0 to 10 step 2): " << rangeVar << std::endl;

        var slicedVar = slice(arrayVar, 1, 6, 2);
        std::cout << "Sliced Array Var (1 to 6 step 2): " << slicedVar << std::endl;

        // 10. Demonstrating len function
        std::cout << "Length of Array Var: " << len(arrayVar) << std::endl;
        std::cout << "Length of Table Var: " << len(tableVar) << std::endl;

        // 11. Demonstrating deep copy
        var copiedVar = intVar; // Copying an int var
        std::cout << "Copied Int Var: " << copiedVar << std::endl;

        var deepCopiedArray = arrayVar; // Deep copy of array
        std::cout << "Deep Copied Array Var: " << deepCopiedArray << std::endl;

        // Modifying the original array to ensure deep copy
        setElement(arrayVar, 0, makeInt(100));
        std::cout << "Original Array Var after modification: " << arrayVar << std::endl;
        std::cout << "Deep Copied Array Var remains unchanged: " << deepCopiedArray << std::endl;

        // 12. Demonstrating that copying unique_ptr_var sets it to nullptr
        var uniqueCopyVar = uniquePtrVar; // Should be nullptr due to copy prohibition
        std::cout << "Copied Unique Pointer Var: " << uniqueCopyVar << std::endl;

    }
    catch (const std::exception& ex) {
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
    }

    return 0;
}