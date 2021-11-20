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
#include <QtMultimedia/QSoundEffect>
#include "Keyboard.h"


class Display64x32 : public QObject{
    Q_OBJECT
private:
    //uint64_t array[32] = {0};
    uint8_t mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    QSoundEffect beeping;

public:
    Keyboard* keyboardSingleton = nullptr;
    QImage* canvas;
    QLabel* label;
#define DPI 8

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
        //this->canvas->scanLine(27)[20] = 0xff;
        this->label = new QLabel();
        label->setPixmap(QPixmap::fromImage(*canvas));

        label->show();

        //sound stuff
        beeping.setSource(QUrl::fromLocalFile("../res/loop_beep.wav"));
        beeping.setLoopCount(QSoundEffect::Infinite);
        beeping.setVolume(0.15f);

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
        uint8_t pixelToInsert = ((this->canvas->scanLine(y * DPI)[x * DPI /8] & 0x8) >> 3) ^ spritePixel;
        if (pixelToInsert == 1){
            pixelToInsert = 0xfe;
        }
        assert(pixelToInsert == 0 || pixelToInsert == 0xfe);

        for (int realX = x * DPI / 8; realX < (x+1) * DPI / 8 ; realX++) {
            for (int realY = y * DPI; realY < (y+1) * DPI - 1; ++realY) {
                //std::cout << realX << " " << realY << " " << (canvas->pixelColor(x * DPI, y * DPI).value()) << std::endl;
                this->canvas->scanLine(realY)[realX] = pixelToInsert;
                //std::cout << "Pixels: " << realX << " " << realY << " " << +pixelToInsert << std::endl;
            }
        }
        return spritePixel != pixelToInsert;
    }


    public slots:
    void clear() {
        canvas->fill(0);
        label->setPixmap(QPixmap::fromImage(*canvas));
        //todo update label?
    }

    // retAndDone is set to 3 until this method is done, then execution can continue
    void addSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t spriteSize, uint8_t* retAndDone) {
        bool collision = false;
        for (int j = 0; j < spriteSize; ++j) {
            for (int i = 0; i < 8; ++i) {
                bool changed = xorPixel((x+i) % 64, (y+j) % 32, (sprite[j] & mask[7-i]) > 0 ? 1 : 0);
                //std::cout << "Printed pixel: " << (x+i) % 64 << ", " << (y+j) % 32 << " " << ((sprite[j] & mask[i]) > 0 ? 1 : 0) << std::endl;
                //std::cout << ((sprite[j] & mask[7-i]) > 0 ? 1 : 0);
                collision = collision || changed;
            }
            //std::cout << std::endl;
        }
        label->setPixmap(QPixmap::fromImage(*canvas));
        //label->show();
        label->update(); //todo try the update inside a key press event and see
        *retAndDone = collision ? 1 : 0; //true if a collision happens
    }

    void playBeep() {
        this->beeping.play();
    }

    void stopBeep() {
        this->beeping.stop();
    }
};



#endif //CHIP8_DISPLAY_H
