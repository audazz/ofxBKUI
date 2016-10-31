#include "ofxBKImage.h"
ofxBKImage::~ofxBKImage()
{
    //std::cout << "!ofxBKImage!" << this <<"!";
    unloadImage();
}

ofxBKImage::ofxBKImage(float _x, float _y, float _width,float _height)
{
	init(_x, _y, _width,_height);
}

void ofxBKImage::init(float _x, float _y, float _width,float _height)
{
    maxSize = 1024;
	//printf("BT init\n");
	drawedImage = nullptr; //be sure there is no pointer there

	ofxBKUIComponent::init(_x, _y, _width, _height);

	fitMode = IMAGE_FIT;

    isResizedForDraw = false;
	isLinked = false;
	linkedImage = nullptr;
}

void ofxBKImage::draw()
{
	ofxBKUIComponent::draw();
	ofSetColor(0,50);
	ofRect(0,0,width,height);
	ofSetColor(255);

	if(drawedImage != nullptr)
		drawedImage->draw(imageRect);
}

void ofxBKImage::setSize(float _width, float _height, bool notify)
{
	ofxBKUIComponent::setSize(_width, _height, notify);
	updateImagePosition();
}

void ofxBKImage::updateImagePosition()
{
	if(drawedImage == nullptr) return;
	float tx = 0;
	float ty = 0;
	float tw = drawedImage->width;
	float th = drawedImage->height;

	float ratio = width/height;
	float imageRatio = drawedImage->width*1.0/drawedImage->height;

	switch(fitMode)
	{
	case IMAGE_FIT:
		if(imageRatio > ratio)
		{
			tw = width;
			th = tw/imageRatio;
			ty = (height-th)/2;
		}else
		{
			th = height;
			tw = th*imageRatio;
			tx = (width-tw)/2;
		}
		break;

	case IMAGE_FILL:
		if(imageRatio < ratio)
		{
			tw = width;
			th = height*tw/width;
		}else
		{
			th = height;
			tw = width*th/height;
		}
		break;

	case IMAGE_ORIGINAL:
		break;

	case IMAGE_STRETCH:
		tw = width;
		th = height;
		break;
	}

	imageRect.set(tx, ty, tw, th);
}

void ofxBKImage::processImageForDrawing()
{
    // unallocate previously allocated resized image
    unallocateDrawImage();
    if (targetImage == nullptr)
        return;

    float iw = targetImage->width;
    float ih = targetImage->height;

    if((maxSize > 0) && ((iw > maxSize) || (ih > maxSize)))
    {
        isResizedForDraw = true;
        printf("cloning image for display... ");
        drawedImage = new ofImage;
        drawedImage->clone(*targetImage);
        printf("ok!\n");
        if(iw >= ih && iw > maxSize)
            drawedImage->resize(maxSize,ih*maxSize/iw);
        else if(ih > iw && ih > maxSize)
            drawedImage->resize(iw*maxSize/ih,maxSize);
    }else{
        drawedImage = targetImage;
        isResizedForDraw = false;
    }

}

void ofxBKImage::loadImage(string path)
{
	unlink();
    isResizedForDraw = false;
    imageInternal = new ofImage();
	imageInternal->loadImage(path);
	isAllocated = true;
	targetImage = imageInternal;
	processImageForDrawing();
}

void ofxBKImage::setFromPixels(ofPixels p)
{
    unloadImage();
	unlink();
    imageInternal = new ofImage();
    isResizedForDraw = false;
	imageInternal->getPixelsRef() = p;
    imageInternal->update();
	processImageForDrawing();
}

void ofxBKImage::unloadImage()
{
    if (imageInternal!=nullptr)
        delete imageInternal;
    imageInternal = nullptr;

    unallocateDrawImage();
    isResizedForDraw = false;
}

void ofxBKImage::unallocateDrawImage()
{
    if (isResizedForDraw){
        if (drawedImage != nullptr){
            delete drawedImage;
        }
    }
	isAllocated = false;
    drawedImage = nullptr;
}

void ofxBKImage::linkToOfImage(ofImage *_linkedImage)
{
	isLinked = true;
    isResizedForDraw = false;
	linkedImage = _linkedImage;
	targetImage = linkedImage;
	processImageForDrawing();
	updateImagePosition();
}

void ofxBKImage::unlink()
{
	isLinked = false;
	linkedImage = nullptr;

	unallocateDrawImage();

    isResizedForDraw = false;
}

void ofxBKImage::printInfo()
{
    ofxBKUIComponent::printInfo();
    std::cout << "   ofxBKImage:"  << "" << std::endl;
}

