#include "ofxColorQuantizerHelper.h"

#ifdef USE_IM_GUI__QUANTIZER

//--------------------------------------------------------------
void ofxColorQuantizerHelper::refreshImageGuiTexture()
{
	ofLogNotice("ofxColorQuantizerHelper") << "refreshImageGuiTexture()";

	// Loaded ofImage
	ofLogNotice("ofxColorQuantizerHelper") << "image path: " << getImagePath();

	bool b = ofGetUsingArbTex();//push

	ofDisableArbTex();

	//TODO: 
	// Could improve reusing the image from the quantizer too..
	// avoiding to re load the file!
	if (image.isAllocated())
		tex = image.getTexture();
	else
		ofLoadImage(tex, getImagePath());

	fbo.allocate(tex.getWidth(), tex.getHeight());
	fbo.createAndAttachTexture(GL_RGB, 0); //Position
	fbo.createAndAttachTexture(GL_RGBA32F, 1); //velocity
	fbo.createAndAttachRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	fbo.checkStatus();

	fbo.begin();
	ofClear(0);
	fbo.end();

	if (b) ofEnableArbTex(); // pop/restore

	fbo.begin(); // draw once only
	ofClear(0, 0, 0, 0);
	tex.draw(0, 0);
	fbo.end();
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw_ImGuiPicture()
{
	if (!bGui_Picture) return;

	float wdef = 300;
	float hdef = ofGetHeight() / 2;
	ImGuiCond cond = ImGuiCond_FirstUseEver;
	if (bDoResetPic)
	{
		bDoResetPic = false;
		ui->bAutoResize = true;
		cond = ImGuiCond_Always;
	}
	ImGui::SetNextWindowSize(ImVec2(wdef, hdef), cond);

	if (ui->BeginWindow(bGui_Picture))
	{
		float _w100 = ui->getWidgetsWidth(1);
		float _spcx = ui->getWidgetsSpacingX();
		float hb = ui->getWidgetsHeightUnit();

		//--

		ui->AddMinimizerToggle();
		if (ui->isMaximized())
		{
			ui->AddDebugToggle();
			ui->AddAutoResizeToggle();
		}
		ui->AddSpacingSeparated();

		ui->Add(bGui_Library, OFX_IM_TOGGLE_ROUNDED);
		if (bGui_Library && ui->isMaximized())
		{
			ui->Indent();
			ui->Add(bGuiLinked, OFX_IM_TOGGLE_ROUNDED_MINI);
			ui->Add(bAutoResizeLib, OFX_IM_TOGGLE_ROUNDED_MINI);
			ui->Unindent();
		}

		ui->AddSpacingSeparated();

		//--

		ImGuiColorEditFlags colorEdiFlags =
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_NoPicker |
			ImGuiColorEditFlags_NoTooltip;

		//--

		// 1. Scrollable list names

		if (!imageNames.empty())
		{
			if (ui->isMaximized())
			{
				ui->AddLabelBig("FILE");
			}

			ui->AddSpacing();

			if (ui->isMaximized())
			{
				// Index / total
				std::string s = ofToString(currentImage.get()) + "/" + ofToString(getAmountFiles() - 1);
				ui->AddLabel(s);
			}

			ui->AddCombo(currentImage, imageNames);
		}

		ui->AddSpacing();

		//--

		if (ui->AddButton("<", OFX_IM_BUTTON_MEDIUM, 2))
		{
			loadPrev();
		}

		ui->SameLine();

		if (ui->AddButton(">", OFX_IM_BUTTON_MEDIUM, 2))
		{
			loadNext();
		}

		//--

		ui->AddSpacing();

		//if (ui->isMaximized()) ui->AddLabelBig("PICTURE");

		// Pict name
		//if (ui->isMaximized()) ui->AddLabel(currentImage_name.get());

		//--

		// 2. Image Preview

		if (tex.isAllocated())
		{
			float w = tex.getWidth();
			float h = tex.getHeight();
			float ratio = h / w;

			float ww;
			float hh;

			//TODO: weird flick. should be border size..
			float offset = 0;
			//float offset = 20;

			ww = _w100 - 2 * _spcx - offset;
			hh = ww * ratio;

			const float H_MAX = 300;
			if (hh > H_MAX)
			{
				ww = H_MAX / ratio - 2 * _spcx;
			}

			//if (w *ratio > 400)

			//float hMax = 400;
			//if (h > hMax) 
			//{
			//	h = hMax;
			//	w = h / ratio;
			//	ww = _w50;
			//}
			//else
			//{
			//	ww = _w99 - 20; // hardcoded pad to avoid flickering bug...
			//}

			if (ImGui::ImageButton(
				(ImTextureID)(uintptr_t)fbo.getTexture(0).getTextureData().textureID,
				ImVec2(ww, hh)))
			{
				ofLogNotice("ofxColorQuantizerHelper") << "Image clicked";

				doSortNext();
			}

			//--

			ImGuiIO& io = ImGui::GetIO();
			ImRect rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
			ImVec2 mouseUVCoord = (io.MousePos - rc.Min) / rc.GetSize();
			mouseUVCoord.y = 1.f - mouseUVCoord.y;

			//Image pickerImage;

			// Obtener un const unsigned char* const a partir de la textura
			//const unsigned char* const bits = static_cast<const unsigned char* const>(tex.getTextureData().textureID);
			//const unsigned char* const bits = static_cast<const unsigned char* const>(tex.getTextureData().textureID);
			//const unsigned char* const bits = static_cast<const unsigned char* const>(tex.getTextureData().textureTarget);
			//const unsigned char* const bits = tex.getTextureData().textureID;
			//const unsigned char* const bits = textureSourceID[0];
			//const unsigned char* const bits = (ImTextureID)(uintptr_t)fbo.getTexture(0).getTextureData().textureID;

			ImVec2 displayedTextureSize = ImVec2(ww, hh);

			if (io.KeyShift && io.MouseDown[0] && mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f)
			{
				//int width = ww;
				//int height = hh;

				//ImageInspect::inspect(width, height, data, mouseUVCoord, displayedTextureSize);
				//ImageInspect::inspect(width, height, (void*)data, mouseUVCoord, displayedTextureSize);

				//ImageInspect::inspect(width, height, pickerImage.GetBits(), mouseUVCoord, displayedTextureSize);
				//ImageInspect::inspect(width, height, bits, mouseUVCoord, displayedTextureSize);
			}

			//--
		}

		//--

		ui->AddSpacing();
		ui->AddSpacing();

		//--

		// 1. Palette colors

		const auto p = getPalette(true);

		// fit width
		float wb = (_w100 / (int)p.size()); // -_spc;

		//TODO: fix width
		//ui->AddLabelBig("COLORS");

		for (int n = 0; n < p.size(); n++)
		{
			ImGui::PushID(n);

			// fit width
			if (n != 0) ImGui::SameLine(0, 0);

			// box colors
			if (ImGui::ColorButton("##paletteQuantizer", p[n], colorEdiFlags, ImVec2(wb, hb * 2)))
			{
				doUpdatePointerColor(n);
			}

			ImGui::PopID();
		}

		ui->Add(amountColors, OFX_IM_STEPPER);
		ui->AddSpacing();

		//--

		// Gui parameters

		if (ui->AddButton("SORT", OFX_IM_BUTTON, 1))
		{
			doSortNext();
		}

		ui->AddLabel(sortedType_name);

		if (ui->isMaximized())
		{
			ui->AddSpacingSeparated();
			ui->AddAdvancedToggle();
			ui->AddSpacing();

			//if (ImGui::CollapsingHeader("ADVANCED"))
			if (ui->isAdvanced())
			{
				ui->Indent();

				ui->Add(bReBuild, OFX_IM_BUTTON, 2);

				ui->SameLine();

				if (ui->AddButton("Refresh", OFX_IM_BUTTON, 2))
				{
					// workflow
					// to allow add more files on runtime
					int currentImage_ = currentImage;
					refreshFiles();
					currentImage = currentImage_;
					buildQuantize();
				}

				//ui->AddSpacing();

				if (ui->AddButton("Open File", OFX_IM_BUTTON_SMALL))
				{
					setImage();
				}

				//--

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL

				ui->AddSpacingSeparated();

				//if (ImGui::CollapsingHeader("LIBRARY", ImGuiTreeNodeFlags_None))
				//ui->Add(bGui_Library, OFX_IM_TOGGLE_SMALL, 1);
				if (bGui_Library)
				{
					if (ui->isMaximized()) ui->AddLabelBig("THUMBS");

					//if (!bResponsive)
					{
						SurfingGuiTypes t = OFX_IM_HSLIDER_MINI;
						//SurfingGuiTypes t = OFX_IM_STEPPER;

						ui->Add(thumbsSize, t);
						ui->Add(thumbsSpacing, t);
						ui->Add(thumbsBorder, t);

						//if (ui->AddButton("Fit", OFX_IM_BUTTON_SMALL, 2))
						//{
						//	thumbsSize = __widthPicts;
						//}
					}

					ui->AddSpacing();
					if (ui->AddButton("Reset", OFX_IM_BUTTON))
					{
						doReset();
					};
				}
#endif
				ui->AddSpacingSeparated();

				if (bUseNativeWidgets)
				{
					ui->Add(bGui_Help, OFX_IM_TOGGLE_ROUNDED_MINI);
					ui->Add(bGui_WidgetInfo, OFX_IM_TOGGLE_ROUNDED_MINI);
				}
				ui->Add(bKeys, OFX_IM_TOGGLE_ROUNDED_MINI);
				ui->Add(ui->bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);

				ui->Unindent();
			}
		}

		//----
#endif
		ui->EndWindow();
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw_ImGuiLibrary()
{
#ifdef USE_IM_GUI__QUANTIZER_INTERNAL

	if (!bGui_Library) return;

	static int _countPerRow = 1;

	ImGuiStyle& style = ImGui::GetStyle();
	int _amountImages = dir.size();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	if (bAutoResizeLib)
	{
		window_flags += ImGuiWindowFlags_AlwaysAutoResize;
	}

	//float offset = 0;
	float offset = 2 * style.WindowPadding.x;

	for (int i = 0; i < _countPerRow; i++)
	{
		offset += style.FrameBorderSize;
		offset += style.FramePadding.x;
		offset += style.ItemSpacing.x;
		offset += style.ItemInnerSpacing.x;
	}

	// reset
	float wdef = 400;
	float hdef = ofGetHeight() / 2;

	// constraints
	float wmax = ofGetWidth();
	float hmax = ofGetHeight() - 50;

	// workround fix grid size troubles..
	bool bFix = 1;
	if (bFix) ImGui::SetNextWindowSizeConstraints(ImVec2(thumbsSize + offset, wdef), ImVec2(wmax, hmax));
	//if (bFix) ImGui::SetNextWindowSizeConstraints(ImVec2(thumbsSize, wdef), ImVec2(wmax, hmax));

	ImGuiCond cond = ImGuiCond_FirstUseEver;

	if (bDoResetLib) {
		bDoResetLib = false;
		cond = ImGuiCond_Always;
	}

	ImGui::SetNextWindowSize(ImVec2(wdef, hdef), cond);

	if (bGui_Picture && bGuiLinked) ui->setNextWindowAfterWindowNamed(bGui_Picture);

	//--

	if (ui->BeginWindow(bGui_Library, window_flags))
	{
		currentImage_ = currentImage;

		//--

		ImVec2 szImgButton;
		float _ww;
		float _hh;
		float _ratio;

		//TODO; responsive
		//ImVec2 szImgButton((float)thumbsSize.get(), (float)thumbsSize.get());
		//
		//__widthPicts = _w100 - _spcx;
		//__widthPicts -= ImGui::GetStyle().ItemInnerSpacing.x;
		// 
		//if (bResponsive.get())
		//{
		//	static int sizeThumb_ = 1;
		//	if (sizeThumb_ != thumbsSize.get())
		//	{
		//		sizeThumb_ = thumbsSize;
		//		//thumbsSize = __widthPicts;
		//		//_ww = thumbsSize;
		//		_ww = sizeThumb_;
		//	}
		//	else _ww = thumbsSize;
		//}
		//else
		//{
		//	//if (bResponsive) _ww = __widthPicts;
		//	//else _ww = (float)thumbsSize.get();
		//	_ww = (float)thumbsSize.get();
		//}

		_ww = (float)thumbsSize.get();

		//--

		//TODO:
		//_countPerRow = 0;

		for (int n = 0; n < _amountImages; n++)
		{
			// Respect aspect ratio images with width fit
			_ratio = textureSource[n].getHeight() / textureSource[n].getWidth();
			_hh = _ww * _ratio;
			szImgButton = ImVec2(_ww, _hh);

			//--

			ImGui::PushID(n);

			// Tweak spacing and border
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, (float)thumbsBorder.get());
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((float)thumbsSpacing.get(), (float)thumbsSpacing.get()));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2((float)thumbsSpacing.get(), (float)thumbsSpacing.get()));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2((float)thumbsSpacing.get(), (float)thumbsSpacing.get()));

			//string name = ofToString(n);

			// Customize colors
			if (n == currentImage_) // when selected
			{
				ofFloatColor c;
				ofxSurfingHelpers::setColorFadeBlinkBW(c);
				ImGui::PushStyleColor(ImGuiCol_Border, c);

				//float a = ofxSurfingHelpers::getFadeBlink();
				////float a = 0.7;
				//const ImVec4 color1_ = style.Colors[ImGuiCol_Text];
				//const ImVec4 color1 = ImVec4(color1_.x, color1_.y, color1_.z, color1_.w * a);
				//ImGui::PushStyleColor(ImGuiCol_Border, color1);

				////const ImVec4 color1 = style.Colors[ImGuiCol_ButtonActive];
				////ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color1);
				////ImGui::PushStyleColor(ImGuiCol_Button, color1);
			}
			else // when not selected
			{
				const ImVec4 color2 = style.Colors[ImGuiCol_Button]; // do not changes the color
				ImGui::PushStyleColor(ImGuiCol_Border, color2);
				//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color2);
			}

			//--

			// Image button

			bool b = (ImGui::ImageButton((ImTextureID)(uintptr_t)textureSourceID[n], szImgButton));
			//bool b = (ImGui::ImageButton(GetImTextureID(textureSourceID[n]), szImgButton));
			if (b)
			{
				ofLogNotice("ofxColorQuantizerHelper") << "THUMB: #" + ofToString(n) + " / " + dir.getName(n);

				currentImage_ = n;

				//--

				// Here will trig the callback!
				currentImage = currentImage_;
			}

			//--

			// Customize colors
			ImGui::PopStyleColor(); // border
			//ImGui::PopStyleColor(); // hovered

			//--

			// Spacing and border
			ImGui::PopStyleVar(4);
			//ImGui::PopStyleVar();
			//ImGui::PopStyleVar();
			//ImGui::PopStyleVar();
			//ImGui::PopStyleVar();

			ImGui::PopID();

			float xBorderRight = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			float xLastButton = ImGui::GetItemRectMax().x;
			float xNextButton = xLastButton + style.ItemSpacing.x + szImgButton.x;

			// Expected position if next button was on same line
			if ((n < _amountImages - 1) // last one do not requires same line
				&& (xNextButton < xBorderRight))
			{
				ui->SameLine();
			}
			//else _countPerRow++;
		}

		ui->EndWindow();
	}
