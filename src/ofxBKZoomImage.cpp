#include "ofxBKZoomImage.h"
#include "ofxBKStyle.h"

ofxBKZoomImage::ofxBKZoomImage(float _x, float _y, float _width, float _height)
{
	init(_x, _y, _width, _height);
}

void ofxBKZoomImage::init(float _x, float _y, float _width, float _height)
{
    isInzoomMode = false;
    zoomLevel = 2;
    zoomSensitivity = 0.5;
    zoomBckColor = ofColor(0,0,0);
    zoomRecConstSize = true;
    mouseStartAbsPos.set(0,0);

    zoomRectMaxSize[0] = 400;// in pixel !
    zoomRectMaxSize[1] = 400;

	ofxBKImage::init(_x,_y,_width,_height);
	zoomRect.set(0,0,zoomRectMaxSize[0],
                     zoomRectMaxSize[1]);
	zoomAnchor.set(0,-1);
}

void ofxBKZoomImage::draw()
{
	ofxBKImage::draw();

	if(isPressed && drawedImage != NULL)
	{
	    if (ofGetMousePressed(0))
            zoomLevel = 1;
        else if (ofGetMousePressed(2))
            zoomLevel = 2;

        int wCropRect = (int)(std::min( zoomRectMaxSize[0], (int)targetImage->getWidth()  ) /zoomLevel);
        int hCropRect = (int)(std::min( zoomRectMaxSize[1], (int)targetImage->getHeight() ) /zoomLevel);

        // smooth mouse position
        ofVec2f pointerPos = getSmoothMousePosition();

		ofVec2f pointerPosInImage;

		ofVec2f deltaIm(wCropRect, hCropRect);
        ofVec2f scaleIm(targetImage->getWidth(), targetImage->getHeight());

		// position relative to image coordinate (in percent)
		ofVec2f deltaDraw(imageRect.x, imageRect.y);
		ofVec2f scaleDraw(imageRect.width, imageRect.height);
		ofVec2f pointerRel = (pointerPos - deltaDraw)/scaleDraw;

		// bound the pointer position
		if (pointerRel.x<0) pointerRel.x = 0; if (pointerRel.x>1) pointerRel.x = 1;
		if (pointerRel.y<0) pointerRel.y = 0; if (pointerRel.y>1) pointerRel.y = 1;

		// bound the pointer location to image bound
		pointerPos        = pointerRel * scaleDraw + deltaDraw;

        pointerPosInImage = pointerRel * scaleIm;

		// crop rect in image coordinate
		ofPoint pTop = pointerPosInImage - deltaIm/2;
		ofPoint pBot = pointerPosInImage + deltaIm/2;


        ofVec2f deltaTopFull = pTop - pointerPosInImage;
        ofVec2f deltaBotFull = pBot - pointerPosInImage;

        // bound the points
        if (pTop.x<0) pTop.x = 0; if (pTop.x>scaleIm.x) pTop.x = scaleIm.x;
        if (pTop.y<0) pTop.y = 0; if (pTop.y>scaleIm.y) pTop.y = scaleIm.y;

        if (pBot.x<0) pBot.x = 0; if (pBot.x>scaleIm.x ) pBot.x = scaleIm.x;
        if (pBot.y<0) pBot.y = 0; if (pBot.y>scaleIm.y ) pBot.y = scaleIm.y;

        // rect is define with delta to the center
        ofVec2f deltaTop = pTop - pointerPosInImage;
        ofVec2f deltaBot = pBot - pointerPosInImage;

		zoomRect.set(pointerPosInImage + deltaTop,
                     pointerPosInImage + deltaBot  );

        // zoom rectangle limited by image border
		ofRectangle destRect = ofRectangle( pointerPos + deltaTop * zoomLevel,
                                            pointerPos + deltaBot * zoomLevel);

        // zoom rectangle full size
		ofRectangle destRectFull = ofRectangle( pointerPos + deltaTopFull * zoomLevel,
                                                pointerPos + deltaBotFull * zoomLevel);

        if (!zoomRecConstSize)
            destRectFull = destRect;// limit the frame to image border

        // erase background color
		ofPushStyle();
            ofSetColor(zoomBckColor);
            ofFill();
            ofDrawRectangle(destRectFull);
        ofPopStyle();

        // draw image
        if (isResizedForDraw){
            // memory inefficient crop for big images
            cropImage.cropFrom( *targetImage, zoomRect.x    , zoomRect.y,
                                              zoomRect.width, zoomRect.height     );
            cropImage.draw(destRect.x,destRect.y,destRect.width,destRect.height);
        } else {
            // memory efficient drawSubsection for small images
            targetImage->drawSubsection(destRect.x,destRect.y,destRect.width,destRect.height,
                                        zoomRect.x,zoomRect.y,zoomRect.width,zoomRect.height);
        }

        //and surrounding frame
        ofPushStyle();
            ofSetColor(ofxBKStyle::normalColor);
            ofNoFill();
            ofSetLineWidth(4);
            ofDrawRectangle(destRectFull);
		ofPopStyle();


        isInzoomMode = true;;

	}else {
        isInzoomMode = false;
	}
}



void ofxBKZoomImage::processImageForDrawing()
{
	ofxBKImage::processImageForDrawing();

	//originalImage.cropFrom(*image,0,0,image->width,image->height);
}

void ofxBKZoomImage::mousePressed(ofMouseEventArgs &e)
{
	bringToFront();
}


/** @brief getSmoothMousePosition
  *
  * @todo: document this function
  */
ofVec2f ofxBKZoomImage::getSmoothMousePosition()
{
    ofVec2f pointerPos = getMousePosition();
    if (!isInzoomMode){
        mouseStartAbsPos = pointerPos;
        lastPos.clear();
        //std::cout << "zoom Mode:"<< mouseStartAbsPos << std::endl;
    }

    ofVec2f deltaMouse = pointerPos - mouseStartAbsPos;

    pointerPos = mouseStartAbsPos + deltaMouse * zoomSensitivity;


    lastPos.push_back(pointerPos);

    if (lastPos.size()>5)
        lastPos.erase(lastPos.begin());

    pointerPos.set(0);
    for(unsigned i=0;i<lastPos.size();i++){
        pointerPos += lastPos[i];
    }
    pointerPos /= lastPos.size();

    return pointerPos;
}



void ofxBKZoomImage::printInfo()
{
ofxBKImage::printInfo();
std::cout << "   ofxBKZoomImage:"  << "" << std::endl;
}
