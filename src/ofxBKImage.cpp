#pragma once
#include "ofxBKImage.h"

ofxBKImage::ofxBKImage(float _x, float _y, float _width,float _height)
{	
	init(_x, _y, _width,_height);
}

void ofxBKImage::init(float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	targetImage = NULL; //be sure there is no pointer there

	ofxBKUIComponent::init(_x, _y, _width, _height);

	image = new ofImage(); //need to instantiate image before
	fitMode = IMAGE_FIT;

	isLinked = false;
	linkedImage = NULL;
}

void ofxBKImage::draw()
{
	ofxBKUIComponent::draw();
	ofSetColor(0,50);
	ofRect(0,0,width,height);
	ofSetColor(255);

	if(targetImage != NULL)
	{
		targetImage->draw(imageRect);

	}
}

void ofxBKImage::loadImage(string path)
{
	isLinked = false;
	image->loadImage(path);
	unlink();
	processImage();
}

void ofxBKImage::setFromPixels(ofPixels p)
{
	image->getPixelsRef() = p;
    image->update();
	unlink();
	processImage();
}

void ofxBKImage::processImage()
{
	
}

void ofxBKImage::linkToOfImage(ofImage *_linkedImage)
{
	isLinked = true;
	linkedImage = _linkedImage;
	targetImage = linkedImage;
	updateImagePosition();
}

void ofxBKImage::unlink()
{
	isLinked = false;
	linkedImage = NULL;
	targetImage = image;
	updateImagePosition();
}


void ofxBKImage::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width, _height, notify);
	updateImagePosition();
}

void ofxBKImage::updateImagePosition()
{
	if(targetImage == NULL) return;
	float tx = 0;
	float ty = 0;
	float tw = targetImage->width;
	float th = targetImage->height;

	float ratio = width/height;
	float imageRatio = targetImage->width*1.0/targetImage->height;
		
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

	imageRect.set(tx, ty, tw, th);
	
}