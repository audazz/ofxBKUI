#pragma once
#include "ofxBKUIComponent.h"
#include "ofTrueTypeFont.h"

#define BKUI_TEXTALIGN_LEFT 0
#define BKUI_TEXTALIGN_CENTER 1
#define BKUI_TEXTALIGN_RIGHT 2
#define BKUI_TEXTALIGN_TOP 0
#define BKUI_TEXTALIGN_MIDDLE 1
#define BKUI_TEXTALIGN_BOTTOM 2

class ofxBKLabel : public ofxBKUIComponent
{
public :
	ofxBKLabel();
	ofxBKLabel(string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);

	void setText(string _text);

	ofRectangle textBoundingBox;

	ofTrueTypeFont * font;
	string text;

	ofColor textColor;

	int horizontalAlign;
	int verticalAlign;
	void setAlign(int horizontal = BKUI_TEXTALIGN_LEFT, int vertical = BKUI_TEXTALIGN_TOP);

	virtual void setSize(float _width, float _height, bool notify = true);
	
	bool autoSize;

	void autoSizeOnce();
	void setAutoSize(bool _autoSize);

	void setColor(ofColor color);


protected:
	virtual void init(string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);
	virtual void draw();

	virtual void setPosition(float _x, float _y);

	ofVec2f textOffset;
	void updateTextOffset();
};