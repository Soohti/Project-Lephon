#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <filesystem>

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
        void initializeSongSelection();
        void handleSongSelection();
        void drawSongSelection();
        void drawHoverProgress(float progress, float x, float y, float radius);
    
    private:
        ofVideoGrabber cam;
        ofImage mirroredFrame;
        ofImage handIcon;
        cv::dnn::Net net;
        float confidenceThreshold;
        float handFoundTime;
        float handDuration;

        float buttonWidth;
        float buttonHeight;
        
//        float handX;
//        float handY;
        ofRectangle handIconRect;
    
//        int camWidth;
//        int camHeight;
    
            // Menu items
            ofRectangle playGameButton;
            ofRectangle createChartButton;
            bool isHoveringPlayGame;
            bool isHoveringCreateChart;
            float hoverStartTime;
            float hoverDuration; // Time in seconds to trigger action

            // Current mode
            enum Mode { MENU, GAME, CREATE_CHART, SONG_SELECTION };
            Mode currentMode;

        // Variables for song selection
        std::vector<std::string> mp4Files;
        std::vector<ofRectangle> mp4Buttons;
        std::vector<bool> isHoveringOverSong;
        std::vector<ofImage> coverImages;
        int selectedSongIndex;
        ofVideoPlayer videoPlayer;
        ofImage currentBackgroundImage;
};
