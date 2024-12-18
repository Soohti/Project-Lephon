#pragma once
#include "ofApp.h"
#include "ofMain.h"

class Note {
public:
    float x, y, time, hitDiff;
    bool hit = false;
    ofImage noteImage;

    Note(float x, float y, float time)
        : x(x)
        , y(y)
        , time(time)
    {
    }
    virtual void update(float currentTime, ofRectangle* handIconRect) = 0;
    virtual void draw(float currentTime) = 0;
    virtual ~Note() { }

    void drawHit()
    {
        ofDrawBitmapStringHighlight("HIT!", x, y, ofColor::green, ofColor::black);
    }
    void drawMiss()
    {
        ofDrawBitmapStringHighlight("MISS", x, y, ofColor::red, ofColor::black);
    }
    void drawNote(float timeDiff)
    {
        noteImage.draw(x - noteImage.getWidth() / 2, y - noteImage.getHeight() / 2);
        float radius = ofMap(-timeDiff, 0.0f, 1.0f, 0, 80, true);
        ofDrawCircle(x, y, radius);
    }
    bool isHit(ofRectangle* handIconRect)
    {
        return handIconRect->intersects(ofRectangle(x - noteImage.getWidth() / 2, y - noteImage.getHeight() / 2, noteImage.getWidth(), noteImage.getHeight()));
    }
};

class TapNote : public Note {
public:
    TapNote(float x, float y, float time)
        : Note(x, y, time)
    {
        noteImage.load("tap.png");
        noteImage.resize(ofGetWidth() / 16, ofGetHeight() / 9);
    }
    void update(float currentTime, ofRectangle* handIconRect) override
    {
        float timeDiff = currentTime - time;
        if (timeDiff >= -0.2f && timeDiff <= 0.2f) {
            if (!hit && isHit(handIconRect)) {
                hitDiff = timeDiff;
                hit = true;
            }
        }
    }
    void draw(float currentTime) override
    {
        float timeDiff = currentTime - time;
        if (hit) {
            if (timeDiff - hitDiff <= 0.2f) {
                drawHit();
            }
        } else {
            if (timeDiff >= -1.0f && timeDiff <= 0.2f) {
                drawNote(timeDiff);
            } else { // 0.2 < timeDiff <= 0.4
                drawMiss();
            }
        }
    }
};

class HoldNote : public Note {
public:
    float duration;
    float holdTime = 0.0f;

    HoldNote(float x, float y, float time, float duration)
        : Note(x, y, time)
        , duration(duration)
    {
        noteImage.load("hold.png");
        noteImage.resize(ofGetWidth() / 16, ofGetHeight() / 9);
    }
    void update(float currentTime, ofRectangle* handIconRect) override
    {
        float timeDiff = currentTime - time;
        if (timeDiff >= 0.0f && timeDiff <= duration) {
            if (!hit && isHit(handIconRect)) {
                hitDiff = timeDiff;
                hit = true;
            }
            if (hit && !isHit(handIconRect)) {
                holdTime += timeDiff - hitDiff;
                hit = false;
            }
        } else if (hit) {
            holdTime += duration - hitDiff;
            hit = false;
        }
    }
    void draw(float currentTime) override
    {
        float timeDiff = currentTime - time;
        if (timeDiff >= -1.0f && timeDiff <= duration) {
            drawNote(timeDiff);
            if (timeDiff > 0.0f) {
                if (!hit) {
                    ofApp::drawHoverProgress(timeDiff / duration, x, y, ofColor::black);
                } else {
                    ofApp::drawHoverProgress(timeDiff / duration, x, y, ofColor::green);
                }
            }
        } else { // duration < timeDiff <= duration + 0.2
            if (holdTime >= 0.75f * duration) {
                drawHit();
            } else {
                drawMiss();
            }
        }
    }
};
