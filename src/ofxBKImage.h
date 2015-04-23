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
	
	ofImage *image;
	ofImage *linkedImage;

	bool isLinked;

	int fitMode;

	ofRectangle imageRect;

	virtual void loadImage(string path);
	virtual void linkToOfImage(ofImage * imageRef);
	virtual void unlink();

	virtual void setSize(float _width, float _height, bool notify = true);

protected :
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();

	void updateImagePosition();

	ofImage *targetImage;
};