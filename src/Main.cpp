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
    display->startDisplay(argc, argv);

    p.join();

    delete mem;
    exit(1);
}

int main(int argc, char* argv[]) {
    auto* display = new Display64x32;

    runCode(CodeLoader::loadProgram("../roms/Addition Problems [Paul C. Moews].ch8"), display, argc, argv);

    delete display;
    return 0;
}
