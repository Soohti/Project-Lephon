#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <filesystem>
// TODO: optimize includes

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
    
    private:
        ofVideoGrabber cam;
        ofImage mirroredFrame;
        ofImage handIcon;
        cv::dnn::Net net;
        float confidenceThreshold;
        float handFoundTime;
        float handDuration;
};
