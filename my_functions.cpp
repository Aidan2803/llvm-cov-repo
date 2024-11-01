// my_functions.cpp
#include <iostream>
#include "my_functions.h"

void func() {
    std::cout << "Print from a simple func\n";
}

void NeverCalled() {
    std::cout << "This is never called\n";
}

// Moved runMainLogic here so it can be accessed in tests
void runMainLogic() {
    std::cout << "Just a simple main function\n";
    func();
}

void NotCoveredWithTests(){
    std::cout << "Wont be covered\n";
}