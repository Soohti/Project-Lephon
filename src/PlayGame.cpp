#include "PlayGame.h"

#include <filesystem>

PlayGame::PlayGame(ofRectangle* handIconRect, std::string mp4Path)
    : handIconRect(handIconRect)
{
    videoPlayer.load(mp4Path);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.play();
    startTime = ofGetElapsedTimef();

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
                TapNote* tapNote = new TapNote(x, y, time);
                allNotes.push_back(tapNote);
            } else if (type == 2) {
                iss >> duration;
                HoldNote* holdNote = new HoldNote(x, y, time, duration);
                allNotes.push_back(holdNote);
            }
        }
    }
}

PlayGame::~PlayGame()
{
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

void PlayGame::update()
{
    videoPlayer.update();
    if (videoPlayer.getIsMovieDone()) {
        // ...existing code...
    }
    float currentTime = ofGetElapsedTimef() - startTime;
    // Add notes to activeNotes
    while (nextNoteIndex < allNotes.size()) {
        Note* note = allNotes[nextNoteIndex];
        float appearTime = note->time - 1.0f;
        if (currentTime >= appearTime) {
            activeNotes.push_back(note);
            nextNoteIndex++;
        } else {
            break; // Since allNotes is sorted, we can stop checking
        }
    }
    // Remove notes from activeNotes
    activeNotes.erase(
        std::remove_if(activeNotes.begin(), activeNotes.end(),
            [currentTime](Note* note) {
                float disappearTime = 0.0f;
                if (TapNote* tapNote = dynamic_cast<TapNote*>(note)) {
                    disappearTime = tapNote->time + 0.4f;
                } else if (HoldNote* holdNote = dynamic_cast<HoldNote*>(note)) {
                    disappearTime = holdNote->time + holdNote->duration + 0.2f;
                }
                return currentTime > disappearTime;
            }),
        activeNotes.end());
    // Update notes
    for (auto note : activeNotes) {
        note->update(currentTime, handIconRect);
    }
}

void PlayGame::draw()
{
    ofSetColor(255, 255, 255, 150); // Semi-transparent white
    if (videoPlayer.isLoaded()) {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    if (!inFile.is_open()) {
        ofDrawBitmapStringHighlight("Error: No chart file found", ofGetWidth() / 2 - 100, ofGetHeight() / 2, ofColor::red, ofColor::black);
        return;
    }
    // Draw notes
    ofSetColor(255, 255, 255, 255); // Full opacity
    float currentTime = ofGetElapsedTimef() - startTime;
    for (auto note : activeNotes) {
        note->draw(currentTime);
    }
}
