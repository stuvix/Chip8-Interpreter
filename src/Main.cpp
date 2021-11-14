#include <iostream>

#include "Memory.cpp"
#include "CodeLoader.cpp"
#include "../include/RegisterFile.h"
#include "../include/Display.h"



int main() {
    std::cout << "Hello, World!" << std::endl;

    auto* prog = CodeLoader::loadProgram("../res/15 Puzzle [Roger Ivie].ch8");

    CodeLoader::dumpCode(prog);

    /*auto d = new Display64x32();
    d->printToConsole();*/

    return 0;
}
