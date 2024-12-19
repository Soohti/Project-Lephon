#pragma once
#include "BaseMode.h"

class EndScreen : public BaseMode {
public:
    EndScreen(int totalNotes, int greatNotes, int mode, std::string mp4Path);
    ~EndScreen();

    void update() override;
    void draw() override;

private:
    int totalNotes, greatNotes, mode;
    std::string mp4Path;
    ofImage background;
};
