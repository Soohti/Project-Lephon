#pragma once
#include "BaseMode.h"
#include "Note.h"

#include <vector>
#include <fstream>

class PlayGame : public BaseMode {
public:
    PlayGame(ofRectangle* handIconRect, std::string mp4Path);
    ~PlayGame();

    void update() override;
    void draw() override;

    static constexpr int CODE = 1;

private:
    ofVideoPlayer videoPlayer;
    std::vector<Note*> allNotes;
    std::vector<Note*> activeNotes;
    std::ifstream inFile;
    float startTime;
    size_t nextNoteIndex = 0;

    ofRectangle* handIconRect;
};
