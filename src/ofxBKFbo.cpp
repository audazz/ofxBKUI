#pragma once
#include "ofxBKFbo.h"
#include "ofxBKImage.h"

ofxBKFbo::ofxBKFbo(float _x, float _y, float _width,float _height)
{
	init(_x, _y, _width,_height);
}

void ofxBKFbo::init(float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	targetFbo = NULL; //be sure there is no pointer there

	ofxBKUIComponent::init(_x, _y, _width, _height);

	fbo = new ofFbo(); //need to instantiate image before
	fitMode = IMAGE_FIT;

	isLinked = false;
	linkedFbo = NULL;

	camera.setNearClip(0.001);

	//camera.setScale(1, -1, 1);
	cameraLocked = false;
	mouseOrbitEnabled = false;
	cameraLookAt = ofVec3f::zero();
	cameraLongLat = ofVec2f::zero();
	mouseSensitivity = 1;
	cameraRadius = 1.5;
}

void ofxBKFbo::draw()
{

	ofxBKUIComponent::draw();
	ofSetColor(0,50);
	ofRect(0,0,width,height);
	ofSetColor(255);

	if(targetFbo != NULL) targetFbo->draw(fboRect);
}

void ofxBKFbo::linkToOfFbo(ofFbo *_linkedImage)
{
	isLinked = true;
	linkedFbo = _linkedImage;
	targetFbo = linkedFbo;
	updateFboPosition();
}


void ofxBKFbo::lockCameraTo(ofVec3f lookAt, bool mouseOrbit)
{
	cameraLocked = true;
	mouseOrbitEnabled = mouseOrbit;
	cameraLookAt.set(lookAt);
}

void ofxBKFbo::unlockCamera()
{
	cameraLocked = false;
}

ofEasyCam * ofxBKFbo::getCamera()
{
    return &camera;
}

void ofxBKFbo::beginFbo()
{
	targetFbo->begin(false);

	if(cameraLocked)
	{
		camera.begin();
		camera.orbit(cameraLongLat.x,cameraLongLat.y,cameraRadius,cameraLookAt);
	}
}

void ofxBKFbo::endFbo()
{
	if(cameraLocked)
	{
		camera.end();
	}
	targetFbo->end();
}

void ofxBKFbo::unlink()
{
	isLinked = false;
	linkedFbo = NULL;
	targetFbo = fbo;
	updateFboPosition();
}


void ofxBKFbo::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width, _height, notify);
	updateFboPosition();
}

void ofxBKFbo::updateFboPosition()
{
	if(targetFbo == NULL) return;
	float tx = 0;
	float ty = 0;
	float tw = targetFbo->getWidth();
	float th = targetFbo->getHeight();

	float ratio = width/height;
	float imageRatio = targetFbo->getWidth()*1.0/targetFbo->getHeight();

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

	fboRect.set(tx, ty, tw, th);
}

void ofxBKFbo::mouseDragged(ofMouseEventArgs &e)
{
	ofxBKUIComponent::mouseDragged(e);
	if (isMouseInside()){
        if(e.button == 0) {
            cameraLongLat -= mouseDelta * mouseSensitivity;
            cameraLongLat.y = min<float>(max<float>(cameraLongLat.y,-100),100);
        }
        else if(e.button = 1)
            cameraRadius += mouseDelta.y * mouseSensitivity*.01;
    }
}
