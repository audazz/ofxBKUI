#pragma once
#include "ofxBKUIComponent.h"

#define IMAGE_FIT 1
#define IMAGE_FILL 2
#define IMAGE_ORIGINAL 3
#define IMAGE_STRETCH 4

class ofxBKImage : public ofxBKUIComponent
{
public :
	ofxBKImage(float _x = 0, float _y = 0, float _width = 100, float _height = 100);

	ofImage *imageDefault;//defaut image (empty) -> don't know if it is usefull
	ofImage *imageInternal;// pointer to image when data are stored inside the class (is allocated)
	ofImage *linkedImage;// pointer to image when data are external

	bool isLinked;

	int fitMode;

	ofRectangle imageRect;

	virtual void loadImage(string path);
	virtual void unloadImage();
	virtual void setFromPixels(ofPixels pixels);
	virtual void linkToOfImage(ofImage * imageRef);
	virtual void unlink();
	virtual void processImageForDrawing();

	virtual void setSize(float _width, float _height, bool notify = true);

protected :
	virtual void init(float _x = 0, float _y = 0, float _width = 100, float _height = 100);
	virtual void draw();
	virtual void unallocateDrawImage();


	void updateImagePosition();

	int maxSize;
	ofImage *targetImage;// ptr to image whenever it is linked or allocated
	ofImage *drawedImage;// ptr to the image to be drawn(targetImage or a resized copied version of if targetImage is too large)

	bool isResizedForDraw;
};
