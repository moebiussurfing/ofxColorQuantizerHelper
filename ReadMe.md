ofxColorQuantizerHelper
-----------------------

**openFrameworks** addon to get the dominant colors of a picture.  
Helps the integration of the original **ofxColorQuantizer** coded by @**mantissa** into an OF app. 

## Screenshots
![](/example_ColorQuatizer/Capture.PNG)  
![](/example_ColorQuatizerGradient/Capture.PNG)  

## Overview
* Get a palette with the dominant colors of a picture.  
  * Set the desired number of colors.
  * Threaded quantizer process.
  * Magnifed color picker. Right Click to show. Ctrl + Mouse wheel to set zoom depth.
* Different sorting modes: by **Hue/Saturation/Brightness** or **% of appearance**.
* Put your images also into `bin/data/images/`.
  * Or drag files into the app window. 
* `ImGui` based. Grid preview and folder browser.

## Dependencies
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxColorQuantizer](https://github.com/moebiussurfing/ofxColorQuantizer) / FORK
* ofxOpenCv / OF CORE

## Note
- Sometimes dragging files is not working on *Windows 10*, maybe because of something related to ownership or administration rights.

## Tested Systems
- **Windows10** / **VS2017** / **OF ~0.11+**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020-2023.*  

Original code from **@mantissa**.  
https://github.com/mantissa/ofxColorQuantizer  
Based on the [ocvColorQuantize](https://github.com/cinder/Cinder-OpenCV/blob/master/samples/ocvColorQuantize) demo that ships with Cinder:  
Thanks!  

## License
*MIT License.*
