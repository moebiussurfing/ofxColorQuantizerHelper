
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"
#include "ofxGui.h"

typedef struct
{
    int pos;
    ofColor color;
} colorMapping;

class ofxColorQuantizerHelper
{

public:

    ofxColorQuantizerHelper();
    ~ofxColorQuantizerHelper();

    void setup();
    void update();
    void draw();
    void exit();

    //--

    // API

    void setActive(bool b)
    {
        isActive = b;
        isVisible_gui = b;
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

    void setMinimal(bool b)
    {
        ENABLE_minimal = b;
    }

    vector<ofColor> getPalette()
    {
        return palette;
    }

    void draw_Palette_Preview();

    //--

private:

    string pathFolder = "images/";
    bool isLoadedImage = false;
    bool isVisible_gui = true;

    ofxColorQuantizer colorQuantizer;
    void quantizeImage(string imageName, int numColors);

    ofImage image;
    ofImage imageCopy;
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

    void map_setup();
    map<int, ofColor> colorMap;
    vector<colorMapping> colorMapSortable;

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

    int boxSize;
    int boxSize_h;
    int boxPad;
    int boxW;
    int wPal;

    void kMeansTest();

    // drag and drop images
    vector<ofImage> draggedImages;
    ofPoint dragPt;
    void draw_imageDragged();
    ofParameter<int> currentImage;
    string pathFolerDrag = "images/drag/";
    ofDirectory dir;

    ofParameter<bool> ENABLE_minimal{"MINIMAL", false};

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
    string XML_path = "ofxColorQuantizerHelper.xml";

};

//}