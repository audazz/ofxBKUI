#pragma once
#include "ofColor.h"
#include "ofxBKTTFplus.h"

class ofxBKStyle
{
public:
	static void init();
	static ofColor bgColor;
	static ofColor normalColor;
	static ofColor darkColor;
	static ofColor semiLightColor;
	static ofColor lightColor;
	static ofColor highlightColor;
	static ofColor selectionColor;
	static ofColor blue;
	static ofColor green;
	static ofColor red;
	static ofColor orange;
	static ofColor yellow;

    static ofxBKTTFplus normalFont;
    static ofxBKTTFplus titleFont;
    static ofxBKTTFplus smallFont;
    static ofxBKTTFplus bigFont;
};
