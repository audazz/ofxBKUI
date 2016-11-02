// #ifndef OFXBKTEXTEDIT_H
// #define OFXBKTEXTEDIT_H
// #include "ofxBKContainer.h"
// #include "ofxBKLabelSelect.h"
//
// class ofxBKTextEdit : public ofxBKContainer
// {// would be better to use https://github.com/Flightphase/ofxTextInputField/
//     public:
//         ofxBKTextEdit();
//         ofxBKTextEdit(std::string _text, float _x, float _y, float _width,float _height);
//
//         virtual void setIsEditable(bool _isEditable) {isEditable = _isEditable;};
//
//         virtual void setText(std::string _text);
//
//
//         virtual void mousePressed(ofMouseEventArgs &e);
//         virtual void mouseDragged(ofMouseEventArgs &e);
//         virtual void mouseReleased(ofMouseEventArgs &e);
//         virtual void mouseReleasedOutside(ofMouseEventArgs &e);
//
//
//         ofxBKLabelSelect * labelTF;
//
//         virtual void printInfo();
//
//     protected:
//         enum SELECT_TYPE {ADD, PREVIOUS_START, REMOVE, REPLACE};
//         virtual void init(std::string _text, float _x, float _y, float _width,float _height);
//         virtual void draw();
//
//         bool isEditable = false;
//
//         std::vector<bool> selectedCharsSave = {};
//         std::vector<bool> selectedChars = {};
//
//         SELECT_TYPE selectType;
//         int mouseStartSelect;
//         int mouseEndSelect;
//         int mousePreviousStartSelect;
//         bool isDragging = false;
//         void calculateSelectedChars();
//
//     private:
// };
//
// #endif // OFXBKTEXTEDIT_H
