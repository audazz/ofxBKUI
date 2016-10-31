#pragma once
#include "ofxBKSlider.h"

class ofxBKDoubleSlider : public ofxBKSlider
{
public :
	ofxBKDoubleSlider();
	ofxBKDoubleSlider(string _label, float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	virtual ~ofxBKDoubleSlider() {/*std::cout << "!ofxBKDoubleSlider!" << this <<"!";*/}
	float value2;
	void setValue(float _value2, bool notify = true);
	void setValue2(float _value2, bool notify = true);
	void setValues(float _value, float _value2, bool notify = true);
	ofVec2f getValues();
	ofVec2f getNormalizedValues();

	virtual void mousePressed(ofMouseEventArgs &e);
	virtual void mouseDragged(ofMouseEventArgs & e);

	virtual void printInfo();

protected:
	virtual void init(string _label, float _x = 0, float _y = 0, float _width = 50, float _height = 20);
	virtual void draw();

	virtual void updateLabelTF();

	int currentDraggingValue;
	int getNearestValueIndex(float value);
};
