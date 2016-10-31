#pragma once
#include "ofxBKContainer.h"
#include "ofxBKLabel.h"
class ofxBKLabel;

class ofxBKButton : public ofxBKContainer
{
public:
	ofxBKButton();
	ofxBKButton(string _label, float _x = 0, float _y = 0, float _width = 0, float _height = 0);
	virtual ~ofxBKButton(){/*std::cout << "!ofxBKButton!"<< this << "!";*/};

	ofxBKLabel * label = nullptr;

	bool isToggle;
	bool isSelected;

	//style
	ofColor bgColor;
	ofColor	overColor;
	ofColor pressedColor;
	ofColor selectedColor;

	ofColor labelColor;
	ofColor labelOverColor;
	ofColor labelSelectedColor;

	string getLabel();
	void setLabel(string _label);
	void setSelected(bool value, bool notify = true);

	virtual void setSize(float _width, float _height, bool notify = true);

	ofEvent<ofxBKUIEventArgs> buttonSelected;
	ofEvent<ofxBKUIEventArgs> buttonDeselected;
    ofEvent<ofxBKUIEventArgs> selectionChange;
	virtual void printInfo();

protected:
	virtual void init(string _label, float _x = 0, float _y = 0, float _width = 0,float _height = 0);
	virtual void draw();

	float selectAreaWidth;
	float selectAreaHeight;
	ofVec2f selectAreaStart;

	void updateLabelColor();

	void mouseOut();
	void mousePressed(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
	void mouseReleasedOutside(ofMouseEventArgs &e);

	void notifySelected();
};














