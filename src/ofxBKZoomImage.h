#pragma once
#include "ofxBKImage.h"

class ofxBKZoomImage : public ofxBKImage
{
public:
	ofxBKZoomImage(float _x = 0, float _y = 0, float _width = 100, float _height = 100);

	ofRectangle zoomRect;
	ofVec2f zoomAnchor;

	ofImage cropImage;
	//virtual void loadImage(string path);

	vector<ofVec2f> lastPos;

	ofColor zoomBckColor;
	bool zoomRecConstSize;



protected:
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();

	virtual void processImageForDrawing();

	virtual void mousePressed(ofMouseEventArgs &e);
	ofVec2f getSmoothMousePosition();

	ofVec2f mouseStartAbsPos;
    int zoomRectMaxSize[2];
    float zoomLevel;
    float zoomSensitivity;
    bool isInzoomMode;
};
