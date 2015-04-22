#pragma once
#include "ofxBKButtonBar.h"
#include "ofxBKUIEventArgs.h"

ofxBKButtonBar::ofxBKButtonBar(float _x, float _y, float _width,float _height)
{
	init(_x, _y, _width,_height);
	
}

void ofxBKButtonBar::init(float _x, float _y, float _width,float _height)
{
	ofxBKContainer::init(_x, _y,_width,_height);
	selectedIndex = -1;
	selectedLabel = "";
}

void ofxBKButtonBar::draw()
{
	ofxBKContainer::draw();
	//printf("Draw %i\n",positionMode);
}

void ofxBKButtonBar::updateButtonsPositions()
{
	float gap = 10;
	float btWidth = (width-gap*(numButtons-1))/numButtons;

	for(int i=0;i<buttons.size();i++) 
	{
		ofxBKButton * bt = buttons[i];
		bt->setPosition(i*(btWidth+gap),0);
		bt->setSize(btWidth,height);
	}
}

void ofxBKButtonBar::setSize(float _width, float _height, bool notify)
{
	ofxBKContainer::setSize(_width,_height,notify);
	updateButtonsPositions();
	
}

void ofxBKButtonBar::addOption(string label)
{
	ofxBKButton * bt = new ofxBKButton(label);
	ofAddListener(bt->buttonSelected,this,&ofxBKButtonBar::btSelected);
	addChild(bt);
	buttons.push_back(bt);
	numButtons = buttons.size();
	updateButtonsPositions();
}

void ofxBKButtonBar::setOptions(vector<string> _options)
{
	for(int i=0;i<_options.size();i++) addOption(_options[i]);
}



void ofxBKButtonBar::setSelectedIndex(int index, bool notify)
{
	if(selectedIndex >= 0 && selectedIndex < numButtons)
	{
		buttons[selectedIndex]->setSelected(false,false);
	}

	if(index >= 0 && index < numButtons)
	{
		selectedLabel = buttons[index]->label;
		selectedIndex = index;
		buttons[selectedIndex]->setSelected(true,false);
	}

	if(notify)
	{
		ofxBKUIEventArgs args;
		args.eventType = BKEVENT_SELECTION_CHANGED;
		args.target = this;
		ofNotifyEvent(selectionChanged,args);
	}
}


void ofxBKButtonBar::btSelected(ofxBKUIEventArgs &e)
{
	printf("Selected \n");
	ofxBKButton * bt = (ofxBKButton *)e.target;
	setSelectedIndex(getIndexForButton(bt),true);
}


int ofxBKButtonBar::getIndexForButton(ofxBKButton * bt)
{
	for(int i=0;i<numButtons;i++)
	{
		if(buttons[i] == bt) return i;
	}

	return -1;
}