# GEOGL
GEOGL is a C++ OpenGL Graphics Engine built in C++ for Graphics (not necessarily for games)
## Doxygen Comments
To see a built version of Doxygen, visit https://docs.matthewkrueger.com/#GEOGL and click on GEOGL and the branch that you want.
## Implementation instructions and Wiki Info
See the Page "Related pages" on the generated Doxygen, or visit https://docs.matthewkrueger.com/ci-doxygen/GEOGL/main/pages.html
## History
GEOGL was inspired by a youtube series, The Cherno's Game Engine series. This code is heavily based on his Hazel engine.
However, I have taken a different approach to support and windowing to hopefully support non-real time applications that
require much more explicit control over the graphics hardware. (Think a custom word processor, a visualization app, etc).
I am mostly writing this engine for my own internal use, however, as per the license, anyone may use this engine free
of any charge. I only ask (though I have no legal basis to require) that any modifications be sent back with a pull 
request. 
-----
## Future Plans
Future plans include adding a custom GUI system based loosely on a youtube ThinMatrix windowing system based on Limited
State Machines in an Immediate Mode. Basically, make an easier to use interpretation of the Java AWT library. Basically,
the idea is this for flow
```
    auto yesButton = GEOGL::2DGUI::Button::create();
    yesButton->setCallback(GEOGL_BIND_2DGUI_CALLBACK(processYesCallback));    
```
and then, during the render loop:
```
    GEOGL::Renderer::begin2dRender();
    GEOGL::Renderer::drawGUIComponent(yesButton);
    GEOGL::Renderer::end2dRender();
```
## Additional Info
Until release 1.0, the Public API of GEOGL is not guaranteed to be be consistent between patches, or even commits.