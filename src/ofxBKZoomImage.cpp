#include "ofxBKZoomImage.h"
ofxBKZoomImage::ofxBKZoomImage(float _x, float _y, float _width, float _height)
{
	init(_x, _y, _width, _height);
}

void ofxBKZoomImage::init(float _x, float _y, float _width, float _height)
{
	ofxBKImage::init();
	zoomRect.set(0,0,100,100);
	zoomAnchor.set(0,-1);
}

void ofxBKZoomImage::draw()
{
	ofxBKImage::draw();

	if(isPressed && targetImage != NULL) 
	{
		cropImage.clear();
		cropImage.cropFrom(originalImage,zoomRect.x,zoomRect.y,zoomRect.width,zoomRect.height);

		ofPushMatrix();
		ofTranslate(-position-offset);
		cropImage.draw(ofGetWidth()*.5-200,ofGetHeight()*.5-200,400,400);
		ofPopMatrix();
	}
}

void ofxBKZoomImage::loadImage(string path)
{
	
	ofxBKImage::loadImage(path);
	
	originalImage.cropFrom(*image,0,0,image->width,image->height);

	int maxSize = 2048;
	if(image->width >= image->height && image->width > maxSize) image->resize(maxSize,image->height*maxSize/image->width);
	else if(image->height > image->width && image->height > maxSize) image->resize(maxSize,image->width*maxSize/image->height);
	
}

void ofxBKZoomImage::mouseDragged(ofMouseEventArgs &e)
{
	if(targetImage != NULL) zoomRect.set(ofGetMouseX()*originalImage.width/ofGetWidth(),ofGetMouseY()*originalImage.width/ofGetHeight(),400,400);
}