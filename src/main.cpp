#include "../h/UnitTest.hpp"

extern "C" void trapHandler();

void main() {
    UnitTest& TestRunner = UnitTest::get();
    //TestRunner.test_new_delete();
    TestRunner.test_dequeue();
}


