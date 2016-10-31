#ifndef OFXBKINTEGERINPUT_H
#define OFXBKINTEGERINPUT_H
#include "ofxBKContainer.h"
#include "ofxBKLabel.h"
#include "ofxBKButton.h"
#include "ofxBKUIEventArgs.h"


class ofxBKIntegerInput : public ofxBKContainer
{
    public:
        virtual ~ofxBKIntegerInput(){/*std::cout << "!ofxBKIntegerInput!"<< this << "!";*/};
        ofxBKIntegerInput();
        ofxBKIntegerInput(float _x = 0, float _y = 0, float _width = 100, float _height = 20);

        void setLabel(string _label = "", string _labelSuffix = "");

        int getValue() {return value;};
        void setValue(int _value);

        void setMinMaxValues(int _min, int _max);
        void setMinValue(int _min);
        void setMaxValue(int _max);

        void setAlignment(ofxBKAlign::V_ALIGN _verticalAlign,
                          ofxBKAlign::H_ALIGN horizontalAlign);

        void updateChildrenSize();

        ofEvent<ofxBKUIEventArgs> valueChanged;

        virtual void printInfo();
    protected:
        virtual void init(float _x = 0, float _y = 0, float _width = 50, float _height = 20);
        void btSelected(ofxBKUIEventArgs &e);

        ofxBKButton * plusButton  = nullptr;
        ofxBKButton * minusButton = nullptr;
        ofxBKLabel  * labelTF     = nullptr;

        void updateControlPosition();
        virtual void updateLabelTF();
        void updateOffset(float _width, float _height);
        void notifyValueChanged();
        void normalizeLabelSize();
        float estimateTotalWidth();
        int value;
        bool isMinValueSet = false;
        bool isMaxValueSet = false;
        int minValue = 0;
        int maxValue = 100;
        ofxBKAlign::V_ALIGN verticalAlign;
        ofxBKAlign::H_ALIGN horizontalAlign;
        std::string text="";
        std::string textSuffix="";
        std::string textDisplayed = "";

        float wGap;
        ofVec2f alignOffset;
        float textHeight;
        float textWidth;
        bool autoSize;


    private:
};

#endif // OFXBKINTEGERINPUT_H
