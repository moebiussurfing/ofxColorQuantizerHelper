#include "ofxColorQuantizerHelper.h"
#include "ColorConverter.h"

//--

// sorting helpers

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
void ofxColorQuantizerHelper::loadPrev()
{
	// refresh dir
	dir.listDir(pathFolerDrag);
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.sort();
	currentImage.setMax(dir.size() - 1);
	currentImage--;
	if (currentImage < 0) currentImage = dir.size() - 1;
	//currentImage = 0;
	ofLogNotice(__FUNCTION__) << "currentImage:" << ofToString(currentImage);
	if (dir.size() > 0 && currentImage < dir.size() - 1)
	{
		imageName = dir.getName(currentImage);
		imageName_path = dir.getPath(currentImage);
		buildFromImageFile(imageName_path, numColors);
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::loadNext()
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
	ofLogNotice(__FUNCTION__) << "currentImage:" << ofToString(currentImage);
	if (dir.size() > 0 && currentImage < dir.size())
	{
		imageName = dir.getName(currentImage);
		imageName_path = dir.getPath(currentImage);
		buildFromImageFile(imageName_path, numColors);
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::filesRefresh()
{
	// load dragged images folder
	ofLogNotice(__FUNCTION__) << "list files " << pathFolerDrag;
	dir.listDir(pathFolerDrag);
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.allowExt("JPG");
	dir.allowExt("PNG");
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
		ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);
	}

	currentImage.setMax(dir.size() - 1);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setup()
{
	//-

	ofxSurfingHelpers::setThemeDark_ofxGui();

	// gui
	gui.setup("ofxColorQuantizerHelper");
	parameters.setName("COLOR QUANTIZER");
	parameters.add(numColors.set("Amount Colors", 10, 1, 50));
	parameters.add(sortedType.set("Sort Type", 1, 1, 4));
	parameters.add(labelStr.set(" ", labelStr));
	//parameters.add(labelUrlStr.set("type url", labelUrlStr));
	parameters.add(currentImage.set("File ", 0, 0, dir.size() - 1));
	parameters.add(currentImage_name.set("", ""));
	parameters.add(bReBuild.set("Build", false));
	parameters.add(bVisible.set("GUI", true));
	parameters.add(bInfo.set("Info", false));
	parameters.add(bottomMode.set("Bottom", false));

	currentImage_name.setSerializable(false);

	gui.add(parameters);
	gui.setPosition(500, 5);
	ofAddListener(parameters.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);

	//font.load("assets/fonts/LCD_Solid.ttf", 10, true, true);
	font.load("assets/fonts/overpass-mono-bold.otf", 9, true, true);

	//----

	// STARTUP SETTINGS

	XML_params.setName("ofxColorQuantizerHelper");
	XML_params.add(ENABLE_minimal);
	XML_params.add(numColors);
	XML_params.add(sortedType);

	XML_load_AppSettings(XML_params, XML_path);

	//-

	filesRefresh();

	setPosition(glm::vec2(20, 20));
	setSize(glm::vec2(1000, 700));
}

////--------------------------------------------------------------
//void ofxColorQuantizerHelper::update()
//{
//}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw()
{
	if (isLoadedImage && bVisible && bInfo)
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

			//-

			// 1. debug text

			string str;
			//str = "LOADED IMAGE Path/Url: \n" + imageName_path;
			str = "LOADED IMAGE Path/Url: \n" + currentImage_name.get();
			str += "(data/images/drag/)";
			str += "\n\n";
			str += "UP-DOWN: SELECT IMAGE";
			str += "\n";
			str += "LEFT-RIGHT: COLORS AMOUNT";
			str += "\n";
			str += "BACKSPACE: SORTING \t[ " + labelStr.get() + " ]";
			str += "\n";
			str += "GUI: [G]";
			str += "\n";
			str += "MINI MODE: [A]";
			str += "\n";
			str += "Drag images into the Window !";
			str += "\n";


			int posTxt = 10;
			ofxSurfingHelpers::drawTextBoxed(font, str);
			//ofDrawBitmapStringHighlight(str, 0, posTxt, ofColor::black, ofColor::white);

			ofSetColor(255);

			//-

			// 2. image

			int ImgW = 100;

			int h = ofxSurfingHelpers::getHeightBBtextBoxed(font, str);
			ofTranslate(10, h + 20);

			// draw original image but resized to ImgW pixels width, same aspect ratio
			float imgRatio = image.getHeight() / image.getWidth();
			int imgH = imgRatio * ImgW;

			image.draw(0, 0, ImgW, imgH);

			//-

			// 3. resize box sizes

			//wPal = ofGetWidth() - (x + ImgW + x);
			//wPal = size.x - (x + ImgW + x);
			wPal = size.x - (margin + ImgW + margin);

			boxW = wPal / colorQuantizer.getNumColors();
			boxSize = boxW - boxPad;

			boxSize_h = boxSize;
			//boxSize_h = ofGetHeight() - (imgH + 50 + y + space);

			ofTranslate(0, imgH);

			//// debug info
			//if (sortedType == 1)
			//{
			//    ofSetColor(255, 100);
			//    ofDrawBitmapString("(Original sorting has colors weighted based on their areas, their order is based on their chroma values)", 0, 50);
			//}

			//-

			// 4. all colors % bars
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
				boxW = (float)wPal / colorQuantizer.getNumColors();
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

	if (/*isVisible_gui &&*/ !ENABLE_minimal && bVisible) gui.draw();
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

		rebuildMap();
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
	string _name = e.getName();

	ofLogNotice(__FUNCTION__) << "Changed_parameters: " << _name << ": " << e;

	if (_name == "Sort Type")
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
	else if (_name == "type url")
	{
		imageName_path = labelUrlStr;
		buildFromImageFile(imageName_path, numColors);
		ofLogNotice() << "type url: " << imageName_path;
		ofLogNotice() << "type url: " << imageName_path;
	}
	else if (_name == "Build")
	{
		if (bReBuild)
		{
			bReBuild = false;
			build();
		}
	}
	else if (_name == currentImage.getName())
	{
		// refresh dir
		dir.listDir(pathFolerDrag);
		dir.allowExt("jpg");
		dir.allowExt("png");
		dir.sort();
		currentImage.setMax(dir.size() - 1);

		ofLogNotice(__FUNCTION__) << "currentImage: " << ofToString(currentImage);

		if (dir.size() > 0 && currentImage < dir.size() - 1)
		{
			imageName = dir.getName(currentImage);
			currentImage_name = imageName;
			imageName_path = dir.getPath(currentImage);
			buildFromImageFile(imageName_path, numColors);
		}
	}
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::rebuildMap()
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

	ofLogNotice(__FUNCTION__) << "labels:";
	for (int i = 0; i < labels.rows; ++i)
	{
		ofLogNotice(__FUNCTION__) << labels.at<int>(0, i);
	}

	ofLogNotice(__FUNCTION__) << "\ncenters:" << endl;
	for (int i = 0; i < centers.rows; ++i)
	{
		ofLogNotice(__FUNCTION__) << centers.at<float>(0, i);
	}

	ofLogNotice(__FUNCTION__) << "\ncompactness: " << compactness;
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
	ofLogNotice(__FUNCTION__) << "key: " << key;

	// minimal mode
	if (key == 'a' || key == 'A')
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

		//if (numColors < numColors.getMin()) numColors = numColors.getMin();
	}
	if (key == OF_KEY_RIGHT)
	{
		numColors++;
		if (numColors > numColors.getMax())
			numColors = numColors.getMax();
		bReBuild = true;

		// if (numColors > numColors.getMax()) numColors = numColors.getMax();
	}

	if (key == OF_KEY_UP)
	{
		//currentImage--;
		//if (currentImage < 0)
		//	currentImage = dir.size() - 1;
		////currentImage = 0;

		loadPrev();
	}
	if (key == OF_KEY_DOWN || key == ' ')
	{
		//currentImage++;
		//if (currentImage > dir.size() - 1)
		//	currentImage = 0;

		//currentImage = dir.size() - 1;

		loadNext();
	}

	//-

	// sort types

	if (key == 's' || key == OF_KEY_BACKSPACE)
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
	//if (eventArgs.key == ' ')
	//{
	//}
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
	//ofLogNotice(__FUNCTION__) << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mouseReleased " << x << ", " << y << ", " << button;
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

	exit();
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::dragEvent(ofDragInfo &eventArgs)
{
	auto info = eventArgs;
	ofLogNotice(__FUNCTION__) << "dragEvent ";

	if (info.files.size() > 0)
	{
		dragPt = info.position;
		ofLogNotice(__FUNCTION__) << "info.position: " << dragPt;

		draggedImages.assign(info.files.size(), ofImage());
		for (unsigned int k = 0; k < info.files.size(); k++)
		{
			ofLogNotice(__FUNCTION__) << "draggedImages: " << info.files[k];
			draggedImages[k].load(info.files[k]);

			// save dragged files to data folder

			// 1. use timestamped name
			//string fileName = "img_" + ofToString(ofGetTimestampString() + ".png");
			//draggedImages[k].save(pathFolerDrag + fileName);

			// 2. use original file name
			
			//TODO:
			//set define to choice an OS to adapt file system

			auto strs = ofSplitString(info.files[k], "\\");// Windows
			//auto strs = ofSplitString(info.files[k], "/");// macOS ?

			ofLogNotice(__FUNCTION__) << "route: " << ofToString(strs);
			
			string fileName = strs[strs.size() - 1];
			ofLogNotice(__FUNCTION__) << "fileName: " << ofToString(fileName);

			//TODO:
			//not working, not copying file!

			//pathFolerDrag = ofToDataPath();
			ofxSurfingHelpers::CheckFolder(pathFolerDrag);
			draggedImages[k].save(ofToDataPath(pathFolerDrag + fileName));
			//draggedImages[k].save(pathFolerDrag + fileName);
		}

		//-

		//workflow
		
		// refresh dir
		filesRefresh();

		//-

		// create palette from last file
		imageName_path = info.files[info.files.size() - 1];
		buildFromImageFile(imageName_path, numColors);
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
	ofLogNotice(__FUNCTION__) << "XML_load_AppSettings " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::XML_save_AppSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << "XML_save_AppSettings " << path;

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
void ofxColorQuantizerHelper::setPalette_BACK_RefreshPalette(bool &b)
{
	bUpdated_Palette_BACK = &b;
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::setColor_BACK_Refresh(bool &b)
{
	bUpdated_Color_BACK = &b;
}


//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_BACK_Name(string &n)
{
	myPalette_Name_BACK = &n;
}
