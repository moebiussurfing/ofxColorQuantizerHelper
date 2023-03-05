ofxColorQuantizerHelper
-----------------------

**openFrameworks** add-on to get the dominant colors of a picture. Helps the integration of the original **ofxColorQuantizer** coded by @**mantissa**. 

## Screenshot
![](/example/Capture.PNG)

## Overview
* Get a palette with the dominant colors of a picture.  
* Set the desired number of colors.
* Different sorting colors: by **H/S/B** or `%` of appearance.
* Put your images also into `bin/data/images/`.
* Or drag files into the app window. 
* `ImGui` based. Grid preview and folder browser.

## Dependencies
* ofxOpenCv
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  

Already packed into ```/libs```. No need to add:  
* [ofxColorQuantizer](https://github.com/moebiussurfing/ofxColorQuantizer) [ FORK ]  

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