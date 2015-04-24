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


class ofxBKUI
{
public :

	static bool drawDebug;
	static bool freeze;
	static void init();
};