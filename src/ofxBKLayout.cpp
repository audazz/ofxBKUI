#include "ofxBKLayout.h"
#include "ofxBKUI.h"
#include <stdlib.h>     /* abs */
#include <assert.h>
#include "PrettyPrint.h"

ofxBKLayout::~ofxBKLayout()
{
}

ofxBKLayout::ofxBKLayout(float _x, float _y, float _width, float _height)
{
    init( _x, _y, _width, _height);
}

void ofxBKLayout::init(float _x, float _y, float _width,float _height)
{
	ofxBKContainer::init(_x, _y,_width,_height);
	isDrawable = false;
	printDebug = false;
	normalizeCol = true;
    normalizeRow = true;
	widthGap  = 10;
	heightGap = 5;

	transparentBG = true;
	//autoWidth  = width  == 0; //std::cout << "init:autoWidth: " <<  autoWidth << "" << std::endl;
	//autoHeight = height == 0; //std::cout << "init:autoHeight: " << autoHeight << "" << std::endl;
	elementReplace = false;
    //std::cout << "init" << std::endl;
}

void ofxBKLayout::setAutoSize(bool _height, bool _width)
{
    autoWidth = _width;
    autoHeight = _height;
}

void ofxBKLayout::setNormalizeCol(bool _norm)
{
    if (normalizeCol == _norm) return;

    if (_norm)
        normalizeCol = true;
    else {
        normalizeCol  = false;
        normalizeRow  = true;
    }
    checkTables();
    updateElementPosition();

}

void ofxBKLayout::setNormalizeRow(bool  _norm)
{
    if (normalizeRow == _norm) return;

    if (_norm)
        normalizeRow = true;
    else {
        normalizeRow  = false;
        normalizeCol  = true;
    }
    checkTables();
    updateElementPosition();
}


void ofxBKLayout::setGap(float _width, float _height)
{
    if (_width==-1)
        _width = widthGap;

    if (_height==-1)
        _height = heightGap;

    heightGap = _height;
    widthGap  = _width;


}

void ofxBKLayout::addChild( ofxBKUIComponent * element)
{
   addChild(element, -1);
}

void ofxBKLayout::addChild( ofxBKUIComponent * element, int _position)
{
    int _row(0), _col(0);
    switch (layoutShape) {
    case HORIZONTAL: {
        _row = 0;
        if (_position ==-1){
            if (children.empty())
                _col = 0;
            else
                _col = childLayoutSize.col;
        } else
            _col = _position;
        break; }

    case VERTICAL: {
        if (_position ==-1)
            if (children.empty())
                _row = 0;
            else
                _row = childLayoutSize.row;
        else
            _row = _position;
        _col = 0;
        break;}

    case TABLE: {
        assert((false)  && "You must provide row and col when adding child to a TABLE layout!");
        break;}
    }
    addChild(element, _row , _col);
}

void ofxBKLayout::addChild( ofxBKUIComponent * element, int _row , int _col)
{
    ofxBKContainer::addChild(element);
    if (printDebug){
        printf("\naddChild:position: [%i,%i]\n",_row,_col);
    }

    pushinVector(element, _row, _col);

    checkTables();
    if (printDebug){
        std::cout << "addChild:childLayout:"      << childLayout      << "" << std::endl;
        std::cout << "addChild:childHeightType:"  << childHeightType  << "" << std::endl;
        std::cout << "addChild:childHeightTypeI:" << childHeightTypeI << "" << std::endl;
        std::cout << "addChild:childHeightValue:" << childHeightValue << "" << std::endl;
        std::cout << "addChild:childHeightValueI:"<< childHeightValueI<< "" << std::endl;
        std::cout << "addChild:childWidthType:"   << childWidthType   << "" << std::endl;
        std::cout << "addChild:childWidthTypeI:"  << childWidthTypeI  << "" << std::endl;
        std::cout << "addChild:childWidthValue:"  << childWidthValue  << "" << std::endl;
        std::cout << "addChild:childWidthValueI:" << childWidthValueI << "" << std::endl;
        std::cout << "addChild:childPosition:"    << childPosition    << "" << std::endl;
        std::cout << "addChild:childSize:"        << childSize        << "" << std::endl;
        std::cout << "addChild:cumFixedWidth:"    << cumFixedWidth    << "" << std::endl;
        std::cout << "addChild:cumFixedHeight:"   << cumFixedHeight   << "" << std::endl;
    }
    updateSize();

}