#endif
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw_ImGuiWidgets()
{
	//TODO:
	////Image pickerImage;
	//float w = tex.getWidth();
	//float h = tex.getHeight();
	//auto io = ImGui::GetIO();

	//ImGui::ImageButton((ImTextureID)(uintptr_t)fbo.getTexture(0).getTextureData().textureID, ImVec2(w, h));
	//ImRect rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	//ImVec2 mouseUVCoord = (io.MousePos - rc.Min) / rc.GetSize();
	//mouseUVCoord.y = 1.f - mouseUVCoord.y;

	//if (io.KeyShift && io.MouseDown[0] && mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f)
	//{
	//	//int width = pickerImage.mWidth;
	//	//int height = pickerImage.mHeight;

	//	//imageInspect(w, h, pickerImage.GetBits(), mouseUVCoord, displayedTextureSize);
	//}

	//--

	if (!bGui) return;

	draw_ImGuiPicture();
	draw_ImGuiLibrary();
}

//----

// Sorting helpers

#include "ColorConverter.h"

bool comparePos(const colorMapping& s1, const colorMapping& s2)
{
	return s1.pos < s2.pos;
}
bool compareBrightness(const colorMapping& s1, const colorMapping& s2)
{
	return s1.color.getBrightness() < s2.color.getBrightness();
}
bool compareHue(const colorMapping& s1, const colorMapping& s2)
{
	return s1.color.getHue() < s2.color.getHue();
}
bool compareSaturation(const colorMapping& s1, const colorMapping& s2)
{
	return s1.color.getSaturation() < s2.color.getSaturation();
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::loadPrev()
{
	if (currentImage <= 0) currentImage = dir.size() - 1;
	else currentImage--;

	ofLogNotice("ofxColorQuantizerHelper") << "loadPrev: " << ofToString(currentImage);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::loadNext()
{
	if (currentImage < dir.size() - 1) currentImage++;
	else if (currentImage == dir.size() - 1) currentImage = 0;

	ofLogNotice("ofxColorQuantizerHelper") << "loadNext: " << ofToString(currentImage);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::randomPalette()
{
	int pmin, pmax;
	pmin = 0;
	pmax = dir.size();
	currentImage = ofRandom(pmin, pmax);

	ofLogNotice("ofxColorQuantizerHelper") << "randomPalette: " << ofToString(currentImage);

	//ofLogNotice("ofxColorQuantizerHelper") << "currentImage: " << ofToString(currentImage);
	//if (dir.size() > 0 && currentImage < dir.size())
	//{
	//	imageName = dir.getName(currentImage);
	//	imageName_path = dir.getPath(currentImage);
	//	buildFromImageFile(imageName_path, amountColors);
	//}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::refreshFiles()
{
	// Load dragged images folder
	ofLogNotice("ofxColorQuantizerHelper") << "list files " << pathFolerDrag;

	dir.listDir(pathFolerDrag);
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.allowExt("JPG");
	dir.allowExt("PNG");
	dir.sort();

	// Startup quantizer color
	//imageName_path = "0.jpg";
	//imageName = "0";
	//buildFromImageFile(pathFolder + imageName_path, amountColors);

	// Load first file in dir
	if (dir.size() > 0) currentImage = 0;
	else currentImage = -1;

	if (dir.size() > 0 && currentImage < dir.size() - 1)
	{
		imageName = dir.getName(currentImage);
		imageName_path = dir.getPath(currentImage);

		buildFromImageFile(imageName_path, amountColors);
	}

	// Log files on folder
	imageNames.clear();
	for (int i = 0; i < dir.size(); i++)
	{
		ofLogNotice("ofxColorQuantizerHelper") << "#" << ofToString(i) << " " << dir.getName(i);

		imageNames.push_back(dir.getName(i));
	}
	currentImage.setMax(dir.size() - 1);

	//--

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	//TODO:
	// Preview image / grid picker
	textureSource.clear();
	textureSource.resize(dir.size());
	textureSourceID.clear();
	textureSourceID.resize(dir.size());

	for (int i = 0; i < dir.size(); i++)
	{
		auto _ui = ui->getGuiPtr();
		textureSourceID[i] = _ui->loadTexture(textureSource[i], dir.getPath(i));
		//textureSourceID[i] = ofxImGuiSurfing::load(textureSource[i], dir.getPath(i));
	}
#endif
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::refreshFilesSorting(std::string name)
{
	ofLogNotice("ofxColorQuantizerHelper") << "----------------- SORTING FILES-----------------";
	ofLogNotice("ofxColorQuantizerHelper") << "Search Iindex for file:";
	ofLogNotice("ofxColorQuantizerHelper") << name;

	// Load dragged images folder
	ofLogNotice("ofxColorQuantizerHelper") << "list files " << pathFolerDrag;

	dir.listDir(pathFolerDrag);
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.allowExt("JPG");
	dir.allowExt("PNG");
	dir.sort();

	// Log files on folder
	imageNames.clear();
	for (int i = 0; i < dir.size(); i++)
	{
		ofLogNotice("ofxColorQuantizerHelper") << "file " << "[" << ofToString(i) << "] " << dir.getName(i);

		imageNames.push_back(dir.getName(i));
	}
	currentImage.setMax(dir.size() - 1);

	// Go to ne index after adding a new preset
	ofLogNotice("ofxColorQuantizerHelper") << "Rearrenge index file";

	// Locate new position of old (saved) preset
	int ii = -1;

	for (int i = 0; i < dir.size() && ii == -1; i++)
	{
		std::string _name = dir.getName(i);

		ofLogNotice("ofxColorQuantizerHelper") << i << " " << imageNames[i];

		if (_name == name)//found file index of the preset
		{
			ii = i;
			ofLogNotice("ofxColorQuantizerHelper") << "Found " << _name << " at index " << i << " !";

			continue;
		}
	}

	// Reload the same preset at newer index
	if (ii != -1) // found the name
	{
		currentImage = ii;

		// Load
		imageName = dir.getName(currentImage);
		imageName_path = dir.getPath(currentImage);
		buildFromImageFile(imageName_path, amountColors);
	}
	else // not found
	{
		ofLogError("ofxColorQuantizerHelper") << name << " filename not Found !";
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setup()
{
	//--

	// Layout
	if (bUseNativeWidgets)
	{
		setPosition(glm::vec2(370, 650));
		setSize(glm::vec2(1000, 700));//?
	}

	//--

	//if (bUseNativeWidgets)
	{
		infoHelp = "IMAGE QUANTIZER \n\n";
		infoHelp += "H                   HELP";
		infoHelp += "\n";
		infoHelp += "   +|-              AMOUNT COLORS";
		infoHelp += "\n";
		infoHelp += "Left|Right          BROWSE IMAGES";
		infoHelp += "\n";
		infoHelp += "  Up|Down           SORT TYPE";
		infoHelp += "\n";
		//infoHelp += "Path/Url         " + currentImage_name.get();
		infoHelp += "\n";
		infoHelp += "Drag images here!";
	}

	//--

#ifdef USE_OFX_GUI__QUANTIZER
	ofxSurfingHelpers::setThemeDark_ofxGui();
#endif

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL

	params_Thumbs.setName("THUMBS");

	thumbsSize.set("Size", 100, 40, 300);
	thumbsSpacing.set("Spacing", 5, 0, 20);
	thumbsBorder.set("Border", 1, 0, 10);

	params_Thumbs.add(thumbsSize);
	params_Thumbs.add(thumbsSpacing);
	params_Thumbs.add(thumbsBorder);
#endif

	//--

	parameters.setName("IMAGE QUANTIZER");
	parameters.add(bReBuild.set("Rebuild", false));
	parameters.add(amountColors.set("Colors", 10, 1, 20));
	parameters.add(sortedType.set("Sort Type", 1, 1, 4));
	parameters.add(sortedType_name.set("SortedTypeName", sortedType_name));
	parameters.add(currentImage.set("FileIndex", 0, 0, dir.size() - 1));
	parameters.add(currentImage_name.set("FileName", ""));
	//parameters.add(labelUrlStr.set("type url", labelUrlStr));

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	parameters.add(bGui_Library.set("LIBRARY", true));
	parameters.add(bGui_Picture.set("PICTURE", true));
	parameters.add(bGuiLinked.set("Link", true));
	parameters.add(thumbsSize);
	//parameters.add(bResponsive.set("Responsive", true));

	bAutoResizeLib.set("Auto Resize Lib", true);
	parameters.add(bAutoResizeLib);
#endif

	parameters_Advanced.setName("ADVANCED");
	if (bUseNativeWidgets)
	{
		parameters_Advanced.add(bGui_Help.set("Help", false));
		parameters_Advanced.add(bGui_WidgetInfo.set("Info", false));
	}
	parameters_Advanced.add(bKeys.set("Keys", true));
	parameters_Advanced.add(bGui.set("IMAGE QUANTIZER", true));

	currentImage_name.setSerializable(false);
	bReBuild.setSerializable(false);

	ofAddListener(parameters.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);
	ofAddListener(parameters_Advanced.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);

	//--

#ifdef USE_OFX_GUI__QUANTIZER
	gui.setup("ofxColorQuantizerHelper");
	gui.add(parameters);
	gui.add(parameters_Advanced);
	gui.setPosition(500, 5);
#endif

	//--

	font.load("assets/fonts/JetBrainsMono-Bold.ttf", 11, true, true);
	font2.load("assets/fonts/JetBrainsMono-Bold.ttf", 6, true, true);

	//--

	refreshFiles();

	//----

	// Startup settings

	params_AppSettings.setName("ofxColorQuantizerHelper");
	params_AppSettings.add(amountColors);
	params_AppSettings.add(currentImage);
	params_AppSettings.add(sortedType);
	params_AppSettings.add(bKeys);
	if (bUseNativeWidgets)
	{
		params_AppSettings.add(bGui_WidgetInfo);
		params_AppSettings.add(bGui_Help);
	}
#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_Picture);
	params_AppSettings.add(bGui_Library);
	params_AppSettings.add(bGuiLinked);
	params_AppSettings.add(bAutoResizeLib);
	params_AppSettings.add(params_Thumbs);
	//params_AppSettings.add(bResponsive);
#endif

	doReset();

	loadAppSettings(params_AppSettings, path_Global + path_AppSettings);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw()
{
	bool bCenter = true;

	//if (ui->bDebug && ui->isMaximized() && ui->bAdvanced)
	if (ui->bDebug)
	{
		// Debug elapsed time to process image quantizer
		if (imageSmall.isAllocated())
		{
			ofPushStyle();

			float p = 6;
			float p2 = 5;

			float w = imageSmall.getWidth();
			float h = imageSmall.getHeight();
			float x = ofGetWidth() - w - p;
			float y = ofGetHeight() - h - p;

			//bg
			ofRectangle r(x, y, w, h);
			r.setWidth(r.width + p2);
			r.setHeight(r.height + p2);
			r.translateX(-p2 / 2.f);
			r.translateY(-p2 / 2.f);
			ofFill();
			ofSetColor(0, 225);
			ofDrawRectRounded(r, 3);

			//img
			ofSetColor(255, 255);
			imageSmall.draw(x, y);

			//txt
			ofDrawBitmapStringHighlight(ofToString(d1, 0) + " ms", x + 4, y + 14);

			ofPopStyle();
		}
	}

	//--

	//if (isLoadedImage && bGui && bGui_Info)
	if (bUseNativeWidgets)
	{
		if (bGui_WidgetInfo)
		{
			ofPushStyle();

			// 1. Debug big window
			//if (!bGui_Help)
			{
				ofPushMatrix();
				{
					//--

					// pad between boxes
					boxPad = 0;
					//boxPad = 2;
					int margin = 5;
					//int space = 40;

					//----

					// 1. Image

					int imgW = 200;//set all sizes relatives to this

					// Draw original image but resized to imgW pixels width, same aspect ratio
					float imgRatio = image.getHeight() / image.getWidth();
					int imgH = imgRatio * imgW;

					//--

					// 2. Resize box sizes

					size = glm::vec2(500, 400);
					//size = glm::vec2(1440, 900);

					wPal = size.x - (margin + imgW + margin);
					boxW = wPal / colorQuantizer.getNumColors();
					boxBgSize = boxW - boxPad;
					boxSize_h = 50;

					setPosition(glm::vec2(370, ofGetHeight() - imgH - 50));

					//--

					// 3. Big bg box

					//boxSize_h = boxBgSize;
					float _wb = boxBgSize + boxPad;
					float _ww = colorQuantizer.getNumColors() * boxBgSize;

					//--

					// 4. Layout modes

					if (!bCenter) {
						int x = position.x;//x pad for left/right window
						int y = position.y;
						ofTranslate(x, y);
					}
					else {
						int y = position.y;
						ofTranslate(ofGetWidth() * 0.5 - _ww * 0.5 - imgW, y);
					}

					// Image
					ofSetColor(255, 255);
					image.draw(0, 0, imgW, imgH);

					ofSetColor(255, 100);
					ofSetLineWidth(1.0);
					ofNoFill();
					ofDrawRectangle(0, 0, imgW, imgH);

					//--

					ofTranslate(0, imgH);

					//// Debug info
					//if (sortedType == 1)
					//{
					//    ofSetColor(255, 100);
					//    ofDrawBitmapString("(Original sorting has colors weighted based on their areas, their order is based on their chroma values)", 0, 50);
					//}

					//--

					// 4. All colors % bars
					int _space = 10;
					ofTranslate(imgW + _space, 0);

					// Bg box
					ofSetColor(0, 128);
					ofFill();
					ofDrawRectangle(0, 0, _ww, -imgH);

					// Bg box border
					ofSetColor(255, 100);
					ofSetLineWidth(1.0);
					ofNoFill();
					ofDrawRectangle(0, 0, _ww, -imgH);

					// % Colors
					ofFill();
					for (int i = 0; i < colorQuantizer.getNumColors(); i++)
					{
						// Box scaled
						ofSetColor(sortedColors[i].color, 255);
						ofDrawRectangle(i * _wb, 0, boxBgSize, ofMap(sortedColors[i].weight, 0, 1, 0, -imgH));

						// Label
						ofSetColor(255, 255);
						//ofDrawBitmapString(ofToString(int(sortedColors[i].weight * 100)) + "%", i * _wb, 15);
						font2.drawString(ofToString(int(sortedColors[i].weight * 100)) + "%", i * _wb + 8, 15);
					}

					//// Palette preview
					//ofTranslate(0, space);
					////// debug text
					////std::string str = "sorted palette preview (" + ofToString(sortedType_name) + ")";
					////ofDrawBitmapStringHighlight(str, 5, -10, ofColor::black, ofColor::white);
					//draw_Palette_Preview();
				}
				ofPopMatrix();
			}

			//--

			ofPopStyle();
		}

		//----

		// 1. Help info
		if (bGui_Help)
		{
			std::string str = infoHelp;
			ofPushStyle();
			ofPushMatrix();
			{
				// Center box
				int w = ofxSurfingHelpers::getWidthBBtextBoxed(font, str);
				int h = ofxSurfingHelpers::getHeightBBtextBoxed(font, str);
				ofTranslate(ofGetWidth() * 0.5 - w * 0.5, ofGetHeight() * 0.5 - h * 0.5);
				ofSetColor(255, 255);
				ofxSurfingHelpers::drawTextBoxed(font, str);
			}
			ofPopMatrix();
			ofPopStyle();
		}
	}

	//--

#ifdef USE_OFX_GUI__QUANTIZER
	if (bGui) gui.draw();
#endif
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::doUpdatePointerColor(int n)
{
	// Assign color back!
	if (palette.size() > 0 && myColor_BACK != nullptr)
	{
		// set BACK color clicked
		myColor_BACK->set(colorMapSortable[n].color);
		//myColor_BACK->set(palette[n]); // auto get first color from palette
		//myColor_BACK->set(colorMap[n]); // auto get first color from palette

		// flag updater color ready
		if (bUpdated_Color_BACK != nullptr)
		{
			(*bUpdated_Color_BACK) = true;
		}
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::buildQuantize()
{
	//return;
	//TODO: Here could be done the threading process!
	static uint32_t t0;
	t0 = ofGetElapsedTimeMillis();

	if (isLoadedImage && imageSmall.isAllocated())
	{
		int _max = amountColors;
		//int _max = amountColors + 1;
		//int _max = colorQuantizer.getNumColors();
		//int _max = colorQuantizer.getNumColors() + 1;

		colorQuantizer.setNumColors(_max);
		colorQuantizer.quantize(imageSmall.getPixels());

		sortedColors.clear();;
		//sortedColors.resize(colorQuantizer.getNumColors());
		sortedColors.resize(_max);

		for (int i = 0; i < _max; i++)
		{
			ofFloatColor fc = ofFloatColor(
				colorQuantizer.getColors()[i].r / 255.0f,
				colorQuantizer.getColors()[i].g / 255.0f,
				colorQuantizer.getColors()[i].b / 255.0f);
			ofVec3f labCol = ColorConverter::rgbToLab(fc);

			sortedColors[i].distance = ColorConverter::calcChroma(labCol);
			sortedColors[i].color = colorQuantizer.getColors()[i];
			sortedColors[i].weight = colorQuantizer.getColorWeights()[i];
		}

		std::sort(sortedColors.begin(), sortedColors.end(), by_distance());

		//TODO:
		d1 = ofGetElapsedTimeMillis() - t0;
		auto t1 = ofGetElapsedTimeMillis();
		ofLogNotice("ofxColorQuantizerHelper") << "Time Quantizer: \n" << d1 << "ms";

		//--

		buildSorting();

		////TODO:
		//auto t2 = ofGetElapsedTimeMillis();
		//auto d2 = t2 - t1;
		//ofLogNotice("ofxColorQuantizerHelper") << "Time Quantizer (SORT): \n" << d2 << "ms";

		//ofLogNotice("ofxColorQuantizerHelper") << "Done";
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::loadImageAndQuantize(std::string _pathImg, int _numColors)
{
	if (amountColors.get() != _numColors) {
		amountColors = _numColors;
	}

	// Cargar una imagen desde un archivo usando stb_image
	///*unsigned char**/ data = stbi_load(_pathImg.c_str(), &width, &height, &channels, 0);


	isLoadedImage = image.load(_pathImg);
	if (isLoadedImage)
	{
		ofLogNotice("ofxColorQuantizerHelper") << "loadImageAndQuantize: " << _pathImg;

		// Resize smaller to speed up quantizer
		imageSmall.clear();
		imageSmall = image;
		//imageSmall.load(_pathImg);

		//TODO:
		float iw = image.getWidth();
		float ih = image.getHeight();
		float sz = MAX(iw, ih);

		////float factor = 8.f;
		//float factor = 4.f;
		//if (sz > 1000) factor *= 1.5f;

		// reduce proportionally to pict size!
		float factor = ofMap(iw, 10, 1920, 2, 6);

		imageSmall.resize(imageSmall.getWidth() / factor, imageSmall.getHeight() / factor);

		buildQuantize();
	}
	else
	{
		ofLogError("ofxColorQuantizerHelper") << "image file not found! " << _pathImg;
	}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::Changed_parameters(ofAbstractParameter& e)
{
	std::string _name = e.getName();

	ofLogNotice("ofxColorQuantizerHelper") << _name << ": " << e;

	if (0) {}

	// Index
	else if (_name == currentImage.getName())
	{
		ofLogNotice("ofxColorQuantizerHelper") << "currentImage: " << ofToString(currentImage);

		static int currentImage_ = -1;
		if (currentImage.get() != currentImage_) {
			currentImage_ = currentImage;
		}
		else return;

		if (dir.size() > 0 && currentImage <= dir.size() - 1)
		{
			imageName = dir.getName(currentImage);
			currentImage_name = imageName;
			imageName_path = dir.getPath(currentImage);

			buildFromImageFile(imageName_path, amountColors);
		}

		bUpdate = true;

		refreshImageGuiTexture();
	}

	// Sort
	else if (_name == sortedType.getName())
	{
		static int sortedType_PRE = -1;
		if (sortedType.get() != sortedType_PRE) {
			sortedType_PRE = sortedType;
		}
		else return;

		if (sortedType < 1 && sortedType > 4) {
			sortedType.setWithoutEventNotifications(ofClamp(sortedType, 1, 4));
			return;
		}

		buildSorting();
	}

	else if (_name == bReBuild.getName())
	{
		if (bReBuild)
		{
			bReBuild = false;
			buildQuantize();
		}
	}

	else if (_name == amountColors.getName())
	{
		static int amountColors_PRE = -1;
		if (amountColors != amountColors_PRE) {
			amountColors_PRE = amountColors;
		}
		else return;

		amountColors = ofClamp(amountColors, amountColors.getMin(), amountColors.getMax());

		if (amountColors < amountColors.getMin() && amountColors > amountColors.getMax()) return;

		// workflow
		buildQuantize();
	}

	//TODO: allow drag internet browser images..
	//else if (_name == "type url")
	//{
	//	imageName_path = labelUrlStr;
	//	buildFromImageFile(imageName_path, amountColors);
	//	ofLogNotice("ofxColorQuantizerHelper") << "type url: " << imageName_path;
	//	ofLogNotice("ofxColorQuantizerHelper") << "type url: " << imageName_path;
	//}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::buildSorting()
{
	int sz = amountColors;
	//int sz = colorQuantizer.getNumColors();

	palette.clear();
	palette.resize(sz);

	for (int i = 0; i < sortedColors.size(); i++)
	{
		palette[i] = sortedColors[i].color;
	}

	colorMap.clear();
	colorMapSortable.clear();

	for (int i = 0; i < sz; i++)
	{
		colorMap[i] = palette[i];
	}

	// Color Map
	for (unsigned int i = 0; i < sz; i++)
	{
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
		sortedType_name = "Original";
		break;
	case 2:
		ofSort(colorMapSortable, compareHue);
		sortedType_name = "Hue";
		break;
	case 3:
		ofSort(colorMapSortable, compareBrightness);
		sortedType_name = "Brightness";
		break;
	case 4:
		ofSort(colorMapSortable, compareSaturation);
		sortedType_name = "Saturation";
		break;
	default:
		break;
	}

	//--

	// Pointers back to 'communicate externally'
	doUpdatePointers();
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::doUpdatePointers()
{
	// Palette
	int szPalette = palette.size();

	if (szPalette > 0 && myPalette_BACK != nullptr)
	{
		// set BACK name clicked
		if (myPalette_Name_BACK != nullptr)
		{
			(*myPalette_Name_BACK) = imageName;
		}

		//--

		// Set BACK palette colors
		myPalette_BACK->clear();
		myPalette_BACK->resize(szPalette);

		//// 1. Get unsorted palette
		//(*myPalette_BACK) = palette;

		// 2. Get palette sorted
		for (int col = 0; col < palette.size(); col++)
		{
			(*myPalette_BACK)[col] = colorMapSortable[col].color;
		}

		// Mark update flag
		if (bUpdated_Palette_BACK != nullptr)
		{
			(*bUpdated_Palette_BACK) = true;
		}
	}

	// Color
	doUpdatePointerColor(0);
	//if (szPalette > 0 && myColor_BACK != nullptr)
	//{
	//	// Set BACK color clicked
	//	myColor_BACK->set(colorMapSortable[0].color); // auto get first color from palette
	//	//myColor_BACK->set(palette[0]); // auto get first color from palette
	//	//myColor_BACK->set(colorMap[0]); // auto get first color from palette

	//	// Flag updater color ready to refresh
	//	if (bUpdated_Color_BACK != nullptr)
	//	{
	//		(*bUpdated_Color_BACK) = true;
	//	}
	//}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::buildFromImageFile(std::string path, int num)
{
	//return;

	//TODO: improve with threading load..

	loadImageAndQuantize(path, num);
	//loadImageAndQuantize(pathFolder + path, num);
}

//TODO:
//--------------------------------------------------------------
void ofxColorQuantizerHelper::buildFromImageUrl(std::string url, int num)
{
	//TODO: improve with threading load and some HTTP image browsing api..
	loadImageAndQuantize(url, num);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::draw_Palette_Preview()
{
	ofPushMatrix();
	ofPushStyle();

	ofRectangle r;
	r = ofRectangle(0, 0, boxBgSize, boxSize_h);

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

		ofTranslate(boxBgSize + boxPad, 0);
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

	//ofAddListener(parameters.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);
	//ofAddListener(parameters_Advanced.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::keyPressed(ofKeyEventArgs& eventArgs)
{
	if (!bKeys) return;

	const int& key = eventArgs.key;
	ofLogNotice("ofxColorQuantizerHelper") << "key: " << key;

	if (key == 'G')
	{
		bGui = !bGui;
	}

	if (bUseNativeWidgets)
	{
		if (key == 'H')
		{
			bGui_Help = !bGui_Help;
		}
	}

	//-

	// dragged files

	if (key == '-')
	{
		amountColors--;
		if (amountColors < amountColors.getMin()) amountColors = amountColors.getMin();
		bReBuild = true;

		//if (amountColors < amountColors.getMin()) amountColors = amountColors.getMin();
	}
	if (key == '+')
	{
		amountColors++;
		if (amountColors > amountColors.getMax()) amountColors = amountColors.getMax();
		bReBuild = true;

		// if (amountColors > amountColors.getMax()) amountColors = amountColors.getMax();
	}

	if (key == OF_KEY_UP)
	{
		loadPrev();
	}
	if (key == OF_KEY_DOWN)// || key == ' ')
	{
		loadNext();
	}

	if (key == OF_KEY_RETURN)
	{
		randomPalette();
	}

	//-

	// sort types
	//if (key == OF_KEY_LEFT)
	//{
	//	sortedType++;
	//	if (sortedType > 4) sortedType = 1;
	//}
	//if (key == OF_KEY_RIGHT)
	//{
	//	sortedType--;
	//	if (sortedType < 1) sortedType = 4;
	//}

	if (key == 's' || key == OF_KEY_BACKSPACE)
	{
		sortedType++;
		if (sortedType > 4)
			sortedType = 1;
	}

	//-

	//// test kMeansTest
	//if (key == 'k')
	//{
	//    kMeansTest();
	//}
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::keyReleased(ofKeyEventArgs& eventArgs)
{
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
void ofxColorQuantizerHelper::mouseDragged(ofMouseEventArgs& eventArgs)
{
	const int& x = eventArgs.x;
	const int& y = eventArgs.y;
	const int& button = eventArgs.button;
	//ofLogNotice("ofxColorQuantizerHelper") << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mousePressed(ofMouseEventArgs& eventArgs)
{
	const int& x = eventArgs.x;
	const int& y = eventArgs.y;
	const int& button = eventArgs.button;
	//ofLogNotice("ofxColorQuantizerHelper") << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::mouseReleased(ofMouseEventArgs& eventArgs)
{
	const int& x = eventArgs.x;
	const int& y = eventArgs.y;
	const int& button = eventArgs.button;
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
	ofxSurfingHelpers::CheckFolder(path_Global);
	saveAppSettings(params_AppSettings, path_Global + path_AppSettings);
}

//--------------------------------------------------------------
ofxColorQuantizerHelper::~ofxColorQuantizerHelper()
{
	exit();

	//removeKeysListeners();
	//removeMouseListeners();
	setActive(false);

	// crashes?
	ofRemoveListener(parameters.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);
	ofRemoveListener(parameters_Advanced.parameterChangedE(), this, &ofxColorQuantizerHelper::Changed_parameters);

	//stbi_image_free(data);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::dragEvent(ofDragInfo& eventArgs)
{
	auto info = eventArgs;
	ofLogNotice("ofxColorQuantizerHelper");
	std::string fileName = "-1";

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

			//std::string fileName = "img_" + ofToString(ofGetTimestampString() + ".png");
			//draggedImages[k].save(pathFolerDrag + fileName);

			// 2. use original file name

			//TODO:
			// set define to choice an OS to adapt file system

			auto strs = ofSplitString(info.files[k], "\\");// Windows
			//auto strs = ofSplitString(info.files[k], "/");// macOS ?

			ofLogNotice("ofxColorQuantizerHelper") << "route: " << ofToString(strs);

			std::string fileName = strs[strs.size() - 1];
			ofLogNotice("ofxColorQuantizerHelper") << "fileName: " << ofToString(fileName);

			//TODO:
			// not working, not copying file!

			//pathFolerDrag = ofToDataPath();
			ofxSurfingHelpers::CheckFolder(pathFolerDrag);
			draggedImages[k].save(ofToDataPath(pathFolerDrag + fileName));
			//draggedImages[k].save(pathFolerDrag + fileName);
		}

		//-

		//// workflow

		//// refresh dir
		//refreshFiles();

		////-

		//// create palette from last file
		//imageName_path = info.files[info.files.size() - 1];
		////buildFromImageFile(imageName_path, amountColors);

		//-

		// workflow
		if (fileName != "-1") refreshFilesSorting(fileName);
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
void ofxColorQuantizerHelper::loadAppSettings(ofParameterGroup& g, std::string path)
{
	ofLogNotice("ofxColorQuantizerHelper") << "loadAppSettings " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::saveAppSettings(ofParameterGroup& g, std::string path)
{
	ofLogNotice("ofxColorQuantizerHelper") << "saveAppSettings " << path;

	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

//--

// Pointers back to 'communicate externally'

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setColorPtr(ofColor& c)
{
	myColor_BACK = &c;
}
//--------------------------------------------------------------
void ofxColorQuantizerHelper::setColor_BACK(ofColor& c)
{
	myColor_BACK = &c;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_BACK(vector<ofColor>& p)
{
	myPalette_BACK = &p;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_BACK_RefreshPalette(bool& b)
{
	bUpdated_Palette_BACK = &b;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setColor_BACK_Refresh(bool& b)
{
	bUpdated_Color_BACK = &b;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setPalette_BACK_Name(std::string& n)
{
	myPalette_Name_BACK = &n;
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::addImage(std::string path)
{
	ofLogNotice("ofxColorQuantizerHelper");

	ofFile file(path);

	auto strs = ofSplitString(path, "\\");// Windows
	//auto strs = ofSplitString(path, "/");// macOS ?

	ofLogNotice("ofxColorQuantizerHelper") << "route: " << ofToString(strs);

	std::string fileName = strs[strs.size() - 1];//last string
	ofLogNotice("ofxColorQuantizerHelper") << "fileName: " << ofToString(fileName);

	//-

	// copy file into images folder

	ofxSurfingHelpers::CheckFolder(pathFolerDrag);
	file.copyTo(ofToDataPath(pathFolerDrag + fileName));

	refreshFilesSorting(fileName);
}

//--------------------------------------------------------------
void ofxColorQuantizerHelper::setImage()
{
	ofLogNotice("ofxColorQuantizerHelper") << "----------------- SET IMAGE -----------------";

	//Open the Open File Dialog
	std::string str = "Select image file ";
	str += "to extract dominant colors ";
	str += "and create Palette\n";

	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

	//Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		string path = openFileResult.getPath();
		addImage(path);

		ofLogNotice("ofxColorQuantizerHelper") << "Image file to extract: " << path;

		// workflow
		//SHOW_Quantizer = true;
	}
	else
	{
		ofLogNotice("ofxColorQuantizerHelper") << "User hit cancel";
	}
}