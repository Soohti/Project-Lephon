#include <CreateChart.h>
#include <EndScreen.h>
#include <PlayGame.h>

EndScreen::EndScreen(int totalNotes, int greatNotes, int mode, std::string mp4Path)
    : totalNotes(totalNotes)
    , greatNotes(greatNotes)
    , mode(mode)
    , mp4Path(mp4Path)
{
    std::string coverImagePath = mp4Path.substr(0, mp4Path.find_last_of('.')) + ".jpg";
    background.load(coverImagePath);
}

EndScreen::~EndScreen()
{
}

void EndScreen::update()
{
}

void EndScreen::draw()
{
    // draw background
    ofSetColor(ofColor::white, 150); // Semi-transparent white
    if (background.isAllocated()) {
        background.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    ofSetColor(ofColor::white); // Full opacity
    ofFill();
    // draw a rectangle for song name
    ofDrawRectangle(ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 100, 200, 50);
    // draw another rectangle for statistics
    ofDrawRectangle(ofGetWidth() / 2 - 100, ofGetHeight() / 2, 200, 60);
    // draw a rectangle for exit message
    ofDrawRectangle(ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 100, 200, 20);
    ofSetColor(ofColor::black);
    ofDrawBitmapString(mp4Path.substr(mp4Path.find_last_of('/') + 1, mp4Path.find_last_of('.') - mp4Path.find_last_of('/') - 1), ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 85);
    if (mode == PlayGame::CODE) {
        ofDrawBitmapString("Great Notes: " + std::to_string(greatNotes), ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 15);
        ofDrawBitmapString("Total Notes: " + std::to_string(totalNotes), ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 35);
        ofDrawBitmapString("Score: " + std::to_string((int)((float)greatNotes / totalNotes * 100)) + "%", ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 55);
    } else if (mode == CreateChart::CODE) {
        ofDrawBitmapString("Total Notes: " + std::to_string(totalNotes), ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 15);
    }
    ofDrawBitmapString("Please exit the game.", ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 115);
}
