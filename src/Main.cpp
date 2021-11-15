#include <iostream>

#include "Memory.cpp"
#include "CodeLoader.cpp"
#include "../include/Processor.h"
#include "../include/RegisterFile.h"
#include "../include/Display.h"

#include <gtkmm/application.h>


static void runCode(program* prog) {
    auto* mem = new Memory();
    auto* display = new Display64x32();


    auto* processor = new Processor(display, mem, prog);
    processor->runProgam();

    processor->dumpRegisters();
}

int main() {
    //TODO change to qt5 for gui?

    //auto* prog = CodeLoader::loadProgram("../res/15 Puzzle [Roger Ivie].ch8");

    //CodeLoader::dumpCode(prog);

    /*auto d = new Display64x32();
    d->printToConsole();*/

    /*auto* mem = new Memory();
    mem->initDigits();
    mem->dumpMemory("../out/mem_dump");*/

    //runCode(CodeLoader::loadProgram("../res/calltest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/endtest.ch8"));
    //runCode(CodeLoader::loadProgram("../res/decrtest.ch8"));

    auto window = Gtk::Application::create("org.gtkmm.example");

    Display64x32 display;

    window->run(display);

    return 0;
}
