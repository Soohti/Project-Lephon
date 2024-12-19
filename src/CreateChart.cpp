#include "CreateChart.h"
#include "EndScreen.h"

CreateChart::CreateChart(ofRectangle* handIconRect, std::string mp4Path)
    : handIconRect(handIconRect)
{
    totalNotes = 0;
    // mp4Path - ".mp4" + "_gen.txt"
    outFile.open(mp4Path.substr(0, mp4Path.size() - 4) + "_gen.txt", std::ios::ate);
    videoPlayer.load(mp4Path);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.play();
    startTime = ofGetElapsedTimef();
}

CreateChart::~CreateChart()
{
    videoPlayer.stop();
    videoPlayer.close();
    outFile.close();
}

void CreateChart::update()
{
    videoPlayer.update();
    if (videoPlayer.getIsMovieDone()) {
        nextMode = new EndScreen(totalNotes, 0, CODE, videoPlayer.getMoviePath());
    }
}

void CreateChart::draw()
{
    ofSetColor(ofColor::white, 150); // Semi-transparent white
    if (videoPlayer.isLoaded()) {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}

void CreateChart::keyPressed(int key)
{
    if (key == ' ') {
        if (!holding) {
            holding = true;
            holdStartTime = ofGetElapsedTimef() - startTime;
        }
    } else {
        float centerX = handIconRect->getX() + handIconRect->getWidth() / 2;
        float centerY = handIconRect->getY() + handIconRect->getHeight() / 2;
        outFile << "1 " << centerX << " " << centerY << " " << ofGetElapsedTimef() - startTime << std::endl;
        totalNotes++;
    }
}

void CreateChart::keyReleased(int key)
{
    if (key == ' ') {
        float centerX = handIconRect->getX() + handIconRect->getWidth() / 2;
        float centerY = handIconRect->getY() + handIconRect->getHeight() / 2;
        outFile << "2 " << centerX << " " << centerY << " " << holdStartTime << " " << ofGetElapsedTimef() - startTime - holdStartTime << std::endl;
        totalNotes++;
        holding = false;
    }
}
