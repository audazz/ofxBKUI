#include "ofxBKUiDataDraw.h"
#include "CameraAutoFOV.h"
#include "CPMColorDispatched.h"
#include "PrettyPrint.h"

ofxBKUiDataDraw::ofxBKUiDataDraw(ptVector & _points, ofxBKFbo * _bkFbo) :
    bkFbo(_bkFbo),
    points(_points),
    uniqueGp(0),
    BckColor(90)
{
    CPMScatterPoint pt;
    for (auto p : points) {
        if ( std::find(std::begin(uniqueGp), std::end(uniqueGp), p.groupID)
                == std::end(uniqueGp))
            uniqueGp.push_back(p.groupID);
    }
    std::sort (uniqueGp.begin(), uniqueGp.end());
    for (unsigned i=0 ; i<uniqueGp.size() ; ++i) {
        uniqueGpMap[uniqueGp[i]] = i;
    }
    /*
    // calculate the center for each group
    gpCenter.resize(uniqueGp.size(),ofVec3f(0));
    std::vector<int> nIngp(uniqueGp.size(),0);
    for (auto p : points) {
        gpCenter[uniqueGpMap[p.groupID]] += p.coord;
        nIngp[p.groupID]++;
    }
    for (int i=0; i<gpCenter.size();++i){
        gpCenter[i] /= nIngp[i];
    }
    */
    CPMColorDispatched cmap;
    cmap.setNumberOfPoints(uniqueGp.size());
    cmap.setBackgroundColors({BckColor});
    groupColors  = cmap.getColorMapPtr();

    if (bkFbo != nullptr) {
        bkFbo->printInfo();
        bkFbo->addFunctionCalledInDraw(this);
    }
}

ofxBKUiDataDraw::~ofxBKUiDataDraw()
{
    if (bkFbo != nullptr)
        bkFbo->removeFunctionCalledInDraw(this);
}

void ofxBKUiDataDraw::setRange(CPMScatterRange _range)
{
    isAutoRange = false;
    rangeOriginal = _range;
    setUpScatter();
}

void ofxBKUiDataDraw::setRange()
{
    isAutoRange = true;
    calculateRange();
    setUpScatter();
}


void ofxBKUiDataDraw::setNormalize(bool _normalize)
{
    if (isNormalized != _normalize) {
        isNormalized = _normalize;
        std::cout << "isNormalized:" << isNormalized << "" << std::endl;
        setUpScatter();
    }
}
void ofxBKUiDataDraw::calculateRange()
{
    if (points.empty())
        return;

    CPMRangeScatterPoint rangeTmp(points[0]);
    rangeOriginal = for_each(points.begin(),points.end(),rangeTmp).value();
    rangeOriginal.calculateCenter();
}

void ofxBKUiDataDraw::initialize()
{

    setUpFBO();

    if (isAutoRange){
        calculateRange();
    }

    setUpScatter();


}

void ofxBKUiDataDraw::setUpFBO()
{
    if (fbo == nullptr)
        fbo = ofFboPtr(new ofFbo());

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
    settings.depthStencilAsTexture = false;
	settings.width  = 1000;
	settings.height = 1000;

	fbo->allocate(settings);
	bkFbo->linkToOfFbo(fbo.get());
}

void ofxBKUiDataDraw::setUpScatter()
{
    if ((fbo == nullptr) || (bkFbo == nullptr))
        return;


    pointsDisplayed.resize(points.size());
    std::copy(points.begin(), points.end(), pointsDisplayed.begin());

    // normalize points bt 0 and 1
    if (isNormalized){
        ofVec3f diff(rangeOriginal.getRange()); //TODO:??? unused
        for (unsigned i=0; i<points.size(); ++i) {
            pointsDisplayed[i].coord -= rangeOriginal.mini;
            pointsDisplayed[i].coord /= diff;
        }
        if (!pointsDisplayed.empty()) {
                std::cout << "pointsDisplayed:" << pointsDisplayed[0].coord << "" << std::endl;
                std::cout << "pointsDisplayed:" << pointsDisplayed[1].coord << "" << std::endl;
            CPMRangeScatterPoint rangeTmp(pointsDisplayed[0]);
            range = for_each(pointsDisplayed.begin(),pointsDisplayed.end(),rangeTmp).value();
            std::cout << "range:" << range<< std::endl;
        } else {
            range = CPMScatterRange();
        }
    }else{
        std::cout << "range:" << range<< std::endl;
        std::cout << "rangeOriginal:" << rangeOriginal<< std::endl;
        range = rangeOriginal;
        std::cout << "range:(after)" << range<< std::endl;
    }
    std::cout << "range:(afteragain)" << range<< std::endl;
    range.calculateCenter();
	bkFbo->lockCameraTo(range.center);
    bkFbo->setRotationAxes(ofxBKFbo::Z_ON_TOP);

	CameraAutoFOV autoView(bkFbo,range,10,0.8);
    pointDiameter = autoView.getPointSize() * 1.2;

    // prepare axis
    axes.axisFromRange(range);

    // calculate the center for each group
    gpCenter.resize(uniqueGp.size(),ofVec3f(0));
    std::vector<int> nIngp(uniqueGp.size(),0);
    for (auto p : pointsDisplayed) {
        gpCenter[uniqueGpMap[p.groupID]] += p.coord;
        nIngp[p.groupID]++;
    }
    for (int i=0; i<gpCenter.size();++i){
        gpCenter[i] /= nIngp[i];
    }

    if (dataPointPicking != nullptr)
        dataPointPicking.reset();
	dataPointPicking = ptPickingoPtr( new ptPicking(pointsDisplayed,bkFbo));
	dataPointPicking->setPointShape(CPMScatterPoint::SPHERE);
	dataPointPicking->setDiameter(pointDiameter);
	dataPointPicking->setTolerance(pointPickingTolerance);
	dataPointPicking->setDrawMouse(true);
	isInitialized = true;

    if (fbo != nullptr && bkFbo != nullptr)
        updateFbo();
}