void ofxBKLayout::removeChild(ofxBKUIComponent* element)
{
    for (int r=0; r<childLayoutSize.row; ++r) {
        for (int c=0; c< childLayoutSize.col ; ++c){
            if (childLayout[r][c] == element) {

                childLayout[r][c]= nullptr;
                childHeightType[r][c]   =  defaultType;
                childHeightTypeI[r][c]  =  defaultType;
                childHeightValue[r][c]  =  defaultValue;
                childHeightValueI[r][c] =  defaultValue;
                childWidthType[r][c]    =  defaultType;
                childWidthTypeI[r][c]   =  defaultType;
                childWidthValue[r][c]   =  defaultValue;
                childWidthValueI[r][c]  =  defaultType;
                childPosition[r][c]     =  ofVec2f(0);
                childSize[r][c]         =  ofVec2f(0);
                ofxBKContainer::removeChild(element);
                checkTables();
                return;
            }
        }
    }
}

void ofxBKLayout::clearChildren()
{
    resizeChildVectors(1,1);
    removeChild(childLayout[0][0]);
}

void ofxBKLayout::actualizeChild(ofxBKUIComponent * element)
{
    int r,c;
    bool found(false);
    for (r=0 ; r<childLayoutSize.row ; ++r) {
        auto it = std::find(childLayout[r].begin(), childLayout[r].end(), element);
        if (it != childLayout[r].end()){
            found = true;
            c = it - childLayout[r].begin();
            break;
        }
    }
    if (!found){
            if (printDebug)
                std::cout << "Child not found ! No actualization" << std::endl;
        return;
    }
    actualizeChild(r , c);
}

void ofxBKLayout::actualizeChild(int _position)
{
    int _row(-1), _col(-1);
    switch (layoutShape) {
    case HORIZONTAL: {
        _row = 0;
        _col = _position;
        break; }

    case VERTICAL: {
        _row = _position;
        _col = 0;
        break;}

    case TABLE: {
        assert((false)  && "You must provide row and col when adding child to a TABLE layout!");
        break;}
    }

    actualizeChild(_row , _col);
}

void ofxBKLayout::actualizeChild(int _row , int _col)
{
    bool elementReplaceSave(elementReplace);
    elementReplace = true;
    pushinVector(childLayout[_row][_col], _row, _col);
    elementReplace = elementReplaceSave;

    checkTables();
    updateSize();
}


float ofxBKLayout::getElementMinWidth(ofxBKUIComponent * element)
{
    if (element->minSize.x == 0 )
        return element->width;
    else
        return element->minSize.x;
}

float ofxBKLayout::getElementMinHeight(ofxBKUIComponent * element)
{
    if (element->minSize.y == 0 )
        return element->height;
    else
        return element->minSize.y;
}


void ofxBKLayout::updateSize()
{
    float widthTmp, heightTmp;
    if (autoWidth ) {
        widthTmp = std::max(width, *std::max_element(cumFixedWidth.begin(),
                                                     cumFixedWidth.end())
                                     + (childLayoutSize.col -1) * widthGap );
    }
    if (autoHeight) {
        heightTmp = std::max(height,*std::max_element(cumFixedHeight.begin(),
                                                      cumFixedHeight.end())
                                      + (childLayoutSize.row -1) * widthGap );
    }
    bool sizeUpdate = true; // setsize update position -> prevent double update
    if (autoWidth && autoHeight){
        if (printDebug)
           printf("addChild::autoSize setting size [%f,%f]\n",heightTmp, widthTmp);
        setSize(widthTmp,heightTmp);
        sizeUpdate = false;
    } else {
        if (autoWidth){
            if (printDebug)
                printf("addChild::autoWidth setting width [%f]\n",widthTmp);
            setSize(widthTmp,-1);
            sizeUpdate = false;
        }
        if (autoHeight){
            if (printDebug)
                printf("addChild::autoHeight setting height [%f]\n",heightTmp);
            sizeUpdate = false;
            setSize(-1,heightTmp);
        }
    }
    if (sizeUpdate)
        updateElementPosition();
}

