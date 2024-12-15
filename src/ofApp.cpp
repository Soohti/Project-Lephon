#include "ofApp.h"
#include <fstream>
#include <sstream>

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
        float buttonWidth = 200;
        float buttonHeight = 50;
        float centerX = ofGetWidth() / 2;
        float startY = ofGetHeight() / 2;
        playGameButton.set(centerX - buttonWidth / 2, startY - 100, buttonWidth, buttonHeight);
        createChartButton.set(centerX - buttonWidth / 2, startY + 50, buttonWidth, buttonHeight);

        isHoveringPlayGame = false;
        isHoveringCreateChart = false;
        hoverStartTime = 0;
        hoverDuration = 3.0; // seconds

        currentMode = MENU;
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();

    if(cam.isFrameNew()){
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
}

//--------------------------------------------------------------
void ofApp::draw(){
    mirroredFrame.draw(0, 0);

    // Draw hand icon at the detected position
    handIcon.draw(handIconRect);
    
        if (currentMode == MENU){
            // Draw menu items
            ofSetColor(255, 255, 255, 150); // Semi-transparent white
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

                float radius = 30;
                float centerX, centerY;
                if (isHoveringPlayGame){
                    centerX = playGameButton.getCenter().x;
                    centerY = playGameButton.getCenter().y;
                } else {
                    centerX = createChartButton.getCenter().x;
                    centerY = createChartButton.getCenter().y;
                }

                // Draw progress circle
//                ofNoFill();
//                ofSetColor(0, 255, 0);
//                ofSetLineWidth(5);
                ofDrawCircle(centerX, centerY, radius);
                // Draw arc representing progress
                ofPath path;
                path.setCircleResolution(100);
                path.setFilled(false);
                path.setStrokeColor(ofColor(0, 255, 0));
                path.setStrokeWidth(5);
                path.arc(centerX, centerY, radius, radius, -90, -90 + progress * 360);
                path.draw();
            }
        }
        else if (currentMode == GAME){
            // Draw game screen
//            ofSetColor(0);
            ofDrawBitmapString("Game Mode", ofGetWidth()/2 - 40, ofGetHeight()/2);
        }
        else if (currentMode == CREATE_CHART){
            // Draw create chart screen
//            ofSetColor(0);
            ofDrawBitmapString("Create Chart Mode", ofGetWidth()/2 - 60, ofGetHeight()/2);
        }
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
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
