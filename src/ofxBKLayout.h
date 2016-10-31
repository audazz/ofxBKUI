#ifndef OFXBKLAYOUT_H
#define OFXBKLAYOUT_H
#include "ofxBKContainer.h"
//class ofxBKUIComponent;

class ofxBKLayout  : public ofxBKContainer
{
    public:
        virtual ~ofxBKLayout();
        ofxBKLayout(float _x = 0, float _y = 0, float _width = 50, float _height = 20);
        void init(float _x, float _y, float _width,float _height);

        enum LAYOUT_SHAPE {HORIZONTAL, VERTICAL, TABLE};
        ofxBKLayout * setLayoutType(LAYOUT_SHAPE _shape) {layoutShape = _shape; return this;};
        void setAutoSize(bool _height, bool _width);
        void setNormalizeCol(bool _norm);
        void setNormalizeRow(bool _norm);
        void setGap(float _width = -1, float _height=-1);

        virtual void addChild( ofxBKUIComponent * element);
        virtual void addChild( ofxBKUIComponent * element, int _position);
        virtual void addChild( ofxBKUIComponent * element, int _row , int _col);
        virtual void removeChild(ofxBKUIComponent* element);
        void clearChildren();

        void actualizeChild(ofxBKUIComponent * element);
        void actualizeChild(int _position);
        void actualizeChild(int _row , int _col);
        //virtual void addChild( ofxBKUIComponent * element, vector<int> _position);

        bool elementReplace;
        virtual void updateChildrenSize();

        virtual void printInfo();
        bool printDebug;

    protected:
        enum SIZE_TYPE {FLUID, FIXED};
        struct coordType {
            coordType(int r=0,int c=0): row(r), col(c){};
            int row, col;
        };
        void adaptLayout( int _row, int _col);
        void updateSize();
        void updateElementPosition();
        void checkTables();
        void normalizeFluid(vector<vector<SIZE_TYPE>> & typeTable, vector<vector<float>> & valueTable );
        void uniformizeFluid(vector<vector<SIZE_TYPE>> & typeTable);
        void pushinVector(ofxBKUIComponent * _child, int _col, int _row);
        void resizeChildVectors(int _ncol, int _nrow);
        void insertCol(int _pos);
        void insertRow(int _pos);

        virtual void draw();

        float getElementMinWidth(  ofxBKUIComponent * element);
        float getElementMinHeight( ofxBKUIComponent * element);

        LAYOUT_SHAPE    layoutShape = VERTICAL;
        const SIZE_TYPE defaultType = FLUID;
        const float defaultValue = 0;
        vector<vector<ofxBKUIComponent * >> childLayout      = {{nullptr}};
        vector<vector<SIZE_TYPE>>           childHeightType  = {{defaultType}};
        vector<vector<SIZE_TYPE>>           childHeightTypeI = {{defaultType}};
        vector<vector<float>>               childHeightValue = {{defaultValue}};
        vector<vector<float>>               childHeightValueI = {{defaultValue}};
        vector<vector<SIZE_TYPE>>           childWidthType   = {{defaultType}};
        vector<vector<SIZE_TYPE>>           childWidthTypeI  = {{defaultType}};
        vector<vector<float>>               childWidthValue  = {{defaultValue}};
        vector<vector<float>>               childWidthValueI = {{defaultValue}};
        vector<vector<ofVec2f>>             childPosition    = {{ofVec2f(0,0)}};
        vector<vector<ofVec2f>>             childSize        = {{ofVec2f(0,0)}};
        vector<float>                       cumFixedWidth    = {0};
        vector<float>                       cumFixedHeight   = {0};

        coordType childLayoutSize = coordType(1,1);

        float widthGap;
        float heightGap;
        bool extendEmpty = true;
        bool autoWidth;
        bool autoHeight;
        bool normalizeCol;
        bool normalizeRow;
    private:

};

#endif // OFXBKLAYOUT_H
