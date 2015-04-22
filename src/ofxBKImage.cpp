#pragma once
#include "ofxBKImage.h"

ofxBKImage::ofxBKImage(float _x, float _y, float _width,float _height)
{	
	init(_x, _y, _width,_height);
}

void ofxBKImage::init(float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	ofxBKUIComponent::init(_x, _y, _width, _height);
	image = new ofImage();
	fitMode = IMAGE_FIT;
}

void ofxBKImage::draw()
{
	ofxBKUIComponent::draw();
	ofSetColor(0,50);
	ofRect(0,0,width,height);
	ofSetColor(255);

	float tx = 0;
	float ty = 0;
	float tw = image->width;
	float th = image->height;

	float ratio = width/height;
	float imageRatio = image->width*1.0/image->height;
		
	switch(fitMode)
	{
	case IMAGE_FIT:
		if(imageRatio > ratio)
		{
			tw = width;
			th = tw/imageRatio;
			ty = (height-th)/2;
		}else
		{
			th = height;
			tw = th*imageRatio;
			tx = (width-tw)/2;
		}
		break;

	case IMAGE_FILL:
		if(imageRatio < ratio)
		{
			tw = width;
			th = height*tw/width;
		}else
		{
			th = height;
			tw = width*th/height;
		}
		break;

	case IMAGE_ORIGINAL:
		break;

	case IMAGE_STRETCH:
		tw = width;
		th = height;
		break;
	}

	image->draw(tx,ty,tw,th);
}