#pragma once
#include "ofxBKUIComponent.h"

#define BKEVENT_SELECTED 1
#define BKEVENT_DESELECTED 2
#define BKEVENT_SELECTION_CHANGED 3
#define BKEVENT_RESIZED 4
#define BKEVENT_VALUE_CHANGED 5

class ofxBKUIEventArgs
{
public:
	ofxBKUIComponent * target;
	int eventType;
};