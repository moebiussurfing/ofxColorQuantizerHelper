#pragma once

/*

	TODO:
	
	add threading for loading file picture.
	
	small library window breaks grid layout 
		only showing first 4 picts..

*/


#include "ofMain.h"

//--

// OPTIONAL
//#define USE_OFX_GUI__QUANTIZER
// comment to disable internal gui
#define USE_IM_GUI__QUANTIZER
#define USE_IM_GUI__QUANTIZER_INTERNAL
//TODO: to make grid thumbs browser we need the ImGui instance internal.. 

//--

//#define PANEL_WIDGETS_WIDTH 250
//#define PANEL_WIDGETS_HEIGHT 100
//#define BUTTON_BIG_HEIGHT 50
//#define NUM_QUANTIZER_COLORS_PER_ROW 4

#ifdef USE_OFX_GUI__QUANTIZER
#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"
#endif

#ifdef USE_IM_GUI__QUANTIZER
#include "ofxSurfingImGui.h"
#endif

#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"
#include "ofxSurfingHelpers.h"
#include "ofxThreadedImageLoader.h"

//TODO:
//#include "imgInspect.h"

typedef struct
{
	int pos;
	ofColor color;
} colorMapping;

//--

class ofxColorQuantizerHelper
{
	//-

public:
	ofxColorQuantizerHelper();
	~ofxColorQuantizerHelper();

	void setup();
	void draw();
	void exit();

	void drawImGuiWindows() { draw_ImGuiWidgets(); };


private:
	void draw_ImGuiWidgets();
	void draw_ImGuiPicture();
	void draw_ImGuiLibrary();

private:
	ofTexture tex;
	ofFbo fbo;
	void refreshImageTexture();

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	//TODO:
	// should remove some parameters..
	// grid picker
	ofParameter<int> sizeLibColBox;
	vector<ofTexture> textureSource;
	vector<GLuint> textureSourceID;
	ofParameter<int> indexBrowser;
	ofParameter<int> thumbsSize;
	ofParameter<int> thumbsSpacing;
	ofParameter<int> thumbsBorder;
	ofParameterGroup params_Thumbs;

	float __widthPicts;
	//ofParameter<std::string> nameMat;
	//int dirLoadIndex;

	//--

private:
	ofxSurfingGui* ui;
public:
	void setUiPtr(ofxSurfingGui* ui_) {
		ui = ui_;
	};
	ofxSurfingGui* getUiPtr() {
		return ui;
	};

	//--

public:
	ofParameter<bool> bGui_Library;

private:
	ofParameter<bool> bAutoResizeLib;
	//ofParameter<bool> bResponsive;

#endif

	//--

private:
	bool bUseNativeWidgets = true;

public:
	void setEnableNativeWidgets(bool b) {
		bUseNativeWidgets = b;
	}

	//--

public:
	int getPaletteSize() {
		return palette.size();
	}
	std::string getImagePath() {
		return imageName_path;
	}

	void loadNext();
	void loadPrev();
	void randomPalette();

private:
	void doReset() {
		thumbsSize = 200;
		thumbsSpacing = 2;
		thumbsBorder = 2;
	};

private:
	// easy callback
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

	//private:
	//ofParameter<bool> bGui_Info;

private:
	//public:
	std::string infoHelp;//key commands

	//--

public:
	ofParameter<bool> bGui;

public:
	void setVisible(bool b)
	{
		bGui = b;
	}
	void setToggleVisible()
	{
		bGui = !bGui;
	}
	//#endif

	//build palette from already quantized and sorted colors
	//void rebuildMap();

	ofParameterGroup getParameters() {
		return parameters;
	}

	//--

	// API

private:
	void refresh_Files();
	ofTrueTypeFont font;
	ofTrueTypeFont font2;

	//public:
private:
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

