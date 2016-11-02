#include "ofxBKUI.h"


ofxBKUIComponent::ofxBKUIComponent()
{
}

ofxBKUIComponent::~ofxBKUIComponent()
{
    //std::cout << "calling destructor of(cpn):" << this << "" << std::endl;
    ofRemoveListener(ofEvents().draw         ,this, &ofxBKUIComponent::drawHandler);
	ofRemoveListener(ofEvents().mousePressed ,this, &ofxBKUIComponent::mousePressedHandler);
	ofRemoveListener(ofEvents().mouseReleased,this, &ofxBKUIComponent::mouseReleasedHandler);
	ofRemoveListener(ofEvents().mouseDragged ,this, &ofxBKUIComponent::mouseDraggedHandler);
    setParent(nullptr);
}


void ofxBKUIComponent::init(float _x, float _y, float _width,float _height)
{
	//printf("UIComponent init %f %f\n",_width,_height);
	setVisible(false);
	parentSetVisible(true);
	setEnabled(true);
	parentSetEnabled(true);

	isOver = false;
	isPressed = false;
	mouseEnabled = true;

	autoResizeWithParents = true;
	isSizeUpdateSuccess = true;
	printResizeError = false;

	drawDebug = false;

	parent = nullptr;

	offset.set(0,0);

	minSize = ofVec2f::zero();
	maxSize = ofVec2f::zero();

	setAbsolutePosition(_x,_y);
	setSize(_width,_height);

	ofAddListener(ofEvents().mousePressed ,this, &ofxBKUIComponent::mousePressedHandler);
	ofAddListener(ofEvents().mouseReleased,this, &ofxBKUIComponent::mouseReleasedHandler);
	ofAddListener(ofEvents().mouseDragged ,this, &ofxBKUIComponent::mouseDraggedHandler);
}

void ofxBKUIComponent::draw()
{
	//printf("Draw !\n");
	//to be overridden
	if(ofxBKUI::freeze) return;

	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofPushStyle();
		ofSetColor(ofColor::red,100);
        ofDrawLine(-10,   0, 10,  0);
        ofDrawLine(  0, -10,  0, 10);
		ofSetColor(ofColor::purple,100);
		ofNoFill();
        ofDrawRectangle(0,0,width,height);

		ofPopStyle();
	}
}

void ofxBKUIComponent::drawHandler(ofEventArgs& eventArgs)
{
	if(!isVisible() || ofxBKUI::freeze) return;

	ofPushMatrix();
		ofTranslate(position);
		ofTranslate(offset);
		ofPushStyle();
            // this-> make sure the derived draw is called even in case of a
            // base container
			this->draw();

			// draw
			for (auto c : functionsCalledInDraw)  c->draw();

		ofPopStyle();
	ofPopMatrix();

	if(isEnabled() && mouseEnabled)
	{
		bool mouseInside = isMouseInside();
		if(mouseInside && !isOver) mouseOver();
		if(!mouseInside && isOver) mouseOut();

		isOver = mouseInside;
	}
}

void ofxBKUIComponent::addFunctionCalledInDraw(ofxBKUIEventHandlers * ptr)
{
    functionsCalledInDraw.push_back(ptr);
}

void ofxBKUIComponent::removeFunctionCalledInDraw(ofxBKUIEventHandlers * ptr)
{
    for (auto it=functionsCalledInDraw.begin(); it!=functionsCalledInDraw.end();) {
        if ((*it) == ptr)
				it = functionsCalledInDraw.erase(it);
        else
            ++it;
    }
}

void ofxBKUIComponent::clearFunctionCalledInDraw()
{
    functionsCalledInDraw.clear();
}



void ofxBKUIComponent::mousePressedHandler(ofMouseEventArgs& eventArgs)
{
	if(!isEnabled() || !isVisible()  || !mouseEnabled) return;

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
	if(!isEnabled() || !isVisible()  || !mouseEnabled) return;


	if(isMouseInside()) mouseReleased(eventArgs);
	else if(isPressed) mouseReleasedOutside(eventArgs);
	isPressed = false;

}

void ofxBKUIComponent::mouseDraggedHandler(ofMouseEventArgs& eventArgs)
{
	if(!isEnabled() || !isVisible()  || !mouseEnabled) return;

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
	parentResized();
}

void ofxBKUIComponent::parentResized()
{
    //isSizeUpdateSuccess = true;
	//printf("Parent resized ! %i\n",parent);
	if (autoResizeWithParents){
        updatePosition();
    }
}

void ofxBKUIComponent::updatePosition()
{
	float tx = position.x;
	float ty = position.y;
	float tw = width;
	float th = height;

	float outerTarget = 0;

	if(parent != nullptr)
	{
		float pw = parent->getInnerWidth();
		float ph = parent->getInnerHeight();

		if(fixedWidth)
		{
			float leftOffset = left * width;
			outerTarget = 0;

			outerTarget = rightIsRelative ? right*pw : right;
			if(leftIsRelative)
                outerTarget = pw - outerTarget;

			tx = outerTarget - leftOffset;
		} else {
			tx = leftIsRelative ? pw*left : left;
			float tx2 = rightIsRelative ? right*pw : pw-right;
			tw = tx2 - tx;
		}

		if(fixedHeight)
		{
			float topOffset = top*height;
			outerTarget = 0;

			outerTarget = bottomIsRelative ? bottom*ph : bottom;
			if(topIsRelative)
                outerTarget = ph - outerTarget;

			ty = outerTarget - topOffset;
		} else {
			ty = topIsRelative ? ph*top : top;
			float ty2 = bottomIsRelative ? bottom*ph : ph-bottom;
			th = ty2 - ty;
		}

		//setPosition(tx,ty);//TODO: see if it is a problem
		//setSize(tw,th);
	}

	setSize(tw,th);

	if (isSizeUpdateSuccess){
        setPosition(tx,ty);
        if (printResizeError)
            std::cout << this <<" allowing resize (component update position)" << std::endl;
	}

}

