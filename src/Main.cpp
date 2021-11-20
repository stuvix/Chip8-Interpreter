#include <iostream>
#include <thread>
#include <QtCore//QCoreApplication>


#include "Memory.cpp"
#include "CodeLoader.cpp"
#include "../include/Processor.h"




static void runCode(program* prog, Display64x32* display, int argc, char* argv[]) {
    auto* mem = new Memory();

    auto* processor = new Processor(display, mem, prog);


    qRegisterMetaType<uint8_t>("uint8_t");
    QObject::connect(processor, SIGNAL(requestClearScreen()), display, SLOT(clear()));
    QObject::connect(processor, &Processor::requestSpriteOnScreen,
                     display, &Display64x32::addSprite);


    std::thread p = std::thread([=] { processor->runProgam(); });
    /*std::thread d = */display->startDisplay(argc, argv);

    processor->dumpRegisters();

    p.join();

    delete mem;
    exit(1);
}

int main(int argc, char* argv[]) {

    //auto* prog = CodeLoader::loadProgram("../res/15 Puzzle [Roger Ivie].ch8");

    //CodeLoader::dumpCode(prog);

    auto* display = new Display64x32;


    //runCode(CodeLoader::loadProgram("../res/calltest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/endtest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/decrtest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/presstest1.ch8"), display);

    runCode(CodeLoader::loadProgram("../res/IBM Logo.ch8"), display, argc, argv);
    //runCode(CodeLoader::loadProgram("../res/15 Puzzle [Roger Ivie].ch8"), display, argc, argv);
    //runCode(CodeLoader::loadProgram("../res/Guess [David Winter].ch8"), display, argc, argv);

    delete display;
    return 0;
}