void ofxBKLayout::updateChildrenSize()
{
    if (printDebug)
        std::cout << "updateChildrenSize:updateChildrenSize " << std::endl;
    if (!children.empty()) {
        if (printDebug)
            std::cout << "updateChildrenSize:updateElementPosition" << std::endl;
        updateElementPosition();
    }
}

void ofxBKLayout::updateElementPosition()
{
    if (printDebug)
        std::cout << "updateElementPosition:updateElementPosition" << std::endl;
    // assumes all table are checked (consistent information in rows and cols
    // calculate the width and vertical position
    for (int r=0 ; r<childLayoutSize.row ; ++r) {
        if (printDebug)
            std::cout << "updateElementPosition:width:row:" << r << std::endl;
        if (printDebug)
            printf("   childLayoutSize.col:%i\n",childLayoutSize.col);
        float rmWidth(getInnerWidth() - cumFixedWidth[r] - (childLayoutSize.col-1) * widthGap);
        rmWidth = std::max((float)0,rmWidth);
        if (printDebug){
            printf("  size [%f,%f] innersize [%f,%f]\n",width, height, getInnerWidth(),getInnerHeight());
            printf("  cumWidthFixed:%f\n",cumFixedWidth[r]);
            printf("  rmWidth:%f\n",rmWidth);
        }

        float w(0);
        for (int c=0 ; c<childLayoutSize.col ; ++c) {
            float wtmp;
            switch (childWidthType[r][c]) {
                case FIXED:{
                    wtmp = childWidthValue[r][c];
                    break;}
                case FLUID:{
                    wtmp = childWidthValue[r][c] * rmWidth;
                    break;}
            }
            childPosition[r][c].x = w;
            if (printDebug)
                printf("   childPosition[%i,%i].x=%f\n",r,c,w);

            childSize[r][c].x    = wtmp;
            if (printDebug)
                printf("   childSize[%i,%i].x=%f\n",r,c,wtmp);

            w += (wtmp + widthGap);

            if (printDebug)
                printf("   wTotnext=%f\n",w);
        }
    }

    // calculate the height
    for (int c=0 ; c<childLayoutSize.col ; ++c) {
        if (printDebug){
            std::cout << "updateElementPosition:height:col:" << c << std::endl;
            printf("  childLayoutSize.row:%i\n",childLayoutSize.row);
        }
        float rmHeight(getInnerHeight() - cumFixedHeight[c] - (childLayoutSize.row-1) * heightGap);
        rmHeight = std::max((float)0,rmHeight);
        if (printDebug){
            printf("  size [%f,%f] innersize [%f,%f]\n",width, height, getInnerWidth(),getInnerHeight());
            printf("  cumHeightFixed:%f\n",cumFixedHeight[c]);
            printf("  rmHeight:%f\n",rmHeight);
        }

        float h(0);
        for (int r=0 ; r<childLayoutSize.row ; ++r) {
            float htmp;
            switch (childHeightType[r][c]) {
                case FIXED:{
                    htmp = childHeightValue[r][c];
                    break;}
                case FLUID:{
                    if (printDebug)
                        printf("   HeightValue:%f rmHeight:%f\n",childHeightValue[r][c],rmHeight);
                    htmp = childHeightValue[r][c] * rmHeight;
                    break;}
            }
            childPosition[r][c].y = h;
            if (printDebug)
                printf("   childPosition[%i,%i].h=%f\n",r,c,h);
            childSize[r][c].y    = htmp;
            if (printDebug)
                printf("   childSize[%i,%i].y=%f\n",r,c,htmp);
            h += htmp + heightGap;
            if (printDebug)
                printf("   wTotnext=%f\n",h);
        }
    }

    // apply to childs
    if (printDebug)
        std::cout << "updateElementPosition:apply to childs" << std::endl;
    for (int r=0 ; r<childLayoutSize.row ; ++r) {
        for (int c=0 ; c<childLayoutSize.col ; ++c) {
            if (printDebug){
                printf("   childLayout[%i][%i]:",r,c);
                std::cout << "" <<  childLayout[r][c]<< "" << std::endl;
                printf("   size:[%f,%f] ",childSize[r][c].x,childSize[r][c].y);
                printf("   position:[%f,%f]\n",childPosition[r][c].x,childPosition[r][c].y);
            }
            if (childLayout[r][c] != nullptr){

                if ((childSize[r][c].x <=0) ||
                    (childSize[r][c].y <=0)    )  {

                    if (printDebug)
                            std::cout << "    negative size, hiding element" << std::endl;
                    isSizeUpdateSuccess = false;
                    childLayout[r][c]->parentSetVisible(false);

                }else {
                    childLayout[r][c]->parentSetVisible(isVisible());

                    childLayout[r][c]->setSize(childSize[r][c].x,
                                               childSize[r][c].y );
                    if (!childLayout[r][c]->isSizeUpdateSuccess)
                        isSizeUpdateSuccess = false;

                    childLayout[r][c]->setAbsolutePosition(childPosition[r][c].x,
                                                           childPosition[r][c].y );
                }

            }
        }
    }

}



