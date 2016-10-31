#include "ofxBKTTFplus.h"
#include "ofxBKTTFplus.h"
//--------------------------

#include "ft2build.h"

#ifdef TARGET_LINUX
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
#include <fontconfig/fontconfig.h>
#else
#include "freetype2/freetype/freetype.h"
#include "freetype2/freetype/ftglyph.h"
#include "freetype2/freetype/ftoutln.h"
#include "freetype2/freetype/fttrigon.h"
#endif

#include <algorithm>

#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/Latin9Encoding.h"
#include "ofTrueTypeFont.cpp"
#include <iostream>


//--------------------------------------------------------
void ofxBKTTFplus::setGlobalDpi(int newDpi){
	ttfGlobalDpi = newDpi;
}



bool ofxBKTTFplus::initLibraries(){
	if(!librariesInitialized){
	    FT_Error err;
	    err = FT_Init_FreeType( &library );

	    if (err){
			ofLogError("ofxBKTTFplus") << "loadFont(): couldn't initialize Freetype lib: FT_Error " << err;
			return false;
		}
#ifdef TARGET_LINUX
		FcBool result = FcInit();
		if(!result){
			return false;
		}
#endif
#ifdef TARGET_WIN32
		initWindows();
#endif
		librariesInitialized = true;
	}
    return true;
}

void ofxBKTTFplus::finishLibraries(){
	if(librariesInitialized){
#ifdef TARGET_LINUX
		//FcFini();
#endif
		FT_Done_FreeType(library);
	}
}


//------------------------------------------------------------------
ofxBKTTFplus::ofxBKTTFplus(){
	bLoadedOk		= false;
	bMakeContours	= false;
	#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
		all_fonts().insert(this);
	#endif
	//cps				= NULL;
	letterSpacing = 1;
	spaceSize = 1;

	// 3 pixel border around the glyph
	// We show 2 pixels of this, so that blending looks good.
	// 1 pixels is hidden because we don't want to see the real edge of the texture

	border			= 3;
	stringQuads.setMode(OF_PRIMITIVE_TRIANGLES);
	binded = false;

	dpi = ttfGlobalDpi;
	fontSize = 0;
	blend_src = GL_SRC_ALPHA;
	blend_dst = GL_ONE_MINUS_SRC_ALPHA;
	simplifyAmt = 0.3;
	nCharacters = 0;
	blend_enabled = true;
	lineHeight = 0;
	bAntiAliased = true;
	texture_2d_enabled = true;
	encoding = OF_ENCODING_UTF8;
	bFullCharacterSet = true;
}

//------------------------------------------------------------------
ofxBKTTFplus::~ofxBKTTFplus(){

	if (bLoadedOk){
		unloadTextures();
	}

	#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
		all_fonts().erase(this);
	#endif
}

void ofxBKTTFplus::unloadTextures(){
	if(!bLoadedOk) return;

	texAtlas.clear();
	bLoadedOk = false;
}

void ofxBKTTFplus::reloadTextures(){
	loadFont(filename, fontSize, bAntiAliased, bFullCharacterSet, bMakeContours, simplifyAmt, dpi);
}

