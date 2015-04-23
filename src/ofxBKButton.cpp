#pragma once
#include "ofxBKButton.h"
#include "ofxBKUIEventArgs.h"
#include "ofxBKStyle.h"

ofxBKButton::ofxBKButton(string _label, float _x, float _y, float _width,float _height)
{	
	init(_label, _x, _y, _width,_height);
}

void ofxBKButton::init(string _label, float _x, float _y, float _width,float _height)
{
	ofxBKContainer::init(_x, _y, _width, _height);

	label = new ofxBKLabel(_label,0,0,_width,_height);
	if(label->autoSize) setSize(label->textBoundingBox.getWidth()+50,label->textBoundingBox.getHeight()+10);

	label->horizontalAlign = BKUI_TEXTALIGN_CENTER;
	label->verticalAlign = BKUI_TEXTALIGN_MIDDLE;
	label->setFluidWidth()->setFluidHeight();
	addChild(label);
	
	isToggle = false;
	isSelected = false;
	isPressed = false;

	bgColor = ofxBKStyle::bgColor;
	overColor = ofxBKStyle::normalColor;
	selectedColor = ofxBKStyle::highlightColor;
	labelColor = ofxBKStyle::lightColor;
	labelOverColor = ofxBKStyle::lightColor;
	labelSelectedColor =  ofxBKStyle::darkColor;
}


void ofxBKButton::draw()
{
	ofxBKContainer::draw();
	//printf("draw, selected, over, pressed %i %i %i\n",isSelected,isOver,isPressed);
	//printf("draw, position %f %f\n",width,height);
	ofColor btColor = (isSelected)?selectedColor:(isOver?overColor:bgColor);
	ofSetColor(btColor,enabled?255:50);
	ofRect(0,0,width,height);
	/*
	if(label->text.length() > 0) 
	{
		ofSetColor(labelColor,enabled?255:100);
		ofDrawBitmapString(label,10,15);
	}
	*/
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
	args.eventType = BKEVENT_SELECTED;
	args.target = this;
	ofNotifyEvent(buttonSelected,args);
}

//size
/*
void ofxBKButton::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width,_height,notify);
	label->setSize(_width,_height);
}
*/

//mouse interaction

void ofxBKButton::mouseOut()
{
	if(!isToggle && isPressed) isSelected = false;
}

void ofxBKButton::mousePressed(ofMouseEventArgs& eventArgs)
{
	
	ofxBKContainer::mousePressed(eventArgs);

	printf("mouse pressed in button\n");
	if(!isToggle) isSelected = true;
	else isSelected = !isSelected;
}

void ofxBKButton::mouseReleased(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mouseReleased(eventArgs);
	printf("mouse released in button\n");

	if(!isToggle) isSelected = false;

	setSelected(isSelected,true);
}

void ofxBKButton::mouseReleasedOutside(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mouseReleasedOutside(eventArgs);
	//printf("mouse released outside uiComponent\n");
	if(!isToggle) isSelected = false;	
}