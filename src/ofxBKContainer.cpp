#include "ofxBKUI.h"

ofxBKContainer::~ofxBKContainer()
{
    //std::cout << "Calling destructor of(ctn):" << this << std::endl;
}

ofxBKContainer::ofxBKContainer()
{

}

ofxBKContainer::ofxBKContainer(const ofxBKContainer& obj) :
    ofxBKUIComponent(obj),
    paddingTop(           obj.paddingTop ),
    paddingBottom(        obj.paddingBottom ),
    paddingLeft(          obj.paddingLeft ),
    paddingRight(         obj.paddingRight ),
    bgColor(              obj.bgColor ),
    transparentBG(        obj.transparentBG ),
    autoResizeChildren(   obj.autoResizeChildren),
    accountForChildMinSize( obj.accountForChildMinSize)
{//copy//

}

ofxBKContainer& ofxBKContainer::operator=(const ofxBKContainer& obj)
{// assignemnt operator

    ofxBKUIComponent::operator=(obj);
    paddingTop           = obj.paddingTop;
    paddingBottom        = obj.paddingBottom;
    paddingLeft          = obj.paddingLeft;
    paddingRight         = obj.paddingRight;
    bgColor              = obj.bgColor;
    transparentBG        = obj.transparentBG;
    autoResizeChildren   = obj.autoResizeChildren;
    accountForChildMinSize = obj.accountForChildMinSize;

    return *this;
}


ofxBKContainer::ofxBKContainer(float _x, float _y, float _width, float _height )
{
	init(_x,_y,_width,_height);
}

void ofxBKContainer::init(float _x, float _y, float _width, float _height )
{
	numChildren = 0;
	ofxBKUIComponent::init(_x,_y,_width,_height);
	paddingTop = 0;
	paddingBottom = 0;
	paddingLeft = 0;
	paddingRight = 0;
	//bgColor = ofColor(150,100);
	bgColor = ofxBKStyle::bgColor-10;
	transparentBG = false;
	autoResizeChildren = true;
	accountForChildMinSize = true;
}


void ofxBKContainer::draw()
{
	ofxBKUIComponent::draw();

    ofPushStyle();
	if(!transparentBG)
	{
		ofSetColor(bgColor);
        ofDrawRectangle(0,0,width,height);
	}

	if(drawDebug || ofxBKUI::drawDebug)
	{
		ofSetColor(ofColor::orange,100);
		ofNoFill();
        ofDrawRectangle(paddingLeft,paddingTop,getInnerWidth(),getInnerHeight());
	}

	ofPopStyle();

}


void ofxBKContainer::addChild(ofxBKUIComponent * element)
{

    if (! children.empty()) {
        auto it = std::find_if(children.begin(), children.end(),
                        [&] (ofxBKUIComponentPtr& p) {return (p.get() == element);} );
        if (it != children.end()) {
            std::cout << "Element already added to childs" << std::endl;
            return;
        }
    }
	children.push_back(ofxBKUIComponentPtr(element));
	childrenUpdateOrder.push_back(children.size()-1);
	numChildren = children.size();
	element->setVisible(true); //TODO: give a better default
	element->setParent(this);

	updateChildOffset(element);
}

void ofxBKContainer::removeChild(ofxBKUIComponent * element)
{
    auto it = std::find_if(children.begin(), children.end(),
                    [&] (ofxBKUIComponentPtr& p) {return (p.get() == element);} );
    if(it != children.end()){
        element->setVisible(false);
        children.erase(it);
        childrenUpdateOrder.erase(it-children.begin() +childrenUpdateOrder.begin());
    }
	numChildren = children.size();
}

void ofxBKContainer::releaseChild(ofxBKUIComponent * element)
{
    auto it = std::find_if(children.begin(), children.end(),
                    [&] (ofxBKUIComponentPtr& p) {return (p.get() == element);} );
    if(it != children.end()){
        element->setVisible(false);
        (*it).release();
        children.erase(it);
        childrenUpdateOrder.erase(it-children.begin() +childrenUpdateOrder.begin());
    }
	numChildren = children.size();
}

void ofxBKContainer::setVisible(bool value)
{
	ofxBKUIComponent::setVisible(value);
	if (parent != nullptr)
        value = value && parent->isVisible();
	for (auto & it : children)
        it->parentSetVisible(value);
}

/** \brief disable container and all its child but save their original state
 * \return
 *
 */
void ofxBKContainer::parentSetVisible(bool _visible)
{
    ofxBKUIComponent::parentSetVisible(_visible);
    _visible = _visible && visible;
    for (auto & it : children)
        it->parentSetVisible(_visible);
}

void ofxBKContainer::setEnabled(bool value)
{
	ofxBKUIComponent::setEnabled(value);
	if (parent != nullptr)
        value = value && parent->isEnabled();
	for (auto & it : children)
        it->parentSetEnabled(value);
}

