#pragma once
#include "ofxBKImage.h"

ofxBKImage::ofxBKImage(float _x, float _y, float _width,float _height)
{
	init(_x, _y, _width,_height);
}

void ofxBKImage::init(float _x, float _y, float _width,float _height)
{
    isResizedForDraw = false;
    maxSize = 1024;
	//printf("BT init\n");
	drawedImage = NULL; //be sure there is no pointer there

	ofxBKUIComponent::init(_x, _y, _width, _height);

	imageDefault = new ofImage(); //need to instantiate image before
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

	if(drawedImage != NULL)
		drawedImage->draw(imageRect);
}

void ofxBKImage::loadImage(string path)
{
	unlink();
    isResizedForDraw = false;
	imageInternal->loadImage(path);
	targetImage = imageInternal;
	processImageForDrawing();
}
void ofxBKImage::setFromPixels(ofPixels p)
{
	unlink();
    isResizedForDraw = false;
	imageInternal->getPixelsRef() = p;
    imageInternal->update();
	processImageForDrawing();
}
void ofxBKImage::unloadImage()
{
    if (imageInternal!=NULL)
        delete imageInternal;
    imageInternal = NULL;

	targetImage = imageDefault;
    unallocateDrawImage();
    isResizedForDraw = false;
}


void ofxBKImage::linkToOfImage(ofImage *_linkedImage)
{
	isLinked = true;
    isResizedForDraw = false;
	linkedImage = _linkedImage;
	targetImage = linkedImage;
	processImageForDrawing();
	updateImagePosition();
}
void ofxBKImage::unlink()
{
	isLinked = false;
	linkedImage = NULL;

	drawedImage = imageDefault;
	unallocateDrawImage();

    isResizedForDraw = false;
}

void ofxBKImage::processImageForDrawing()
{
    // unallocate previously allocated resized image
    unallocateDrawImage();

	float iw = targetImage->width;
	float ih = targetImage->height;

	if(maxSize > 0 && iw > maxSize || ih > maxSize)
	{
	    isResizedForDraw = true;
	    printf("cloning... ");
	    drawedImage = new ofImage;
	    drawedImage->clone(*targetImage);
	    printf("ok!\n");
		if(iw >= ih && iw > maxSize)
            drawedImage->resize(maxSize,ih*maxSize/iw);
		else if(ih > iw && ih > maxSize)
            drawedImage->resize(iw*maxSize/ih,maxSize);
	}else{
        drawedImage = targetImage;
	}
}


void ofxBKImage::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width, _height, notify);
	updateImagePosition();
}

void ofxBKImage::updateImagePosition()
{
	if(drawedImage == NULL) return;
	float tx = 0;
	float ty = 0;
	float tw = drawedImage->width;
	float th = drawedImage->height;

	float ratio = width/height;
	float imageRatio = drawedImage->width*1.0/drawedImage->height;

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


void ofxBKImage::unallocateDrawImage()
{
    if (isResizedForDraw){
        if (drawedImage != NULL){
            delete drawedImage;
        }
    }
    drawedImage = NULL;
}

