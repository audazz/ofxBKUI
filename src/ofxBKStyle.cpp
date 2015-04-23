#include "ofxBKStyle.h"

void ofxBKStyle::init()
{
	printf("Init Style\n");
	normalFont.loadFont("Yanone.ttf", 12);
	titleFont.loadFont("Yanone.ttf", 16);
	smallFont.loadFont("Yanone.ttf", 10);
	bigFont.loadFont("Yanone.ttf", 32);
	 
}	 


ofTrueTypeFont ofxBKStyle::normalFont;
ofTrueTypeFont ofxBKStyle::titleFont;
ofTrueTypeFont ofxBKStyle::smallFont;
ofTrueTypeFont ofxBKStyle::bigFont;


ofColor ofxBKStyle::bgColor(30,30,30);
ofColor ofxBKStyle::normalColor(50,50,50);
ofColor ofxBKStyle::darkColor(40,40,40);
ofColor ofxBKStyle::semiLightColor(120,120,120);
ofColor ofxBKStyle::lightColor(233, 233, 233);
ofColor ofxBKStyle::highlightColor(252, 194, 3);
ofColor ofxBKStyle::blue (63, 160, 239);
ofColor ofxBKStyle::green(170, 226, 74);
ofColor ofxBKStyle::red(226, 80, 74);
ofColor ofxBKStyle::orange(242, 141, 51);
ofColor ofxBKStyle::yellow(255, 240, 29);