void ofxBKLayout::pushinVector(ofxBKUIComponent * element, int _row, int _col)
{
    if (printDebug){
        printf("pushinVector:pushinVector[%i,%i]\n",_row,_col);
        printf("pushinVector:childLayoutSize[%i,%i]\n",childLayoutSize.row,childLayoutSize.col);
    }
    adaptLayout(_row,_col);
    childLayout[_row][_col] = element;
    if (element != nullptr){
        if (element->fixedHeight){
            if (printDebug)
                printf("pushinVector:element fill: fixedHeight(%f)\n",element->height);
            childHeightTypeI[_row][_col] = FIXED;
            childHeightValueI[_row][_col] = element->height;
        }else {
            if ((!(element->topIsRelative))){
                std::cout << "element->topIsRelative true" << std::endl;
                printInfo();
                element->printInfo();
                assert((element->topIsRelative)  && "Only relative fluid Height is supported");
            }
            if ((!(element->bottomIsRelative))){
                std::cout << "element->bottomIsRelative true" << std::endl;
                printInfo();
                element->printInfo();
                assert((element->bottomIsRelative)  && "Only relative fluid Height is supported");
            }
            childHeightTypeI[_row][_col]  = FLUID;
            childHeightValueI[_row][_col] = abs(element->top - element->bottom);
            if (printDebug)
                printf("pushinVector:element fill: fluidHeight(%f)\n",childHeightValueI[_row][_col]);
        }

        if (element->fixedWidth) {
            if (printDebug)
                printf("pushinVector:element fill: fixedWidth(%f)\n",element->width);
            childWidthTypeI[_row][_col]  = FIXED;
            childWidthValueI[_row][_col] = element->width;
        } else {
            if ((!(element->leftIsRelative))){
                printInfo();
                element->printInfo();
                assert((element->leftIsRelative)  && "Only relative fluid width is supported");
            }
            if ((!(element->rightIsRelative))){
                printInfo();
                element->printInfo();
                assert((element->rightIsRelative) && "Only relative fluid width is supported");
            }
            childWidthTypeI[_row][_col]  = FLUID;
            childWidthValueI[_row][_col] = abs(element->right - element->left);
            if (printDebug)
                printf("pushinVector:element fill: fluidWidth(%f)\n",childWidthValueI[_row][_col]);
        }
    } else {
        childWidthType[   _row][_col] = defaultType;
        childWidthTypeI[  _row][_col] = defaultType;
        childWidthValue[  _row][_col] = defaultValue;
        childWidthValueI[ _row][_col] = defaultValue;
        childHeightType[  _row][_col] = defaultType;
        childHeightTypeI[ _row][_col] = defaultType;
        childHeightValue[ _row][_col] = defaultValue;
        childHeightValueI[_row][_col] = defaultValue;
        childWidthValueI[ _row][_col] = defaultValue;

    }
}

