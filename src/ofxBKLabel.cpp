#include "ofxBKUI.h"

ofxBKLabel::ofxBKLabel()
{
}

ofxBKLabel::ofxBKLabel(string _text, float _x, float _y, float _width,float _height)
{
	init(_text,_x,_y,_width,_height);
}

void ofxBKLabel::init(string _text, float _x, float _y, float _width,float _height)
{
	ofxBKUIComponent::init(_x,_y,_width,_height);
	font = &ofxBKStyle::normalFont;

    alignWidthTextLine =  false;

	fbo = ofFboPtr(new ofFbo());
	drawDebug = false;

	textColor = ofxBKStyle::lightColor;

	textOffset = ofVec2f::zero();
	autoSize = (width == 0) || (height == 0);
	autoMinSize = false;

	setAlign();
	setText(_text);
	//drawDebug = true;
}

void ofxBKLabel::draw()
{
	ofxBKUIComponent::draw();

	//printf("Label draw %s %f\n",text.c_str(),position.y);
	//if(strlen(text.c_str()) > 0 && textBoundingBox.width == 0) setText(text); //force refresh
    debugDraw();

	textDraw();
}

void ofxBKLabel::textDraw()
{
	ofPushStyle();
        ofSetColor(textColor);
        auto pos(getTextStartPosition());
        font->drawString(text, pos.x, pos.y);//TODO: boundary check in draw string
    ofPopStyle();
}

void ofxBKLabel::debugDraw()
{
	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofPushStyle();

		ofSetColor(ofxBKStyle::red);
		ofNoFill();
		ofLine(0,0,textOffset.x,textOffset.y); // TODO: Probably wrong
		ofRect(textBoundingBox+getTextStartPosition());// TODO: Probably wrong
        //ofRect(textBoundingBoxReal);
		ofDrawSphere(getTextStartPosition(),2);

		ofPopStyle();
	}
}

void ofxBKLabel::calculateBoundingBox()
{
	textBoundingBox.set(font->getStringBoundingBox(text,0,0));
	if (alignWidthTextLine) {
        //size_t n = std::count(text.begin(), text.end(), '\n');

        ofRectangle tmp(font->getStringBoundingBox("M",0,0));
        textBoundingBox.width  = std::max(textBoundingBox.width,tmp.width);
        tmp = font->getStringBoundingBox("l",0,0);
        textBoundingBox.y      = std::min(tmp.y, textBoundingBox.y);
        tmp = font->getStringBoundingBox("lgp",0,0);
        double h(std::max(textBoundingBox.height,tmp.height));
        textBoundingBox.height = h;
	}
}

ofVec2f ofxBKLabel::getTextStartPosition()
{
    return ofVec2f(textOffset.x - textBoundingBox.x,
                   textOffset.y - textBoundingBox.getMaxY());
}


void ofxBKLabel::setText(string _text)
{
	text = _text;
	calculateBoundingBox();

    if (autoMinSize){
        setMinSize(textBoundingBox.getWidth()  + 2,
                   textBoundingBox.getHeight() + 2);
    }
	if(autoSize) autoSizeOnce();

    this->updateTextOffset();

}

void ofxBKLabel::setAlignTextLine(bool _alignWidthTextLine)
{
   alignWidthTextLine = _alignWidthTextLine;
   calculateBoundingBox();
   updateTextOffset();
}

void ofxBKLabel::setColor(ofColor color)
{
	textColor.set(color);
}

void ofxBKLabel::setAutoMinSize(bool _autoMinSize)
{
    autoMinSize = _autoMinSize;
    setText(text);
}


void ofxBKLabel::setAlign(ofxBKAlign::H_ALIGN horizontal, ofxBKAlign::V_ALIGN vertical)
{
	horizontalAlign = horizontal;
	verticalAlign   = vertical;
	updateTextOffset();
}

void ofxBKLabel::setPosition(float _x, float _y)
{
	ofxBKUIComponent::setPosition(_x,_y);

}

void ofxBKLabel::setSize(float _width, float _height, bool notify)
{
	autoSize = false;
	ofxBKUIComponent::setSize(_width,_height,notify);

	updateTextOffset();
}

void ofxBKLabel::setAutoSize(bool _autoSize)
{
	autoSize = _autoSize;
	if (autoSize)
        autoSizeOnce();
}

void ofxBKLabel::autoSizeOnce()
{
	width   = (width==0)  ? textBoundingBox.width  : width;
	height  = (height==0) ? textBoundingBox.height : height;
}

void ofxBKLabel::updateTextOffset()
{
	switch(horizontalAlign)
	{
		case ofxBKAlign::LEFT:
			textOffset.x = 0;
		break;

		case ofxBKAlign::CENTER:
			textOffset.x = (width-textBoundingBox.width)/2;
		break;

		case ofxBKAlign::RIGHT:
			textOffset.x = width-textBoundingBox.width;
		break;
	}

	switch(verticalAlign)
	{
		case ofxBKAlign::TOP:
			textOffset.y = textBoundingBox.height;
		break;

		case ofxBKAlign::MIDDLE:
			textOffset.y = textBoundingBox.height + (height-textBoundingBox.height)/2;
		break;

		case ofxBKAlign::BOTTOM:
			textOffset.y = textBoundingBox.height + height-textBoundingBox.height;
		break;
	}
}


void ofxBKLabel::printInfo()
{
    ofxBKUIComponent::printInfo();
    std::cout << "   ofxBKLabel:" << text << "" << std::endl;
}








