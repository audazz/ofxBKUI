#include "ofxBKTextEdit.h"
#include "ofxBKUI.h"
#include "PrettyPrint.h"
#include <GL/glut.h>

ofxBKTextEdit::ofxBKTextEdit()
{
    //ctor
}

ofxBKTextEdit::ofxBKTextEdit(std::string _text, float _x, float _y, float _width,float _height)
{
    init(_text, _x, _y, _width, _height);

}

void ofxBKTextEdit::init(std::string _text, float _x, float _y, float _width,float _height)
{

	ofxBKContainer::init(_x, _y, _width, _height);

    labelTF = new ofxBKLabelSelect(_text);
	addChild(labelTF);
    labelTF->setText(_text);

	labelTF->setAlign(ofxBKAlign::CENTER, ofxBKAlign::MIDDLE);
	labelTF->setFluidWidth()->setFluidHeight(); // fill the container
	//labelTF->setColor(ofColor::white);

    selectedChars = labelTF->getSelectedChar();
}

void ofxBKTextEdit::setText(std::string _text)
{
    labelTF->setText(_text);
    selectedChars = labelTF->getSelectedChar();
}

void ofxBKTextEdit::draw()
{
	ofxBKContainer::draw();
}

void ofxBKTextEdit::mousePressed(ofMouseEventArgs &e)
{
	ofxBKContainer::mousePressed(e);

    selectType = REPLACE; // default behaviour

    if (ofGetKeyPressed(OF_KEY_LEFT_CONTROL)){
        selectType = ADD;
    }

    isDragging = true;
	selectedChars = labelTF->getSelectedChar();
	selectedCharsSave = selectedChars;
    mouseStartSelect  = labelTF->getInterCharFromMouse();
}

void ofxBKTextEdit::mouseDragged(ofMouseEventArgs &e)
{
	ofxBKContainer::mouseDragged(e);

	mouseEndSelect = labelTF->getInterCharFromMouse();
	calculateSelectedChars();
	labelTF->setSelectedChar(selectedChars);

}

void ofxBKTextEdit::mouseReleased(ofMouseEventArgs &e)
{
	ofxBKContainer::mouseReleased(e);

	mouseEndSelect = labelTF->getInterCharFromMouse();
	calculateSelectedChars();
	labelTF->setSelectedChar(selectedChars);

	isDragging = false;
}

void ofxBKTextEdit::mouseReleasedOutside(ofMouseEventArgs &e)
{
	isDragging = false;
}

void ofxBKTextEdit::calculateSelectedChars()
{
    int s(labelTF->getSelectedChar().size());
    int selMin, selMax, startTmp;

    if (selectType == PREVIOUS_START)
        startTmp = mousePreviousStartSelect;
    else
        startTmp = mouseStartSelect;

    if (mouseStartSelect<=mouseEndSelect) {
            selMin = startTmp;
            selMax = mouseEndSelect;
    }else{
            selMin = mouseEndSelect;
            selMax = startTmp;
    }
    selMin = std::max(selMin,0);
    selMax = std::min(selMax,s); // interchar size = s+1
    switch (selectType){
    case REPLACE: case PREVIOUS_START:
        selectedChars.assign(s,false);
        for (auto i=selMin;i<selMax;++i){
            selectedChars[i] = true;
        }

        break;
    case REMOVE:
        selectedChars = selectedCharsSave;
        selectedChars.resize(s, false);
        for (auto i=selMin;i<selMax;++i){
            selectedChars[i] = false;
        }


        break;
    case ADD:
        selectedChars = selectedCharsSave;
        selectedChars.resize(s, false);
        for (auto i=selMin;i<selMax;++i){
            selectedChars[i] = true;
        }
        break;
    }
}

void ofxBKTextEdit::printInfo()
{
    ofxBKContainer::printInfo();
    std::cout << "   ofxBKTextEdit:" << labelTF->text << "" << std::endl;
}






// test







