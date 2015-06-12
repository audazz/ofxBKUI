#include "ofxBKUI.h"

ofxBKUIComponent::ofxBKUIComponent()
{
}

void ofxBKUIComponent::init(float _x, float _y, float _width,float _height)
{
	//printf("UIComponent init %f %f\n",_width,_height);
	visible = false;
	enabled = true;
	isOver = false;
	isPressed = false;
	mouseEnabled = true;

	drawDebug = false;

	parent = NULL;

	offset.set(0,0);

	minSize = ofVec2f::zero();
	maxSize = ofVec2f::zero();

	setAbsolutePosition(_x,_y);
	setSize(_width,_height);


	ofAddListener(ofEvents().draw,this,&ofxBKUIComponent::drawHandler);
	ofAddListener(ofEvents().mousePressed,this,&ofxBKUIComponent::mousePressedHandler);
	ofAddListener(ofEvents().mouseReleased,this,&ofxBKUIComponent::mouseReleasedHandler);
	ofAddListener(ofEvents().mouseDragged,this,&ofxBKUIComponent::mouseDraggedHandler);
}

void ofxBKUIComponent::draw()
{
	//printf("Draw !\n");
	//to be overriden
	if(ofxBKUI::freeze) return;

	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofPushStyle();
		ofSetColor(ofColor::red,100);
		ofLine(-10,0,10,0);
		ofLine(0,-10,0,10);
		ofSetColor(ofColor::purple,100);
		ofNoFill();
		ofRect(0,0,width,height);

		ofPopStyle();
	}
}


void ofxBKUIComponent::drawHandler(ofEventArgs& eventArgs)
{
	if(!visible || ofxBKUI::freeze) return;

	ofPushMatrix();
		ofTranslate(offset);
		ofTranslate(position);
		ofPushStyle();

			draw();

		ofPopStyle();
	ofPopMatrix();

	if(enabled && mouseEnabled)
	{
		bool mouseInside = isMouseInside();
		if(mouseInside && !isOver) mouseOver();
		if(!mouseInside && isOver) mouseOut();

		isOver = mouseInside;
	}
}

void ofxBKUIComponent::mousePressedHandler(ofMouseEventArgs& eventArgs)
{
	if(!enabled || !visible || !mouseEnabled) return;

	if(isMouseInside())
	{
		isPressed = true;
		initMousePos = getMousePosition();
		mouseDelta = ofVec2f::zero();
		mouseAbsoluteDelta = ofVec2f::zero();

		mousePressed(eventArgs);
	}

}

void ofxBKUIComponent::mouseReleasedHandler(ofMouseEventArgs& eventArgs)
{
	if(!enabled || !visible || !mouseEnabled) return;


	if(isMouseInside()) mouseReleased(eventArgs);
	else if(isPressed) mouseReleasedOutside(eventArgs);
	isPressed = false;

}

void ofxBKUIComponent::mouseDraggedHandler(ofMouseEventArgs& eventArgs)
{
	if(!enabled || !visible || !mouseEnabled) return;

	ofVec2f newDelta = getMousePosition()-initMousePos;
	mouseDelta = newDelta - mouseAbsoluteDelta;
	mouseAbsoluteDelta = newDelta;

	if(isPressed) mouseDragged(eventArgs);
}


//Functions to override
void ofxBKUIComponent::mouseOver()
{
	//printf("mouse OVER\n");
}

void ofxBKUIComponent::mouseOut()
{
	//printf("mouse OUT\n");
}

void ofxBKUIComponent::mousePressed(ofMouseEventArgs& eventArgs)
{
	//printf("mouse pressed in uiComponent\n");

}

void ofxBKUIComponent::mouseReleased(ofMouseEventArgs& eventArgs)
{
	//printf("mouse released in uiComponent\n");

}

void ofxBKUIComponent::mouseReleasedOutside(ofMouseEventArgs& eventArgs)
{
	//printf("mouse released outside uiComponent\n");

}

void ofxBKUIComponent::mouseDragged(ofMouseEventArgs& eventArgs)
{
	//printf("mouse dragged in uiComponent\n");
}


void ofxBKUIComponent::parentResizedHandler(ofxBKUIEventArgs &e)
{
	//(printf("Parent resized ! %i\n",parent);
	updatePosition();
}

void ofxBKUIComponent::updatePosition()
{
	float tx = position.x;
	float ty = position.y;
	float tw = width;
	float th = height;

	float outerTarget = 0;

	if(parent != NULL)
	{
		float pw = parent->getInnerWidth();
		float ph = parent->getInnerHeight();

		if(fixedWidth)
		{
			float leftOffset = left*width;
			outerTarget = 0;

			outerTarget = rightIsRelative?right*pw:right;
			if(leftIsRelative) outerTarget = pw-outerTarget;

			tx = outerTarget - leftOffset;
		}else
		{
			tx = leftIsRelative?pw*left:left;
			float tx2 = rightIsRelative?right*pw:pw-right;
			tw = tx2-tx;
		}

		if(fixedHeight)
		{
			float topOffset = top*height;
			outerTarget = 0;

			outerTarget = bottomIsRelative?bottom*ph:bottom;
			if(topIsRelative) outerTarget = ph-outerTarget;

			ty = outerTarget - topOffset;
		}else
		{
			ty = topIsRelative?ph*top:top;
			float ty2 = bottomIsRelative?bottom*ph:ph-bottom;
			th = ty2-ty;
		}

		setPosition(tx,ty);
		setSize(tw,th);
	}

	setPosition(tx,ty);
	setSize(tw,th);

}