/** \brief disable container and all its child but save their original state
 * \return
 *
 */
void ofxBKContainer::parentSetEnabled(bool _enable)
{
    ofxBKUIComponent::parentSetEnabled(_enable);
    _enable = _enable && enabled;
    for (auto & it : children)
        it->parentSetEnabled(_enable);
}


float ofxBKContainer::getInnerWidth()
{
	return width-paddingLeft-paddingRight;
}

float ofxBKContainer::getInnerHeight()
{
	return height-paddingTop-paddingBottom;
}

void ofxBKContainer::setOffset(float _offsetX, float _offsetY)
{
	//if(offset.x == _offsetX && offset.y == _offsetY) return;
	ofxBKUIComponent::setOffset(_offsetX, _offsetY);
	updateChildrenOffsets();
}

void ofxBKContainer::setPosition(float _x, float _y)
{
	//if(position.x == _x && position.y == _x) return;
	ofxBKUIComponent::setPosition(_x, _y);
    updateChildrenOffsets();
}

void ofxBKContainer::setSize(float _width, float _height, bool notify)
{
    float oldWidth  = width;
    float oldHeight = height;
    /*
    if (numChildren == 1){
        float inWidth( getInnerWidth() );
        float inHeight(getInnerHeight());
        _width  = std::max(inWidth , children[0]->minSize.x) + (width  - inWidth);
        _height = std::max(inHeight, children[0]->minSize.y) + (height - inHeight);
    }
    */

    if (printResizeError){
        std::cout << std::endl<< this << " updating size" << std::endl;
        this->printInfo();
    }
	ofxBKUIComponent::setSize(_width, _height, notify);

    if (isSizeUpdateSuccess) {
        if (autoResizeChildren){
            if (printResizeError)
                std::cout << std::endl<< this << " updating children size" << std::endl;
            this->updateChildrenSize();
        }
    }

    if ((!isSizeUpdateSuccess)){

        if (printResizeError){
            std::cout << std::endl<< this << "Undoing resize ";
            printf("[%f,%f]->[%f,%f]\n",_width,_height,oldWidth,oldHeight);
        }

        // force refresh
        bool save(ofxBKUI::childMinSize2Parents);
        ofxBKUI::childMinSize2Parents = false;
        ofxBKUIComponent::setSize(oldWidth, oldHeight, notify);
        ofxBKUI::childMinSize2Parents = save;

        isSizeUpdateSuccess = false; // let isSizeUpdateSuccess being false
        /*
        if (autoResizeChildren){
            this->updateChildrenSize();
        }
        */
    } else {
        if (printResizeError)
            std::cout << std::endl<< this << " children resize success" << std::endl;

    }

}


void ofxBKContainer::updateChildrenSize()
{
        for (auto index : childrenUpdateOrder) {
            children[index].get()->parentResized();
            if (printResizeError) {
                std::cout << "Resizing child " <<index << std::endl;
                children[index].get()->printInfo();
            }

            if ((ofxBKUI::childMinSize2Parents && accountForChildMinSize)) {
                if ( !(children[index].get()->isSizeUpdateSuccess) ) {
                    isSizeUpdateSuccess = false;
                    std::iter_swap(childrenUpdateOrder.begin() + index,
                                   childrenUpdateOrder.end()   - 1  );
                    break;
                }
            }
        }
}

void ofxBKContainer::setAccountForChildMinSize(bool _value)
{
    accountForChildMinSize = _value;
    for (auto & child : children){
        child->setAccountForChildMinSize(accountForChildMinSize);
    }
    updateChildrenSize();
}


void ofxBKContainer::setPadding(float top, float bottom, float left, float right)
{
	paddingTop    = top;
	paddingBottom = bottom;
	paddingLeft   = left;
	paddingRight  = right;
	updateChildrenOffsets();
}

void ofxBKContainer::updateChildrenOffsets()
{
	for(auto & it : children)
		updateChildOffset(it.get());
}

void ofxBKContainer::updateChildOffset(ofxBKUIComponent * child)
{
	child->setOffset(offset+position+ofVec2f(paddingLeft,paddingTop));
}

void ofxBKContainer::setResizeChildren(bool _resizeChildren)
{
    autoResizeChildren = _resizeChildren;
    if (autoResizeChildren)
        setSize(width, height, true);

}



void ofxBKContainer::printInfo()
{
    ofxBKUIComponent::printInfo();
    std::cout << "   ofxBKContainer";
    std::cout << "   accountForChildMinSize:" << accountForChildMinSize << "" << std::endl;
    std::cout << "   autoResizeChildren:"     << autoResizeChildren << "" << std::endl;
    std::cout << "   children:[";
    for (auto & c : children) std::cout << c.get()<<" ; ";
    std::cout<< "\b\b\b]" << std::endl;
}










