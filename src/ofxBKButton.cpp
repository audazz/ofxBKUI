#include "ofxBKButton.h"
#include "ofxBKUIEventArgs.h"
#include "ofxBKStyle.h"
#include <algorithm>

ofxBKButton::ofxBKButton()
{
}

ofxBKButton::ofxBKButton(string _label, float _x, float _y, float _width,float _height)
{
	init(_label, _x, _y, _width,_height);
}

void ofxBKButton::init(string _label, float _x, float _y, float _width,float _height)
{
	ofxBKContainer::init(_x, _y, _width, _height);

	label = new ofxBKLabel(_label,0,0,_width,_height);
	if(label->autoSize) {
        if (_width==0)
            _width = label->width + 50;
        if (_height==0)
            _height = label->height + 10;
        setSize(_width,_height);
    }
	addChild(label);
	label->setAlign(ofxBKAlign::CENTER,
                    ofxBKAlign::MIDDLE);
	label->setFluidWidth()->setFluidHeight();
	label->setAutoMinSize(true);

	isToggle   = false;
	isSelected = false;
	isPressed  = false;

	bgColor = ofxBKStyle::bgColor+10;
	overColor = ofxBKStyle::normalColor;
	pressedColor = ofxBKStyle::semiLightColor;
	selectedColor = ofxBKStyle::highlightColor;
	labelColor = ofxBKStyle::lightColor;
	labelOverColor = ofxBKStyle::lightColor;
	labelSelectedColor =  ofxBKStyle::darkColor;
	selectAreaStart.set(0,0);

	transparentBG = true;

	printResizeError = false;
}


void ofxBKButton::draw()
{
	ofxBKContainer::draw();

	ofColor targetColor;
	if(isPressed && isOver) targetColor.set(pressedColor);
	else if(isSelected)     targetColor.set(selectedColor);
	else                    targetColor.set(bgColor);

	if(isOver){
        targetColor.set(min(targetColor.r + 30, 255),
                        min(targetColor.g + 30, 255),
                        min(targetColor.b + 30, 255));
	}

	ofSetColor(targetColor,(isEnabled())? 255 : 50);
    ofDrawRectangle(selectAreaStart.x,
           selectAreaStart.y,
           selectAreaWidth,
           selectAreaHeight);
}


string ofxBKButton::getLabel()
{
if (label != nullptr)
	return label->text;
else
    return "";
}

void ofxBKButton::setLabel(string _label)
{
	label->setText(_label);
}

void ofxBKButton::updateLabelColor()
{
	label->setColor((isSelected)?labelSelectedColor:(isOver ? labelOverColor : labelColor));
}

//selection handling
void ofxBKButton::setSelected(bool value, bool notify)
{
	isSelected = value;
	updateLabelColor();

	if(notify) notifySelected();
}

void ofxBKButton::notifySelected()
{
	ofxBKUIEventArgs args;

	args.eventType = isSelected ? ofxBKUIEventArgs::SELECTED :
                                  ofxBKUIEventArgs::DESELECTED;
	args.target = this;
	ofNotifyEvent(isSelected ? buttonSelected : buttonDeselected , args);
	ofNotifyEvent(selectionChange,args);
}

//size

void ofxBKButton::setSize(float _width, float _height, bool notify)
{
	ofxBKContainer::setSize(_width,_height,notify);
	selectAreaWidth = width;
	selectAreaHeight = height;
}

//mouse interaction

void ofxBKButton::mouseOut()
{
	if(!isToggle && isPressed) isSelected = false;
}

void ofxBKButton::mousePressed(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mousePressed(eventArgs);
}

void ofxBKButton::mouseReleased(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mouseReleased(eventArgs);

	if(!isToggle)
	{
		setSelected(true,true);
		setSelected(false,true);
	}else
	{
		setSelected(!isSelected);
	}


}

void ofxBKButton::mouseReleasedOutside(ofMouseEventArgs& eventArgs)
{
	ofxBKContainer::mouseReleasedOutside(eventArgs);
	//printf("mouse released outside uiComponent\n");
	if(!isToggle) isSelected = false;
}



void ofxBKButton::printInfo()
{
    ofxBKContainer::printInfo();
    std::cout << "   ofxBKButton:" << getLabel()<< "" << std::endl;
}



