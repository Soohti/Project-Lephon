#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "BaseMode.h"

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
        ofImage camFrame;
        ofImage handIcon;
        cv::dnn::Net net;
        float confidenceThreshold;
        float handFoundTime;
        float handDuration;
        ofRectangle handIconRect;
    
        BaseMode* currentMode;
};
