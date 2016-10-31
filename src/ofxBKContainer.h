#pragma once
#include "ofxBKUIComponent.h"

class ofxBKContainer : public ofxBKUIComponent
{
public:
    virtual ~ofxBKContainer();
    explicit ofxBKContainer();
    ofxBKContainer(const ofxBKContainer& obj);//copy//
    ofxBKContainer& operator=(const ofxBKContainer& obj);// assignment
    // default move constructor is fine
	ofxBKContainer(float _x, float _y, float _width = 300, float _height = 200);


	virtual void setPadding(float top, float bottom, float left, float right);
	virtual void setPadding(float padding) { setPadding(padding,padding,padding,padding);};
	virtual void setPadding(float topBottom, float leftRight){setPadding(topBottom,topBottom,leftRight,leftRight);};

	virtual void setOffset(float _offsetX, float _offsetY);
	virtual void setSize(float _width, float _height, bool notify = true);

	virtual void setEnabled(bool value);
	virtual void parentSetEnabled(bool value);

	virtual void setVisible(bool value);
	virtual void parentSetVisible(bool value);


    virtual void setResizeChildren(bool _resizeChildren);
    virtual void setAccountForChildMinSize(bool _value);
    virtual bool getAccountForChildMinSize() { return accountForChildMinSize;};// no implementation just to work along with Container
    virtual void updateChildrenSize();

	float getInnerWidth();
	float getInnerHeight();


	virtual void addChild(     ofxBKUIComponent * element);
	virtual void removeChild(  ofxBKUIComponent * element);
	virtual void releaseChild( ofxBKUIComponent * element);


    virtual void printInfo();

	using ofxBKUIComponentPtr = std::unique_ptr<ofxBKUIComponent>;
	vector<ofxBKUIComponentPtr> children;
	int numChildren;

	float paddingTop;
	float paddingBottom;
	float paddingLeft;
	float paddingRight;

	ofColor bgColor;
	bool transparentBG;

protected:
	virtual void init(float _x = 0, float _y = 0, float _width = 300, float _height = 200);
	virtual void draw();

	virtual void setPosition(float _x, float _y);

	virtual void updateChildrenOffsets();
	virtual void updateChildOffset(ofxBKUIComponent * child);

	vector<int> childrenUpdateOrder;
	vector<int> childDrawOrder;
	bool alreadyUndone=false;
    bool autoResizeChildren;
	bool accountForChildMinSize;

};
