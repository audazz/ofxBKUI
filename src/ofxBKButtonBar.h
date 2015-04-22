#pragma once
#include "ofxBKContainer.h"
#include "ofxBKButton.h"

class ofxBKButtonBar : public ofxBKContainer
{
public:
	ofxBKButtonBar(float _x = 0, float _y = 0, float _width = 50, float _height = 20);

	void init(float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	void draw();

	void setOptions(vector<string> _options);
	void addOption(string label);

	int selectedIndex;
	void setSelectedIndex(int index, bool notify = true);
	string selectedLabel;

	virtual void setSize(float _width, float _height, bool notify = true);

	ofEvent<ofxBKUIEventArgs> selectionChanged;

	void updateButtonsPositions();
	
private :
	vector<ofxBKButton *> buttons;
	int numButtons;

	void btSelected(ofxBKUIEventArgs &e);
	
	ofxBKButton * currentBT;

	int getIndexForButton(ofxBKButton * bt);
};