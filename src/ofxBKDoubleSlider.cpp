#pragma once
#include "ofxBKDoubleSlider.h"
#include "ofxBKUIEventArgs.h"

ofxBKDoubleSlider::ofxBKDoubleSlider()
{

}

ofxBKDoubleSlider::ofxBKDoubleSlider(string _label, float _x, float _y, float _width,float _height)
{	
	init(_label, _x, _y, _width,_height); 
	setValue2(.75);
	setValue(.25);
}

void ofxBKDoubleSlider::init(string _label, float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	ofxBKSlider::init(_label,_x, _y, _width, _height);
	currentDraggingValue = -1;
}

void ofxBKDoubleSlider::draw()
{
	ofSetColor(isOver?overColor:bgColor,enabled?255:100);
	ofRect(0,0,width,height);
	ofSetColor(selectedColor,enabled?255:100);

	float tx1 = getNormalizedValue(value)*(width-4);
	float tx2 = getNormalizedValue(value2)*(width-4);
	ofRect(2+tx1,2,tx2-tx1,height-4);

	if(isDragging)
	{
		float tx = currentDraggingValue == 1?tx1:tx2;
		ofSetLineWidth(3);
		ofSetColor(ofColor(200,10,0));
		ofLine(tx+2,2,tx+2,height-2);
	}
}

void ofxBKDoubleSlider::setValue(float _value, bool notify)
{
	_value = min(_value,value2);
	ofxBKSlider::setValue(_value,notify);
}

void ofxBKDoubleSlider::setValue2(float _value, bool notify)
{
	_value = max(value,_value);
	_value = min(max(_value,minValue),maxValue);

	if(value2 == _value) return;
	value2 = _value;	
	updateLabelTF();
	if(notify) notifyValueChanged();
}

void ofxBKDoubleSlider::setValues(float _value, float _value2,bool notify)
{
	setValue(_value,false);
	setValue2(_value2,notify);
}


ofVec2f ofxBKDoubleSlider::getValues()
{
	return ofVec2f(value,value2);
}

ofVec2f ofxBKDoubleSlider::getNormalizedValues()
{
	return ofVec2f(getNormalizedValue(value),getNormalizedValue(value2));
}


void ofxBKDoubleSlider::updateLabelTF()
{
	char text[256];
	sprintf(text, "%s : %.2f / %.2f",label.c_str(),value, value2);
	labelTF->setText(string(text));	
}



void ofxBKDoubleSlider::mousePressed(ofMouseEventArgs &e)
{
	currentDraggingValue = getNearestValueIndex(getValueForPosition(getMousePosition().x));
	mouseDragOffset = getValueForPosition(getMousePosition().x) - (currentDraggingValue == 1?value:value2);
	isDragging = true;
}

void ofxBKDoubleSlider::mouseDragged(ofMouseEventArgs &e)
{
	float targetVal = getValueForPosition(getMousePosition().x)-mouseDragOffset;
	if(currentDraggingValue == 1) setValue(targetVal);
	else if(currentDraggingValue == 2) setValue2(targetVal);
	
}

int ofxBKDoubleSlider::getNearestValueIndex(float _value)
{
	if(abs(_value-value) < abs(_value-value2)) return 1;
	return 2;
}

