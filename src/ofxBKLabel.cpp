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

	drawDebug = false;

	setAlign();

	textColor = ofxBKStyle::lightColor;

	textOffset = ofVec2f::zero();

	autoSize = width== 0 || height == 0;
	setText(_text);
}

void ofxBKLabel::draw()
{
	ofxBKUIComponent::draw();

	//printf("Label draw %s %f\n",text.c_str(),position.y);
	//if(strlen(text.c_str()) > 0 && textBoundingBox.width == 0) setText(text); //force refresh
	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofPushStyle();

		ofSetColor(ofxBKStyle::red);
		ofNoFill();
		ofLine(0,0,textOffset.x,textOffset.y);
		ofRect(textBoundingBox+textOffset);

		ofPopStyle();
	}

	ofSetColor(textColor);
	ofPushStyle();
	font->drawString(text,-textBoundingBox.x+textOffset.x,-textBoundingBox.getMaxY()+textOffset.y);
}



void ofxBKLabel::setText(string _text)
{
	text = _text;
	textBoundingBox.set(font->getStringBoundingBox(text,0,0));
	if(autoSize) autoSizeOnce();
	updateTextOffset();
}

void ofxBKLabel::setColor(ofColor color)
{
	textColor.set(color);
}

void ofxBKLabel::setAlign(int horizontal, int vertical)
{
	horizontalAlign = horizontal;
	verticalAlign = vertical;
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
	autoSizeOnce();
}

void ofxBKLabel::autoSizeOnce()
{
	width = textBoundingBox.width;
	height = textBoundingBox.height;
}

void ofxBKLabel::updateTextOffset()
{
	switch(horizontalAlign)
	{
		case BKUI_TEXTALIGN_LEFT:
			textOffset.x = 0;
		break;

		case BKUI_TEXTALIGN_CENTER:
			textOffset.x = (width-textBoundingBox.width)/2;
		break;

		case BKUI_TEXTALIGN_RIGHT:
			textOffset.x = width-textBoundingBox.width;
		break;
	}

	switch(verticalAlign)
	{
		case BKUI_TEXTALIGN_TOP:
			textOffset.y = textBoundingBox.height;
		break;

		case BKUI_TEXTALIGN_MIDDLE:
			textOffset.y = textBoundingBox.height + (height-textBoundingBox.height)/2;
		break;

		case BKUI_TEXTALIGN_BOTTOM:
			textOffset.y = textBoundingBox.height + height-textBoundingBox.height;
		break;
	}
}
