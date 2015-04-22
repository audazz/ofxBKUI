#pragma once
#include "ofxBKContainer.h"

class ofxBKPanel : public ofxBKContainer
{
public:
	ofxBKPanel();
	ofxBKPanel(string _title, float _x = 0, float _y = 0, float _width = 300, float _height = 200);

	virtual void init(string _title, float _x = 0, float _y = 0, float _width = 300, float _height = 200);
	virtual void draw();

	float headerHeight;

	virtual void setPadding(float top, float bottom, float left, float right);
	virtual void setPadding(float padding) { setPadding(padding,padding,padding,padding);};
	virtual void setPadding(float topBottom, float leftRight){setPadding(topBottom,topBottom,leftRight,leftRight);};
	
	string title;
	bool showTitle;
	
	bool miniModeEnabled;
	bool isInMiniMode;
	
	bool draggable;

};