void ofxBKLayout::adaptLayout( int _row, int _col)
{
    switch (layoutShape) {
    case HORIZONTAL: {
        _row=0;
        if (_col > (childLayoutSize.col-1)) {
            if (printDebug)
                printf("pushinVector:simple HORIZONTAL resize[%i,%i]\n",_row+1,_col+1);
            insertCol(_col);
        } else if ((!elementReplace) && (childLayout[_row][_col] != nullptr)) {
            //move forward
            if (printDebug)
                printf("pushinVector:push HORIZONTAL resize[%i,%i]\n",_row+1,_col+1);
            insertCol(_col);
        } else {

        }

        break; }
    case VERTICAL: {
        _col=0;
        if (_row > childLayoutSize.row-1) {
            if (printDebug)
                printf("pushinVector:simple VERTICAL resize[%i,%i]\n",_row+1,_col+1);
            insertRow(_row);
        } else if ((!elementReplace) && (childLayout[_row][_col] != nullptr)) {
            if (printDebug)
                printf("pushinVector:push VERTICAL resize[%i,%i]\n",_col+1,_row+1);
            insertRow(_row);
        } else {
            if (printDebug)
                printf("pushinVector:No resize\n");
        }
        break; }
    case TABLE: {
            if (_row > childLayoutSize.row-1)
                resizeChildVectors(_row+1,childLayoutSize.col );

            if (_col > childLayoutSize.col-1)
                resizeChildVectors(childLayoutSize.row,_col+1 );

        break; }
    }

}

void ofxBKLayout::insertRow(int _pos)
{
    resizeChildVectors(childLayoutSize.row + 1,
                       childLayoutSize.col);
    for (int r=childLayoutSize.row-2 ; r>=_pos ; --r) {
        for (int c=0 ; c<childLayoutSize.col ; ++c) {
            childLayout[r+1][c]=childLayout[r][c];
            childHeightType[r+1][c]   =  childHeightType[r][c];
            childHeightTypeI[r+1][c]  =  childHeightTypeI[r][c];
            childHeightValue[r+1][c]  =  childHeightValue[r][c];
            childHeightValueI[r+1][c] =  childHeightValueI[r][c];
            childWidthType[r+1][c]    =  childWidthType[r][c];
            childWidthTypeI[r+1][c]   =  childWidthTypeI[r][c];
            childWidthValue[r+1][c]   =  childWidthValue[r][c];
            childWidthValueI[r+1][c]  =  childWidthValueI[r][c];
            childPosition[r+1][c]     =  childPosition[r][c];
            childSize[r+1][c]         =  childSize[r][c];
        }
    cumFixedWidth[r+1] = cumFixedWidth[r];
    }

    // set inserted Row to default
    for (int c=0 ; c<childLayoutSize.col ; ++c){
        childLayout[_pos][c]       =  nullptr;
        childHeightType[_pos][c]   =  defaultType;
        childHeightTypeI[_pos][c]  =  defaultType;
        childHeightValue[_pos][c]  =  defaultValue;
        childHeightValueI[_pos][c] =  defaultValue;
        childWidthType[_pos][c]    =  defaultType;
        childWidthTypeI[_pos][c]   =  defaultType;
        childWidthValue[_pos][c]   =  defaultValue;
        childWidthValueI[_pos][c]  =  defaultValue;
        childPosition[_pos][c]     =  ofVec2f(0);
        childSize[_pos][c]         =  ofVec2f(0);
    }
    cumFixedWidth[_pos] = 0;
}

