#ifndef OFXBKUITEXTBOX_H
#define OFXBKUITEXTBOX_H
#include "ofxBKLabel.h"

class ofxBKUITextBox : public ofxBKLabel
{
    public:
        ofxBKUITextBox(std::string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);

        void setStartPositionX(float _startPosition = 0);
        void setStartPositionY(float _startPosition = 0);

    protected:
        virtual void init(std::string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);
        virtual void textDraw();
        virtual void debugDraw();

        void updateStartPosition();
        virtual void updateTextOffset();

        ofVec2f startPosition = {0,0};
        ofVec2f startPositionRelative = {0,0};

    private:
};

#endif // OFXBKUITEXTBOX_H
