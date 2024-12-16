#include "MainMenu.h"

MainMenu::MainMenu() {
    // Initialize menu variables
    float buttonWidth = 200, buttonHeight = 50;
    float centerX = ofGetWidth() / 2;
    float startY = ofGetHeight() / 2;

    playGameButton.set(centerX - buttonWidth / 2, startY - 100, buttonWidth, buttonHeight);
    createChartButton.set(centerX - buttonWidth / 2, startY + 50, buttonWidth, buttonHeight);

    isHoveringPlayGame = false;
    isHoveringCreateChart = false;
    hoverStartTime = 0;
    hoverDuration = 3.0;
}

MainMenu::~MainMenu() {}

void MainMenu::update() {
    // Logic for updating hover states and hand detection
    if (handIconRect.intersects(playGameButton)) {
        if (!isHoveringPlayGame) {
            isHoveringPlayGame = true;
            hoverStartTime = ofGetElapsedTimef();
        } else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration) {
            // Trigger game mode switch
            nextMode = new SongSelection(1);
        }
        isHoveringCreateChart = false;
    } else if (handIconRect.intersects(createChartButton)) {
        if (!isHoveringCreateChart) {
            isHoveringCreateChart = true;
            hoverStartTime = ofGetElapsedTimef();
        } else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration) {
            // Trigger chart creation mode switch
            nextMode = new SongSelection(2);
        }
        isHoveringPlayGame = false;
    } else {
        isHoveringPlayGame = false;
        isHoveringCreateChart = false;
    }
}

void MainMenu::draw() {
    // TODO: Draw the camera feed
    ofSetColor(255, 255, 255, 255);

    // Draw menu items
    ofDrawBitmapString("Welcome to Project Lephon!", ofGetWidth() / 2 - 100, 50);
    ofNoFill();
    ofSetLineWidth(5);
    ofDrawRectangle(playGameButton);
    ofDrawRectangle(createChartButton);
    ofDrawBitmapString("Play the game", playGameButton.x + 20, playGameButton.y + 30);
    ofDrawBitmapString("Create new chart", createChartButton.x + 20, createChartButton.y + 30);

    // Draw hover progress
    if (isHoveringPlayGame || isHoveringCreateChart) {
        float elapsedTime = ofGetElapsedTimef() - hoverStartTime;
        float progress = elapsedTime / hoverDuration;
        if (progress > 1.0) progress = 1.0;

        ofDrawCircle(playGameButton.getCenter().x, playGameButton.getCenter().y, 30 * progress);
    }
}
    
void MainMenu::keyPressed(int key) {
    
}

void MainMenu::keyReleased(int key) {
    
}

