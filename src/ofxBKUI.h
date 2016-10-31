#pragma once
#include "ofMain.h"

#include "ofxBKUIEventArgs.h"
#include "ofxBKStyle.h"

#include "ofxBKContainer.h"
#include "ofxBKButton.h"
#include "ofxBKCheckBox.h"
#include "ofxBKButtonBar.h"
#include "ofxBKSlider.h"
#include "ofxBKDoubleSlider.h"
#include "ofxBKPanel.h"
#include "ofxBKImage.h"
#include "ofxBKZoomImage.h"
#include "ofxBKFbo.h"
#include "ofxBKUIComponent.h"
#include "ofxBKTextEdit.h"
#include "ofxBKLayout.h"
#include "ofxBKLabelSelect.h"
#include "ofxBKIntegerInput.h"
#include "ofxBKUITextBox.h"


class ofxBKUI
{
public :

	static bool drawDebug;
	static bool freeze;
	static bool childMinSize2Parents;
	static void init();

};
