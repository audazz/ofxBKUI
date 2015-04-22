#pragma once
#include "ofxBKUIComponent.h"


class ofxBKButton : public ofxBKUIComponent
{
public:
	ofxBKButton(string _label, float _x = 0, float _y = 0, float _width = 50, float _height = 20);

	void draw();

	string label;

	bool isToggle;
	bool isSelected;

	//style
	ofColor bgColor;
	ofColor	overColor;
	ofColor selectedColor;
	
	ofColor labelColor;
	ofColor labelOverColor;
	ofColor labelSelectedColor;

	void setSelected(bool value, bool notify = true);


	ofEvent<ofxBKUIEventArgs> buttonSelected;
	ofEvent<ofxBKUIEventArgs> buttonDeselected;

protected:
	void init(string _label, float _x, float _y, float _width,float _height);

	/*void mouseOver();
	void mouseOut();
	*/
	void mouseOut();
	void mousePressed(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
	void mouseReleasedOutside(ofMouseEventArgs &e);

	void notifySelected();
};
