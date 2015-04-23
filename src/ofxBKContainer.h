#pragma once
#include "ofxBKUIComponent.h"

class ofxBKContainer : public ofxBKUIComponent
{
public:
	ofxBKContainer();
	ofxBKContainer(float _x, float _y, float _width = 300, float _height = 200);

	
	float paddingTop;
	float paddingBottom;
	float paddingLeft;
	float paddingRight;

	virtual void setPadding(float top, float bottom, float left, float right);
	virtual void setPadding(float padding) { setPadding(padding,padding,padding,padding);};
	virtual void setPadding(float topBottom, float leftRight){setPadding(topBottom,topBottom,leftRight,leftRight);};
	
	float getInnerWidth();
	float getInnerHeight();

	virtual void setOffset(float _offsetX, float _offsetY);
	virtual void setSize(float _width, float _height, bool notify = true);

	vector<ofxBKUIComponent *> children;
	void addChild(ofxBKUIComponent * element);
	int numChildren;


	virtual void setVisible(bool value);

	ofColor bgColor;
	
protected:
	virtual void init(float _x = 0, float _y = 0, float _width = 300, float _height = 200);
	virtual void draw();

	virtual void setPosition(float _x, float _y);
	

	virtual void updateChildrenOffsets();
	virtual void updateChildOffset(ofxBKUIComponent * child);

};
