#pragma once
#include "ofMain.h"

#define BKUI_LAYOUT_BASIC 1
#define BKUI_LAYOUT_ADVANCED 2

#define BKUI_ANCHOR_TOP 0
#define BKUI_ANCHOR_BOTTOM 1
#define BKUI_ANCHOR_LEFT 0
#define BKUI_ANCHOR_RIGHT 1
#define BKUI_ANCHOR_MIDDLE .5
#define BKUI_ANCHOR_CENTER .5

class ofxBKUIEventArgs; //avoid cross dependency "undeclared identifier" error
class ofxBKContainer;

class ofxBKUIComponent
{
public:
	ofxBKUIComponent();
	
	bool drawDebug;

	bool enabled;
	bool visible;
	
	virtual void setVisible(bool value);
	virtual void setEnabled(bool value);

	
	virtual void setSize(float _width, float _height, bool notify = true);

	bool mouseEnabled;

	ofxBKContainer * parent;
	void setParent(ofxBKContainer *_parent);

	float width;
	float height;

	ofVec2f minSize;
	ofVec2f maxSize;

	virtual ofxBKUIComponent * setMinSize(float _minW, float _minH);
	virtual ofxBKUIComponent * setMaxSize(float _maxW, float _maxH);

	ofVec2f offset;
	ofVec2f position;

	bool fixedWidth,fixedHeight;
	bool topIsRelative, bottomIsRelative,leftIsRelative,rightIsRelative;
	float top, bottom, left, right;

	virtual void setOffset(float _offsetX, float _offsetY);
	virtual void setOffset(ofVec2f _offset){ setOffset(_offset.x,_offset.y); };
	
	virtual void setAbsolutePosition(float _x, float _y);
	
	virtual ofxBKUIComponent * setAdvancedFixedWidth(float _width, float _innerAnchor = BKUI_ANCHOR_CENTER, float _outerAnchor = BKUI_ANCHOR_CENTER, bool outerIsRelative = true, bool outerIsFromRight = false);
	
	virtual ofxBKUIComponent * setAdvancedFixedHeight(float _height, float innerAnchor = BKUI_ANCHOR_MIDDLE, float outerAnchor = BKUI_ANCHOR_MIDDLE, bool outerIsRelative = true, bool outerIsFromBottom = false);

	virtual ofxBKUIComponent * setFluidWidth(float _left = 0, float _right = 1, bool _leftIsRelative = true, bool _rightIsRelative = true);
	virtual ofxBKUIComponent * setFluidHeight(float _top = 0, float _bottom = 1, bool _topIsRelative = true, bool _bottomIsRelative = true);

	ofRectangle getLocalBounds();
	ofRectangle getGlobalBounds();

	ofEvent<ofxBKUIEventArgs> resized;

	virtual ofVec2f getMousePosition(bool relative = false);

protected :
	virtual void init(float _x, float _y, float _width,float _height);
	virtual void draw();

	void drawHandler(ofEventArgs& eventArgs);
	void mousePressedHandler(ofMouseEventArgs& eventArgs);
	void mouseReleasedHandler(ofMouseEventArgs& eventArgs);
	void mouseDraggedHandler(ofMouseEventArgs& eventArgs);

	virtual void mouseOver();
	virtual void mouseOut();
	virtual void mousePressed(ofMouseEventArgs &e);
	virtual void mouseReleased(ofMouseEventArgs &e);
	virtual void mouseReleasedOutside(ofMouseEventArgs &e);
	virtual void mouseDragged(ofMouseEventArgs &e);

	bool isPressed;
	bool isOver;

	bool isMouseInside();

	ofVec2f initMousePos;
	ofVec2f mouseDelta;
	ofVec2f mouseAbsoluteDelta;

	virtual void setPosition(float _x, float _y);
	virtual void setPosition(ofVec2f _position){ setOffset(_position.x,_position.y); };

	void updatePosition();
	virtual void parentResizedHandler(ofxBKUIEventArgs &e);
};