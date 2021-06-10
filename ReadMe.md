ofxColorQuantizerHelper
-----------------------

**openFrameworks** addon to get the dominant colors of a picture. Helps the integration of the original **ofxColorQuantizer** coded by @**mantissa**. 

## Screenshot
![image](/readme_images/Capture1.PNG?raw=true "image")

## Overview
* Drag images into window to get a palette with the dominant colors.  
* Set the desired number of colors.
* Different sorting colors: by **H/S/B** or % of appearance.
* ImGui GUI based. Docking and image folder grid preview and browser.

## Dependencies
* ofxOpenCv
* [ofxImGui](https://github.com/Daandelange/ofxImGui/tree/ofParameters-Helpers-Test) from @**Daandelange**  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* ofxGui

Already packed into ```/libs```. No need to add:  
* [ofxColorQuantizer](https://github.com/moebiussurfing/ofxColorQuantizer) [ FORK ]  

## Note
- Sometimes dragging files is not working on *Windows 10*, maybe because something related to ownership or administration rights.

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*  

Original code from **@mantissa**. Thanks!  
https://github.com/mantissa/ofxColorQuantizer  

## License
*MIT License.*