#pragma once
#include "ofxBKZoomImage.h"
#include "ofxBKStyle.h"

ofxBKZoomImage::ofxBKZoomImage(float _x, float _y, float _width, float _height)
{
	init(_x, _y, _width, _height);
}

void ofxBKZoomImage::init(float _x, float _y, float _width, float _height)
{
	ofxBKImage::init();
	zoomRect.set(0,0,400,400);
	zoomAnchor.set(0,-1);
}

void ofxBKZoomImage::draw()
{
	ofxBKImage::draw();

	if(isPressed && targetImage != NULL) 
	{
		
		ofVec2f pointerPos;
		lastPos.push_back(getMousePosition());
		if(lastPos.size() > 5) lastPos.erase(lastPos.begin());
		for(int i=0;i<lastPos.size();i++) pointerPos += lastPos[i];
		pointerPos /= lastPos.size();


		//ofVec2f pointerPos = getMousePosition();

		ofVec2f rp = pointerPos - ofVec2f(imageRect.x,imageRect.y);
		rp /= ofVec2f(imageRect.width,imageRect.height);
		zoomRect.set(rp.x*originalImage.width-zoomRect.width/2,rp.y*originalImage.height-zoomRect.height/2,zoomRect.width,zoomRect.height);

		cropImage.clear();
		cropImage.cropFrom(originalImage,zoomRect.x,zoomRect.y,zoomRect.width,zoomRect.height);

		ofRectangle destRect = ofRectangle(pointerPos.x-zoomRect.width/2,pointerPos.y-zoomRect.height/2,zoomRect.width,zoomRect.height);
		cropImage.draw(destRect);


		ofPushStyle();
		ofSetColor(ofxBKStyle::normalColor);
		ofNoFill();
		ofSetLineWidth(4);
		ofRect(destRect);
		ofPopStyle();
	}
}



void ofxBKZoomImage::processImage()
{
	ofxBKImage::processImage();
	//originalImage.cropFrom(*image,0,0,image->width,image->height);	
	originalImage.setFromPixels(image->getPixelsRef());

	int maxSize = 1024;
	float iw = image->width;
	float ih = image->height;	

	if(maxSize > 0 && iw > maxSize || ih > maxSize)
	{	
		if(iw >= ih && iw > maxSize) image->resize(maxSize,ih*maxSize/iw);
		else if(ih > iw && ih > maxSize) image->resize(iw*maxSize/ih,maxSize);
	}
	
}

void ofxBKZoomImage::mousePressed(ofMouseEventArgs &e)
{
	bringToFront();
}