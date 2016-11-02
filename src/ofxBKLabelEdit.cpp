// #include "ofxBKLabelEdit.h"
//
// ofxBKLabelEdit::ofxBKLabelEdit()
// {
//
// }
//
// ofxBKLabelEdit::ofxBKLabelEdit(std::string _text, float _x, float _y, float _width, float _height)
// {
//     init(_text, _x, _y, _width, _height);
// }
//
// void ofxBKLabelEdit::init(std::string _text, float _x, float _y, float _width,float _height)
// {
//     ofxBKLabel::init(_text,_x,_y,_width,_height);
//
//     monoLineTextInput.setup();
// 	monoLineTextInput.text = _text;
//     monoLineTextInput.bounds.x      = _x;
//     monoLineTextInput.bounds.y      = _y;
// 	monoLineTextInput.bounds.height = _height;
//     monoLineTextInput.bounds.width  = _width;
//
// }
//
//
// void ofxBKLabelEdit::setText(std::string _text)
// {
//     /*
//     ofxBKLabel::setText(_text);
//     selectedChars.resize(text.length(),false);
//     calculateCharacterCumWidth();
//     */
// }
//
// void ofxBKLabelEdit::setSelectedChar(std::vector<bool> _selectedChars)
// {
//     /*
//     if (_selectedChars.size() != text.length()){
//             std::cout << "return" << std::endl;
//             std::cout << "text.length():       "<< text.length() << std::endl;
//             std::cout << "_selectedChar.size():"<< _selectedChars.size() << std::endl;
//         return;
//     }
//     selectedChars = _selectedChars;
//     */
// }
//
// void ofxBKLabelEdit::draw()
// {
// 	ofxBKUIComponent::draw();
//
//     debugDraw();
// 	monoLineTextInput.draw();
//
// }
//
//
// void ofxBKLabelEdit::textDraw()
// {
// 	ofPushStyle();
//         ofSetColor(textColor);
//         monoLineTextInput.draw();
//         auto pos(getTextStartPosition());
//         font->drawString(text,pos.x,pos.y);//TODO: boundary check in draw string
//     ofPopStyle();
//
// }
//
// void ofxBKLabelEdit::printInfo()
// {
//     ofxBKLabel::printInfo();
//     std::cout << "   ofxBKLabelEdit:"  << "" << std::endl;
// }
