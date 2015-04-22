#pragma once
#include "ofxBKUIComponent.h"

#define IMAGE_FIT 1
#define IMAGE_FILL 2
#define IMAGE_ORIGINAL 3
#define IMAGE_STRETCH 4

class ofxBKImage : public ofxBKUIComponent
{
public :
	ofxBKImage(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();

	ofImage *image;
	int fitMode;
};