void ofxBKLayout::insertCol(int _pos)
{
    resizeChildVectors(childLayoutSize.row,
                         childLayoutSize.col+1);

    for (int c=childLayoutSize.col-2 ; c>=_pos ; --c) {
        for (int r=0 ; r<childLayoutSize.row ; ++r) {
            childLayout[r][c+1]       =  childLayout[r][c];
            childHeightType[r][c+1]   =  childHeightType[r][c];
            childHeightTypeI[r][c+1]  =  childHeightTypeI[r][c];
            childHeightValue[r][c+1]  =  childHeightValue[r][c];
            childHeightValueI[r][c+1] =  childHeightValueI[r][c];
            childWidthType[r][c+1]    =  childWidthType[r][c];
            childWidthTypeI[r][c+1]   =  childWidthTypeI[r][c];
            childWidthValue[r][c+1]   =  childWidthValue[r][c];
            childWidthValueI[r][c+1]  =  childWidthValueI[r][c];
            childPosition[r][c+1]     =  childPosition[r][c];
            childSize[r][c+1]         =  childSize[r][c];
        }
        cumFixedHeight[c+1] = cumFixedHeight[c];
    }

    // set inserted col to default
    for (int r=0 ; r<childLayoutSize.row ; ++r) {
        childLayout[r][_pos]       =  nullptr;
        childHeightType[r][_pos]   =  defaultType;
        childHeightTypeI[r][_pos]  =  defaultType;
        childHeightValue[r][_pos]  =  defaultValue;
        childHeightValueI[r][_pos] =  defaultValue;
        childWidthType[r][_pos]    =  defaultType;
        childWidthTypeI[r][_pos]   =  defaultType;
        childWidthValue[r][_pos]   =  defaultValue;
        childWidthValueI[r][_pos]  =  defaultValue;
        childPosition[r][_pos]     =  ofVec2f(0);
        childSize[r][_pos]         =  ofVec2f(0);
    }
    cumFixedHeight[_pos] = 0;
}

void ofxBKLayout::resizeChildVectors(int _nrow, int _ncol)
{
    if (printDebug)
        printf("resizePositionVector:resizePositionVector to (%i,%i)\n",_nrow,_ncol);
    if ((_ncol<1)||(_nrow<1)) return;

    if (childLayoutSize.col != _ncol){
        if (printDebug)
            printf("resizePositionVector:resizing col from %i to %i\n",childLayoutSize.col,_ncol);
        // remove from children
        if (_ncol < childLayoutSize.col){
            for (int r=0 ; r<childLayoutSize.row ; ++r){
                for (int c=_ncol ; c<childLayoutSize.col ; ++c){
                    if (childLayout[r][c] != nullptr){
                        removeChild(childLayout[r][c]);
                    }
                }
            }
        }
        for (int r=0 ; r<childLayoutSize.row ; ++r){
            childLayout[r].resize( _ncol, nullptr);

            childHeightType[r].resize(  _ncol, defaultType);
            childHeightTypeI[r].resize( _ncol, defaultType);
            childHeightValue[r].resize( _ncol, defaultValue);
            childHeightValueI[r].resize(_ncol, defaultValue);
            childWidthType[r].resize(   _ncol, defaultType);
            childWidthTypeI[r].resize(  _ncol, defaultType);
            childWidthValue[r].resize(  _ncol, defaultValue);
            childWidthValueI[r].resize( _ncol, defaultType);
            childPosition[r].resize(    _ncol, ofVec2f(0));
            childSize[r].resize(        _ncol, ofVec2f(0));
            cumFixedHeight.resize(_ncol ,0);
        }
        childLayoutSize.col = _ncol;
    }

    if (childLayoutSize.row != _nrow) {
        if (printDebug)
            printf("resizePositionVector:resizing row from %i to %i\n",childLayoutSize.row,_nrow);
        // remove from children
        if (_nrow < childLayoutSize.row ){
            for (int r=_nrow ; r<childLayoutSize.row ; ++r){
                for (int c=0 ; c<childLayoutSize.col ; ++c){
                    if (childLayout[r][c] != nullptr){
                        ofxBKContainer::removeChild(childLayout[r][c]);
                    }
                }
            }
        }
        vector<ofxBKUIComponent * > vtmp1(childLayoutSize.col, nullptr);
        vector<SIZE_TYPE>           vtmp2(childLayoutSize.col, defaultType);
        vector<float>               vtmp3(childLayoutSize.col, defaultValue);
        vector<ofVec2f>             vtmp4(childLayoutSize.col, ofVec2f(0));

        childLayout.resize(      _nrow, vtmp1);

        childHeightType.resize(  _nrow, vtmp2);
        childHeightTypeI.resize( _nrow, vtmp2);
        childWidthType.resize(   _nrow, vtmp2);
        childWidthTypeI.resize(  _nrow, vtmp2);

        childHeightValue.resize( _nrow, vtmp3);
        childHeightValueI.resize(_nrow, vtmp3);
        childWidthValue.resize(  _nrow, vtmp3);
        childWidthValueI.resize( _nrow, vtmp3);

        childSize.resize(        _nrow, vtmp4);
        childPosition.resize(    _nrow, vtmp4);

        cumFixedWidth.resize(    _nrow ,0);

        childLayoutSize.row = _nrow;
    }

}

