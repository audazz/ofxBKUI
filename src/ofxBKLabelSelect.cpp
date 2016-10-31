#include "ofxBKLabelSelect.h"

#include "ofxBKLabelSelect.h"
#include "ofxBKUI.h"
#include "PrettyPrint.h"

ofxBKLabelSelect::ofxBKLabelSelect()
{

}

ofxBKLabelSelect::ofxBKLabelSelect(std::string _text, float _x, float _y, float _width, float _height)
{
    init(_text, _x, _y, _width, _height);

}

void ofxBKLabelSelect::init(std::string _text, float _x, float _y, float _width,float _height)
{
    ofxBKLabel::init(_text,_x,_y,_width,_height);

    selectedChars.resize(_text.length(), false);

            std::cout << "return" << std::endl;
            std::cout << "text.length():       "<< text.length() << std::endl;
            std::cout << "_selectedChar.size():"<< selectedChars.size() << std::endl;

    calculateCharacterCumWidth();
    selectionColor = ofxBKStyle::selectionColor;
}

void ofxBKLabelSelect::setText(std::string _text)
{
    ofxBKLabel::setText(_text);
    selectedChars.resize(text.length(),false);
    calculateCharacterCumWidth();
}

void ofxBKLabelSelect::setSelectedChar(std::vector<bool> _selectedChars)
{
    if (_selectedChars.size() != text.length()){
            std::cout << "return" << std::endl;
            std::cout << "text.length():       "<< text.length() << std::endl;
            std::cout << "_selectedChar.size():"<< _selectedChars.size() << std::endl;
        return;
    }
    selectedChars = _selectedChars;
}

std::vector<bool> ofxBKLabelSelect::getSelectedChar()
{
    return selectedChars;
}

void ofxBKLabelSelect::draw()
{
	ofxBKUIComponent::draw();

    debugDraw();

    selectedAreaFromSelectedChar(getTextStartPosition()); //TODO: should be in "update"
    ofPushStyle();
        ofSetColor(selectionColor);
        ofFill();
        for (auto rec : selectedArea) ofRect(rec);//TODO:check boundary
    ofPopStyle();

    textDraw();
}

void ofxBKLabelSelect::selectedAreaFromSelectedChar(ofVec2f startPosition)
{
    startPosition.y -= stringHeight;
    float k = 2;// num pixel
    startPosition.y -= k;
    selectedArea.resize(0);
    selectedChars.resize(text.length(),false); // ensure selectedChar has the right size
    bool in(false);
    int inStart(0);
    for (unsigned i=0;i<selectedChars.size();++i) {
        if (selectedChars[i] && !in){
            in = true;
            inStart = i;
        }
        if (in){
            if ((i==selectedChars.size()-1) || (selectedChars[i+1]==false) ) {
                ofVec2f selectPos(startPosition.x + characterCumWidth[inStart],
                                  startPosition.y);
                selectedArea.push_back(
                       ofRectangle(selectPos,
                                   characterCumWidth[i+1]-characterCumWidth[inStart],
                                   stringHeight + k*2) );
                in = false;
            }
        }
    }
}

void ofxBKLabelSelect::calculateCharacterCumWidth() //TODO: needs to be run each time text is changed
{
    float d;
    int itmp;
    bool err(false);
    characterCumWidth.resize(text.length()+1);
    characterCumWidth[0] = 0;
    for (int i=1 ; i< (int)characterCumWidth.size() ; ++i) {
        characterCumWidth[i] = font->stringWidth(text.substr(0,i));

        // workaround for the fact that spaces at the end of strings are not considered by stringwidth
        if (!err && (characterCumWidth[i] == characterCumWidth[i-1])){
            d = characterCumWidth[i-1];
            itmp = i-1;
            err = true;
        }else if (err){
            err = false;
            float dtotal(characterCumWidth[i]- d);
            dtotal /= i-itmp;
            for (int j=itmp+1 ; j<i ; ++j){
                characterCumWidth[j] = characterCumWidth[j-1]+dtotal;
            }
        }
    }
    stringHeight = font->stringHeight(text);
}

ofVec2f  ofxBKLabelSelect::getMouseInString()
{
    return getMousePosition()-getTextStartPosition();
}

int ofxBKLabelSelect::getInterCharFromMouse()
{
    // retuns the index of the element for which next element is greater than
    // mouse x position within the ofxBKLabelSelect box
    ofVec2f mousePos(getMouseInString());
    if (mousePos.x<characterCumWidth[0])
        return 0;

    auto it = std::find_if(characterCumWidth.begin(),characterCumWidth.end(),
                         [=](float &w){return w>=mousePos.x;} );
    return (std::distance(characterCumWidth.begin(), it)-1);
}


void ofxBKLabelSelect::printInfo()
{
    ofxBKLabel::printInfo();
    std::cout << "   ofxBKLabelSelect:"  << "" << std::endl;

}


