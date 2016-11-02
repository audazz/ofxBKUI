#include "ofxBKFbo.h"
#include "ofxBKImage.h"

ofxBKFbo::ofxBKFbo(float _x, float _y, float _width,float _height)
{
	init(_x, _y, _width,_height);
}

void ofxBKFbo::init(float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	targetFbo = nullptr; //be sure there is no pointer there

	ofxBKUIComponent::init(_x, _y, _width, _height);

	fbo = new ofFbo(); //need to instantiate image before
	fitMode = IMAGE_FIT;

	isLinked = false;
	linkedFbo = nullptr;

	camera.disableMouseInput();
	//camera.disableMouseMiddleButton();
	//camera.enableMouseInput();

	//camera.setNearClip(1);

	//camera.setScale(1, -1, 1);
	cameraLocked = false;
	mouseOrbitEnabled = false;
	cameraLookAt = ofVec3f::zero();
	cameraLongLat = ofVec2f::zero();
	mouseSensitivity = 1;
	cameraRadius = 1.5;
	nearClip = 0.001;
	farClip = 0;

	camera.setNearClip(nearClip);
	camera.setFarClip(farClip);
	camera.setDistance(cameraRadius);
    lockCameraTo(cameraLookAt, true);

    // default rotation axes:
    setRotationAxes();
}

void ofxBKFbo::draw()
{

	ofxBKUIComponent::draw();
	ofSetColor(0,50);
    ofDrawRectangle(0,0,width,height);
	ofSetColor(255);

	if(targetFbo != nullptr) targetFbo->draw(fboRect);
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

void ofxBKFbo::setCameraRadius(float radius)
{
    cameraRadius = radius;
}

void ofxBKFbo::setFOV(float fov)
{
    cameraFov = fov;
}

void ofxBKFbo::setClipping(float _near, float _far)
{
    nearClip = _near;
    farClip = _far;
}


void ofxBKFbo::setRotationAxes(const ofVec2f & ax1, const ofVec2f & ax2)
{
    rotateAx1.set(ax1);
    rotateAx2.set(ax2);
}


void ofxBKFbo::setRotationAxes(CAM_ROTATION rotParam)
{
    switch (rotParam) {
    case Z_IN_FRONT:
        rotateAx1.set(1,0,0);
        rotateAx2.set(0,0,1);
        break;

    case Z_ON_TOP:
        rotateAx1.set(0,1,0);
        rotateAx2.set(0,0,1);
        break;
    case X_ON_TOP:
        rotateAx1.set(0,0,1);
        rotateAx2.set(1,0,0);
        break;

    }

}


const ofEasyCam * ofxBKFbo::getCamera()
{
    return &camera;
}

void ofxBKFbo::beginFbo()
{
	targetFbo->begin(false);

	if(cameraLocked)
	{
		camera.begin();
		cameraLongLat.y = ofClamp(cameraLongLat.y,-89.99,89.99);
		ofVec3f p = orbit(rotateAx1, cameraLongLat.y,
                          rotateAx2, cameraLongLat.x,
                          cameraRadius);
        camera.setPosition(p);
		camera.setTarget(cameraLookAt);
		camera.lookAt(cameraLookAt,rotateAx2);
		//camera.lookAt()
		camera.setFov(cameraFov);
		camera.setNearClip(nearClip);
		camera.setFarClip(farClip);
		// TODO:set scale
        //camera.orbit(cameraLongLat.x,cameraLongLat.y,cameraRadius,cameraLookAt);
	}
}

void ofxBKFbo::endFbo()
{
	if(cameraLocked) {
		camera.end();
		cameraChanged = false;
	}
	targetFbo->end();
}

void ofxBKFbo::unlink()
{
	isLinked = false;
	linkedFbo = nullptr;
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
	if(targetFbo == nullptr) return;
	float tx = 0;
	float ty = 0;
	float tw = targetFbo->getWidth();
	float th = targetFbo->getHeight();

	float ratio = width/height;
	float imageRatio = targetFbo->getWidth()*1.0/targetFbo->getHeight();


	switch(fitMode) {
	case IMAGE_FIT: {
		if(imageRatio > ratio)
		{
			tw = width;
			th = tw/imageRatio;
			ty = (height-th)/2;
		} else {
			th = height;
			tw = th*imageRatio;
			tx = (width-tw)/2;
		}
		break;}

	case IMAGE_FILL: {
		if(imageRatio < ratio)
		{
			tw = width;
			th = height*tw/width;
		}else {
			th = height;
			tw = width*th/height;
		}
		break;}

	case IMAGE_ORIGINAL: {
		break;}

	case IMAGE_STRETCH: {
		tw = width;
		th = height;
		break;}
	}

	fboRect.set(tx, ty, tw, th);
}


ofVec3f ofxBKFbo::orbit(ofVec3f ax1, float angle1, ofVec3f ax2, float angle2, float radius)
{//ofClamp(angle1, -89, 89)

    ofVec3f p = - ax2.getPerpendicular(ax1).normalize() * radius;
	p.rotate(angle1, ax1);
	p.rotate(angle2, ax2);
	p += cameraLookAt;
	return p;
}


void ofxBKFbo::mouseDragged(ofMouseEventArgs &e)
{
	ofxBKUIComponent::mouseDragged(e);
	//std::cout << "mouse drag:" << e.button << "" << std::endl;
	//std::cout << "isMouseInside():" << isMouseInside() << "" << std::endl;
	if (isMouseInside()){
        if(e.button == 0) {
            cameraLongLat -= mouseDelta * mouseSensitivity;
            cameraChanged = true;
            //cameraLongLat.y = min<float>(max<float>(cameraLongLat.y,-100),100);
        } else if(e.button == 2) {
            cameraRadius += cameraRadius * mouseDelta.y * mouseSensitivity*.01;
            cameraChanged = true;
        }

    }
}


void ofxBKFbo::printInfo()
{
    ofxBKUIComponent::printInfo();
    std::cout << "   ofxBKFbo:"  << "" << std::endl;
}