//Setter

void ofxBKUIComponent::setParent(ofxBKContainer * _parent)
{
    if (parent != nullptr) {
        //ofRemoveListener(parent->resized,this,&ofxBKUIComponent::parentResizedHandler);
        if(_parent != nullptr){
            parent->releaseChild(this);
        }
    }

	parent = _parent;
	if(parent != nullptr){
        updatePosition();
	}
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

/** \brief attach the component to the given anchors

    \param _height : height of the component (fixed)
    \param innerAnchor : anchor in the component
    \param outerAnchor : anchor in its parent to which the inner anchor is attached
**/
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
    if (_width  == -1) _width  = width;
    if (_height == -1) _height = height;

    isSizeUpdateSuccess = true; //initalize the error checking variable

    if (_width < minSize.x) {
        isSizeUpdateSuccess = false;
        _width = minSize.x;
        if (printResizeError) {
            printf("min x M: %f /m: %f\n",_width,minSize.x);
            printInfo();
        }
    }
    if (_height < minSize.y) {
        isSizeUpdateSuccess = false;
        _height = minSize.y;
        if (printResizeError) {
            printf("min y M: %f /m: %f\n",_height,minSize.y);
            printInfo();
        }
    }
    if ((maxSize.x > 0) && (_width > maxSize.x)) {
        isSizeUpdateSuccess = false;
        _width = maxSize.x;
        if (printResizeError) {
            printf("max x M: %f /m: %f\n",_width,maxSize.x);
            printInfo();
        }
    }
    if ((maxSize.y > 0) && (_height > maxSize.y)) {
        isSizeUpdateSuccess = false;
        _height = maxSize.y;
        if (printResizeError) {
            printf("max y M: %f /m: %f\n",_height,maxSize.y);
            printInfo();
        }
    }

    /*
    _width =  max(_width, minSize.x);
	_height = max(_height, minSize.y);
	if(maxSize.x > 0) _width = min(_width,maxSize.x);
	if(maxSize.y > 0) _width = min(_height,maxSize.y);
    */
	if(_width == width && _height == height)
        return; //too hard ?

	width  = _width;
	height = _height;
 /*
	if(notify) {
		ofxBKUIEventArgs args;
		args.eventType = BKEVENT_RESIZED;
		args.target = this;
		ofNotifyEvent(resized, args);
	}
*/
}

ofxBKUIComponent * ofxBKUIComponent::setMinSize(float _minW, float _minH)
{
	minSize.set(_minW,_minH);
	updatePosition();
	//setSize(width,height);

	return this;
}

ofxBKUIComponent * ofxBKUIComponent::setMaxSize(float _maxW, float _maxH)
{
	maxSize.set(_maxW,_maxH);
	setSize(width,height);

	return this;
}

void ofxBKUIComponent::setVisible(bool value)
{
	visible = value;
	updateDrawListener();
}

void ofxBKUIComponent::parentSetVisible(bool value)
{
	parentVisible = value;
	updateDrawListener();
}

void ofxBKUIComponent::updateDrawListener()
{
	if (isVisible() && isDrawable) {
        ofAddListener(   ofEvents().draw ,this, &ofxBKUIComponent::drawHandler);
	} else {
        ofRemoveListener(ofEvents().draw ,this, &ofxBKUIComponent::drawHandler);
	}
}


void ofxBKUIComponent::setEnabled(bool value)
{
	enabled = value;
}

void ofxBKUIComponent::parentSetEnabled(bool value)
{
	parentEnabled = value;
}

ofRectangle ofxBKUIComponent::getLocalBounds()
{
	//ofVec2f finalPos = position;
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
    if (isVisible()) {
        ofRemoveListener(ofEvents().draw, this, &ofxBKUIComponent::drawHandler);
        ofAddListener(   ofEvents().draw, this, &ofxBKUIComponent::drawHandler);
    }
}


void ofxBKUIComponent::printInfo()
{
    /*
    std::cout << "ofxBKUIComponent:" << this << "" << std::endl;
    printf(      "    visible[%d]\n",visible);
    printf(      "    enabled[%d]\n",enabled);
    printf(      "    parentEnabled[%d]\n",parentEnabled);
    printf(      "    parentVisible[%d]\n",parentVisible);
    printf(      "     size[%f,%f]\n",width,height);
    printf(      "  minsize[%f,%f]\n",minSize.x, minSize.y);
    printf(      "  maxsize[%f,%f]\n",maxSize.x, maxSize.y);
    printf(      "  fixed[w:%d,h:%d]\n",fixedHeight, fixedWidth);
    printf(      "  widthRelative[left:%d,right:%d]\n",leftIsRelative, rightIsRelative);
    printf(      "  widthAnchor[left:%f,right:%f]\n",left, right);
    printf(      "  heightRelative[top:%d,bot:%d]\n",topIsRelative, bottomIsRelative);
    printf(      "  heightAnchor[top:%f,bot:%f]\n",top, bottom);
    if (!functionsCalledInDraw.empty()) {
        std::cout << "  also drawing: [";
        for(auto f : functionsCalledInDraw)
            std::cout << f << " ; ";
        printf("\b\b\b]\n");
    }
    */
}





