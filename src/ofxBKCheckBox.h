#pragma once
#include "ofxBKButton.h"

class ofxBKCheckbox : public ofxBKButton
{
public :
	ofxBKCheckbox();
	ofxBKCheckbox(string _label,float _x = 0, float _y =0, float _width = 0, float _height = 0);
	virtual ~ofxBKCheckbox();

	virtual void setSize(float _width, float _height, bool notify = true);

protected:
	virtual void init(string _label,float _x = 0, float _y =0, float _width = 0, float _height = 0);
	virtual void draw();

	
};