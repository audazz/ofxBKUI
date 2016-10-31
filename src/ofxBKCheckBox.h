#pragma once
#include "ofxBKButton.h"
class ofxBKButton;

class ofxBKCheckbox : public ofxBKButton
{
public :
	ofxBKCheckbox();
	ofxBKCheckbox(std::string _label,float _x = 0, float _y =0, float _width = 0, float _height = 0);
	virtual ~ofxBKCheckbox() {};

	virtual void setSize(float _width, float _height, bool notify = true);
    virtual void updateChildrenSize();
protected:
    void updateselectArea();
    float labelOfset;
	virtual void init(std::string _label,float _x = 0, float _y =0, float _width = 0, float _height = 0);
	virtual void draw();

};
