#ifndef OFXBKLABELSELECT_H
#define OFXBKLABELSELECT_H
#include "ofxBKLabel.h"

class ofxBKLabelSelect  : public ofxBKLabel
{
    public:
        ofxBKLabelSelect();
        ofxBKLabelSelect(std::string _text, float _x = 0, float _y = 0, float _width = 0,float _height = 0);

        virtual void setText(std::string _text);
        void setSelectedChar(std::vector<bool> _selectedChar);
        std::vector<bool> getSelectedChar();
        int getInterCharFromMouse();

        ofColor selectionColor;
        virtual void printInfo();
    protected:
        virtual void init(std::string _text, float _x, float _y, float _width, float _height);
        virtual void draw();

        bool isEditable = false;

        std::vector<bool> selectedChars       = { };
        std::vector<ofRectangle> selectedArea = { };
        std::vector<float> characterCumWidth  = {0};

        float stringHeight;

        void selectedAreaFromSelectedChar(ofVec2f startPosition);
        void calculateCharacterCumWidth();
        ofVec2f getMouseInString();

    private:
};

#endif // OFXBKLABELSELECT_H
