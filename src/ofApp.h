#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

struct ChartEntry {
    float hitTime; // in milliseconds
    float x;
    float y;
    bool hit;
};

enum class GameMode {
    Menu,
    Gameplay,
    ChartGeneration
};

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
    
        ofVideoGrabber cam;
        ofImage mirroredFrame;
        ofImage handIcon;
        cv::dnn::Net net;
        float confidenceThreshold;
        float handFoundTime;
        float handDuration;
        
//        float handX;
//        float handY;
        ofRectangle handIconRect;
    
        int camWidth;
        int camHeight;
    
            // Menu items
            ofRectangle playGameButton;
            ofRectangle createChartButton;
            bool isHoveringPlayGame;
            bool isHoveringCreateChart;
            float hoverStartTime;
            float hoverDuration; // Time in seconds to trigger action

            // Current mode
            enum Mode { MENU, GAME, CREATE_CHART };
            Mode currentMode;
};