//-----------------------------------------------------------
bool ofxBKTTFplus::loadFont(string _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi) {

	initLibraries();

	//------------------------------------------------
	if (bLoadedOk == true){

		// we've already been loaded, try to clean up :
		unloadTextures();
	}
	//------------------------------------------------

	if( _dpi == 0 ){
		_dpi = ttfGlobalDpi;
	}



	bLoadedOk 			= false;
	bAntiAliased 		= _bAntiAliased;
	bFullCharacterSet 	= _bFullCharacterSet;
	fontSize			= _fontSize;
	bMakeContours 		= _makeContours;
	simplifyAmt			= _simplifyAmt;
	dpi 				= _dpi;

	//--------------- load the library and typeface


	FT_Face face;
	if(!loadFontFace(_filename,_fontSize,face,filename)){
        return false;
	}


	FT_Set_Char_Size( face, fontSize << 6, fontSize << 6, dpi, dpi);
	lineHeight = fontSize * 1.43f;

	//------------------------------------------------------
	//kerning would be great to support:
	//ofLogNotice("ofxBKTTFplus") << "FT_HAS_KERNING ? " <<  FT_HAS_KERNING(face);
	//------------------------------------------------------

	nCharacters = (bFullCharacterSet ? 256 : 128) - NUM_CHARACTER_TO_START;

	//--------------- initialize character info and textures
	cps.resize(nCharacters);

	if(bMakeContours){
		charOutlines.clear();
		charOutlines.assign(nCharacters, ofTTFCharacter());
		charOutlinesNonVFlipped.assign(nCharacters, ofTTFCharacter());
	}

	vector<ofPixels> expanded_data(nCharacters);

	long areaSum=0;
	FT_Error err;


	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){

		//------------------------------------------ anti aliased or not:
		int glyph = (unsigned char)(i+NUM_CHARACTER_TO_START);
		if (glyph == 0xA4) glyph = 0x20AC; // hack to load the euro sign, all codes in 8859-15 match with utf-32 except for this one
		err = FT_Load_Glyph( face, FT_Get_Char_Index( face, glyph ), FT_LOAD_DEFAULT );
        if(err){
			ofLogError("ofxBKTTFplus") << "loadFont(): FT_Load_Glyph failed for char " << i << ": FT_Error " << err;

		}

		if (bAntiAliased == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

		//------------------------------------------
		FT_Bitmap& bitmap= face->glyph->bitmap;


		// prepare the texture:
		/*int width  = ofNextPow2( bitmap.width + border*2 );
		int height = ofNextPow2( bitmap.rows  + border*2 );


		// ------------------------- this is fixing a bug with small type
		// ------------------------- appearantly, opengl has trouble with
		// ------------------------- width or height textures of 1, so we
		// ------------------------- we just set it to 2...
		if (width == 1) width = 2;
		if (height == 1) height = 2;*/


		if(bMakeContours){
			if(printVectorInfo){
				ofLogNotice("ofxBKTTFplus") <<  "character " << char(i+NUM_CHARACTER_TO_START);
			}

			//int character = i + NUM_CHARACTER_TO_START;
			charOutlines[i] = makeContoursForCharacter( face );

			charOutlinesNonVFlipped[i] = charOutlines[i];
			charOutlinesNonVFlipped[i].translate(ofVec3f(0,cps[i].height));
			charOutlinesNonVFlipped[i].scale(1,-1);


			if(simplifyAmt>0)
				charOutlines[i].simplify(simplifyAmt);
			charOutlines[i].getTessellation();

			if(simplifyAmt>0)
				charOutlinesNonVFlipped[i].simplify(simplifyAmt);
			charOutlinesNonVFlipped[i].getTessellation();
		}


		// -------------------------
		// info about the character:
		cps[i].character		= i;
		cps[i].height 			= face->glyph->bitmap_top;
		cps[i].width 			= face->glyph->bitmap.width;
		cps[i].setWidth 		= face->glyph->advance.x >> 6;
		cps[i].topExtent 		= face->glyph->bitmap.rows;
		cps[i].leftExtent		= face->glyph->bitmap_left;

		int width  = cps[i].width;
		int height = bitmap.rows;


		cps[i].tW				= width;
		cps[i].tH				= height;



		GLint fheight	= cps[i].height;
		GLint bwidth	= cps[i].width;
		GLint top		= cps[i].topExtent - cps[i].height;
		GLint lextent	= cps[i].leftExtent;

		GLfloat	corr, stretch;

		//this accounts for the fact that we are showing 2*visibleBorder extra pixels
		//so we make the size of each char that many pixels bigger
		stretch = 0;//(float)(visibleBorder * 2);

		corr	= (float)(( (fontSize - fheight) + top) - fontSize);

		cps[i].x1		= lextent + bwidth + stretch;
		cps[i].y1		= fheight + corr + stretch;
		cps[i].x2		= (float) lextent;
		cps[i].y2		= -top + corr;


		// Allocate Memory For The Texture Data.
		expanded_data[i].allocate(width, height, 2);
		//-------------------------------- clear data:
		expanded_data[i].set(0,255); // every luminance pixel = 255
		expanded_data[i].set(1,0);


		if (bAntiAliased == true){
			ofPixels bitmapPixels;
			bitmapPixels.setFromExternalPixels(bitmap.buffer,bitmap.width,bitmap.rows,1);
			expanded_data[i].setChannel(1,bitmapPixels);
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(int j=0; j <bitmap.rows;j++) {
				unsigned char b=0;
				unsigned char *bptr =  src;
				for(int k=0; k < bitmap.width ; k++){
					expanded_data[i][2*(k+j*width)] = 255;

					if (k%8==0){
						b = (*bptr++);
					}

					expanded_data[i][2*(k+j*width) + 1] = b&0x80 ? 255 : 0;
					b <<= 1;
				}
				src += bitmap.pitch;
			}
			//-----------------------------------
		}

		areaSum += (cps[i].width+border*2)*(cps[i].height+border*2);
	}

	vector<charProps> sortedCopy = cps;
	sort(sortedCopy.begin(),sortedCopy.end(),&compare_cps);

	// pack in a texture, algorithm to calculate min w/h from
	// http://upcommons.upc.edu/pfc/bitstream/2099.1/7720/1/TesiMasterJonas.pdf
	//ofLogNotice("ofxBKTTFplus") << "loadFont(): areaSum: " << areaSum

	bool packed = false;
	float alpha = logf(areaSum)*1.44269;

	int w;
	int h;
	while(!packed){
		w = pow(2,floor((alpha/2.f) + 0.5)); // there doesn't seem to be a round in cmath for windows.
		//w = pow(2,round(alpha/2.f));
		h = w;//pow(2,round(alpha - round(alpha/2.f)));
		int x=0;
		int y=0;
		int maxRowHeight = sortedCopy[0].tH + border*2;
		for(int i=0;i<(int)cps.size();i++){
			if(x+sortedCopy[i].tW + border*2>w){
				x = 0;
				y += maxRowHeight;
				maxRowHeight = sortedCopy[i].tH + border*2;
				if(y + maxRowHeight > h){
					alpha++;
					break;
				}
			}
			x+= sortedCopy[i].tW + border*2;
			if(i==(int)cps.size()-1) packed = true;
		}

	}



	ofPixels atlasPixelsLuminanceAlpha;
	atlasPixelsLuminanceAlpha.allocate(w,h,2);
	atlasPixelsLuminanceAlpha.set(0,255);
	atlasPixelsLuminanceAlpha.set(1,0);


	int x=0;
	int y=0;
	int maxRowHeight = sortedCopy[0].tH + border*2;
	for(int i=0;i<(int)cps.size();i++){
		ofPixels & charPixels = expanded_data[sortedCopy[i].character];

		if(x+sortedCopy[i].tW + border*2>w){
			x = 0;
			y += maxRowHeight;
			maxRowHeight = sortedCopy[i].tH + border*2;
		}

		cps[sortedCopy[i].character].t2		= float(x + border)/float(w);
		cps[sortedCopy[i].character].v2		= float(y + border)/float(h);
		cps[sortedCopy[i].character].t1		= float(cps[sortedCopy[i].character].tW + x + border)/float(w);
		cps[sortedCopy[i].character].v1		= float(cps[sortedCopy[i].character].tH + y + border)/float(h);
		charPixels.pasteInto(atlasPixelsLuminanceAlpha,x+border,y+border);
		x+= sortedCopy[i].tW + border*2;
	}

	ofPixels atlasPixels;
	atlasPixels.allocate(atlasPixelsLuminanceAlpha.getWidth(),atlasPixelsLuminanceAlpha.getHeight(),4);
	atlasPixels.setChannel(0,atlasPixelsLuminanceAlpha.getChannel(0));
	atlasPixels.setChannel(1,atlasPixelsLuminanceAlpha.getChannel(0));
	atlasPixels.setChannel(2,atlasPixelsLuminanceAlpha.getChannel(0));
	atlasPixels.setChannel(3,atlasPixelsLuminanceAlpha.getChannel(1));
	texAtlas.allocate(atlasPixels,false);

	if(bAntiAliased && fontSize>20){
		texAtlas.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);
	}else{
		texAtlas.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	}
	texAtlas.loadData(atlasPixels);

     int cy = (int)'H' - NUM_CHARACTER_TO_START;
	ascenderHeight = cps[cy].height;

	// ------------- close the library and typeface
	FT_Done_Face(face);
  	bLoadedOk = true;
	return true;
}

