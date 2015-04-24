#include "ofxBKUI.h"

void ofxBKUI::init()
{
	ofxBKStyle::init();
	drawDebug = false;
	freeze = false;
}

bool ofxBKUI::drawDebug;
bool ofxBKUI::freeze;