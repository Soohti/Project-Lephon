#include "PlayGame.h"

PlayGame::PlayGame(ofRectangle* handIconRect, std::string mp4Path) {
    this->handIconRect = handIconRect;
    videoPlayer.load(mp4Path);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.play();
}

PlayGame::~PlayGame() {
    videoPlayer.stop();
    videoPlayer.close();
}

void PlayGame::update() {
    videoPlayer.update();
    if (videoPlayer.getIsMovieDone()) {
        nextMode = new MainMenu(handIconRect);
    }
}

void PlayGame::draw() {
    ofSetColor(255, 255, 255, 150); // Semi-transparent white
    if (videoPlayer.isLoaded()) {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}
