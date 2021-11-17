//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <bitset>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "Keyboard.h"


class Display64x32 {
private:
    //uint64_t array[32] = {0};
    uint8_t mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};


public:
    Keyboard* keyboardSingleton = nullptr;
    QImage* canvas;
    QLabel* label;
#define DPI 16

    Display64x32() {
    }

    ~Display64x32() {

    }

    void startDisplay(int argc, char* argv[]) {
        QApplication app(argc, argv);

        auto* window = new Keyboard();
        window->resize(250, 150);
        window->setWindowTitle("Chip-8 Display");
        window->show();

        this->canvas = new QImage(64 * DPI, 32 * DPI, QImage::Format_Mono);
        canvas->fill(0);
        this->label = new QLabel();
        label->setPixmap(QPixmap::fromImage(*canvas));
        label->show();

        Display64x32::keyboardSingleton = window;

        QApplication::exec();

        delete window;
        delete label;
        delete canvas;
    }

    std::thread startDisplayThread(int argc, char* argv[]) {
        return std::thread([=] { startDisplay(argc, argv); });
    }

    bool xorPixel(int x, int y, int spritePixel) {
        //canvas->setPixel(realX, realY, 1);
        //p = canvas->pixelColor(realX, realY);
        for (int realX = x * DPI; realX < (x+1) * DPI; realX++) {
            for (int realY = y * DPI; realY < (y+1) * DPI; ++realY) {
                canvas->setPixel(realX, realY, canvas->pixelColor(realX, realY).value() ^ spritePixel);
            }
        }
        return canvas->pixelColor(x * DPI, y * DPI).value() > 0 && spritePixel > 0;
    }

    void clear() {
        canvas->fill(0);
        label->setPixmap(QPixmap::fromImage(*canvas));
        //todo update label?
    }


    bool addSprite(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t spriteSize) {
        bool collision = false;

        for (int j = 0; j < spriteSize; ++j) {
            for (int i = 0; i < 8; ++i) {
                collision |= xorPixel((x+i) % 64, (y+j) % 32, sprite[j] & mask[j] > 0 ? 1 : 0);
            }
        }
        label->setPixmap(QPixmap::fromImage(*canvas));
        return collision; //true if a collision happens
    }


};



#endif //CHIP8_DISPLAY_H