void ofxBKLayout::checkTables()
{
    if (printDebug)
        std::cout << "checkTables:checkTables" << std::endl;
    // uniformize col (set all col width to its max)

    for (int r=0; r<childLayoutSize.row ; ++r) {
        for (int c=0; c<childLayoutSize.col;++c) {
            childHeightType[r][c]  = childHeightTypeI[r][c];
            childHeightValue[r][c] = childHeightValueI[r][c];
            childWidthType[r][c]   = childWidthTypeI[r][c];
            childWidthValue[r][c]  = childWidthValueI[r][c];
        }
    }

    if (normalizeRow) {
        if (printDebug){
            std::cout << "checkTables:uniformize col ";
            printf("[%i,%i]\n",childLayoutSize.row,childLayoutSize.col);
        }
        for (int r=0; r<childLayoutSize.row ; ++r) {
            float hmax(0);
            float hmaxFl(0);
            if (printDebug)
                printf("  row(%i) ",r);
            for (int c=0; c<childLayoutSize.col;++c) {
                if (printDebug)
                    printf("col(%i) ",c);
                if (childHeightType[r][c] == FIXED) {
                    hmax = std::max(hmax, childHeightValue[r][c]);
                    if (printDebug)
                        printf("hmax:%f ||",hmax);
                } else {
                    hmaxFl = std::max(hmaxFl, childHeightValue[r][c]);
                }
            }
            if (printDebug)
                printf("\n");
            if (hmaxFl>0) {
                for (int c=0; c<childLayoutSize.col;++c) {
                    if (childHeightType[r][c] == FLUID) {
                        childHeightValue[r][c] = hmaxFl;
                    }
                    if (printDebug) {
                        printf("checkTables:setting element (%i,%i) to (%f)\n",r,c,hmaxFl);
                        printInfo();
                    }
                }
            }
            if (hmax>0) { // if a fixed width has been set, set it for the remaining
                for (int c=0; c<childLayoutSize.col;++c) {
                    childHeightValue[r][c] = hmax;
                    childHeightType[r][c]  = FIXED;
                }
                if (printDebug) {
                    printf("checkTables:setting row (%i) to fixed (%f)\n",r,hmax);
                    printInfo();
                }
            }
        }
    }

    // uniformize row
    if (normalizeCol) {
        if (printDebug){
            std::cout << "checkTables:uniformize row ";
            printf("[%i,%i]\n",childLayoutSize.row,childLayoutSize.col);
        }
        for (int c=0; c<childLayoutSize.col;++c) {
            float wmax(0);
            float wmaxFl(0);
            if (printDebug)
                printf("  col(%i) ",c);
            for (int r=0; r< childLayoutSize.row;++r) {
                if (printDebug)
                    printf("row(%i) ",r);
                if (childWidthType[r][c] == FIXED) {
                    wmax = std::max(wmax, childWidthValue[r][c]);
                    if (printDebug)
                        printf("wmax:%f ||",wmax);
                } else {
                    wmaxFl = std::max(wmaxFl, childWidthValue[r][c]);
                }
            }
            if (printDebug)
                printf("\n");
            if (wmaxFl>0) {
                for (int r=0 ; r<childLayoutSize.row ; ++r) {
                    if (childWidthType[r][c] == FLUID) {
                        childWidthValue[r][c] = wmaxFl;
                    }
                    if (printDebug) {
                        printf("checkTables:setting element (%i,%i) to (%f)\n",r,c,wmaxFl);
                        printInfo();
                    }
                }
            }
            if (wmax>0) { // if a fixed height has been set, set it for the remaining
                for (int r=0 ; r< childLayoutSize.row ; ++r) {
                    childWidthValue[r][c] = wmax;
                    childWidthType[r][c]  = FIXED;
                }
                if (printDebug) {
                    printf("checkTables:setting col (%i) to fixed (%f)\n",c,wmax);
                    printInfo();
                }
            }
        }
    }

    // calculate cummulative fixed width and height
    for (auto & v : cumFixedWidth )  v=0;
    for (auto & v : cumFixedHeight ) v=0;
    if (printDebug)
        std::cout << "checkTables:cummulative fixed width and height" << std::endl;
    for (int r=0; r< childLayoutSize.row ; ++r) {
        for (int c=0 ; c<childLayoutSize.col ; ++c) {
            if (childWidthType[r][c]  == FIXED) {
                cumFixedWidth[r] += childWidthValue[r][c];
            }
            if (childHeightType[r][c] == FIXED) {
                cumFixedHeight[c] += childHeightValue[r][c];
            }
        }
    }

    if (printDebug)
        std::cout << "checkTables:normalize  fluid height" << std::endl;
    for (int r=0; r< childLayoutSize.row;++r) {
        float htot(0);
        int   nFlu(0);
        for (int c=0; c<childLayoutSize.col;++c) {
            if (childWidthType[r][c] == FLUID) {
                htot += childWidthValue[r][c];
                nFlu++;
            }
        }
        if (nFlu>0){
            if (htot>0) {
                for (int c=0 ; c<childLayoutSize.col ; ++c) {
                    if (childWidthType[r][c] == FLUID) {
                        childWidthValue[r][c] /= htot;
                    }
                }
            } else {
                htot = 1.0 / nFlu;
                for (int c=0 ; c<childLayoutSize.col ; ++c) {
                    if (childWidthType[r][c] == FLUID) {
                        childWidthValue[r][c] = htot;
                    }
                }
            }
        }
    }

    // normalize fluid width
    if (printDebug)
        std::cout << "checkTables:normalize  fluid width" << std::endl;
    for (int c=0; c< childLayoutSize.col;++c) {
        float wtot(0);
        int   nFlu(0);
        for (int r=0; r<childLayoutSize.row;++r) {
            if (childHeightType[r][c] == FLUID) {
                wtot += childHeightValue[r][c];
                nFlu++;
            }
        }
        if (nFlu>0){
            if (wtot==0) {
                wtot = 1/nFlu;
                for (int r=0; r<childLayoutSize.row;++r) {
                    if (childHeightType[r][c] == FLUID) {
                        childHeightValue[r][c] = wtot;
                    }
                }
            } else {
                for (int r=0; r<childLayoutSize.row;++r) {
                    if (childHeightType[r][c] == FLUID) {
                        childHeightValue[r][c] /= wtot;
                    }
                }
            }
        }
    }

}

