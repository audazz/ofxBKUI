#pragma once
#include "ofxBKContainer.h"
#include "ofxBKButton.h"

class ofxBKButtonBar : public ofxBKContainer
{
public:
	ofxBKButtonBar(float _x = 0, float _y = 0, float _width = 50, float _height = 20);
    virtual ~ofxBKButtonBar(){/*std::cout << "!ofxBKButtonBar!"<< this << "!";*/};
	//virtual void setSize(float _width, float _height, bool notify = true);

	void updateButtonsPositions();

	void setOptions( vector<string> const _options);
	void addOption(string const label);
	void removeOption(string const label);
    void clearOptions();
	void setAllowMultipleSelection(bool allow, int _maxNumbSelected = 0);

	void setSelectedIndex(int buttonIndex, bool notify = true);
	void setSelectedIndex(const std::vector<int> buttonIndex, bool notify = true);
	void setDeselectedIndex(int buttonIndex, bool notify = true);
	const vector<int> & getSelectedIndices()   {return selectedIndices;};
	const vector<string> & getSelectedLabels() {return selectedLabels; };
	virtual void updateChildrenSize();

    void setButtonToMinSize(bool _autosize = true);

    void setStartPosition(float _startPosition = 0);


	bool allowMultipleSelection;
	int lastSelect;
	string selectedLabel;


	ofEvent<ofxBKUIEventArgs> selectionChanged;
    virtual void printInfo();


private :
	void init(float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	void draw();
    void addButonToSelected(int buttonIndex);
    void rmButonFromSelected(int buttonIndex);

	void btSelected(ofxBKUIEventArgs &e);
	void btDeselected(ofxBKUIEventArgs &e);

	int getSelectedIndexForButtonIndex(int buttonIndex);
	int getIndexForButton(ofxBKButton * bt);
	float getTotalWidth();
	void updateStartPosition();

	vector<int>    selectedIndices;
	vector<string> selectedLabels;

	std::vector<ofxBKButton *>::iterator removeOption(int const index);

	ofxBKButton * currentBT;

	int numButtons;
	int maxNumbSelected;
	bool buttonToMinSize;
	float startPosition = 0;
	float startPositionRelative = 0;

	vector<ofxBKButton *> buttons;
	vector<string> buttonLabel;

};
