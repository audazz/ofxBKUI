#pragma once
#include "ofxBKUIComponent.h"
#include "ofxBKTTFplus.h"

/*
#define BKUI_TEXTALIGN_LEFT 0
#define BKUI_TEXTALIGN_CENTER 1
#define BKUI_TEXTALIGN_RIGHT 2
#define BKUI_TEXTALIGN_TOP 0
#define BKUI_TEXTALIGN_MIDDLE 1
#define BKUI_TEXTALIGN_BOTTOM 2
*/
namespace ofxBKAlign
{
	enum H_ALIGN { LEFT, CENTER, RIGHT  };
	enum V_ALIGN { TOP,  MIDDLE, BOTTOM };
}

class ofxBKLabel : public ofxBKUIComponent
{
public :
	ofxBKLabel();
	ofxBKLabel(std::string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);
    virtual ~ofxBKLabel() {/*std::cout << "!ofxBKLabel!" << this << "!";*/};

	virtual void setText(std::string _text);


	ofRectangle textBoundingBox;

    ofxBKTTFplus * font;
	std::string text;

	ofColor textColor;
	ofxBKAlign::H_ALIGN horizontalAlign;
	ofxBKAlign::V_ALIGN verticalAlign;

	void setAlign(ofxBKAlign::H_ALIGN horizontal = ofxBKAlign::LEFT,
                  ofxBKAlign::V_ALIGN vertical   = ofxBKAlign::TOP);
	void setAlignTextLine(bool _alignWidthTextLine = true);

	virtual void setSize(float _width, float _height, bool notify = true);

	bool autoSize;

	void autoSizeOnce();
	void setAutoSize(bool _autoSize);

	void setColor(ofColor color);

	void setAutoMinSize(bool _autoMinSize);

	virtual void printInfo();
protected:
	virtual void init(std::string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);
	virtual void draw();
	virtual void textDraw();
	virtual void debugDraw();

	virtual void setPosition(float _x, float _y);

	virtual ofVec2f getTextStartPosition();
	virtual void calculateBoundingBox();

	ofVec2f textOffset;
	virtual void updateTextOffset();
	bool autoMinSize;
	bool alignWidthTextLine;
	using ofFboPtr = std::unique_ptr<ofFbo>;
    ofFboPtr fbo;
};
