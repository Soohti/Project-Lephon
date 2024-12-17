#include "BaseMode.h"

BaseMode::~BaseMode() {
}

void BaseMode::keyPressed(int key) {
}

void BaseMode::keyReleased(int key) {
}

bool BaseMode::streamWebcam() {
    return false;
}

void BaseMode::drawHoverProgress(float progress, float x, float y, float radius) {
    // Draw progress circle
    ofDrawCircle(x, y, radius);
    // Draw arc representing progress
    ofPath path;
    path.setCircleResolution(100);
    path.setFilled(false);
    path.setStrokeColor(ofColor(0, 255, 0));
    path.setStrokeWidth(5);
    path.arc(x, y, radius, radius, -90, -90 + progress * 360);
    path.draw();
}

BaseMode *BaseMode::getNextMode() {
    return nextMode;
}
