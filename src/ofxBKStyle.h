#pragma once
#include "ofColor.h"
#include "ofTrueTypeFont.h"

class ofxBKStyle
{
public:
	static void init();
	static ofColor bgColor,normalColor, darkColor, lightColor,highlightColor,blue, green,red,orange,yellow;
	static ofTrueTypeFont normalFont,titleFont,smallFont,bigFont;
};
