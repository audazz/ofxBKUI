#ifndef OFXBKUITTFPLUS_H
#define OFXBKUITTFPLUS_H

#include "ofTrueTypeFont.h"
#include "ofRectangle.h"

class ofxBKTTFplus : public ofTrueTypeFont
{
public:

    void drawStringInRect(const std::string& s, float x, float y, ofRectangle rect) const;

    void drawStringAsShapesInRect(const std::string& s, float x, float y, ofRectangle rect) const;
};

#endif // OFXBKUITTFPLUS_H

