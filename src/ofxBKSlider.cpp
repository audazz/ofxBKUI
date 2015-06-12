#pragma once
#include "ofxBKSlider.h"
#include "ofxBKUIEventArgs.h"
#include "ofxBKStyle.h"

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
	ofxBKContainer::init(_x, _y, _width, _height);

	label = _label;
	labelTF = new ofxBKLabel(_label);
	labelTF->horizontalAlign = BKUI_TEXTALIGN_CENTER;
	labelTF->verticalAlign = BKUI_TEXTALIGN_MIDDLE;
	labelTF->setFluidWidth()->setFluidHeight();
	labelTF->setColor(ofColor::white);
	addChild(labelTF);

	labelSuffix = "";

	minValue = 0;
	maxValue = 1;
	setValue(.5);
	mouseDragOffset = 0;
	isDragging = false;

	bgColor = ofxBKStyle::bgColor;
	overColor = ofxBKStyle::normalColor;
	selectedColor = ofxBKStyle::highlightColor;
	labelColor = ofxBKStyle::normalColor;
	labelOverColor = ofxBKStyle::lightColor;
	labelSelectedColor =  ofxBKStyle::normalColor;

	barColor = ofxBKStyle::blue;
}

void ofxBKSlider::draw()
{
	ofxBKContainer::draw();

	ofSetColor(isOver?overColor:bgColor,enabled?255:100);
	ofRect(0,0,width,height);
	ofSetColor(barColor,enabled?255:100);
	float tw = getNormalizedValue()*(width-4);
	ofRect(2,2,tw,height-4);

	if(isDragging)
	{
		ofSetLineWidth(3);
		ofSetColor(ofxBKStyle::red);
		ofLine(tw+2,2,tw+2,height-2);
	}
}
/*
void ofxBKSlider::setSize(float _width, float _height, bool notify)
{
	ofxBKContainer::setSize(_width,_height,notify);
	labelTF->setSize(_width,_height);
}
*/

void ofxBKSlider::setLabel(string _label, string _labelSuffix)
{
	label = _label;
	labelSuffix = _labelSuffix;
	updateLabelTF();
}

float ofxBKSlider::getValueFromNormalized()
{
	return getValueFromNormalized(value);
}

float ofxBKSlider::getValueFromNormalized(float _normValue)
{
	return (_normValue * (maxValue-minValue) + minValue);
}

float ofxBKSlider::getNormalizedValue()
{
	return getNormalizedValue(value);
}

float ofxBKSlider::getNormalizedValue(float _value)
{
	return (_value-minValue)/(maxValue-minValue);
}

void ofxBKSlider::setValue(float _value, bool notify)
{
	_value = min(max(_value,minValue),maxValue);
	if(value == _value) return;
	value = _value;
	if(notify) notifyValueChanged();
	updateLabelTF();
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
	setValue((maxValue+minValue)/2);
}

void ofxBKSlider::updateLabelTF()
{
	char text[256];
	sprintf(text, "%s : %.2f %s",label.c_str(),value,labelSuffix.c_str());
	labelTF->setText(string(text));
}


void ofxBKSlider::mousePressed(ofMouseEventArgs &e)
{
	ofxBKContainer::mousePressed(e);
	std::cout << "posv " << getNormalizedValue() << "" << std::endl;
	std::cout << "posm " << getValueForPosition(getMousePosition().x) << "" << std::endl;
	mouseDragOffset = getValueForPosition(getMousePosition().x) - getNormalizedValue();
	ofHideCursor();

	#ifdef _WIN32
	LPPOINT pt = LPPOINT();
	int x = ofGetWindowPositionX()+getGlobalBounds().x +value*width;
	int y = ofGetWindowPositionY()+getGlobalBounds().y+getMousePosition().y;
	SetCursorPos(x,y);
	mouseDragOffset = 0;
	#endif

	isDragging = true;
}

void ofxBKSlider::mouseDragged(ofMouseEventArgs &e)
{
	ofxBKContainer::mouseDragged(e);
	setValue(getValueFromNormalized(getValueForPosition(getMousePosition().x)-mouseDragOffset));
}

void ofxBKSlider::mouseReleased(ofMouseEventArgs &e)
{
	ofShowCursor();
	isDragging = false;
}

void ofxBKSlider::mouseReleasedOutside(ofMouseEventArgs &e)
{
	ofShowCursor();
	isDragging = false;
}

float ofxBKSlider::getValueForPosition(float pos)
{
	return pos/width;
}
