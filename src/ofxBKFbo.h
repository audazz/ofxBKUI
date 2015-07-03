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
	float cameraFov, nearClip, farClip;
	float mouseSensitivity;

	void lockCameraTo(ofVec3f lookAt = ofVec3f::zero(), bool mouseOrbit = true);
	void unlockCamera();
    void setCameraRadius(float radius);
    void setFOV(float fov);
    void setClipping(float near = 0, float far = 0);

    enum CAM_ROTATION{
        Z_IN_FRONT,
        Z_ON_TOP,
        X_ON_TOP
    };
    void setRotationAxes(const ofVec2f & ax1, const ofVec2f & ax2);
    void setRotationAxes(CAM_ROTATION rotParam = Z_IN_FRONT);

    const ofEasyCam * getCamera();


	virtual void setSize(float _width, float _height, bool notify = true);

    ofVec3f orbit(ofVec3f ax1, float angle1, ofVec3f ax2, float angle2, float radius);

protected :
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();

	void updateFboPosition();
	void mouseDragged(ofMouseEventArgs &e);

	ofFbo *targetFbo;
	ofEasyCam camera;
	ofVec3f rotateAx1, rotateAx2;
};
