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

	title = _title;
	showTitle = strlen(_title.c_str()) > 0;
	headerHeight = showTitle?20:0;
	paddingTop = headerHeight;

	draggable = false;
	miniModeEnabled = false;
	isInMiniMode = false;

	numChildren = 0;
}

void ofxBKPanel::draw()
{
	ofxBKContainer::draw();

	ofSetColor(200);
	if(showTitle) ofDrawBitmapString(title,10,15);
}

void ofxBKPanel::setPadding(float top, float bottom, float left, float right)
{
	ofxBKContainer::setPadding(top + headerHeight,bottom,left,right);
}