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
