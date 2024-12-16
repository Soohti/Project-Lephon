#pragma once
#include "ofMain.h"

class BaseMode
{
public:
    virtual ~BaseMode() {}
    virtual void update() = 0; // Pure virtual methods
    virtual void draw() = 0;
    virtual void keyPressed(int key) = 0;
    virtual void keyReleased(int key) = 0;

    BaseMode *getNextMode()
    {
        return nextMode;
    }

protected:
    BaseMode *nextMode;
};