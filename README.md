# Codecatch
A Simple Injectable Garry's Mod Lua Grabber written in C++ - *much like ScriptHook (but [GPLv3'd](LICENSE)!)*

## How to use
Build the project and then inject the library into your Garry's Mod, it should them dump any code ran by the Lua state into a folder, *currently this is hard coded as specified in the disclaimer section.* 

## Disclaimer

### Static Directory
The directory it saves to is hard coded, I know, I apologise but I do not want to touch Garry's mod again so just bodged it to work, however feel free to contribute to the project if you want to change the dump directory.

> This can be done [here](/Codecatch/file.cpp) by modifying the `SaveCode( ... )` function!