ofTextEncoding ofxBKTTFplus::getEncoding() const {
	return encoding;
}

void ofxBKTTFplus::setEncoding(ofTextEncoding _encoding) {
	encoding = _encoding;
}

//-----------------------------------------------------------
bool ofxBKTTFplus::isLoaded() {
	return bLoadedOk;
}

//-----------------------------------------------------------
bool ofxBKTTFplus::isAntiAliased() {
	return bAntiAliased;
}

//-----------------------------------------------------------
bool ofxBKTTFplus::hasFullCharacterSet() {
	return bFullCharacterSet;
}

//-----------------------------------------------------------
int ofxBKTTFplus::getSize() {
	return fontSize;
}

//-----------------------------------------------------------
void ofxBKTTFplus::setLineHeight(float _newLineHeight) {
	lineHeight = _newLineHeight;
}

//-----------------------------------------------------------
float ofxBKTTFplus::getLineHeight(){
	return lineHeight;
}

//-----------------------------------------------------------
void ofxBKTTFplus::setLetterSpacing(float _newletterSpacing) {
	letterSpacing = _newletterSpacing;
}

//-----------------------------------------------------------
float ofxBKTTFplus::getLetterSpacing(){
	return letterSpacing;
}

//-----------------------------------------------------------
void ofxBKTTFplus::setSpaceSize(float _newspaceSize) {
	spaceSize = _newspaceSize;
}

