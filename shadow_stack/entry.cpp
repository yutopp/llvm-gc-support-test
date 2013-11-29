#include <iostream>

extern "C" void f();

int main()
{
    std::cout << "begin -> into LLVM world" << std::endl;
    f();
    std::cout << "<- exit from LLVM world. end" << std::endl;
}
