#pragma once
#include "ofxBKUI.h"

ofxBKContainer::ofxBKContainer()
{

}

ofxBKContainer::ofxBKContainer(float _x, float _y, float _width, float _height )
{
	init(_x,_y,_width,_height);
}

void ofxBKContainer::init(float _x, float _y, float _width, float _height )
{
	numChildren = 0;
	ofxBKUIComponent::init(_x,_y,_width,_height);
	paddingTop = 0;
	paddingBottom = 0;
	paddingLeft = 0;
	paddingRight = 0;
	bgColor = ofColor(150,100);
	transparentBG = false;
}


void ofxBKContainer::draw()
{
	ofxBKUIComponent::draw();

	if(!transparentBG)
	{
		ofPushStyle();
		ofSetColor(bgColor);
		ofRect(0,0,width,height);
	}

	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofSetColor(ofColor::orange,100);
		ofNoFill();
		ofRect(paddingLeft,paddingTop,getInnerWidth(),getInnerHeight());
	}

	ofPopStyle();

}


void ofxBKContainer::addChild(ofxBKUIComponent * element)
{
	children.push_back(element);
	numChildren = children.size();
	element->setVisible(true);
	element->setParent(this);

	updateChildOffset(element);
}


void ofxBKContainer::setVisible(bool value)
{
	ofxBKUIComponent::setVisible(value);
	for(int i=0;i<numChildren;i++) children[i]->setVisible(value);
}

float ofxBKContainer::getInnerWidth()
{
	return width-paddingLeft-paddingRight;
}

float ofxBKContainer::getInnerHeight()
{
	return height-paddingTop-paddingBottom;
}

void ofxBKContainer::setOffset(float _offsetX, float _offsetY)
{
	//if(offset.x == _offsetX && offset.y == _offsetY) return;
	ofxBKUIComponent::setOffset(_offsetX, _offsetY);
	updateChildrenOffsets();
}

void ofxBKContainer::setPosition(float _x, float _y)
{
	//if(position.x == _x && position.y == _x) return;
	ofxBKUIComponent::setPosition(_x, _y);
	updateChildrenOffsets();
}

void ofxBKContainer::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width, _height, notify);
	//update children ?

}

void ofxBKContainer::setPadding(float top, float bottom, float left, float right)
{
	paddingTop = top;
	paddingBottom = bottom;
	paddingLeft = left;
	paddingRight = right;
	updateChildrenOffsets();
}

void ofxBKContainer::updateChildrenOffsets()
{
	for(int i=0;i<numChildren;i++)
	{
		updateChildOffset(children[i]);
	}
}

void ofxBKContainer::updateChildOffset(ofxBKUIComponent * child)
{
	child->setOffset(offset+position+ofVec2f(paddingLeft,paddingTop));
}