//-----------------------------------------------------------
float ofxBKTTFplus::getSpaceSize(){
	return spaceSize;
}

//------------------------------------------------------------------
ofTTFCharacter ofxBKTTFplus::getCharacterAsPoints(int character, bool vflip){
	if( bMakeContours == false ){
		ofLogError("ofxTrueTypeFont") << "getCharacterAsPoints(): contours not created, call loadFont() with makeContours set to true";
		return ofTTFCharacter();
	}
    if (character - NUM_CHARACTER_TO_START >= nCharacters || character < NUM_CHARACTER_TO_START){
        ofLogError("ofxTrueTypeFont") << "getCharacterAsPoint(): char " << character + NUM_CHARACTER_TO_START
		<< " not allocated: line " << __LINE__ << " in " << __FILE__;

        return ofTTFCharacter();
    }

    if(vflip){
    	return charOutlines[character - NUM_CHARACTER_TO_START];
    }else{
    	return charOutlinesNonVFlipped[character - NUM_CHARACTER_TO_START];
    }
}

//-----------------------------------------------------------
void ofxBKTTFplus::drawChar(int c, float x, float y) {

	if (c >= nCharacters){
		//ofLogError("ofxBKTTFplus") << "drawChar(): char " << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}

	GLfloat	x1, y1, x2, y2;
	GLfloat t1, v1, t2, v2;
	t2		= cps[c].t2;
	v2		= cps[c].v2;
	t1		= cps[c].t1;
	v1		= cps[c].v1;

	x1		= cps[c].x1+x;
	y1		= cps[c].y1;
	x2		= cps[c].x2+x;
	y2		= cps[c].y2;

	int firstIndex = stringQuads.getVertices().size();

	if(!ofIsVFlipped()){
       y1 *= -1;
       y2 *= -1;
	}

    y1 += y;
    y2 += y;

	stringQuads.addVertex(ofVec3f(x1,y1));
	stringQuads.addVertex(ofVec3f(x2,y1));
	stringQuads.addVertex(ofVec3f(x2,y2));
	stringQuads.addVertex(ofVec3f(x1,y2));

	stringQuads.addTexCoord(ofVec2f(t1,v1));
	stringQuads.addTexCoord(ofVec2f(t2,v1));
	stringQuads.addTexCoord(ofVec2f(t2,v2));
	stringQuads.addTexCoord(ofVec2f(t1,v2));

	stringQuads.addIndex(firstIndex);
	stringQuads.addIndex(firstIndex+1);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+3);
	stringQuads.addIndex(firstIndex);
}

