#pragma once
#include "ofxBKContainer.h"
#include "ofxBKLabel.h"

class ofxBKPanel : public ofxBKContainer
{
public:
	ofxBKPanel();
	ofxBKPanel(string _title, float _x = 0, float _y = 0, float _width = 300, float _height = 200);

	virtual ~ofxBKPanel(){/*std::cout << "!ofxBKPanel!"<< this << "!";*/};

	float headerHeight;

	virtual void setPadding(float top, float bottom, float left, float right);
	virtual void setPadding(float padding) { setPadding(padding,padding,padding,padding);};
	virtual void setPadding(float topBottom, float leftRight){setPadding(topBottom,topBottom,leftRight,leftRight);};

	ofxBKLabel * title = nullptr;
	bool showTitle;

	bool miniModeEnabled;
	bool isInMiniMode;

	bool draggable;

	string getTitle();
	void setTitle(string _title);
	//virtual void setSize(float _width, float _height, bool notify = true);

	virtual void printInfo();
protected :
	virtual void init(string _title, float _x = 0, float _y = 0, float _width = 300, float _height = 200);
	virtual void draw();



};
