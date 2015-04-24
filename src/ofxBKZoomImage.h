#pragma once
#include "ofxBKImage.h"

class ofxBKZoomImage : public ofxBKImage
{
public:
	ofxBKZoomImage(float _x = 0, float _y = 0, float _width = 100, float _height = 100);

	ofRectangle zoomRect;
	ofVec2f zoomAnchor;

	ofImage originalImage;
	ofImage cropImage;
	ofImage testImage;
	//virtual void loadImage(string path);

	vector<ofVec2f> lastPos;

	
protected:
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();
	
	virtual void processImage();

	virtual void mousePressed(ofMouseEventArgs &e);
};