#include "ofxBKPanel.h"
ofxBKPanel::ofxBKPanel()
{

}

ofxBKPanel::ofxBKPanel(string _title, float _x, float _y, float _width, float _height )
{
	init(_title,_x,_y,_width,_height);
}

void ofxBKPanel::init(string _title,  float _x, float _y, float _width, float _height )
{
	ofxBKContainer::init(_x,_y,_width,_height);

	numChildren = 0;

	title = new ofxBKLabel(_title,5,5);
	//printf("After assignation position %s :%f\n",title->text.c_str(),title->position.y);
	//title->setPosition(50,50);
	addChild(title);

	setTitle(_title); //force showTitle evaluation
	//setPadding(headerHeight,paddingBottom,paddingLeft,paddingRight);

	draggable = false;
	miniModeEnabled = false;
	isInMiniMode = false;
	transparentBG = false;
	bgColor = ofColor(25);
	//title->setPosition(0,-paddingTop);
}

void ofxBKPanel::draw()
{
	ofxBKContainer::draw();
}

string ofxBKPanel::getTitle()
{
    if (title != nullptr)
        return title->text;
    else
        return "";
}

void ofxBKPanel::setTitle(string _title)
{
	showTitle = strlen(_title.c_str()) > 0;
	headerHeight = showTitle ? 20 : 0;
	title->setText(_title);
}

void ofxBKPanel::setPadding(float top, float bottom, float left, float right)
{
	ofxBKContainer::setPadding(top + headerHeight,bottom,left,right);
	title->setAbsolutePosition(5,-headerHeight+5);

}

void ofxBKPanel::printInfo()
{
    ofxBKContainer::printInfo();
    std::cout << "   ofxBKPanel:" << getTitle() << "" << std::endl;
}
