ofxColorQuantizerHelper
-----------------------

**openFrameworks** add-on to get the dominant colors of a picture.  
Helps the integration of the original **ofxColorQuantizer** coded by @**mantissa** into an OF app. 

## Screenshot
![](/example_ColorQuatizer/Capture.PNG)

## Overview
* Get a palette with the dominant colors of a picture.  
* Set the desired number of colors.
* Different sorting modes: by **Hue/Saturation/Brightness** or **% of appearance**.
* Put your images also into `bin/data/images/`.
* Or drag files into the app window. 
* `ImGui` based. Grid preview and folder browser.
* Threaded quantizer process.

## Dependencies
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxColorQuantizer](https://github.com/moebiussurfing/ofxColorQuantizer)
* ofxOpenCv

## Note
- Sometimes dragging files is not working on *Windows 10*, maybe because of something related to ownership or administration rights.

## Tested Systems
- **Windows10** / **VS2017** / **OF ~0.11+**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020-2023.*  

Original code from **@mantissa**.  
https://github.com/mantissa/ofxColorQuantizer  
Thanks!  

## License
*MIT License.*
