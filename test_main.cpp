// test_main.cpp
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "my_functions.h"
void runMainLogic();  // Declare runMainLogic

// Helper function to capture std::cout output
std::string CaptureStdOut(void (*func)()) {
    std::ostringstream oss;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    
    func();
    
    std::cout.rdbuf(original_cout);
    return oss.str();
}

// Test case for func()
TEST(FuncTest, OutputCheck) {
    std::string output = CaptureStdOut(func);
    EXPECT_EQ(output, "Print from a simple func\n");
}

// Test case for NeverCalled
TEST(NeverCalledTest, OutputCheck) {
    std::string output = CaptureStdOut(NeverCalled);
    EXPECT_EQ(output, "This is never called\n");
}

// Test case for main logic
TEST(MainTest, OutputCheck) {
    std::string output = CaptureStdOut(runMainLogic);
    EXPECT_EQ(output, "Just a simple main function\nPrint from a simple func\n");
}
 