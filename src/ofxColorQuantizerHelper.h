#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"
#include "ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

typedef struct
{
	int pos;
	ofColor color;
} colorMapping;

class ofxColorQuantizerHelper
{

public:
	int getPaletteSize() {
		return palette.size();
	}
	std::string getImagePath() {
		return imageName_path;
	}
	void loadNext();
	void loadPrev();
	//easy callback
	bool bUpdate = false;
	bool isUpdated() {
		if (bUpdate) {
			bUpdate = false;
			return true;
		}
		else {
			return false;
		}
	}
	
	ofParameter<bool> bInfo;
	ofParameter<bool> bVisible;

	void setVisible(bool b)
	{
		bVisible = b;
	}
	void setToggleVisible()
	{
		bVisible = !bVisible;
	}
	
	//build palette from already quantized and sorted colors
	//void rebuildMap();
	ofParameterGroup getParameters() {
		return parameters;
	}

	//-

public:
	ofxColorQuantizerHelper();
	~ofxColorQuantizerHelper();

	void setup();
	//void update();
	void draw();
	void exit();

	//--

	// API
private:
	void filesRefresh();
	ofTrueTypeFont font;

public:
	bool isActivated() {
		return isActive;
	}
	void setToggleActive()
	{
		setActive(!isActivated());
	}
	void setActive(bool b)
	{
		isActive = b;
		//isVisible_gui = b;
		if (b)
		{
			addKeysListeners();
			addMouseListeners();
			addDragListeners();
		}
		else
		{
			removeKeysListeners();
			removeMouseListeners();
			removeDragListeners();
		}
	}

private:

	ofParameter<bool> bottomMode;

public:

	void setBottomMode(bool b)
	{
		bottomMode = b;
	}

	void setMinimal(bool b)
	{
		ENABLE_minimal = b;
	}

	glm::vec2 getPosition()
	{
		return position;
	}
	glm::vec2 getSize()
	{
		return size;
	}
	void setPosition(glm::vec2 p)
	{
		position = p;
	}
	void setSize(glm::vec2 s)
	{
		size = s;
	}

	vector<ofColor> getPalette()
	{
		return palette;
	}

	//--

	// pointers back to 'communicate externally'
	void setPalette_BACK_Name(string &n);
	void setPalette_BACK(vector<ofColor> &p);
	void setPalette_BACK_RefreshPalette(bool &b);
	void setColor_BACK(ofColor &c);
	void setColor_BACK_Refresh(bool &b);

private:
	//build palette from already quantized and sorted colors
	void rebuildMap();

private:

	//--

	// pointers back to 'communicate externally'
	ofColor *myColor_BACK;
	vector<ofColor> *myPalette_BACK;
	string *myPalette_Name_BACK;
	bool *bUpdated_Palette_BACK;
	bool *bUpdated_Color_BACK;

	void draw_Palette_Preview();

	bool bUseBorder = true;

	glm::vec2 position = glm::vec2(10, 10);
	glm::vec2 size = glm::vec2(1440, 900);

	string pathFolder = "images/";
	bool isLoadedImage = false;
	bool isVisible_gui = true;

	ofxColorQuantizer colorQuantizer;
	void quantizeImage(string imageName, int numColors);

	ofImage &getImage() {
		return image;
	}

	ofImage image;
	ofImage imageCopy;
	string imageName_path;
	string imageName;

	struct weightedColor
	{
		ofColor color;
		float weight;
		float distance;
	};

	vector<weightedColor> sortedColors;

	struct by_distance
	{
		bool operator()(weightedColor const &a, weightedColor const &b)
		{
			return a.distance > b.distance;
		}
	};

	void build();//split from quantizer to avoid reload image

	//-

	void buildFromImageFile(string path, int num);
	void buildFromImageUrl(string url, int num);

	map<int, ofColor> colorMap;
	vector<colorMapping> colorMapSortable;

	//-

	ofxPanel gui;
	ofParameter<int> sortedType;
	ofParameter<int> numColors;
	ofParameter<string> labelStr;
	ofParameter<string> labelUrlStr;
	ofParameter<bool> bReBuild;
	ofParameterGroup parameters;
	void Changed_parameters(ofAbstractParameter &e);

	// main palette
	vector<ofColor> palette;

	float boxSize;
	int boxSize_h;
	int boxPad;
	float boxW;
	int wPal;

	void kMeansTest();

	// drag and drop images
	vector<ofImage> draggedImages;
	ofPoint dragPt;
	void draw_imageDragged();
	ofParameter<int> currentImage;
	ofParameter<string> currentImage_name;
	//string pathFolerDrag = "";
	string pathFolerDrag = "images/";
	ofDirectory dir;

	ofParameter<bool> ENABLE_minimal{ "MINIMAL", false };

	void dragEvent(ofDragInfo &eventArgs);
	void addDragListeners();
	void removeDragListeners();

	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void addMouseListeners();
	void removeMouseListeners();

	bool isActive = true;

	// APP SETTINGS XML
	void XML_save_AppSettings(ofParameterGroup &g, string path);
	void XML_load_AppSettings(ofParameterGroup &g, string path);
	ofParameterGroup XML_params;
	string XML_path = "ofxColorQuantizerHelper/ofxColorQuantizerHelper_Settings.xml";
};
