
#ifndef OFXBKUIDATADRAW_H
#define OFXBKUIDATADRAW_H
#include "ofxBKFbo.h"
#include "CPMPointPickingInBkFbo.h"
#include "CPMScatterPoint.h"
#include <array>
#include "CPMScatterAxis.h"
#include <unordered_map>



class ofxBKUiDataDraw : public ofxBKUIEventHandlers
{
    public:
        using ptVector = std::vector<CPMScatterPoint>;
        ofxBKUiDataDraw(ptVector & _points, ofxBKFbo * _bkFbo);
        virtual ~ofxBKUiDataDraw();
        void setRange(CPMScatterRange range);
        void setRange();
        void setDrawLines(bool _drawLine) { toggleLines = _drawLine;};

        void initialize();

        int getLastSelected() {return lastSelected; };
        int getLastClicked()  {return lastClicked;}
        void setNormalize(bool _normalize);

        void    setBackgroundColor(ofColor c) {BckColor.set(c);};
        ofColor getBackgroundColor() {return BckColor;};
        ofEvent<ofxBKUIEventArgs> pointSelected;
        ofEvent<ofxBKUIEventArgs> pointClicked;
        virtual void draw();


    protected:

        void setUpFBO();
        void setUpScatter();
        void calculateRange();
        void drawAxis();
        void drawPoints();
        void findSelected();
        void updateFbo();
        void notifyPointSelected();
        void notifyPointClicked();

        ofxBKFbo * bkFbo = nullptr;
        using ofFboPtr = std::unique_ptr<ofFbo>;
        ofFboPtr fbo;
        ptVector & points;
        ptVector pointsDisplayed;
        vector<int> uniqueGp;
        std::unordered_map <int, int> uniqueGpMap;
        vector<ofColor> * groupColors;
        std::vector<ofVec3f> gpCenter;

        bool isAutoRange = true;
        bool isNormalized = false;
        CPMScatterRange range;
        CPMScatterRange rangeOriginal;
        CPMScatterAxis axes;

        int lastSelected = -1;
        int lastClicked  = -1;
        int lastClickedTmp  = -1;
        int mouseOver    = -1;
        bool mouseButtonDown = false;


        ofColor BckColor;
        bool toggleLines  = false;
        bool toggleMode   = false;

        using ptPicking = CPMPointPickingInBkFbo< ptVector>;
        using ptPickingoPtr = std::unique_ptr<ptPicking>;
        ptPickingoPtr dataPointPicking;
        float pointPickingTolerance = 1.5;
        float pointDiameter;

        bool isInitialized = false;
        bool isFboUpdated = false;
        bool isDataNormalize = false;

};

#endif // OFXBKUIDATADRAW_H