//Setter

void ofxBKUIComponent::setParent(ofxBKContainer * _parent)
{
	if(parent != NULL) ofRemoveListener(parent->resized,this,&ofxBKUIComponent::parentResizedHandler);
	parent = _parent;
	updatePosition();
	if(parent != NULL) ofAddListener(parent->resized,this,&ofxBKUIComponent::parentResizedHandler);
}


void ofxBKUIComponent::setOffset(float _offsetX, float _offsetY)
{
	offset.set(_offsetX,_offsetY);
}

void ofxBKUIComponent::setPosition(float _x, float _y)
{
	position.set(_x, _y);
}

void ofxBKUIComponent::setAbsolutePosition(float _x, float _y)
{
	position.set(_x, _y);
	top = 0;
	bottom = _y;
	left = 0;
	right = _x;

	fixedWidth = true;
	fixedHeight = true;

	topIsRelative = false;
	bottomIsRelative = false;
	leftIsRelative = false;
	rightIsRelative = false;

	updatePosition();
}


ofxBKUIComponent * ofxBKUIComponent::setAdvancedFixedWidth(float _width, float innerAnchor, float outerAnchor, bool outerIsRelative, bool outerIsFromRight)
{
	fixedWidth = true;
	width = _width;
	left = innerAnchor; //use left for innerAnchor and right for outer
	right = outerAnchor;
	rightIsRelative = outerIsRelative; //uses same variable to avoid too many variables in class
	leftIsRelative = outerIsFromRight;
	updatePosition();

	return this;
}


ofxBKUIComponent * ofxBKUIComponent::setAdvancedFixedHeight(float _height, float innerAnchor, float outerAnchor, bool outerIsRelative, bool outerIsFromBottom)
{
	fixedHeight = true;
	height = _height;
	top = innerAnchor; //use top for innerAnchor and bottom for outer
	bottom = outerAnchor;
	bottomIsRelative = outerIsRelative;//uses same variable to avoid too many variables in class
	topIsRelative = outerIsFromBottom;
	updatePosition();

	return this;
}

ofxBKUIComponent * ofxBKUIComponent::setFluidWidth(float _left, float _right, bool _leftIsRelative, bool _rightIsRelative)
{
	fixedWidth = false;
	left = _left;
	right = _right;
	leftIsRelative = _leftIsRelative;
	rightIsRelative = _rightIsRelative;
	updatePosition();

	return this;
}

ofxBKUIComponent * ofxBKUIComponent::setFluidHeight(float _top, float _bottom, bool _topIsRelative, bool _bottomIsRelative)
{
	fixedHeight = false;
	top = _top;
	bottom = _bottom;
	topIsRelative = _topIsRelative;
	bottomIsRelative = _bottomIsRelative;
	updatePosition();

	return this;
}


void ofxBKUIComponent::setSize(float _width, float _height, bool notify)
{
	_width = max(_width,minSize.x);
	_height = max(_height, minSize.y);
	if(maxSize.x > 0) _width = min(_width,maxSize.x);
	if(maxSize.y > 0) _width = min(_height,maxSize.y);

	if(_width == width && _height == height) return; //too hard ?

	width = _width;
	height = _height;

	if(notify)
	{
		ofxBKUIEventArgs args;
		args.eventType = BKEVENT_RESIZED;
		args.target = this;
		ofNotifyEvent(resized,args);
	}
}

ofxBKUIComponent * ofxBKUIComponent::setMinSize(float _minW, float _minH)
{
	minSize.set(_minW,_minH);
	setSize(width,height);

	return this;
}

ofxBKUIComponent * ofxBKUIComponent::setMaxSize(float _maxW, float _maxH)
{
	minSize.set(_maxW,_maxH);
	setSize(width,height);

	return this;
}

void ofxBKUIComponent::setVisible(bool value)
{
	visible = value;
}

void ofxBKUIComponent::setEnabled(bool value)
{
	enabled = value;
}


ofRectangle ofxBKUIComponent::getLocalBounds()
{
	ofVec2f finalPos = position;
	return ofRectangle(position,width,height);
}

ofRectangle ofxBKUIComponent::getGlobalBounds()
{
	ofVec2f finalPos = position+offset;
	return ofRectangle(finalPos,width,height);
}

bool ofxBKUIComponent::isMouseInside()
{
	//printf("Is Mouse Inside ? %f %f %f %f\n",ofGetMouseX(), ofGetMouseY(),position.x, position.y);

	return getGlobalBounds().inside(ofGetMouseX(),ofGetMouseY());
	//return (ofGetMouseX() - finalPos && ofGetMouseX() < position.x+offset.x + width && ofGetMouseY() > position.y && ofGetMouseY() < position.y+height);
}

ofVec2f ofxBKUIComponent::getMousePosition(bool relative)
{
	ofVec2f mp = ofVec2f(ofGetMouseX(),ofGetMouseY())-getGlobalBounds().position;
	if(relative) mp /= ofVec2f(width,height);
	return mp;
}



//MEGA TEMP BRING TO FRONT

void ofxBKUIComponent::bringToFront()
{
	ofRemoveListener(ofEvents().draw,this,&ofxBKUIComponent::drawHandler);
	ofAddListener(ofEvents().draw,this,&ofxBKUIComponent::drawHandler);
}
