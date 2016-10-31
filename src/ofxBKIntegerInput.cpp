#include "ofxBKIntegerInput.h"

ofxBKIntegerInput::ofxBKIntegerInput()
{
    //ctor
}
ofxBKIntegerInput::ofxBKIntegerInput(float _x, float _y, float _width,float _height)
{
    init(_x, _y, _width, _height);
}

void ofxBKIntegerInput::init(float _x, float _y, float _width, float _height)
{
    ofxBKContainer::init(_x, _y, _width, _height);
    value = 1000;
    wGap = 3;
    verticalAlign   = ofxBKAlign::MIDDLE;
    horizontalAlign = ofxBKAlign::CENTER;
    autoSize = width ==0;

    plusButton  = new ofxBKButton("+");
    plusButton->setSize(15,15);
    plusButton->isToggle = false;
    ofAddListener(plusButton->buttonSelected,this, &ofxBKIntegerInput::btSelected);

    minusButton = new ofxBKButton("-");
    minusButton->setSize(15,15);
    minusButton->isToggle = false;
    ofAddListener(minusButton->buttonSelected,this, &ofxBKIntegerInput::btSelected);

    addChild(plusButton);
    addChild(minusButton);

    labelTF = new ofxBKLabel("",0,0,5,5);

    normalizeLabelSize();
    labelTF->setAutoMinSize(true);
    updateLabelTF();
    labelTF->setAlign(ofxBKAlign::CENTER, ofxBKAlign::BOTTOM);
    addChild(labelTF);

    setSize(estimateTotalWidth(),-1);

    setValue(0);


}

void ofxBKIntegerInput::setLabel(string _label, string _labelSuffix)
{
    text       = _label;
    textSuffix = _labelSuffix;
    updateLabelTF();
}

void ofxBKIntegerInput::setValue(int _value)
{
    _value = (isMinValueSet && (_value < minValue)) ? minValue : _value;
    _value = (isMaxValueSet && (_value > maxValue)) ? maxValue : _value;

    value = _value;
    updateLabelTF();
	updateControlPosition();

    notifyValueChanged();
}

void ofxBKIntegerInput::setMinValue(int _min)
{
    isMinValueSet = true;
    minValue = _min;
    setValue(value);
}

void ofxBKIntegerInput::setMaxValue(int _max)
{
    isMaxValueSet = true;
    maxValue = _max;
    setValue(value);
}

void ofxBKIntegerInput::setMinMaxValues(int _min, int _max)
{
    setMinValue(_min);
    setMaxValue(_max);
}

void ofxBKIntegerInput::setAlignment(ofxBKAlign::V_ALIGN _verticalAlign,
                                     ofxBKAlign::H_ALIGN _horizontalAlign)
{
    verticalAlign = _verticalAlign;
    horizontalAlign = _horizontalAlign;
    updateControlPosition();
}

void ofxBKIntegerInput::updateLabelTF()
{
	labelTF->setText(text + std::to_string(value) + textSuffix);
}

void ofxBKIntegerInput::btSelected(ofxBKUIEventArgs &e)
{
    if (e.target == plusButton) {
        setValue(value + 1);
    } else if (e.target == minusButton) {
        setValue(value - 1);
    }
}


void ofxBKIntegerInput::notifyValueChanged()
{
	ofxBKUIEventArgs args;
	args.eventType = ofxBKUIEventArgs::VALUE_CHANGED;
	args.target = this;
	ofNotifyEvent(valueChanged,args);

}

void ofxBKIntegerInput::updateChildrenSize()
{
    updateControlPosition();
}

void ofxBKIntegerInput::updateControlPosition()
{
    if (minusButton == nullptr ||
        labelTF     == nullptr ||
        plusButton  == nullptr)
        return;

    updateOffset(estimateTotalWidth() , textHeight);

    float x(alignOffset.x);
    float y(alignOffset.y);
    float dyButton((labelTF->height - plusButton->width)/2);

    minusButton->setAbsolutePosition(x,y  + dyButton);
    x += plusButton->width + wGap;

    labelTF->setAbsolutePosition(x,y-2);
    x += labelTF->width + wGap;

    plusButton->setAbsolutePosition(x,y + dyButton);

}

void ofxBKIntegerInput::updateOffset(float _width, float _height)
{
	switch(horizontalAlign)
	{
		case ofxBKAlign::LEFT:
			alignOffset.x = 0;
		break;

		case ofxBKAlign::CENTER:
			alignOffset.x = (width-_width)/2;
		break;

		case ofxBKAlign::RIGHT:
			alignOffset.x = width-_width;
		break;
	}

	switch(verticalAlign)
	{
		case ofxBKAlign::TOP:
			alignOffset.y = 0;
		break;

		case ofxBKAlign::MIDDLE:
			alignOffset.y = (height-_height)/2;
		break;

		case ofxBKAlign::BOTTOM:
			alignOffset.y =  height-_height;
		break;
	}

}

void ofxBKIntegerInput::normalizeLabelSize()
{
    textHeight = labelTF->font->getStringBoundingBox("-01234567890",0,0).height + 4;
    textWidth  = labelTF->font->getStringBoundingBox("1000",0,0).width          + 4;
    labelTF->setMinSize(textWidth,textHeight);
    if (autoSize)
        labelTF->setSize(textWidth,textHeight);
}

float ofxBKIntegerInput::estimateTotalWidth()
{
    return plusButton->width + minusButton->width + labelTF->width + wGap*2;
}


void ofxBKIntegerInput::printInfo()
{
    ofxBKContainer::printInfo();
    printf("   ofxBKIntegerInput:%i\n", value);
}




















