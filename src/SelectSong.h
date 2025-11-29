#ifndef SELECTSONG_H
#define SELECTSONG_H

#pragma once
#include "BaseMode.h"

class SelectSong : public BaseMode {
public:
    SelectSong(ofRectangle* handIconRect, int mode);
    ~SelectSong();

    void update() override;
    void draw() override;

private:
    std::vector<std::string> mp4Files;
    std::vector<ofRectangle> mp4Buttons;

    std::vector<ofImage> coverImages;
    ofImage currentBackgroundImage;

    std::vector<bool> isHoveringOverSong;
    float hoverStartTime;
    float hoverDuration;

    ofRectangle* handIconRect;
    int mode;
};

#endif // SELECTSONG_H
