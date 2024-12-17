#pragma once
#include "ofMain.h"

class Note {
public:
    float x, y, time;
    virtual void draw() = 0;
};

class TapNote : public Note {
public:
    void draw() override {
        ofDrawCircle(x, y, 10);
    }
};

class HoldNote : public Note {
public:
    float duration;

    void draw() override {
        ofDrawRectangle(x - 5, y, 10, duration);
    }
};
