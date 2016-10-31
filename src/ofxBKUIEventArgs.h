#pragma once

#define BKEVENT_SELECTED 1
#define BKEVENT_DESELECTED 2
#define BKEVENT_SELECTION_CHANGED 3
#define BKEVENT_RESIZED 4
#define BKEVENT_VALUE_CHANGED 5
#define BKEVENT_VALUE_CHANGED 5
class ofxBKUIEventHandlers
{
    public:
        virtual void draw() = 0;
};

class ofxBKUIEventArgs
{
public:
    enum EVENT_TYPE{SELECTED,
                    DESELECTED,
                    SELECTION_CHANGED,
                    RESIZED,
                    VALUE_CHANGED,
                    CLICKED,
                    PROCEEDED};
	ofxBKUIEventHandlers * target;
	EVENT_TYPE eventType;
};
