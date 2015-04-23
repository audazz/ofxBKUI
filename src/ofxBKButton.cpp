#pragma once
#include "ofxBKButton.h"
#include "ofxBKUIEventArgs.h"
#include "ofxBKStyle.h"

ofxBKButton::ofxBKButton()
{
}

ofxBKButton::ofxBKButton(string _label, float _x, float _y, float _width,float _height)
{	
	init(_label, _x, _y, _width,_height);
}

void ofxBKButton::init(string _label, float _x, float _y, float _width,float _height)
{
	ofxBKContainer::init(_x, _y, _width, _height);

	label = new ofxBKLabel(_label,0,0,_width,_height);
	if(label->autoSize) setSize(label->textBoundingBox.getWidth()+50,label->textBoundingBox.getHeight()+10);

	label->setAlign(BKUI_TEXTALIGN_CENTER,BKUI_TEXTALIGN_MIDDLE);
	label->setFluidWidth()->setFluidHeight();
	addChild(label);
	
	isToggle = false;
	isSelected = false;
	isPressed = false;

	bgColor = ofxBKStyle::bgColor;
	overColor = ofxBKStyle::normalColor;
	pressedColor = ofxBKStyle::semiLightColor;
	selectedColor = ofxBKStyle::highlightColor;
	labelColor = ofxBKStyle::lightColor;
	labelOverColor = ofxBKStyle::lightColor;
	labelSelectedColor =  ofxBKStyle::darkColor;

	transparentBG = true;
}


void ofxBKButton::draw()
{
	ofxBKContainer::draw();

	ofColor targetColor;
	if(isPressed && isOver) targetColor.set(pressedColor);
	else if(isSelected) targetColor.set(selectedColor);
	else targetColor.set(bgColor);

	if(isOver) targetColor.set(min(targetColor.r+30,255),min(targetColor.g+30,255),min(targetColor.b+30,255));

	ofSetColor(targetColor,enabled?255:50);
	ofRect(0,0,bgWidth,bgHeight);
}


string ofxBKButton::getLabel()
{
	return label->text;

}
void ofxBKButton::setLabel(string _label)
{
	label->setText(_label);
}

void ofxBKButton::updateLabelColor()
{
	label->setColor((isSelected)?labelSelectedColor:(isOver?labelOverColor:labelColor));
}

//selection handling
void ofxBKButton::setSelected(bool value, bool notify)
{
	isSelected = value;
	updateLabelColor();
	
	if(notify) notifySelected();
}

void ofxBKButton::notifySelected()
{
	ofxBKUIEventArgs args;

	args.eventType = isSelected?BKEVENT_SELECTED:BKEVENT_DESELECTED;
	args.target = this;
	ofNotifyEvent(isSelected?buttonSelected:buttonDeselected,args);
}

//size

void ofxBKButton::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width,_height,notify);
	bgWidth = width;
	bgHeight = height;
}


//mouse interaction

void ofxBKButton::mouseOut()
{
	if(!isToggle && isPressed) isSelected = false;
}

void ofxBKButton::mousePressed(ofMouseEventArgs& eventArgs)
{
	
	ofxBKContainer::mousePressed(eventArgs);	
}

void ofxBKButton::mouseReleased(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mouseReleased(eventArgs);

	if(!isToggle) 
	{
		setSelected(true,true);
		setSelected(false,true);
	}else
	{
		setSelected(!isSelected);
	}

	
}

void ofxBKButton::mouseReleasedOutside(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mouseReleasedOutside(eventArgs);
	//printf("mouse released outside uiComponent\n");
	if(!isToggle) isSelected = false;	
}