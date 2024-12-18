#include "ofApp.h"
#include "MainMenu.h"
#include "ofxCv.h"

#include <filesystem>

//--------------------------------------------------------------
void ofApp::setup()
{
    cam.setup(ofGetWidth(), ofGetHeight());

    net = cv::dnn::readNetFromDarknet(
        ofToDataPath("cross-hands-tiny-prn.cfg"),
        ofToDataPath("cross-hands-tiny-prn.weights"));
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    handIcon.load("hand.png");
    // resize the hand icon based on window size
    handIcon.resize(ofGetWidth() / 16, ofGetHeight() / 9);

    confidenceThreshold = 0.3;
    handFoundTime = 0;
    handDuration = 1.0;

    currentMode = new MainMenu(&handIconRect);
}

//--------------------------------------------------------------
void ofApp::update()
{
    cam.update();
    if (cam.isFrameNew()) {
        // Get and process the frame
        camFrame.setFromPixels(cam.getPixels());
        camFrame.mirror(false, true);
        cv::Mat handFrame = ofxCv::toCv(camFrame);
        cv::cvtColor(handFrame, handFrame, cv::COLOR_RGB2BGR);

        // Prepare the input blob
        cv::Mat blob = cv::dnn::blobFromImage(handFrame, 1 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
        net.setInput(blob);

        // Forward pass
        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        // Process detections
        for (size_t i = 0; i < outputs.size(); ++i) {
            float* data = (float*)outputs[i].data;
            bool handFound = false;
            for (int j = 0; j < outputs[i].rows; ++j, data += outputs[i].cols) {
                float confidence = data[4];
                if (confidence > confidenceThreshold) {
                    handIconRect.set(data[0] * handFrame.cols - handIcon.getWidth() / 2,
                        data[1] * handFrame.rows - handIcon.getHeight() / 2,
                        handIcon.getWidth(),
                        handIcon.getHeight());
                    handFound = true; // Stop after finding one hand
                    handFoundTime = ofGetElapsedTimef();
                    break;
                }
            }
            if (handFound) {
                break;
            }
        }
        // if hand not found for 1 seconds, reset the hand icon position
        if (ofGetElapsedTimef() - handFoundTime > handDuration) {
            handIconRect.set(0, 0, handIcon.getWidth(), handIcon.getHeight());
        }
    }

    currentMode->update();
    BaseMode* nextMode = currentMode->getNextMode();
    if (nextMode != nullptr) {
        delete currentMode;
        currentMode = nextMode;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (currentMode->streamWebcam() && camFrame.isAllocated()) {
        ofSetColor(255, 255, 255, 150); // Semi-transparent white
        camFrame.draw(0, 0);
    }

    currentMode->draw();
    // Draw hand icon with full opacity
    ofSetColor(255, 255, 255, 255);
    handIcon.draw(handIconRect);
}

//--------------------------------------------------------------
void ofApp::exit()
{
    delete currentMode;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    currentMode->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    currentMode->keyReleased(key);
}
