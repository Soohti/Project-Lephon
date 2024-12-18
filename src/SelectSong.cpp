#include "SelectSong.h"
#include "CreateChart.h"
#include "PlayGame.h"

SelectSong::SelectSong(ofRectangle* handIconRect, int mode)
    : handIconRect(handIconRect)
    , mode(mode)
{
    // Initialize song selection
    namespace fs = std::filesystem;
    mp4Files.clear();
    mp4Buttons.clear();
    isHoveringOverSong.clear();
    coverImages.clear();
    hoverStartTime = 0;
    hoverDuration = 3.0;
    float buttonWidth = 200, buttonHeight = 50;
    float startY = 200;
    int index = 0;
    for (const auto& entry : fs::directory_iterator(ofToDataPath("songs"))) {
        if (entry.path().extension() == ".mp4") {
            std::string mp4Path = entry.path().string();
            mp4Files.push_back(mp4Path);
            mp4Buttons.push_back(ofRectangle(ofGetWidth() / 2 - buttonWidth / 2, startY + index * (buttonHeight + 100), buttonWidth, buttonHeight));
            isHoveringOverSong.push_back(false);

            // Load cover image
            std::string coverImagePath = mp4Path.substr(0, mp4Path.find_last_of('.')) + ".jpg";
            ofImage coverImage;
            if (coverImage.load(coverImagePath)) {
                coverImages.push_back(coverImage);
            } else {
                // Handle missing cover image (use a default image or a blank image)
                ofImage defaultImage;
                defaultImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
                defaultImage.setColor(ofColor::black);
                coverImages.push_back(defaultImage);
            }

            index++;
        }
    }

    // Set current background image to the cover image of the first song
    if (!coverImages.empty()) {
        currentBackgroundImage = coverImages[0];
    }
}

SelectSong::~SelectSong() { }

void SelectSong::update()
{
    for (size_t i = 0; i < mp4Buttons.size(); ++i) {
        if (handIconRect->intersects(mp4Buttons[i])) {
            if (!isHoveringOverSong[i]) {
                isHoveringOverSong[i] = true;
                hoverStartTime = ofGetElapsedTimef();
                // Update current background image to the cover image of the hovered song
                currentBackgroundImage = coverImages[i];
            } else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration) {
                if (mode == PlayGame::CODE) {
                    nextMode = new PlayGame(handIconRect, mp4Files[i]);
                } else if (mode == CreateChart::CODE) {
                    nextMode = new CreateChart(handIconRect, mp4Files[i]);
                }
                break;
            }
        } else {
            isHoveringOverSong[i] = false;
        }
    }
}

void SelectSong::draw()
{
    // Draw the current background image
    ofSetColor(255, 255, 255, 150); // Semi-transparent white
    if (currentBackgroundImage.isAllocated()) {
        currentBackgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    // Draw song selection buttons
    ofSetColor(255, 255, 255, 255); // Full opacity
    for (size_t i = 0; i < mp4Buttons.size(); ++i) {
        ofNoFill();
        ofSetLineWidth(5);
        ofDrawRectangle(mp4Buttons[i]);
        ofDrawBitmapString(mp4Files[i].substr(mp4Files[i].find_last_of('/') + 1, mp4Files[i].find_last_of('.') - mp4Files[i].find_last_of('/') - 1), mp4Buttons[i].x + 20, mp4Buttons[i].y + 30);

        if (isHoveringOverSong[i]) {
            float elapsedTime = ofGetElapsedTimef() - hoverStartTime;
            float progress = elapsedTime / hoverDuration;
            if (progress > 1.0) {
                progress = 1.0;
            } else {
                ofApp::drawHoverProgress(progress, mp4Buttons[i].getCenter().x, mp4Buttons[i].getCenter().y);
            }
        }
    }
}