void ofxBKUiDataDraw::draw()
{
    //printf("ofxBKUiDataDraw::draw()\n")
    if (isInitialized && bkFbo->isVisible() ) {
        //if ((!isFboUpdated) || bkFbo->getCameraChanged())
        updateFbo();
    }
}

void ofxBKUiDataDraw::updateFbo()
{
    bkFbo->beginFbo();
        ofEnableDepthTest();
        ofClear(BckColor);
        ofPushStyle();
        ofBackground(BckColor);
        ofPushMatrix();
            findSelected();
            drawAxis();
            drawPoints();

        ofPopMatrix();
        ofPopStyle();


        ofDisableDepthTest();
    bkFbo->endFbo();
    isFboUpdated = true;
}

void ofxBKUiDataDraw::drawPoints()
{
    ofPushStyle();
    ofSetLineWidth(1);
    for(unsigned i=0;i<pointsDisplayed.size();++i) {
        ofVec3f p(pointsDisplayed[i].coord);



        if(toggleLines) {
            if(toggleMode)
                ofSetColor((*groupColors)[uniqueGpMap[pointsDisplayed[i].groupID]],200);
            else
                ofSetColor(pointsDisplayed[i].color,200);

            ofLine(p, gpCenter[pointsDisplayed[i].groupID]);
        }

        if(toggleMode)
            ofSetColor((*groupColors)[uniqueGpMap[pointsDisplayed[i].groupID]]);
        else
            ofSetColor(pointsDisplayed[i].color);

        if ((int)i==lastClicked){
            ofSetColor(ofColor::white);
        } else if ((int)i==mouseOver){
            ofSetColor(ofColor::white);
        }
        ofDrawSphere(p,pointDiameter);

    }
    ofPopStyle();
}

void ofxBKUiDataDraw::drawAxis()
{
    axes.draw();
}

void ofxBKUiDataDraw::findSelected()
{
    if (ofGetKeyPressed(OF_KEY_LEFT_SHIFT)) {
        dataPointPicking->setDrawMouse(true);
        isFboUpdated = false;
    } else
        dataPointPicking->setDrawMouse(false);

    bool isMouseButtonPressed = ofGetMousePressed(0);
    mouseOver = dataPointPicking->getPointSelected();

    if (mouseOver>-1) {
        isFboUpdated = false;
        if (!mouseButtonDown) {

            if (isMouseButtonPressed){ //record last point click down
                mouseButtonDown = true;
                lastClickedTmp = mouseOver;

            }else if (lastSelected!=mouseOver){//notify if selection changes
                notifyPointSelected();
                lastSelected = mouseOver;
            }

        }else{
            if (!isMouseButtonPressed) {
                mouseButtonDown = false;
                if ( mouseOver==lastClickedTmp){
                    lastClicked = lastClickedTmp;
                    notifyPointClicked();
                }
            }

        }

    } else  {
        if (!isMouseButtonPressed)
            mouseButtonDown = false;
    }



}


void ofxBKUiDataDraw::notifyPointSelected()
{
	ofxBKUIEventArgs args;
	args.eventType = ofxBKUIEventArgs::SELECTED;
	args.target = this;
	ofNotifyEvent(pointSelected,args);
}

void ofxBKUiDataDraw::notifyPointClicked()
{
	ofxBKUIEventArgs args;
	args.eventType = ofxBKUIEventArgs::CLICKED;
	args.target = this;
	ofNotifyEvent(pointClicked,args);
}