//-----------------------------------------------------------
vector<ofTTFCharacter> ofxBKTTFplus::getStringAsPoints(string str, bool vflip){
	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(str,o);
		str=o;
	}

	vector<ofTTFCharacter> shapes;

	if (!bLoadedOk){
		ofLogError("ofxTrueTypeFont") << "getStringAsPoints(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return shapes;
	};

	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;
	int newLineDirection		= 1;

	if(vflip){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}


	int len = (int)str.length();

	while(index < len){
		int cy = (unsigned char)str[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
			if (str[index] == '\n') {
				Y += lineHeight*newLineDirection;
				X = 0 ; //reset X Pos back to zero
			}else if (str[index] == ' ') {
				int cy = (int)'p' - NUM_CHARACTER_TO_START;
				X += cps[cy].setWidth * letterSpacing * spaceSize;
			} else if(cy > -1){
				shapes.push_back(getCharacterAsPoints((unsigned char)str[index],vflip));
				shapes.back().translate(ofPoint(X,Y));

				X += cps[cy].setWidth * letterSpacing;
			}
		}
		index++;
	}
	return shapes;

}

//-----------------------------------------------------------
void ofxBKTTFplus::drawCharAsShape(int c, float x, float y) {
	if (c - NUM_CHARACTER_TO_START >= nCharacters || c < NUM_CHARACTER_TO_START){
		//ofLogError("ofxBKTTFplus") << "drawCharAsShape(): char " << << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}
	//-----------------------return descenderHeight;

	if(ofIsVFlipped()){
		ofTTFCharacter & charRef = charOutlines[c - NUM_CHARACTER_TO_START];
		charRef.setFilled(ofGetStyle().bFill);
		charRef.draw(x,y);
	}else{
		ofTTFCharacter & charRef = charOutlinesNonVFlipped[c - NUM_CHARACTER_TO_START];
		charRef.setFilled(ofGetStyle().bFill);
		charRef.draw(x,y);
	}
}

//-----------------------------------------------------------
float ofxBKTTFplus::stringWidth(string c) {
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.width;
}


ofRectangle ofxBKTTFplus::getStringBoundingBox(string c, float x, float y){

    ofRectangle myRect;

    if (!bLoadedOk){
    	ofLogError("ofxBKTTFplus") << "getStringBoundingBox(): font not allocated";
    	return myRect;
    }

	GLint		index	= 0;
	GLfloat		xoffset	= 0;
	GLfloat		yoffset	= 0;
    int         len     = (int)c.length();
    float       minx    = -1;
    float       miny    = -1;
    float       maxx    = -1;
    float       maxy    = -1;

    if ( len < 1 || cps.empty() ){
        myRect.x        = 0;
        myRect.y        = 0;
        myRect.width    = 0;
        myRect.height   = 0;
        return myRect;
    }

    bool bFirstCharacter = true;
	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
 	    if (cy < nCharacters){ 			// full char set or not?
	       if (c[index] == '\n') {
				yoffset += lineHeight;
				xoffset = 0 ; //reset X Pos back to zero
	      } else if (c[index] == ' ') {
	     		int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 xoffset += cps[cy].setWidth * letterSpacing * spaceSize;
				 // zach - this is a bug to fix -- for now, we don't currently deal with ' ' in calculating string bounding box
		  } else if(cy > -1){
                GLint height	= cps[cy].height;
            	GLint bwidth	= cps[cy].width * letterSpacing;
            	GLint top		= cps[cy].topExtent - cps[cy].height;
            	GLint lextent	= cps[cy].leftExtent;
            	float	x1, y1, x2, y2, corr, stretch;
            	stretch = 0;//(float)visibleBorder * 2;
				corr = (float)(((fontSize - height) + top) - fontSize);
				x1		= (x + xoffset + lextent + bwidth + stretch);
            	y1		= (y + yoffset + height + corr + stretch);
            	x2		= (x + xoffset + lextent);
            	y2		= (y + yoffset + -top + corr);
				xoffset += cps[cy].setWidth * letterSpacing;
				if (bFirstCharacter == true){
                    minx = x2;
                    miny = y2;
                    maxx = x1;
                    maxy = y1;
                    bFirstCharacter = false;
                } else {
                    if (x2 < minx) minx = x2;
                    if (y2 < miny) miny = y2;
                    if (x1 > maxx) maxx = x1;
                    if (y1 > maxy) maxy = y1;
            }
		  }
	  	}
    	index++;
    }

    myRect.x        = minx;
    myRect.y        = miny;
    myRect.width    = maxx-minx;
    myRect.height   = maxy-miny;
    return myRect;
}

