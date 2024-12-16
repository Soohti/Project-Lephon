#pragma once
#include "BaseMode.h"

class MainMenu : public BaseMode {
public:
    MainMenu();
    ~MainMenu();

    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void keyReleased(int key) override;

private:
    ofRectangle playGameButton;
    ofRectangle createChartButton;
    bool isHoveringPlayGame;
    bool isHoveringCreateChart;
    float hoverStartTime;
    float hoverDuration;

    ofImage handIcon;
    ofRectangle handIconRect;
    float confidenceThreshold;

    BaseMode* nextMode;
};
