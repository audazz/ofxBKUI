#include "ofxBKButtonBar.h"
#include "ofxBKUIEventArgs.h"
#include "PrettyPrint.h"

ofxBKButtonBar::ofxBKButtonBar(float _x, float _y, float _width,float _height)
{
	init(_x, _y, _width,_height);
}

void ofxBKButtonBar::init(float _x, float _y, float _width,float _height)
{
	ofxBKContainer::init(_x, _y,_width,_height);
	numButtons = 0;
	lastSelect = -1;
	selectedLabel = "";
	transparentBG = true;
	printResizeError = false;
	setAllowMultipleSelection(false);
	autoResizeChildren = true;
	buttonToMinSize = false;
	//autoWidthOnce = _width == 0;
}

void ofxBKButtonBar::draw()
{
	ofxBKContainer::draw();
	//printf("Draw %i\n",positionMode);
}

void ofxBKButtonBar::updateButtonsPositions()
{
    float gap = 10;
	float btWidth = (width-gap*(numButtons-1))/numButtons;
    float absPos(startPosition);
    bool tooLarge=false;
    unsigned i(0);
	for (auto & bt : buttons) {
        float tmpWidth;
        if (buttonToMinSize) {
            tmpWidth = bt->label->textBoundingBox.width + 15;
        } else {
            tmpWidth = btWidth;
        }

        if(printResizeError){
            bt->printInfo();
        }
        bt->setSize(tmpWidth,height);
        if (!(bt->isSizeUpdateSuccess)) {
            this->isSizeUpdateSuccess = false;
            if(printResizeError){
                bt->label->printInfo();
            }
        }
        bt->setAbsolutePosition(absPos, 0);
        bool vsble(true);
        if (absPos<0)  vsble = false;
        absPos +=  tmpWidth;
        if ((absPos > width+1) || tooLarge) {
            tooLarge = true;
            vsble = false;
        }
            //isSizeUpdateSuccess = false;// TODO:proper condition to deal with this case in interface | ok ??
        if (vsble) bt->setVisible(visible);
        else       bt->setVisible(false);

        absPos +=  gap;
        ++i;
	}

}

void ofxBKButtonBar::updateChildrenSize()
{
    updateStartPosition();
    updateButtonsPositions();
}

/*
void ofxBKButtonBar::setSize(float _width, float _height, bool notify)
{
	ofxBKContainer::setSize(_width,_height,notify);
}
*/

void ofxBKButtonBar::setButtonToMinSize(bool _autosize)
{
    buttonToMinSize = _autosize;
}

void ofxBKButtonBar::setStartPosition(float _startPosition)
{
    _startPosition = (_startPosition>1) ? 1 : _startPosition;
    _startPosition = (_startPosition<0) ? 0 : _startPosition;
    startPositionRelative = _startPosition;
    updateButtonsPositions();
    updateStartPosition();
    updateButtonsPositions();

}

void ofxBKButtonBar::updateStartPosition()
{
    startPosition = ofMap(startPositionRelative,0,1,0,width-getTotalWidth());
}

float ofxBKButtonBar::getTotalWidth()
{
    if (buttons.size()==0)
        return width;

    return buttons.back()->position.x +
           buttons.back()->width      -
           buttons.front()->position.x;
}

void ofxBKButtonBar::addOption(string const label)
{
	buttons.push_back(new ofxBKButton(label));
	buttonLabel.push_back(label);
	addChild(buttons.back());
	buttons.back()->isToggle = true;
	buttons.back()->autoResizeWithParents = false;
	//buttons.back()->setAlign(ofxBKAlign::CENTER, ofxBKAlign::MIDDLE);
	ofAddListener(buttons.back()->buttonSelected,   this, &ofxBKButtonBar::btSelected);
	ofAddListener(buttons.back()->buttonDeselected, this, &ofxBKButtonBar::btDeselected);

	numButtons = buttons.size();
	updateButtonsPositions();
    updateStartPosition();
	updateButtonsPositions();
}

void ofxBKButtonBar::removeOption(string const label)
{
    // remove button
    for (auto it=buttons.begin();it!=buttons.end();){
        if ((*it)->getLabel() == label){
            it = removeOption(it-buttons.begin());
        }else{
            ++it;
        }
    }
    // remove string
    numButtons = buttons.size();

	updateButtonsPositions();
    updateStartPosition();
	updateButtonsPositions();
}

std::vector<ofxBKButton *>::iterator ofxBKButtonBar::removeOption(int const index)
{
    if (index>= buttons.size())
        return (buttons.end());
    if (index < 0)
        return (buttons.begin());

    ofRemoveListener(buttons[index]->buttonSelected,   this, &ofxBKButtonBar::btSelected);
    ofRemoveListener(buttons[index]->buttonDeselected, this, &ofxBKButtonBar::btDeselected);

    // delete the button
    removeChild(buttons[index]);

    // delete the additional pointer
    auto it = buttons.erase(buttons.begin() + index);

    // delete the label
    buttonLabel.erase(buttonLabel.begin() + index);

    return it;
}

void ofxBKButtonBar::clearOptions()
{
    for (auto it=buttons.begin();it!=buttons.end();)
        it = removeOption(it-buttons.begin());
}

void ofxBKButtonBar::setOptions( vector<string> const _options)
{
    clearOptions();

	for(auto & opt : _options)
        addOption(opt);

}

