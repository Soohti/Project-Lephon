#pragma once
#include "BaseMode.h"

#include <fstream>

class CreateChart : public BaseMode {
public:
    CreateChart(ofRectangle* handIconRect, std::string mp4Path);
    ~CreateChart();

    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;

    static constexpr int CODE = 2;

private:
    ofVideoPlayer videoPlayer;
    float startTime;
    bool holding;
    float holdStartTime;
    int totalNotes;
    std::ofstream outFile;

    ofRectangle* handIconRect;
};
