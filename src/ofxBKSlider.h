#pragma once
#include "ofxBKUIComponent.h"

class ofxBKSlider : public ofxBKUIComponent
{
public:
	ofxBKSlider();
	ofxBKSlider(string _label, float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	virtual void init(string _label, float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	virtual void draw();

	string label;
	float minValue;
	float maxValue;
	float value;
	float getNormalizedValue();

	virtual void setValue(float _value, bool notify = true);
	void setMinMaxValues(float _min, float _max);

	virtual void mousePressed(ofMouseEventArgs &e);
	virtual void mouseDragged(ofMouseEventArgs & e);
	virtual void mouseReleased(ofMouseEventArgs &e);
	virtual void mouseReleasedOutside(ofMouseEventArgs &e);

	bool isDragging;
	float mouseDragOffset; //to avoid changing cursor position

	//style
	ofColor bgColor;
	ofColor	overColor;
	ofColor selectedColor;
	
	ofColor labelColor;
	ofColor labelOverColor;
	ofColor labelSelectedColor;

	ofEvent<ofxBKUIEventArgs> valueChanged;

protected:
	void notifyValueChanged();
	float getValueForPosition(float pos);
	float getNormalizedValue(float value);
};