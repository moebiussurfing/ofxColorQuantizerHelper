#include "ofxColorQuantizerHelper.h"
#include "ColorConverter.h"

bool comparePos(const colorMapping &s1, const colorMapping &s2)
{
    return s1.pos < s2.pos;
}

bool compareBrightness(const colorMapping &s1, const colorMapping &s2)
{
    return s1.color.getBrightness() < s2.color.getBrightness();
}

bool compareHue(const colorMapping &s1, const colorMapping &s2)
{
    return s1.color.getHue() < s2.color.getHue();
}

bool compareSaturation(const colorMapping &s1, const colorMapping &s2)
{
    return s1.color.getSaturation() < s2.color.getSaturation();
}
//--------------------------------------------------------------
void ofxColorQuantizerHelper::filesRefresh()
{
    // load dragged images folder
    ofLogNotice("ofxColorQuantizerHelper") << "list files " << pathFolerDrag;
    dir.listDir(pathFolerDrag);
    dir.allowExt("jpg");
    dir.allowExt("png");
    dir.sort();

    // STARTUP QUANTIZER COLOR
    //imageName_path = "0.jpg";
    //imageName = "0";
    //buildFromImageFile(pathFolder + imageName_path, numColors);

    // load first file in dir
    if (dir.size() > 0)
        currentImage = 0;
    else
        currentImage = -1;

    if (dir.size() > 0 && currentImage < dir.size() - 1)
    {
        imageName = dir.getName(currentImage);
        imageName_path = dir.getPath(currentImage);
        buildFromImageFile(imageName_path, numColors);
    }

    // log files on folder
    for (int i = 0; i < dir.size(); i++)
    {
        ofLogNotice("ofxColorQuantizerHelper") << "file " << "[" << ofToString(i) << "] " << dir.getName(i);
    }
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setup()
{
    //-

    // gui
    gui.setup();
    parameters.setName("COLOR QUANTIZER");
    parameters.add(numColors.set("number of colors", 10, 1, 50));
    parameters.add(sortedType.set("sort type", 1, 1, 4));
    parameters.add(labelStr.set(" ", labelStr));
    //parameters.add(labelUrlStr.set("type url", labelUrlStr));
    parameters.add(currentImage.set("dragged files ", 0, 0, dir.size() - 1));
    parameters.add(bReBuild.set("re build", false));

    gui.add(parameters);
    gui.setPosition(ofGetWidth() - 205, 5);
    ofAddListener(parameters.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);

    //-

    // STARTUP SETTINGS
    XML_params.setName("ofxColorQuantizerHelper");
    XML_params.add(ENABLE_minimal);
    XML_params.add(numColors);
    XML_params.add(sortedType);
    XML_load_AppSettings(XML_params, XML_path);

    //-

    filesRefresh();


}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::update()
{
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw()
{
    if (isLoadedImage)
    {
        // 1. debug big window
        if (!ENABLE_minimal)
        {
            int x = position.x;//x pad for left/right window
            int y = position.y;
            int margin = 5;
            int space = 50;
            boxPad = 2;//pad between boxes

            //-

            ofPushMatrix();
            ofPushStyle();

            ofTranslate(x, y);

            // debug text
            int il = 20;
            int i = 1;
            int posTxt = -210;
            ofDrawBitmapStringHighlight("LOADED IMAGE path/url: '" + imageName_path + "'", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            i++;
            ofDrawBitmapStringHighlight("MINI MODE: [A]", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            ofDrawBitmapStringHighlight("CHANGE COLORS AMOUNT: [LEFT-RIGHT]", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            ofDrawBitmapStringHighlight("CHANGE SORTING: [BACKSPACE]", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            ofDrawBitmapStringHighlight("SORTING: " + labelStr.get(), 0, posTxt + il * i++, ofColor::black, ofColor::white);
            //ofDrawBitmapStringHighlight("SELECT IMAGE:", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            //ofDrawBitmapStringHighlight("FROM /images: [01234567][QWE][ASD][ZXC]", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            i++;
            ofDrawBitmapStringHighlight("SELECT IMAGE FROM /images/drag: [UP-DOWN]", 0, posTxt + il * i++, ofColor::black, ofColor::white);
            ofDrawBitmapStringHighlight("drag images into window (/images/drag)", 0, posTxt + il * i++, ofColor::black, ofColor::white);

            ofSetColor(255);

            // draw original image but resized to ImgW pixels width, same aspect ratio
            float imgRatio = image.getHeight() / image.getWidth();
            int ImgW = 100;
            int imgH = imgRatio * ImgW;
            image.draw(0, 0, ImgW, imgH);

            // resize box sizes
            //wPal = ofGetWidth() - (x + ImgW + x);
            //wPal = size.x - (x + ImgW + x);
            wPal = size.x - (margin + ImgW + margin);

            boxW = wPal / colorQuantizer.getNumColors();
            boxSize = boxW - boxPad;
            //boxSize_h = boxSize;
            boxSize_h = ofGetHeight() - (imgH + 50 + y + space);

            ofTranslate(0, imgH);

            //// debug info
            //if (sortedType == 1)
            //{
            //    ofSetColor(255, 100);
            //    ofDrawBitmapString("(Original sorting has colors weighted based on their areas, their order is based on their chroma values)", 0, 50);
            //}

            // all colors % bars
            ofTranslate(ImgW + 20, 0);
            for (int i = 0; i < colorQuantizer.getNumColors(); i++)
            {
                ofSetColor(0, 50);
                ofDrawRectangle(i * (boxSize + boxPad), 0, boxSize, -imgH);
                ofSetColor(sortedColors[i].color);
                ofDrawRectangle(i * (boxSize + boxPad), 0, boxSize, ofMap(sortedColors[i].weight, 0, 1, 0, -imgH));
                ofSetColor(255);
                ofDrawBitmapString(ofToString(int(sortedColors[i].weight * 100)) + "%", i * (boxSize + boxPad), 15);
            }

            ofTranslate(0, space);

            //// debug text
            //string str = "sorted palette preview (" + ofToString(labelStr) + ")";
            //ofDrawBitmapStringHighlight(str, 5, -10, ofColor::black, ofColor::white);

            // palette preview
            draw_Palette_Preview();

            ofPopMatrix();
            ofPopStyle();
        }

            // 2. MINI MODE user window
        else
        {
            if (bottomMode)//put preview in the bottom window
            {
                //ignore y position and put at the window bottom
                int x = position.x;//x pad for left/right window
                //int y = position.y;
                int pad = 0;
                boxPad = 0;//pad between boxes

                ofPushMatrix();
                ofPushStyle();

                ofTranslate(x, 0);
                ofSetColor(255);

                // 1. draw original image but resized to ImgW pixels width, same aspect ratio
                float imgRatio = image.getHeight() / image.getWidth();

                //// 1. fixed width
                //int ImgW = 200;
                //int imgH = imgRatio * ImgW;

                // 2. fixed height
                int imgH = 125;
                int ImgW = imgH / imgRatio;

                image.draw(0, ofGetHeight() - (imgH + pad), ImgW, imgH);

                // 2. image border
                if (bUseBorder)
                {
                    ofRectangle r;
                    r = ofRectangle(0, ofGetHeight() - (imgH + pad), ImgW, imgH);
                    ofNoFill();
                    ofSetColor(ofColor(ofColor::white, 64));
                    ofDrawRectangle(r);
                }

                // 3. palette position
                ofTranslate(ImgW + pad, 0);

                //// resize box sizes
                //wPal = size.x - (pad + ImgW + pad);
                //boxW = wPal / colorQuantizer.getNumColors();
                //boxSize = boxW - boxPad;
                //boxSize_h = imgH;

                // resize box sizes
                // panel size is ignored
                //wPal = size.x - (pad + ImgW + pad);
                wPal = ofGetWidth() - ImgW;
                boxW = (float) wPal / colorQuantizer.getNumColors();
                boxSize = boxW - boxPad;
                boxSize_h = imgH;

                // palette preview
                ofTranslate(0, ofGetHeight() - (boxSize_h + pad));
                draw_Palette_Preview();

                ofPopStyle();
                ofPopMatrix();
            }
            else
            {
                int x = position.x;//x pad for left/right window
                int y = position.y;
                int pad = 0;
                boxPad = 0;//pad between boxes

                ofPushMatrix();
                ofPushStyle();

                ofTranslate(x, y);
                ofSetColor(255);

                // draw original image but resized to ImgW pixels width, same aspect ratio
                float imgRatio = image.getHeight() / image.getWidth();
                int ImgW = 200;
                int imgH = imgRatio * ImgW;
                image.draw(0, 0, ImgW, imgH);

                // palette position

                //// 1. down
                //ofTranslate(0, imgH);
                //// resize box sizes
                //wPal = ofGetWidth() - (x + ImgW + x);
                //boxW = wPal / colorQuantizer.getNumColors();
                //boxSize = boxW - boxPad;

                // 2. right
                ofTranslate(ImgW + pad, 0);
                // resize box sizes
                wPal = size.x - (pad + ImgW + pad);
                boxW = wPal / colorQuantizer.getNumColors();
                boxSize = boxW - boxPad;
                boxSize_h = imgH;

                // palette preview
                draw_Palette_Preview();

                ofPopStyle();
                ofPopMatrix();
            }
        }
    }

    //-

    if (isVisible_gui && !ENABLE_minimal)
        gui.draw();
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::build()
{
    if (isLoadedImage)
    {
        colorQuantizer.setNumColors(numColors);
        colorQuantizer.quantize(imageCopy.getPixels());

        sortedColors.clear();;
        sortedColors.resize(colorQuantizer.getNumColors());
        for (int i = 0; i < colorQuantizer.getNumColors(); i++)
        {
            ofFloatColor fc = ofFloatColor(colorQuantizer.getColors()[i].r / 255.0, colorQuantizer.getColors()[i].g / 255.0, colorQuantizer.getColors()[i].b / 255.0);
            ofVec3f labCol = ColorConverter::rgbToLab(fc);

            sortedColors[i].distance = ColorConverter::calcChroma(labCol);
            sortedColors[i].color = colorQuantizer.getColors()[i];
            sortedColors[i].weight = colorQuantizer.getColorWeights()[i];
        }

        std::sort(sortedColors.begin(), sortedColors.end(), by_distance());

        //--

        map_setup();
    }
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::quantizeImage(string imgName, int _numColors)
{
    isLoadedImage = false;

    numColors = _numColors;

    if (image.load(imgName))
    {
        isLoadedImage = true;

        imageCopy.clear();
        imageCopy = image;//speed up
        //imageCopy.load(imgName);

        // resize to speed up
        imageCopy.resize(imageCopy.getWidth() / 4, imageCopy.getHeight() / 4);

        build();
    }
    else
    {
        ofLogError() << "image file not found!";
        isLoadedImage = false;
    }
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::Changed_parameters(ofAbstractParameter &e)
{
    string WIDGET_name = e.getName();

    ofLogNotice("ofxColorQuantizerHelper") << "Changed_parameters: " << WIDGET_name << ": " << e;

    if (WIDGET_name == "sort type")
    {
        switch (sortedType)
        {
            case 1:
                ofSort(colorMapSortable, comparePos);
                labelStr = "Original";
                break;
            case 2:
                ofSort(colorMapSortable, compareHue);
                labelStr = "Hue";
                break;
            case 3:
                ofSort(colorMapSortable, compareBrightness);
                labelStr = "Brightness";
                break;
            case 4:
                ofSort(colorMapSortable, compareSaturation);
                labelStr = "Saturation";
                break;
            default:
                break;
        }

        //--

        // pointers back to 'communicate externally'

        int sizePalette = palette.size();
        if (sizePalette > 0 && myPalette_BACK != nullptr)
        {
            // set BACK name clicked
            if (myPalette_Name_BACK != nullptr)
            {
                (*myPalette_Name_BACK) = imageName;
            }

            //-

            // set BACK palette colors
            myPalette_BACK->clear();
            myPalette_BACK->resize(sizePalette);

            //// 1. get unsorted palette
            //(*myPalette_BACK) = palette;

            // 2. get palette sorted
            for (int col = 0; col < palette.size(); col++)
            {
                (*myPalette_BACK)[col] = colorMapSortable[col].color;
            }

            // mark update flag
            if (bUpdated_Palette_BACK != nullptr)
            {
                (*bUpdated_Palette_BACK) = true;
            }
        }
    }
    else if (WIDGET_name == "type url")
    {
        imageName_path = labelUrlStr;
        buildFromImageFile(imageName_path, numColors);
        ofLogNotice() << "type url: " << imageName_path;
    }
    else if (WIDGET_name == "re build")
    {
        if (bReBuild)
        {
            bReBuild = false;
            build();
        }
    }
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::map_setup()
{
    palette.clear();
    int palSize = colorQuantizer.getNumColors();
    palette.resize(palSize);

    //palette = colorQuantizer.getColors();
    for (int i = 0; i < sortedColors.size(); i++)
    {
        palette[i] = sortedColors[i].color;
    }

    colorMap.clear();
    colorMapSortable.clear();

    for (int i = 0; i < palSize; i++)
    {
        colorMap[i] = palette[i];
    }

    for (unsigned int i = 0; i < palSize; i++)
    {//colorNameMap

        map<int, ofColor>::iterator mapEntry = colorMap.begin();
        std::advance(mapEntry, i);

        colorMapping mapping;
        mapping.pos = mapEntry->first;
        mapping.color = mapEntry->second;
        colorMapSortable.push_back(mapping);
    }

    switch (sortedType)
    {
        case 1:
            ofSort(colorMapSortable, comparePos);
            labelStr = "Original";
            break;
        case 2:
            ofSort(colorMapSortable, compareHue);
            labelStr = "Hue";
            break;
        case 3:
            ofSort(colorMapSortable, compareBrightness);
            labelStr = "Brightness";
            break;
        case 4:
            ofSort(colorMapSortable, compareSaturation);
            labelStr = "Saturation";
            break;
        default:
            break;
    }

    //--

    // pointers back to 'communicate externally'

    int sizePalette = palette.size();
    if (sizePalette > 0 && myPalette_BACK != nullptr)
    {
        // set BACK name clicked
        if (myPalette_Name_BACK != nullptr)
        {
            (*myPalette_Name_BACK) = imageName;
        }

        //-

        // set BACK palette colors
        myPalette_BACK->clear();
        myPalette_BACK->resize(sizePalette);

        //// 1. get unsorted palette
        //(*myPalette_BACK) = palette;

        // 2. get palette sorted
        for (int col = 0; col < palette.size(); col++)
        {
            (*myPalette_BACK)[col] = colorMapSortable[col].color;
        }

        // mark update flag
        if (bUpdated_Palette_BACK != nullptr)
        {
            (*bUpdated_Palette_BACK) = true;
        }
    }
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::buildFromImageFile(string path, int num)
{
    //TODO: improve with threading load..

    //quantizeImage(pathFolder + path, num);
    quantizeImage(path, num);
    build();
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::buildFromImageUrl(string url, int num)
{
    //TODO: improve with threading load and some HTTP image browsing api..
    quantizeImage(url, num);
    build();
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw_Palette_Preview()
{
    ofPushMatrix();
    ofPushStyle();

    ofRectangle r;
    r = ofRectangle(0, 0, boxSize, boxSize_h);

    for (int col = 0; col < palette.size(); col++)
    {
        ofSetColor(colorMapSortable[col].color);
        //ofSetColor(palette[col]);

        ofFill();
        ofDrawRectangle(r);

        if (bUseBorder)
        {
            ofNoFill();
            ofSetColor(ofColor(ofColor::white, 64));
            ofDrawRectangle(r);
        }

        ofTranslate(boxSize + boxPad, 0);
    }

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::kMeansTest()
{
    // for testing

    cv::Mat samples = (cv::Mat_<float>(8, 1) << 31, 2, 10, 11, 25, 27, 2, 1);
    cv::Mat labels;

    // double kmeans(const Mat& samples, int clusterCount, Mat& labels,
    cv::TermCriteria termcrit;
    int attempts, flags;
    cv::Mat centers;
    double compactness = cv::kmeans(samples, 3, labels, cv::TermCriteria(), 2, cv::KMEANS_PP_CENTERS, centers);

    ofLogNotice("ofxColorQuantizerHelper") << "labels:";
    for (int i = 0; i < labels.rows; ++i)
    {
        ofLogNotice("ofxColorQuantizerHelper") << labels.at<int>(0, i);
    }

    ofLogNotice("ofxColorQuantizerHelper") << "\ncenters:" << endl;
    for (int i = 0; i < centers.rows; ++i)
    {
        ofLogNotice("ofxColorQuantizerHelper") << centers.at<float>(0, i);
    }

    ofLogNotice("ofxColorQuantizerHelper") << "\ncompactness: " << compactness;
}


//--------------------------------------------------------------
ofxColorQuantizerHelper::ofxColorQuantizerHelper()
{
    setActive(true);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::keyPressed(ofKeyEventArgs &eventArgs)
{
    const int &key = eventArgs.key;
    ofLogNotice("ofxColorQuantizerHelper") << "key: " << key;

    // minimal mode
    if (key == 'a')
    {
        ENABLE_minimal = !ENABLE_minimal;
    }

    //-

    // dragged files

    if (key == OF_KEY_LEFT)
    {
        numColors--;
        if (numColors < numColors.getMin())
            numColors = numColors.getMin();
        bReBuild = true;
    }
    if (key == OF_KEY_RIGHT)
    {
        numColors++;
        if (numColors > numColors.getMax())
            numColors = numColors.getMax();
        bReBuild = true;
    }

    if (key == OF_KEY_UP)
    {
        // refresh dir
        dir.listDir(pathFolerDrag);
        dir.allowExt("jpg");
        dir.allowExt("png");
        dir.sort();
        currentImage.setMax(dir.size() - 1);

        currentImage--;
        if (currentImage < 0)
            currentImage = dir.size() - 1;
        //currentImage = 0;

        ofLogNotice("ofxColorQuantizerHelper") << "currentImage:" << ofToString(currentImage);

        if (dir.size() > 0 && currentImage < dir.size() - 1)
        {
            imageName = dir.getName(currentImage);
            imageName_path = dir.getPath(currentImage);
            buildFromImageFile(imageName_path, numColors);
        }
    }
    if (key == OF_KEY_DOWN || key == ' ')
    {
        // refresh dir
        dir.listDir(pathFolerDrag);
        dir.allowExt("jpg");
        dir.allowExt("png");
        dir.sort();
        currentImage.setMax(dir.size() - 1);

        currentImage++;
        if (currentImage > dir.size() - 1)
            currentImage = 0;

        //currentImage = dir.size() - 1;

        ofLogNotice("ofxColorQuantizerHelper") << "currentImage:" << ofToString(currentImage);

        if (dir.size() > 0 && currentImage < dir.size())
        {
            imageName = dir.getName(currentImage);
            imageName_path = dir.getPath(currentImage);
            buildFromImageFile(imageName_path, numColors);
        }
    }

    //-

    // sort types

    if (key == 's')
    {
        sortedType++;
        if (sortedType > 4)
            sortedType = 1;
    }

    //        if (key == '1'){
    //            if (sortedType != 1){
    //                sortedType = 1;
    //            }
    //        }
    //        if (key == '2'){
    //            if (sortedType != 2){
    //                sortedType = 2;
    //            }
    //        }
    //        else if (key == '3'){
    //            if (sortedType != 3){
    //                sortedType = 3;
    //            }
    //        }
    //        else if (key == '4'){
    //            if (sortedType != 4){
    //                sortedType = 4;
    //            }
    //        }


    //-

    //// test kMeansTest
    //if (key == 'k')
    //{
    //    kMeansTest();
    //}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::keyReleased(ofKeyEventArgs &eventArgs)
{
    if (eventArgs.key == ' ')
    {
    }
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::addDragListeners()
{
    ofAddListener(ofEvents().fileDragEvent, this, &ofxColorQuantizerHelper::dragEvent);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::removeDragListeners()
{
    ofAddListener(ofEvents().fileDragEvent, this, &ofxColorQuantizerHelper::dragEvent);
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::addKeysListeners()
{
    ofAddListener(ofEvents().keyPressed, this, &ofxColorQuantizerHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::removeKeysListeners()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxColorQuantizerHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mouseDragged(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    //ofLogNotice("ofxColorQuantizerHelper") << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mousePressed(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    //ofLogNotice("ofxColorQuantizerHelper") << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mouseReleased(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    //ofLogNotice("ofxColorQuantizerHelper") << "mouseReleased " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::addMouseListeners()
{
    ofAddListener(ofEvents().mouseDragged, this, &ofxColorQuantizerHelper::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &ofxColorQuantizerHelper::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxColorQuantizerHelper::mouseReleased);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::removeMouseListeners()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxColorQuantizerHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::exit()
{
    XML_save_AppSettings(XML_params, XML_path);
}

//--------------------------------------------------------------
ofxColorQuantizerHelper::~ofxColorQuantizerHelper()
{
    //removeKeysListeners();
    //removeMouseListeners();
    setActive(false);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::dragEvent(ofDragInfo &eventArgs)
{
    auto info = eventArgs;
    ofLogNotice("ofxColorQuantizerHelper") << "dragEvent ";

    if (info.files.size() > 0)
    {
        dragPt = info.position;
        ofLogNotice("ofxColorQuantizerHelper") << "info.position: " << dragPt;

        draggedImages.assign(info.files.size(), ofImage());
        for (unsigned int k = 0; k < info.files.size(); k++)
        {
            ofLogNotice("ofxColorQuantizerHelper") << "draggedImages: " << info.files[k];
            draggedImages[k].load(info.files[k]);

            // save dragged files to data folder

            // 1. use timestamped name
            //string fileName = "img_" + ofToString(ofGetTimestampString() + ".png");
            //draggedImages[k].save(pathFolerDrag + fileName);

            // 2. use original file name
            auto strs = ofSplitString(info.files[k], "/");
            //cout << "strs: " << ofToString(strs) << endl;
            string fileName = strs[strs.size() - 1];
            //cout << "filename: " << fileName << endl;
            draggedImages[k].save(pathFolerDrag + fileName);
        }

        // create palette from last file
        imageName_path = info.files[info.files.size() - 1];
        buildFromImageFile(imageName_path, numColors);

        // refresh dir
        dir.listDir(pathFolerDrag);
        dir.allowExt("jpg");
        dir.allowExt("png");
        dir.sort();
        currentImage.setMax(dir.size() - 1);
    }
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw_imageDragged()
{
    ofSetColor(255);

    float dx = dragPt.x;
    float dy = dragPt.y;
    for (unsigned int k = 0; k < draggedImages.size(); k++)
    {
        draggedImages[k].draw(dx, dy);
        dy += draggedImages[k].getHeight() + 10;
    }

    ofSetColor(128);
    ofDrawBitmapString("drag image files into this window", 10, 20);
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::XML_load_AppSettings(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxColorQuantizerHelper") << "XML_load_AppSettings " << path;
    ofXml settings;
    settings.load(path);
    ofDeserialize(settings, g);
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::XML_save_AppSettings(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxColorQuantizerHelper") << "XML_save_AppSettings " << path;

    ofXml settings;
    ofSerialize(settings, g);
    settings.save(path);
}

// pointers back to 'communicate externally'

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setColor_BACK(ofColor &c)
{
    myColor_BACK = &c;
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_BACK(vector<ofColor> &p)
{
    myPalette_BACK = &p;
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_bUpdated_Palette_BACK(bool &b)
{
    bUpdated_Palette_BACK = &b;
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_bUpdated_Color_BACK(bool &b)
{
    bUpdated_Color_BACK = &b;
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_Name_BACK(string &n)
{
    myPalette_Name_BACK = &n;
}
