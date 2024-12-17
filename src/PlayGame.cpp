#include "PlayGame.h"

#include <filesystem>

PlayGame::PlayGame(ofRectangle* handIconRect, std::string mp4Path) {
    this->handIconRect = handIconRect;
    videoPlayer.load(mp4Path);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.play();

    // check for .txt or _gen.txt file
    std::string filename = mp4Path.substr(0, mp4Path.size() - 4);
    std::vector<std::string> potentialPaths = { filename + ".txt", filename + "_gen.txt" };
    for (const auto& path : potentialPaths) {
        if (std::filesystem::exists(path)) {
            inFile.open(path);
            break;
        }
    }
    if (inFile.is_open()) {
        std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        int type;
        float x, y, time, duration;
        iss >> type >> x >> y >> time;
        if (type == 1) {
            TapNote* tapNote = new TapNote();
            tapNote->x = x;
            tapNote->y = y;
            tapNote->time = time;
            allNotes.push_back(tapNote);
        } else if (type == 2) {
            iss >> duration;
            HoldNote* holdNote = new HoldNote();
            holdNote->x = x;
            holdNote->y = y;
            holdNote->time = time;
            holdNote->duration = duration;
            allNotes.push_back(holdNote);
        }
    }
        
    }
}

PlayGame::~PlayGame() {
    videoPlayer.stop();
    videoPlayer.close();
    // Clean up notes
    for (auto note : allNotes) {
        delete note;
    }
    allNotes.clear();

    if (inFile.is_open()) {
        inFile.close();
    }
}

void PlayGame::update() {
    videoPlayer.update();
    if (videoPlayer.getIsMovieDone()) {
//        nextMode = new MainMenu(handIconRect);
    }
}

void PlayGame::draw() {
    ofSetColor(255, 255, 255, 150); // Semi-transparent white
    if (videoPlayer.isLoaded()) {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    if(!inFile.is_open()) {
        ofDrawBitmapStringHighlight("Error: No chart file found", ofGetWidth() / 2 - 100, ofGetHeight() / 2, ofColor::red, ofColor::black);
        return;
    }
}