	//private:
	//ofParameter<bool> bottomMode;
	//public:
	//	void setBottomMode(bool b)
	//	{
	//		bottomMode = b;
	//	}

private:
	void setEnableVisibleHelpInfo(bool b)
	{
		bGui_Help = b;
		bGui_WidgetInfo = b;
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

public:

	//TODO:
	vector<ofColor> getPalette(bool sorted = true) const
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

	// Pointers back to 'communicate externally'
	void setPalette_BACK_Name(std::string& n);
	void setPalette_BACK(vector<ofColor>& p);
	void setPalette_BACK_RefreshPalette(bool& b);
	void setColor_BACK(ofColor& c);
	void setColor_BACK_Refresh(bool& b);

private:
	// Build palette from already quantized and sorted colors
	void rebuildMap();

private:

	//--

	// Pointers back to 'communicate externally'
	ofColor* myColor_BACK = nullptr;
	vector<ofColor>* myPalette_BACK = nullptr;
	std::string* myPalette_Name_BACK = nullptr;
	bool* bUpdated_Palette_BACK = nullptr;
	bool* bUpdated_Color_BACK = nullptr;

	void draw_Palette_Preview();

	bool bUseBorder = true;

	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 size = glm::vec2(1440, 900);
	//bool auto_resize = false;

	std::string pathFolder = "images/";
	bool isLoadedImage = false;
	bool isVisible_gui = true;

	ofxColorQuantizer colorQuantizer;
	void quantizeImage(std::string imageName, int amountColors);

public:
	int getAountFiles() {
		return dir.size();
	}

	ofImage& getImage() {
		return image;
	}

private:
	ofImage image;
	ofImage imageCopy;
	std::string imageName_path;
	std::string imageName;
	vector<std::string> imageNames;

	struct weightedColor
	{
		ofColor color;
		float weight;
		float distance;
	};

	vector<weightedColor> sortedColors;

	struct by_distance
	{
		bool operator()(weightedColor const& a, weightedColor const& b)
		{
			return a.distance > b.distance;
		}
	};

public:
	void build();
	// split from quantizer to avoid reload image

	//--

private:
	void buildFromImageFile(std::string path, int num);
	void buildFromImageUrl(std::string url, int num);

	map<int, ofColor> colorMap;
	vector<colorMapping> colorMapSortable;

	//--

#ifdef USE_OFX_GUI__QUANTIZER
private:
	ofxPanel gui;
#endif

public:
	void setNumColors(int i) {
		amountColors = i;
	}
	ofParameter<int> sortedType;
	ofParameter<int> amountColors;
	ofParameter<std::string> sortedType_name;
	ofParameter<std::string> labelUrlStr;
	ofParameter<bool> bReBuild;
	ofParameter<int> currentImage;
	ofParameter<bool> bKeys;

private:
	ofParameterGroup parameters;
	ofParameterGroup parameters_Advanced;

	void Changed_parameters(ofAbstractParameter& e);

	// Main palette
	vector<ofColor> palette;

	float boxBgSize;
	int boxSize_h;
	int boxPad;
	float boxW;
	int wPal;

	void kMeansTest();

	// Drag and drop images
	vector<ofImage> draggedImages;
	ofPoint dragPt;
	void draw_imageDragged();
	ofParameter<std::string> currentImage_name;
	//std::string pathFolerDrag = "";
	std::string pathFolerDrag = "images/";
	ofDirectory dir;

public:
	ofParameter<bool> bGui_Help;
	ofParameter<bool> bGui_WidgetInfo;
	ofParameter<bool> bGui_Picture;
	ofParameter<bool> bGuiLinked;

public:
	void dragEvent(ofDragInfo& eventArgs);

private:
	void addImage(std::string path);
	void refresh_FilesSorting(std::string name);
	// After saving new preset, refresh files and select the just saved preset

	void addDragListeners();
	void removeDragListeners();

private:
	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	void mouseDragged(ofMouseEventArgs& eventArgs);
	void mousePressed(ofMouseEventArgs& eventArgs);
	void mouseReleased(ofMouseEventArgs& eventArgs);
	void addMouseListeners();
	void removeMouseListeners();

	bool isActive = true;

	// App settings xml
	void saveAppSettings(ofParameterGroup& g, std::string path);
	void loadAppSettings(ofParameterGroup& g, std::string path);
	ofParameterGroup params_AppSettings;
	std::string path_AppSettings = "ofxColorQuantizerHelper_Settings.xml";
	std::string path_Global = "ofxColorQuantizerHelper/";

	void setImage();
};