//-----------------------------------------------------------
float ofxBKTTFplus::stringHeight(string c) {
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.height;
}

void ofxBKTTFplus::createStringMesh(string c, float x, float y){
	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].setWidth * letterSpacing * spaceSize;
		  } else if(cy > -1){
				drawChar(cy, X, Y);
				X += cps[cy].setWidth * letterSpacing;
		  }
		}
		index++;
	}
}

ofMesh & ofxBKTTFplus::getStringMesh(string c, float x, float y){
	stringQuads.clear();
	createStringMesh(c,x,y);
	return stringQuads;
}

ofTexture & ofxBKTTFplus::getFontTexture(){
	return texAtlas;
}

//=====================================================================
void ofxBKTTFplus::drawString(string c, float x, float y) {

	/*glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 texAtlas.draw(0,0);*/

	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(c,o);
		c=o;
	}

	if (!bLoadedOk){
		ofLogError("ofxBKTTFplus") << "drawString(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	};

	bool alreadyBinded = binded;

	if(!alreadyBinded) bind();
	createStringMesh(c,x,y);
	if(!alreadyBinded) unbind();

}

//-----------------------------------------------------------
void ofxBKTTFplus::bind(){
	if(!binded){
	    // we need transparency to draw text, but we don't know
	    // if that is set up in outside of this function
	    // we "pushAttrib", turn on alpha and "popAttrib"
	    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/pushattrib.html

	    // **** note ****
	    // I have read that pushAttrib() is slow, if used often,
	    // maybe there is a faster way to do this?
	    // ie, check if blending is enabled, etc...
	    // glIsEnabled().... glGet()...
	    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/get.html
	    // **************
		// (a) record the current "alpha state, blend func, etc"

		blend_enabled = glIsEnabled(GL_BLEND);
		texture_2d_enabled = glIsEnabled(GL_TEXTURE_2D);
		glGetIntegerv( GL_BLEND_SRC, &blend_src );
		glGetIntegerv( GL_BLEND_DST, &blend_dst );

	    // (b) enable our regular ALPHA blending!
	    glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		texAtlas.bind();
		stringQuads.clear();
		binded = true;
	}
}

//-----------------------------------------------------------
void ofxBKTTFplus::unbind(){
	if(binded){
		stringQuads.drawFaces();
		texAtlas.unbind();

		if( !blend_enabled )
			glDisable(GL_BLEND);
		if( !texture_2d_enabled )
			glDisable(GL_TEXTURE_2D);
		glBlendFunc( blend_src, blend_dst );

		binded = false;
	}
}

//=====================================================================
void ofxBKTTFplus::drawStringAsShapes(string c, float x, float y) {

    if (!bLoadedOk){
    	ofLogError("ofxBKTTFplus") << "drawStringAsShapes(): font not allocated: line " << __LINE__ << " in " << __FILE__;
    	return;
    };

	//----------------------- error checking
	if (!bMakeContours){
		ofLogError("ofxBKTTFplus") << "drawStringAsShapes(): contours not created for this font, call loadFont() with makeContours set to true";
		return;
	}

	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(c,o);
		c=o;
	}

	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].setWidth * letterSpacing * spaceSize;
				 //glTranslated(cps[cy].width, 0, 0);
		  } else if(cy > -1){
				drawCharAsShape((unsigned char)c[index], X, Y);
				X += cps[cy].setWidth * letterSpacing;
				//glTranslated(cps[cy].setWidth, 0, 0);
		  }
		}
		index++;
	}

}

