#include "CreateChart.h"

CreateChart::CreateChart(ofRectangle* handIconRect, std::string mp4Path) {
    this->handIconRect = handIconRect;
    videoPlayer.load(mp4Path);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.play();
}

CreateChart::~CreateChart() {
    videoPlayer.stop();
    videoPlayer.close();
}

void CreateChart::update() {
    videoPlayer.update();
    if (videoPlayer.getIsMovieDone()) {
        nextMode = new MainMenu(handIconRect);
    }
}

void CreateChart::draw() {
    ofSetColor(255, 255, 255, 150); // Semi-transparent white
    if (videoPlayer.isLoaded()) {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}
