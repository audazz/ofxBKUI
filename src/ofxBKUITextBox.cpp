#include "ofxBKUITextBox.h"
#include "ofxBKUI.h"

ofxBKUITextBox::ofxBKUITextBox(std::string _text, float _x, float _y, float _width,float _height)
{
    init(_text,_x,_y,_width,_height);
}

void ofxBKUITextBox::init(string _text, float _x, float _y, float _width, float _height)
{
	ofxBKLabel::init(_text,_x,_y,_width,_height);
    alignWidthTextLine =  true;
}

void ofxBKUITextBox::textDraw()
{
	ofPushStyle();
        ofSetColor(textColor);
        auto pos(getTextStartPosition());
        font->drawString(text, pos.x+startPosition.x, pos.y+startPosition.y, ofRectangle(0,0,width,height));//TODO: boundary check in draw string
    ofPopStyle();
}

void ofxBKUITextBox::debugDraw()
{
	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofPushStyle();

		ofSetColor(ofxBKStyle::red);
		ofNoFill();
		ofLine(0,0,textOffset.x,textOffset.y);
		ofRect(textBoundingBox+getTextStartPosition()+startPosition);
        //ofRect(textBoundingBoxReal);
		ofDrawSphere(getTextStartPosition()+startPosition,2);

		ofPopStyle();
	}
}

void ofxBKUITextBox::updateTextOffset()
{
    ofxBKLabel::updateTextOffset();
    updateStartPosition();
}

void ofxBKUITextBox::setStartPositionX(float _startPosition)
{
    _startPosition = (_startPosition>1) ? 1 : _startPosition;
    _startPosition = (_startPosition<0) ? 0 : _startPosition;
    startPositionRelative.x = _startPosition;
    updateStartPosition();

}

void ofxBKUITextBox::setStartPositionY(float _startPosition)
{
    _startPosition = (_startPosition>1) ? 1 : _startPosition;
    _startPosition = (_startPosition<0) ? 0 : _startPosition;
    startPositionRelative.y = _startPosition;
    updateStartPosition();

}

void ofxBKUITextBox::updateStartPosition()
{
    startPosition.x = ofMap(startPositionRelative.x, 0, 1, 0, width  - textBoundingBox.width  );
    startPosition.y = ofMap(startPositionRelative.y, 0, 1, 0, height - textBoundingBox.height );
    //    std::cout << "startPosition:" << startPosition << "" << std::endl;
}




