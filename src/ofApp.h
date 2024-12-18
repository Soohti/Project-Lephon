#pragma once

#include "BaseMode.h"
#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {

public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;

    static void drawHoverProgress(float progress, float x, float y, ofColor color = ofColor::green)
    {
        // Draw progress circle
        ofDrawCircle(x, y, 30);
        // Draw arc representing progress
        ofPath path;
        path.setCircleResolution(100);
        path.setFilled(false);
        path.setStrokeColor(color);
        path.setStrokeWidth(5);
        path.arc(x, y, 30, 30, -90, -90 + progress * 360);
        path.draw();
    }

private:
    ofVideoGrabber cam;
    ofImage camFrame;
    ofImage handIcon;
    cv::dnn::Net net;
    float confidenceThreshold;
    float handFoundTime;
    float handDuration;
    ofRectangle handIconRect;

    BaseMode* currentMode;
};
