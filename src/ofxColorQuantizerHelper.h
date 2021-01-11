#pragma once
#include "ofMain.h"

//-

// OPTIONAL
//#define USE_OFX_GUI__QUANTIZER // comment to disable internal gui
#define USE_IM_GUI__QUANTIZER
//#define USE_IM_GUI__QUANTIZER_INTERNAL // TODO: to make grid thumbs browser we need the ImGui instance internal.. 

//-

#ifdef USE_OFX_GUI__QUANTIZER
#include "ofxGui.h"
#endif

#ifdef USE_IM_GUI__QUANTIZER
#include "ofxImGui.h"
#include "ofxSurfing_ImGui.h"
#endif

#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

typedef struct
{
	int pos;
	ofColor color;
} colorMapping;

class ofxColorQuantizerHelper
{
	//-

public:
	void gui_Quantizer();
	ofTexture tex;
	ofFbo fbo;
	void quantizerRefreshImage();
	ofxImGui::Settings mainSettings = ofxImGui::Settings();
#define BUTTON_BIG_HEIGHT 50
#define NUM_QUANTIZER_COLORS_PER_ROW 4
	

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	//TODO:
	//grid picker
	ofParameter<int> sizeLibColBox;
	vector<ofTexture> textureSource;
	vector<GLuint> textureSourceID;
	ofParameter<std::string> nameMat;
	ofParameter<int> indexBrowser;
	int dirLoadIndex;
#endif

	//-

public:
	int getPaletteSize() {
		return palette.size();
	}
	std::string getImagePath() {
		return imageName_path;
	}

	void loadNext();
	void loadPrev();

private:
	//easy callback
	bool bUpdate = false;

public:
	bool isUpdated() {
		if (bUpdate) {
			bUpdate = false;
			return true;
		}
		else {
			return false;
		}
	}

private:
	ofParameter<bool> bInfo;
	
#ifdef USE_OFX_GUI__QUANTIZER
	ofParameter<bool> bGuiVisible;
#endif

public:
	std::string infoHelp;//key commands

	
#ifdef USE_OFX_GUI__QUANTIZER
public:
	void setVisible(bool b)
	{
		bGuiVisible = b;
	}
	void setToggleVisible()
	{
		bGuiVisible = !bGuiVisible;
	}
#endif

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

	void setEnableVisibleHelpInfo(bool b)
	{
		ENABLE_HelpInfo = b;
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

	//--

	//TODO:
	vector<ofColor> getPalette(bool sorted = true)
	{
		if (!sorted) return palette;

		else
		{
			vector<ofColor> _palette;
			for (int col = 0; col < palette.size(); col++)
			{
				_palette.push_back(colorMapSortable[col].color);
			}

			return _palette;
		}
	}

	//--

	// pointers back to 'communicate externally'
	void setPalette_BACK_Name(std::string &n);
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
	std::string *myPalette_Name_BACK;
	bool *bUpdated_Palette_BACK;
	bool *bUpdated_Color_BACK;

	void draw_Palette_Preview();

	bool bUseBorder = true;

	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 size = glm::vec2(1440, 900);

	std::string pathFolder = "images/";
	bool isLoadedImage = false;
	bool isVisible_gui = true;

	ofxColorQuantizer colorQuantizer;
	void quantizeImage(std::string imageName, int numColors);

public:
	int getAountFiles() {
		return dir.size();
	}

	ofImage &getImage() {
		return image;
	}

private:
	ofImage image;
	ofImage imageCopy;
	std::string imageName_path;
	std::string imageName;

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

	void buildFromImageFile(std::string path, int num);
	void buildFromImageUrl(std::string url, int num);

	map<int, ofColor> colorMap;
	vector<colorMapping> colorMapSortable;

	//-

#ifdef USE_OFX_GUI__QUANTIZER
private:
	ofxPanel gui;
#endif

public:
	ofParameter<int> sortedType;
	ofParameter<int> numColors;
	ofParameter<std::string> sortedType_name;
	ofParameter<std::string> labelUrlStr;
	ofParameter<bool> bReBuild;
	ofParameter<int> currentImage;
	ofParameter<bool> ENABLE_Keys;

private:
	ofParameterGroup parameters;
	ofParameterGroup parameters_Advanced;
	
	void Changed_parameters(ofAbstractParameter &e);

	// main palette
	vector<ofColor> palette;

	float boxBgSize;
	int boxSize_h;
	int boxPad;
	float boxW;
	int wPal;

	void kMeansTest();

	// drag and drop images
	vector<ofImage> draggedImages;
	ofPoint dragPt;
	void draw_imageDragged();
	ofParameter<std::string> currentImage_name;
	//std::string pathFolerDrag = "";
	std::string pathFolerDrag = "images/";
	ofDirectory dir;

public:
	ofParameter<bool> ENABLE_HelpInfo;// { "HELP INFO", false };

public:
	void dragEvent(ofDragInfo &eventArgs);
	void addDragListeners();
	void removeDragListeners();

private:
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
	void XML_save_AppSettings(ofParameterGroup &g, std::string path);
	void XML_load_AppSettings(ofParameterGroup &g, std::string path);
	ofParameterGroup XML_params;
	std::string XML_path = "ofxColorQuantizerHelper/ofxColorQuantizerHelper_Settings.xml";
};
