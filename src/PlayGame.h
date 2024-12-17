#pragma once
#include "BaseMode.h"

class PlayGame : public BaseMode {
public:
    PlayGame(ofRectangle* handIconRect, std::string mp4Path);
    ~PlayGame();

    void update() override;
    void draw() override;

private:
    ofVideoPlayer videoPlayer;

    ofRectangle* handIconRect;
};
