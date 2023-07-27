#pragma once

/*

	TODO:

	add threading for processing (and loading) file picture.

	allow drag URL / Internet browser images.

	draw internal ImGui widget
		with percent of dominant colors.
		copy from native widget

*/


#include "ofMain.h"

//--

// OPTIONAL
// 
//#define USE_OFX_GUI__QUANTIZER
// comment to disable internal ofxGui
#define USE_IM_GUI__QUANTIZER
#define USE_IM_GUI__QUANTIZER_INTERNAL
//TODO: to make grid thumbs browser we need the ImGui instance internal.. 

//--

#ifdef USE_OFX_GUI__QUANTIZER
#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"
#endif

#ifdef USE_IM_GUI__QUANTIZER
#include "ofxSurfingImGui.h"
#endif

//--

#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"
#include "ofxSurfingHelpers.h"

//--

#include "surfingImageInspect.h"

//--

typedef struct
{
	int pos;
	ofColor color;
} colorMapping;

//--

class ofxColorQuantizerHelper
{
	//--

private:
	//TODO:
	// Magnifying glass 
	//unsigned char* data /*= nullptr*/;
	int width, height, channels;

	//measure times for image load and quantize
	uint32_t timeQuantizeDuration = 0;
	uint32_t timeQuantizeStart = 0;
	uint32_t timeLoadDuration = 0;
	uint32_t timeLoadStart = 0;

	SurfImageInspect imageInspect;
	ofParameter<ofColor> colorPicked{ "Color", ofColor(128, 128) };
	ofEventListener listenerColor;

public:
	ofxColorQuantizerHelper();
	~ofxColorQuantizerHelper();

	void setup();
	void update(ofEventArgs& args);
	void draw(); // Not required if not using OF native widgets!
	void exit();

	void drawImGuiWindows() { draw_ImGuiWidgets(); };

private:
	void draw_ImGuiWidgets();
	void draw_ImGuiPicture();
	void draw_ImGuiLibrary();

private:

	// Textures for pict preview on gui
	ofTexture tex;
	ofFbo fbo;
	void refreshImageGuiTexture();

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	//TODO:
	// Grid picker
	vector<ofTexture> textureSource;
	vector<GLuint> textureSourceID;

	// should remove some parameters..
	//ofParameter<int> sizeLibColBox;

	int currentImage_;

	ofParameterGroup params_Thumbs;
	ofParameter<int> thumbsSize;
	ofParameter<int> thumbsSpacing;
	ofParameter<int> thumbsBorder;

private:
	void doReset() {

		//bDoResetLib = true;
		bDoResetPic = true;

		thumbsSize = 150;
		thumbsSpacing = 2;
		thumbsBorder = 1;
	};

	//TODO: kind of expected/desired width..
	//ofParameter<bool> bResponsive;
	//float __widthPicts;

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

#endif

	//--

private:
	bool bUseNativeWidgets = false;

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
	// Image changed easy callback
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
	// Sort changed easy callback
	bool bUpdateSorting = false;
public:
	bool isUpdatedbSorting() {
		if (bUpdateSorting) {
			bUpdateSorting = false;
			return true;
		}
		else {
			return false;
		}
	}
	bool isProcessing() {
		return colorQuantizer.isProcessing();
	}

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

	ofParameterGroup getParameters() {
		return parameters;
	}

	//--

private:
	void refreshFiles();

	ofTrueTypeFont font;
	ofTrueTypeFont font2;

	bool bDoResetLib = false;
	bool bDoResetPic = false;

	//TODO: not used
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
			return paletteSorted;

			//vector<ofColor> _palette;
			//for (int i = 0; i < palette.size(); i++)
			//{
			//	_palette.push_back(colorMapSortable[i].color);
			//}
			//return _palette;
		}
	}

	//--

	// Pointers back to 'communicate externally'
	void setPalette_BACK_Name(std::string& n);
	void setPalette_BACK(vector<ofColor>& p);
	void setPalette_BACK_RefreshPalette(bool& b);
	void setColorPtr(ofColor& c);//legacy
	void setColor_BACK(ofColor& c);
	void setColor_BACK_Refresh(bool& b);//legacy
	//void setColorPtrRefresh(bool& b);

private:
	// Build palette from already quantized and sorted colors
	void buildSorting();

private:

	//--

	// Pointers back to 'communicate externally'
	ofColor* myColor_BACK = nullptr;
	vector<ofColor>* myPalette_BACK = nullptr;
	std::string* myPalette_Name_BACK = nullptr;
	bool* bUpdated_Palette_BACK = nullptr;
	bool* bUpdated_Color_BACK = nullptr;

	void doUpdatePointers();
	void doUpdatePointerColor(int n = 0);


	void draw_Palette_Preview();

	bool bUseBorder = true;

	// for native widgets usage
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 size = glm::vec2(1440, 900);

	std::string pathFolder = "images/";
	bool isLoadedImage = false;
	bool isVisible_gui = true;

	ofxColorQuantizer colorQuantizer;
	void loadImageAndQuantize(std::string imageName, int amountColors);

	void doSortNext() {
		sortedType = sortedType.get() + 1;
		if (sortedType > 4) sortedType = 1;
	};

public:
	int getAmountFiles() {
		return dir.size();
	}

	ofImage& getImage() {
		return image;
	}

private:
	ofImage image;
	ofImage imageSmall;
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
	void buildQuantize();
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

	// Sorted palette
	vector<ofColor> paletteSorted;
public:
	ofColor getColor(int index) {
		if (index > paletteSorted.size() - 1) return ofColor(0);//error
		else if (index < paletteSorted.size()) return paletteSorted[index];
		else return ofColor(0);//error
	}

private:
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
	void refreshFilesSorting(std::string name);
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
