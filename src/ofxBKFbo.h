#pragma once
#include "ofxBKUIComponent.h"

class ofxBKFbo : public ofxBKUIComponent
{
public :
	ofxBKFbo(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	
	ofFbo * fbo;
	ofFbo * linkedFbo;

	bool isLinked;

	int fitMode;

	ofRectangle fboRect;

	virtual void linkToOfFbo(ofFbo * imageRef);
	virtual void unlink();

	void beginFbo();
	void endFbo();



	bool cameraLocked;
	bool mouseOrbitEnabled;
	ofVec3f cameraLookAt;
	ofVec2f cameraLongLat;
	float cameraRadius;
	float mouseSensitivity;

	void lockCameraTo(ofVec3f lookAt = ofVec3f::zero(),bool mouseOrbit = true);
	void unlockCamera();

	

	virtual void setSize(float _width, float _height, bool notify = true);

protected :
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();

	void updateFboPosition();
	void mouseDragged(ofMouseEventArgs &e);

	ofFbo *targetFbo;
	ofCamera camera;
};