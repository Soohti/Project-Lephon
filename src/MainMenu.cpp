#include "MainMenu.h"
#include "CreateChart.h"
#include "PlayGame.h"
#include "SelectSong.h"

MainMenu::MainMenu(ofRectangle* handIconRect)
    : handIconRect(handIconRect)
{
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

MainMenu::~MainMenu() { }

void MainMenu::update()
{
    // Logic for updating hover states and hand detection
    if (handIconRect->intersects(playGameButton)) {
        if (!isHoveringPlayGame) {
            isHoveringPlayGame = true;
            hoverStartTime = ofGetElapsedTimef();
        } else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration) {
            nextMode = new SelectSong(handIconRect, PlayGame::CODE);
        }
        isHoveringCreateChart = false;
    } else if (handIconRect->intersects(createChartButton)) {
        if (!isHoveringCreateChart) {
            isHoveringCreateChart = true;
            hoverStartTime = ofGetElapsedTimef();
        } else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration) {
            nextMode = new SelectSong(handIconRect, CreateChart::CODE);
        }
        isHoveringPlayGame = false;
    } else {
        isHoveringPlayGame = false;
        isHoveringCreateChart = false;
    }
}

void MainMenu::draw()
{
    ofSetColor(255, 255, 255, 255); // Full opacity

    // Draw menu items
    ofDrawBitmapString("Welcome to Project Lephon!", ofGetWidth() / 2 - 100, 50);
    ofNoFill();
    ofSetLineWidth(5);
    ofDrawRectangle(playGameButton);
    ofDrawRectangle(createChartButton);
    ofDrawBitmapString("Play the game", playGameButton.x + 20, playGameButton.y + 30);
    ofDrawBitmapString("Create new chart", createChartButton.x + 20, createChartButton.y + 30);

    // Draw progress indicator if hovering
    if (isHoveringPlayGame || isHoveringCreateChart) {
        float elapsedTime = ofGetElapsedTimef() - hoverStartTime;
        float progress = elapsedTime / hoverDuration;
        if (progress > 1.0) {
            progress = 1.0;
        }

        float buttonCenterX, buttonCenterY;
        if (isHoveringPlayGame) {
            buttonCenterX = playGameButton.getCenter().x;
            buttonCenterY = playGameButton.getCenter().y;
        } else {
            buttonCenterX = createChartButton.getCenter().x;
            buttonCenterY = createChartButton.getCenter().y;
        }
        ofApp::drawHoverProgress(progress, buttonCenterX, buttonCenterY);
    }
}

bool MainMenu::streamWebcam()
{
    return true;
}