void ofxBKButtonBar::setSelectedIndex(int buttonIndex, bool notify)
{
    if((buttonIndex >= 0) && (buttonIndex < numButtons)){
        auto idxMax = (maxNumbSelected == -1) ? numButtons : maxNumbSelected;
        if ((int)(selectedIndices.size()) < idxMax){
            addButonToSelected(buttonIndex);
        } else {
            if (!selectedIndices.empty()) {
                while ((int)(selectedIndices.size())+1 > idxMax){
                    rmButonFromSelected(selectedIndices[0]);
                }
                addButonToSelected(buttonIndex);
            }
        }


        if(notify)
        {
            ofxBKUIEventArgs args;
            args.eventType = ofxBKUIEventArgs::SELECTION_CHANGED;
            args.target = this;
            ofNotifyEvent(selectionChanged,args);
        }
    }
}


void ofxBKButtonBar::setSelectedIndex(const std::vector<int> buttonIndex, bool notify)
{
    for (auto i=0 ; i<numButtons ; ++i)
        setDeselectedIndex(i, notify);

    for (auto i : buttonIndex)
        setSelectedIndex(i,notify);
}

void ofxBKButtonBar::setDeselectedIndex(int buttonIndex, bool notify)
{
    int selectIndex(getSelectedIndexForButtonIndex(buttonIndex));

    if((buttonIndex >= 0) &&
       (buttonIndex < numButtons) &&
       (selectIndex > -1 ) ) {

        if(!allowMultipleSelection) {
            if (!selectedIndices.empty()) {
                if(selectedIndices.back() == buttonIndex)
                    buttons[buttonIndex]->setSelected(true,false); //can't have no option selected, keep this one selected
                else {
                    rmButonFromSelected(selectedIndices[0]);
                    addButonToSelected(buttonIndex);
                }
            } else {
                addButonToSelected(buttonIndex);
            }
        } else {
            rmButonFromSelected(buttonIndex);
        }

        if(notify) {
            ofxBKUIEventArgs args;
            args.eventType = ofxBKUIEventArgs::SELECTION_CHANGED;
            args.target = this;
            ofNotifyEvent(selectionChanged,args);
        }
   }
}

void ofxBKButtonBar::addButonToSelected(int buttonIndex)
{
    int indexInSelection = getSelectedIndexForButtonIndex(buttonIndex);
    if (indexInSelection==-1) {
        selectedIndices.push_back(buttonIndex);
        selectedLabels.push_back(buttons[buttonIndex]->getLabel());
        buttons[buttonIndex]->setSelected(true,false);
    } else { // readd the same button
        rmButonFromSelected(buttonIndex);
        addButonToSelected( buttonIndex);
        return;
    }
    lastSelect    = buttonIndex;
	selectedLabel = buttons[buttonIndex]->getLabel();
}

void ofxBKButtonBar::rmButonFromSelected(int buttonIndex)
{
    int indexInSelection = getSelectedIndexForButtonIndex(buttonIndex);
    if (indexInSelection>-1) {
        selectedIndices.erase(selectedIndices.begin() + indexInSelection);
        selectedLabels.erase( selectedLabels.begin()  + indexInSelection);
        buttons[buttonIndex]->setSelected(false,false);

        if (!selectedIndices.empty()) {
            lastSelect    = selectedIndices.back();
            selectedLabel = selectedLabels.back();
        } else {
            lastSelect    = -1;
            selectedLabel = "";
        }
    }
}

void ofxBKButtonBar::btSelected(ofxBKUIEventArgs &e)
{
	ofxBKButton * bt = (ofxBKButton *)e.target;
	setSelectedIndex(getIndexForButton(bt),true);
}

void ofxBKButtonBar::btDeselected(ofxBKUIEventArgs &e)
{
	ofxBKButton * bt = (ofxBKButton *)e.target;
	setDeselectedIndex(getIndexForButton(bt),true);
}

void ofxBKButtonBar::setAllowMultipleSelection(bool allow, int _maxNumbSelected)
{
	allowMultipleSelection = allow;
	if (allowMultipleSelection) {
        if (_maxNumbSelected > 0)
            maxNumbSelected = _maxNumbSelected;
        else
            maxNumbSelected = -1;
	} else{
        maxNumbSelected = 1;
	}
}

int ofxBKButtonBar::getIndexForButton(ofxBKButton * bt)
{
	for(int i=0;i<numButtons;++i) {
		if(buttons[i] == bt)
            return i;
	}

	return -1;
}

int ofxBKButtonBar::getSelectedIndexForButtonIndex(int buttonIndex)
{
	for(int i=0;i<(int)(selectedIndices.size());++i) {
		if(selectedIndices[i] == buttonIndex)
            return i;
	}
	//printf("indice(%i) not found in selected indices\n",buttonIndex);
    //std::cout << selectedIndices <<std::endl;
    //printInfo();
	return -1;
}


void ofxBKButtonBar::printInfo()
{
    ofxBKContainer::printInfo();
    std::cout << "   ofxBKButtonBar:" << numButtons << "" << std::endl;
    std::cout << "   maxNumbSelected:" << maxNumbSelected << "" << std::endl;
    std::cout << "   allowMultipleSelection:" << allowMultipleSelection << "" << std::endl;
}
