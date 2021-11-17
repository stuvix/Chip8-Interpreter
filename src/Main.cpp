#include <iostream>
#include <thread>

#include "Memory.cpp"
#include "CodeLoader.cpp"
#include "../include/Processor.h"
#include "../include/RegisterFile.h"
#include "../include/Display.h"



static void runCode(program* prog, Display64x32* display) {
    auto* mem = new Memory();

    while (display->keyboardSingleton == nullptr) {
        sleep(1);
    }

    auto* processor = new Processor(display, mem, prog, display->keyboardSingleton);

    processor->runProgam();

    processor->dumpRegisters();

    delete mem;
    exit(1);
}

int main(int argc, char* argv[]) {
    //TODO change to qt5 for gui?

    //auto* prog = CodeLoader::loadProgram("../res/15 Puzzle [Roger Ivie].ch8");

    //CodeLoader::dumpCode(prog);

    auto* display = new Display64x32;

    std::thread d = display->startDisplayThread(argc, argv);

    //runCode(CodeLoader::loadProgram("../res/calltest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/endtest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/decrtest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/presstest1.ch8"), display);
    runCode(CodeLoader::loadProgram("../res/15 Puzzle [Roger Ivie].ch8"), display);

    d.join();

    delete display;
    return 0;
}