//-----------------------------------------------------------
int ofxBKTTFplus::getNumCharacters() {
	return nCharacters;
}


void ofxBKTTFplus::createStringMesh(string c, float x, float y, ofRectangle _drawRect){
	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int len = (int)c.length();

    bool b_waitNextLine(false);

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {
                b_waitNextLine = false;
				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero

		  } else if (b_waitNextLine) {

		  } else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].setWidth * letterSpacing * spaceSize;
		  } else if(cy > -1){
                switch (checkBoundaryConditions(X, Y, cps[cy].setWidth, cps[cy].height, _drawRect)) {
                    case LEFT :{//printf("LEFT\n");
                    break;}

                    case TOP :{//printf("TOP\n");
                    break;}

                    case RIGHT :{//printf("RIGHT\n");
                        b_waitNextLine = true;
                    break;}

                    case BOTTOM :{//printf("BOTTOM\n");
                        return;
                    break;}

                    case NONE :{//printf("NONE\n");
                        drawChar(cy, X, Y);
                    break;}
                }
				X += cps[cy].setWidth * letterSpacing;
		  }
		}
		index++;
	}
}


void ofxBKTTFplus::drawString(string c, float x, float y, ofRectangle _drawRect) {

	/*glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 texAtlas.draw(0,0);*/

	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(c,o);
		c=o;
	}

	if (!bLoadedOk){
		ofLogError("ofxBKTTFplus") << "drawString(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	};

	bool alreadyBinded = binded;

	if(!alreadyBinded) bind();
	createStringMesh(c,x,y,_drawRect);
	if(!alreadyBinded) unbind();

}


void ofxBKTTFplus::drawStringAsShapes(string c, float x, float y, ofRectangle _drawRect) {

    if (!bLoadedOk){
    	ofLogError("ofxBKTTFplus") << "drawStringAsShapes(): font not allocated: line " << __LINE__ << " in " << __FILE__;
    	return;
    };

	//----------------------- error checking
	if (!bMakeContours){
		ofLogError("ofxBKTTFplus") << "drawStringAsShapes(): contours not created for this font, call loadFont() with makeContours set to true";
		return;
	}

	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(c,o);
		c=o;
	}

	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int len = (int)c.length();

    bool b_waitNextLine(false);
	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {
                b_waitNextLine = false;

				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero

		  } else if (b_waitNextLine){

		  } else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].setWidth * letterSpacing * spaceSize;
				 //glTranslated(cps[cy].width, 0, 0);
		  } else if(cy > -1){
                switch (checkBoundaryConditions(X, Y, cps[cy].setWidth, cps[cy].height, _drawRect)) {
                    case LEFT :{printf("LEFT\n");
                    break;}

                    case TOP :{
                    break;}

                    case RIGHT :{
                        b_waitNextLine = true;
                    break;}

                    case BOTTOM :{
                        return;
                    break;}

                    case NONE :
                        drawCharAsShape((unsigned char)c[index], X, Y);
                    break;
                }
				X += cps[cy].setWidth * letterSpacing;
				//glTranslated(cps[cy].setWidth, 0, 0);
		  }
		}
		index++;
	}

}


inline ofxBKTTFplus::OVERSTEP_TYPE ofxBKTTFplus::checkBoundaryConditions(float x, float y, float charWidth, float charHeight, ofRectangle _drawRect)
{
    //std::cout << "Rect:" << _drawRect << "" << std::endl;
    //printf("[x,y,w] : [%f,%f,%f]\n",x,y,charWidth);
    if (x                        < _drawRect.getMinX()-2 )   return LEFT;
    if (x + charWidth            > _drawRect.getMaxX()+2 )   return RIGHT;
    //if (y + getAscenderHeight()  > _drawRect.getMinY() )   return TOP;
    //if (y + getDescenderHeight() > _drawRect.getMaxY() )   return BOTTOM;
    //printf("recminY:%f y:%f ascenderHeight:%f\n",_drawRect.getMinY(),y,ascenderHeight);
    if (y - charHeight < _drawRect.getMinY()-2 )   return TOP;
    if (y > _drawRect.getMaxY()+2 )                return BOTTOM;

    return NONE;
}





