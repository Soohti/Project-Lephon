#ifndef MAINMENU_H
#define MAINMENU_H

#pragma once
#include "BaseMode.h"

class MainMenu : public BaseMode {
public:
    MainMenu(ofRectangle* handIconRect);
    ~MainMenu();

    void update() override;
    void draw() override;
    bool streamWebcam() override;

private:
    ofRectangle playGameButton;
    ofRectangle createChartButton;
    bool isHoveringPlayGame;
    bool isHoveringCreateChart;
    float hoverStartTime;
    float hoverDuration;

    ofRectangle* handIconRect;
};

#endif // MAINMENU_H
