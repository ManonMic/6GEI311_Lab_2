# 6GEI311 Lab 2
By Jean-Michel Plourde and Manon Michelet.

Implementation of 6GEI311 Lab 1 in Python36

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
* A Windows environment
* The Microsoft DirectShow library installed
* Visual Studio
* Python36 installed

### Installing
- Clone this repo with HTTP or SSH
```
git clone https://github.com/ManonMic/6GEI311_Lab_2.git
```
* Open the project folder in visual studio 
* Set the plateform binary to _x64_ and the configuration to _Release_
* Right-click on the project name in the solution explorer, in _properties_ > _Linker_ > _Input_ > _Additionnal dependencies_ 
    * add _Strmiids.lib_ to the line
* At line 31 in **MediaPlayer.cpp**, change the parameter of the function *RenderFile* that is the string path, for the complete path of a video you wish to play.
* Right-click on the project name in the solution explorer, in _properties_ > General
  * add _videoplayer_ in _Target Name_ line
  * add _.pyd_ in _Target extension_ line
  * add _videoplayer_ in _Configuration Type_ line
* Right-click on the project name in the solution explorer, in _properties_ > VC++ Directories
  * add path to Python36/include in _Include directories_ line
  ```
  C:\Program Files\Python36\include
  ```
  * add path to Python36/include in _Library directories_ line
    ```
    C:\Program Files\Python36\libs
    ```
* Build the solution by pressing _Ctrl + Shift + B_
* Open Powershell or Command Line
* Move to _C:\Users\MyUserName\Documents\Visual Studio 2017\Projects\6GEI311_Lab_2\x64\Release_
* Launch python
```
python
```
* Import the module
```
import videoplayer
```
* Execute the function
```
videoplayer.player()
```
* Use the program
