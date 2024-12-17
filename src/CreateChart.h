#pragma once
#include "BaseMode.h"

class CreateChart : public BaseMode {
public:
    CreateChart(ofRectangle* handIconRect, std::string mp4Path);
    ~CreateChart();

    void update() override;
    void draw() override;

private:
    ofVideoPlayer videoPlayer;

    ofRectangle* handIconRect;
};
