#include "ofxBKTTFplus.h"

void ofxBKTTFplus::drawStringInRect(const string &s, float x, float y, ofRectangle rect) const
{
    glEnable(GL_SCISSOR_TEST );
    glScissor( static_cast<int>(rect.x),     static_cast<int>(rect.y),
               static_cast<int>(rect.width), static_cast<int>(rect.height));
    this->drawString(s,x,y);
    glDisable(GL_SCISSOR_TEST );
}

void ofxBKTTFplus::drawStringAsShapesInRect(const string &s, float x, float y, ofRectangle rect) const
{
    glEnable(GL_SCISSOR_TEST );
    glScissor( static_cast<int>(rect.x),     static_cast<int>(rect.y),
               static_cast<int>(rect.width), static_cast<int>(rect.height));
    this->drawStringAsShapes(s,x,y);
    glDisable(GL_SCISSOR_TEST );
}
