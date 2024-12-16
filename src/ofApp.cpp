#include "ofApp.h"
#include <fstream>
#include <sstream>
#include <filesystem>

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setup(ofGetWidth(), ofGetHeight());

    net = cv::dnn::readNetFromDarknet(
        ofToDataPath("cross-hands-tiny-prn.cfg"),
        ofToDataPath("cross-hands-tiny-prn.weights")
    );
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    handIcon.load("hand.png");
    // resize the hand icon based on window size
    handIcon.resize(ofGetWidth() / 16, ofGetHeight() / 9);

    confidenceThreshold = 0.3;
    handFoundTime = 0;
    handDuration = 1.0;
    
    // Initialize menu items
    buttonWidth = 200;
    buttonHeight = 50;
    float centerX = ofGetWidth() / 2;
    float startY = ofGetHeight() / 2;
    playGameButton.set(centerX - buttonWidth / 2, startY - 100, buttonWidth, buttonHeight);
    createChartButton.set(centerX - buttonWidth / 2, startY + 50, buttonWidth, buttonHeight);

    isHoveringPlayGame = false;
    isHoveringCreateChart = false;
    hoverStartTime = 0;
    hoverDuration = 3.0; // seconds

    currentMode = MENU;

    selectedSongIndex = -1;
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();

    if (cam.isFrameNew()){
        // Get and process the frame
        mirroredFrame.setFromPixels(cam.getPixels());
        mirroredFrame.mirror(false, true);
        cv::Mat frame = ofxCv::toCv(mirroredFrame);
        cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);

        // Prepare the input blob
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1/255.0, cv::Size(416, 416), cv::Scalar(), true, false);
        net.setInput(blob);

        // Forward pass
        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        // Process detections
        for (size_t i = 0; i < outputs.size(); ++i){
            float* data = (float*)outputs[i].data;
            bool handFound = false;
            for (int j = 0; j < outputs[i].rows; ++j, data += outputs[i].cols){
                float confidence = data[4];
                if (confidence > confidenceThreshold){
                    handIconRect.set(data[0] * frame.cols - handIcon.getWidth() / 2,
                                     data[1] * frame.rows - handIcon.getHeight() / 2,
                                     handIcon.getWidth(),
                                     handIcon.getHeight());
                    handFound = true; // Stop after finding one hand
                    handFoundTime = ofGetElapsedTimef();
                    break;
                }
            }
            if (handFound){
                break;
            }
        }
        // if hand not found for 3 seconds, reset the hand icon position
        if (ofGetElapsedTimef() - handFoundTime > handDuration){
            handIconRect.set(0, 0, handIcon.getWidth(), handIcon.getHeight());
        }
        if (currentMode == MENU){
            if (handIconRect.intersects(playGameButton)){
                if (!isHoveringPlayGame){
                    isHoveringPlayGame = true;
                    hoverStartTime = ofGetElapsedTimef();
                }
                else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration){
                    currentMode = GAME;
                    isHoveringPlayGame = false;
                }
                isHoveringCreateChart = false; // Not hovering over the other button
            }
            else if (handIconRect.intersects(createChartButton)){
                if (!isHoveringCreateChart){
                    isHoveringCreateChart = true;
                    hoverStartTime = ofGetElapsedTimef();
                }
                else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration){
                    currentMode = CREATE_CHART;
                    isHoveringCreateChart = false;
                }
                isHoveringPlayGame = false; // Not hovering over the other button
            }
            else {
                isHoveringPlayGame = false;
                isHoveringCreateChart = false;
            }
        }
        
    }

    // Detect mode change to GAME or CREATE_CHART and initialize song selection
    if ((currentMode == GAME || currentMode == CREATE_CHART) && mp4Files.empty()){
        initializeSongSelection();
    }

    if (currentMode == MENU){
        // ...existing code...
    }
    else if (currentMode == GAME || currentMode == CREATE_CHART){
        if (selectedSongIndex == -1){
            handleSongSelection();
        }
        else {
            // Update video playback
            videoPlayer.update();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255, 150); // Semi-transparent white

    if (mirroredFrame.isAllocated() && currentMode == MENU){
        mirroredFrame.draw(0, 0);
    }

    if (currentMode == GAME || currentMode == CREATE_CHART){
        if (selectedSongIndex != -1){
            // Draw the video
            videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }
    
    ofSetColor(255, 255, 255, 255); // Full opacity
    if (currentMode == MENU){
        // Draw menu items
        // ofSetColor(255, 255, 255, 150); // Semi-transparent white
        ofDrawBitmapString("Welcome to Project Lephon!", ofGetWidth()/2 - 100, 50);
        ofNoFill();
        ofSetLineWidth(5);
        ofDrawRectangle(playGameButton);
        ofDrawRectangle(createChartButton);
//            ofSetColor(0);
        ofDrawBitmapString("Play the game", playGameButton.x + 20, playGameButton.y + 30);
        ofDrawBitmapString("Create new chart", createChartButton.x + 20, createChartButton.y + 30);

        // Draw progress indicator if hovering
        if (isHoveringPlayGame || isHoveringCreateChart){
            float elapsedTime = ofGetElapsedTimef() - hoverStartTime;
            float progress = elapsedTime / hoverDuration;
            if (progress > 1.0) progress = 1.0;

            float buttonCenterX, buttonCenterY;
            if (isHoveringPlayGame){
                buttonCenterX = playGameButton.getCenter().x;
                buttonCenterY = playGameButton.getCenter().y;
            } else {
                buttonCenterX = createChartButton.getCenter().x;
                buttonCenterY = createChartButton.getCenter().y;
            }
            drawHoverProgress(progress, buttonCenterX, buttonCenterY, 30);
        }
    }
//         else if (currentMode == GAME){
//             // Draw game screen
// //            ofSetColor(0);
//             ofDrawBitmapString("Game Mode", ofGetWidth()/2 - 40, ofGetHeight()/2);
//         }
    else if (currentMode == GAME || currentMode == CREATE_CHART){
        if (selectedSongIndex == -1){
            drawSongSelection();
        }
    }

    // Draw hand icon with full opacity
    handIcon.draw(handIconRect);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
// Add this function to initialize song selection when entering CREATE_CHART mode
void ofApp::initializeSongSelection(){
    namespace fs = std::filesystem;
    mp4Files.clear();
    mp4Buttons.clear();
    isHoveringOverSong.clear();
    coverImages.clear();
    float startY = 200;
    int index = 0;
    for (const auto& entry : fs::directory_iterator(ofToDataPath("songs"))){
        if (entry.path().extension() == ".mp4"){
            std::string mp4Path = entry.path().string();
            mp4Files.push_back(mp4Path);
            mp4Buttons.push_back(ofRectangle(ofGetWidth()/2 - buttonWidth/2, startY + index * (buttonHeight + 100), buttonWidth, buttonHeight));
            isHoveringOverSong.push_back(false);

            // Load cover image
            std::string coverImagePath = mp4Path.substr(0, mp4Path.find_last_of('.')) + ".jpg";
            ofImage coverImage;
            if (coverImage.load(coverImagePath)){
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
    if (!coverImages.empty()){
        currentBackgroundImage = coverImages[0];
    }
}

void ofApp::handleSongSelection(){
    for (size_t i = 0; i < mp4Buttons.size(); ++i){
        if (handIconRect.intersects(mp4Buttons[i])){
            if (!isHoveringOverSong[i]){
                isHoveringOverSong[i] = true;
                hoverStartTime = ofGetElapsedTimef();

                // Update current background image to the cover image of the hovered song
                currentBackgroundImage = coverImages[i];
            }
            else if (ofGetElapsedTimef() - hoverStartTime >= hoverDuration){
                selectedSongIndex = i;
                // Load and play the selected video
                videoPlayer.load(mp4Files[i]);
                videoPlayer.play();
                isHoveringOverSong[i] = false;
                break;
            }
        }
        else {
            isHoveringOverSong[i] = false;
            // Remove the code that resets the background image
            // Do not reset currentBackgroundImage here
        }
    }
}

void ofApp::drawSongSelection(){
    // Draw the current background image
    if(currentBackgroundImage.isAllocated()){
        ofPushStyle();
        ofSetColor(255, 255, 255, 150); // Semi-transparent white
        currentBackgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }

    // Draw song selection buttons
    for (size_t i = 0; i < mp4Buttons.size(); ++i){
        // ...existing code to draw buttons and hover indicators...
        ofNoFill();
        ofSetLineWidth(5);
        ofDrawRectangle(mp4Buttons[i]);
        ofDrawBitmapString(mp4Files[i].substr(mp4Files[i].find_last_of('/') + 1, mp4Files[i].find_last_of('.') - mp4Files[i].find_last_of('/') - 1), mp4Buttons[i].x + 20, mp4Buttons[i].y + 30);

        // ...existing code for hover progress indicator...
        if (isHoveringOverSong[i]){
            float elapsedTime = ofGetElapsedTimef() - hoverStartTime;
            float progress = elapsedTime / hoverDuration;
            if (progress > 1.0) {
                progress = 1.0;
            } else {
                drawHoverProgress(progress, mp4Buttons[i].getCenter().x, mp4Buttons[i].getCenter().y, 30);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawHoverProgress(float progress, float x, float y, float radius){
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
