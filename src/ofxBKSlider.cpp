#pragma once
#include "ofxBKSlider.h"
#include "ofxBKUIEventArgs.h"

ofxBKSlider::ofxBKSlider()
{
}

ofxBKSlider::ofxBKSlider(string _label, float _x, float _y, float _width,float _height)
{	
	init(_label, _x, _y, _width,_height);
}

void ofxBKSlider::init(string _label, float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	ofxBKUIComponent::init(_x, _y, _width, _height);
	label = _label;

	minValue = 0;
	maxValue = 1;
	value = .5;
	mouseDragOffset = 0;
	isDragging = false;

	bgColor = ofColor(50,50,50); //ofxBKStyle::bgColor;
	overColor = ofColor(80,80,80);//ofxBKStyle::normalColor;
	selectedColor = ofColor(63, 160, 239);//ofxBKStyle::highlightColor;
	labelColor = ofColor(144, 144, 144);//ofxBKStyle::normalColor;
	labelOverColor = ofColor(233, 233, 233);//ofxBKStyle::lightColor;
	labelSelectedColor =  ofColor(50,50,50);//ofxBKStyle::normalColor;

}

void ofxBKSlider::draw()
{
	ofxBKUIComponent::draw();

	ofSetColor(isOver?overColor:bgColor,enabled?255:100);
	ofRect(0,0,width,height);
	ofSetColor(selectedColor,enabled?255:100);
	float tw = getNormalizedValue()*(width-4);
	ofRect(2,2,tw,height-4);

	if(isDragging)
	{
		ofSetLineWidth(3);
		ofSetColor(ofColor(200,10,0));
		ofLine(tw+2,2,tw+2,height-2);
	}

	ofSetColor(255,150);
	char info[64];
	sprintf(info, "%s : %.2f",label.c_str(),value);
	ofDrawBitmapString(info,5,15);
}

float ofxBKSlider::getNormalizedValue()
{
	return getNormalizedValue(value);
}

float ofxBKSlider::getNormalizedValue(float value)
{
	return (value-minValue)/(maxValue-minValue);
}

void ofxBKSlider::setValue(float _value, bool notify)
{
	_value = min(max(_value,minValue),maxValue);
	if(value == _value) return;
	value = _value;
	if(notify) notifyValueChanged();
}

void ofxBKSlider::notifyValueChanged()
{
	ofxBKUIEventArgs args;
	args.eventType = BKEVENT_VALUE_CHANGED;
	args.target = this;
	ofNotifyEvent(valueChanged,args);
	
}

void ofxBKSlider::setMinMaxValues(float _min, float _max)
{
	minValue = _min;
	maxValue = _max;
}

void ofxBKSlider::mousePressed(ofMouseEventArgs &e)
{
	ofxBKUIComponent::mousePressed(e);
	mouseDragOffset = getValueForPosition(getMousePosition().x) - value;
	isDragging = true;
}

void ofxBKSlider::mouseDragged(ofMouseEventArgs &e)
{
	ofxBKUIComponent::mouseDragged(e);
	setValue(getValueForPosition(getMousePosition().x)-mouseDragOffset);
}

void ofxBKSlider::mouseReleased(ofMouseEventArgs &e)
{
	isDragging = false;
}

void ofxBKSlider::mouseReleasedOutside(ofMouseEventArgs &e)
{
	isDragging = false;
}

float ofxBKSlider::getValueForPosition(float pos)
{
	return pos/width;
}