void ofxBKLayout::draw()
{
	ofxBKContainer::draw();
    if(drawDebug || ofxBKUI::drawDebug) {/*
		ofSetColor(ofColor::green,100);
		ofNoFill();
		for (int r=1; r<childLayoutSize.row ; ++r){
            for (int c=1; c<childLayoutSize.col  ; ++c){
                ofLine(ofVec2f(childPosition[r][c].x - widthGap/2, childPosition[r][c].y),
                       ofVec2f(childPosition[r][c].x - widthGap/2, childPosition[r][c].y+childSize[r][c].y));
            }
		}
		for (int r=0; r<childLayoutSize.row -1 ; ++r){
            ofLine();
		}
		*/
	}
}


void ofxBKLayout::printInfo()
{
    ofxBKContainer::printInfo();
    std::cout << "   ofxBKLayout:"  << "" << std::endl;
    std::cout << "   layoutType:" << ((layoutShape==VERTICAL)?"vertical":"horizontal") << "" << std::endl;
    std::cout << "   autoHeight:" << autoHeight << "" << std::endl;
    std::cout << "   autoWidth:"  << autoWidth << "" << std::endl;
    printf(      "   gaps:[w:%f,h:%f]\n",widthGap,heightGap);
    printf(      "   childLayoutSize:[r:%i,c:%i]\n",childLayoutSize.row,childLayoutSize.col);


}


















