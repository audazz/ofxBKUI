#pragma once
#include "ofxBKButton.h"
#include "ofxBKUIEventArgs.h"

ofxBKButton::ofxBKButton(string _label, float _x, float _y, float _width,float _height)
{	
	init(_label, _x, _y, _width,_height);
}

void ofxBKButton::init(string _label, float _x, float _y, float _width,float _height)
{
	//printf("BT init\n");
	ofxBKUIComponent::init(_x, _y, _width, _height);
	label = _label;

	isToggle = false;
	isSelected = false;
	isPressed = false;

	bgColor = ofColor(50,50,50); //ofxBKStyle::bgColor;
	overColor = ofColor(80,80,80);//ofxBKStyle::normalColor;
	selectedColor = ofColor(252, 194, 3);//ofxBKStyle::highlightColor;
	labelColor = ofColor(144, 144, 144);//ofxBKStyle::normalColor;
	labelOverColor = ofColor(233, 233, 233);//ofxBKStyle::lightColor;
	labelSelectedColor =  ofColor(50,50,50);//ofxBKStyle::normalColor;
}


void ofxBKButton::draw()
{
	ofxBKUIComponent::draw();
	//printf("draw, selected, over, pressed %i %i %i\n",isSelected,isOver,isPressed);
	//printf("draw, position %f %f\n",width,height);
	ofColor btColor = (isSelected)?selectedColor:(isOver?overColor:bgColor);
	ofColor labelColor = (isSelected)?labelSelectedColor:(isOver?labelOverColor:labelColor);
	ofSetColor(btColor,enabled?255:50);
	ofRect(0,0,width,height);
	if(label.length() > 0) 
	{
		ofSetColor(labelColor,enabled?255:100);
		ofDrawBitmapString(label,10,15);
	}
}


//selection handling
void ofxBKButton::setSelected(bool value, bool notify)
{
	isSelected = value;
	if(notify) notifySelected();
}

void ofxBKButton::notifySelected()
{
	ofxBKUIEventArgs args;
	args.eventType = BKEVENT_SELECTED;
	args.target = this;
	ofNotifyEvent(buttonSelected,args);
}

//mouse interaction

void ofxBKButton::mouseOut()
{
	if(!isToggle && isPressed) isSelected = false;
}

void ofxBKButton::mousePressed(ofMouseEventArgs& eventArgs)
{
	
	ofxBKUIComponent::mousePressed(eventArgs);

	printf("mouse pressed in button\n");
	if(!isToggle) isSelected = true;
	else isSelected = !isSelected;
}

void ofxBKButton::mouseReleased(ofMouseEventArgs& eventArgs)
{
	ofxBKUIComponent::mouseReleased(eventArgs);
	printf("mouse released in button\n");

	if(!isToggle) isSelected = false;

	setSelected(isSelected,true);
}

void ofxBKButton::mouseReleasedOutside(ofMouseEventArgs& eventArgs)
{
	ofxBKUIComponent::mouseReleasedOutside(eventArgs);
	//printf("mouse released outside uiComponent\n");
	if(!isToggle) isSelected = false;	
}