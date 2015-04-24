#pragma once
#include "ofxBKCheckBox.h"
#include "ofxBKStyle.h"

ofxBKCheckbox::ofxBKCheckbox()
{

}

ofxBKCheckbox::~ofxBKCheckbox()
{
}

ofxBKCheckbox::ofxBKCheckbox(string _label, float _x, float _y, float _width,float _height)
{	
	init(_label, _x, _y, _width,_height);
	labelSelectedColor = ofxBKStyle::lightColor;
	
}

void ofxBKCheckbox::init(string _label, float _x, float _y, float _width,float _height)
{
	label = NULL; //force null to avoid crash in setSize, should be fixed with containers and layouts

	ofxBKButton::init(_label,_x,_y,_width,_height);
	isToggle = true;
	label->setFluidHeight();
	label->setAlign(BKUI_TEXTALIGN_LEFT,BKUI_TEXTALIGN_MIDDLE);
}

void ofxBKCheckbox::draw()
{
	ofxBKButton::draw();
}

void ofxBKCheckbox::setSize(float _width, float _height, bool notify)
{
	ofxBKButton::setSize(_width, _height, notify);
	bgWidth = bgHeight;
	if(label != NULL) label->setFluidWidth(_height+5,1,false);
}