#pragma once
#include "ofMain.h"
#include "ofxBKUIEventArgs.h"

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

class ofxBKUIComponent : public ofxBKUIEventHandlers
{
public:
	ofxBKUIComponent();
    virtual ~ofxBKUIComponent();

	virtual void setSize(float _width, float _height, bool notify = true);
	virtual ofxBKUIComponent * setMinSize(float _minW, float _minH);
	virtual ofxBKUIComponent * setMaxSize(float _maxW, float _maxH);

    virtual void setAccountForChildMinSize(bool _value) {};// no implementation just to work along with Container
    virtual bool getAccountForChildMinSize() { return false;};// no implementation just to work along with Containers
	virtual void parentResized();

	void setParent(ofxBKContainer *_parent);

	virtual void setOffset(float _offsetX, float _offsetY);
	virtual void setOffset(ofVec2f _offset){ setOffset(_offset.x,_offset.y); };

	virtual void setAbsolutePosition(float _x, float _y);
	virtual ofxBKUIComponent * setAdvancedFixedWidth(float _width, float _innerAnchor = BKUI_ANCHOR_CENTER,
                                                                   float _outerAnchor = BKUI_ANCHOR_CENTER,
                                                                   bool outerIsRelative  = true,
                                                                   bool outerIsFromRight = false);
	virtual ofxBKUIComponent * setAdvancedFixedHeight(float _height, float innerAnchor = BKUI_ANCHOR_MIDDLE,
                                                                     float outerAnchor = BKUI_ANCHOR_MIDDLE,
                                                                     bool outerIsRelative   = true,
                                                                     bool outerIsFromBottom = false);
	virtual ofxBKUIComponent * setFluidWidth(float _left = 0, float _right  = 1,
                                             bool _leftIsRelative   = true,
                                             bool _rightIsRelative  = true);
	virtual ofxBKUIComponent * setFluidHeight(float _top = 0, float _bottom = 1,
                                              bool _topIsRelative    = true,
                                              bool _bottomIsRelative = true);

	virtual void setEnabled(bool value);
	virtual void parentSetEnabled(bool value);

	virtual void setVisible(bool value);
	virtual void parentSetVisible(bool value);

    bool isVisible() {return (visible && parentVisible);};
    bool isEnabled() {return (enabled && parentEnabled);};
	virtual void bringToFront();//BRING TO FRONT MEGA TEMP !!

	void setDrawable(bool _drawable) {isDrawable = _drawable;updateDrawListener();}

	void addFunctionCalledInDraw(   ofxBKUIEventHandlers * ptr);
	void removeFunctionCalledInDraw(ofxBKUIEventHandlers * ptr);
	void clearFunctionCalledInDraw();

	ofRectangle getLocalBounds();
	ofRectangle getGlobalBounds();

	virtual ofVec2f getMousePosition(bool relative = false);

	virtual void printInfo();

	ofVec2f position;
	float width;
	float height;
	ofVec2f minSize;
	ofVec2f maxSize;
	ofVec2f offset;

	bool mouseEnabled;


	ofxBKContainer * parent = nullptr;

	bool fixedWidth,fixedHeight;
	bool autoResizeWithParents;
	bool isSizeUpdateSuccess;
	bool topIsRelative, bottomIsRelative,leftIsRelative,rightIsRelative;
	float top, bottom, left, right;

	bool drawDebug;
	bool printResizeError;

	ofEvent<ofxBKUIEventArgs> resized;

protected :
	virtual void init(float _x, float _y, float _width,float _height);

	virtual void draw();
	void drawHandler(ofEventArgs& eventArgs);
    void updateDrawListener();
	void mousePressedHandler(ofMouseEventArgs& eventArgs);
	void mouseReleasedHandler(ofMouseEventArgs& eventArgs);
	void mouseDraggedHandler(ofMouseEventArgs& eventArgs);

	virtual void mouseOver();
	virtual void mouseOut();
	virtual void mousePressed(ofMouseEventArgs &e);
	virtual void mouseReleased(ofMouseEventArgs &e);
	virtual void mouseReleasedOutside(ofMouseEventArgs &e);
	virtual void mouseDragged(ofMouseEventArgs &e);
	bool isMouseInside();

	virtual void setPosition(float _x, float _y);
	virtual void setPosition(ofVec2f _position){ setOffset(_position.x,_position.y); };

	virtual void updatePosition();
	virtual void parentResizedHandler(ofxBKUIEventArgs &e);

	bool enabled;
	bool parentEnabled;
	bool visible;
	bool parentVisible;

	bool isPressed;
	bool isOver;

	ofVec2f initMousePos;
	ofVec2f mouseDelta;
	ofVec2f mouseAbsoluteDelta;

	bool isDrawable = true;

	std::vector< ofxBKUIEventHandlers *> functionsCalledInDraw;

};




