#include "ofxBKUI.h"

void ofxBKUI::init()
{
	ofxBKStyle::init();
	drawDebug = false;
	freeze = false;
	childMinSize2Parents = false;
}

bool ofxBKUI::drawDebug;
bool ofxBKUI::freeze;
bool ofxBKUI::childMinSize2Parents;
