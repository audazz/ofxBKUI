#pragma once
#include "ofxBKContainer.h"
#include "ofxBKButton.h"

class ofxBKButtonBar : public ofxBKContainer
{
public:
	ofxBKButtonBar(float _x = 0, float _y = 0, float _width = 50, float _height = 20);

	void setOptions(vector<string> _options);
	void addOption(string label);

	int selectedIndex;
	void setSelectedIndex(int index, bool notify = true);
	void setDeselectedIndex(int index, bool notify = true);
	string selectedLabel;

	bool allowMultipleSelection;
	vector<int> selectedIndices;
	vector<string> selectedLabels;

	void setAllowMultipleSelection(bool allow);

	virtual void setSize(float _width, float _height, bool notify = true);

	ofEvent<ofxBKUIEventArgs> selectionChanged;

	void updateButtonsPositions();
	
private :
	void init(float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	void draw();


	vector<ofxBKButton *> buttons;
	int numButtons;

	void btSelected(ofxBKUIEventArgs &e);
	void btDeselected(ofxBKUIEventArgs &e);

	int getIndexForSelectedIndex(int sIndex);

	ofxBKButton * currentBT;

	int getIndexForButton(ofxBKButton * bt);
};