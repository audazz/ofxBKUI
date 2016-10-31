#include "ofxBKCheckBox.h"
#include "ofxBKStyle.h"

ofxBKCheckbox::ofxBKCheckbox()
{

}


ofxBKCheckbox::ofxBKCheckbox(std::string _label, float _x, float _y, float _width,float _height)
{
	init(_label, _x, _y, _width,_height);

}

void ofxBKCheckbox::init(std::string _label, float _x, float _y, float _width,float _height)
{
    //TODO: test the above
	ofxBKButton::init(_label,_x,_y,_width,_height);
	labelOfset = 5;
	labelSelectedColor = ofxBKStyle::lightColor;
	isToggle = true;
	label->setFluidHeight();
	label->setAlign(ofxBKAlign::LEFT,ofxBKAlign::MIDDLE);
}

void ofxBKCheckbox::draw()
{
	ofxBKButton::draw();
}


void ofxBKCheckbox::updateChildrenSize()
{
    updateselectArea();
    if(label != nullptr){
        label->setFluidWidth(selectAreaWidth + labelOfset , 1 , false);
        isSizeUpdateSuccess = (isSizeUpdateSuccess && label->isSizeUpdateSuccess);
        //isSizeUpdateSuccess = label->isSizeUpdateSuccess;
	}

}

void ofxBKCheckbox::setSize(float _width, float _height, bool notify)
{
	ofxBKContainer::setSize(_width,_height,notify);

}

void ofxBKCheckbox::updateselectArea()
{
    selectAreaHeight = getInnerHeight() * 0.8;
    selectAreaWidth  = selectAreaHeight;
    selectAreaStart.set( (height - selectAreaWidth )/2 , // height is not an error
                         (height - selectAreaHeight)/2 );
}
