# Implementation
To implement, follow the example Sandbox.
In your translation unit you wish to be your main, do this:
```
#define GEOGL_INCLUDE_MAIN
#define GEOGL_INCLUDE_WIN_MAIN
#include <GEOGL/MainCreator.hpp>
```
and define the function 
```
GEOGL::Application* GEOGL::createApplication();
```
which returns a pointer to the application. An example is below:
```
GEOGL::Application* GEOGL::createApplication(){
    return new Sandbox::ExampleApp();
}
```

-------------------------------------------
To get this pointer, you need to define a subclass of `GEOGL::Application`,
like this:
```
class ExampleApp : public GEOGL::Application {
public:
    ExampleApp();
    ExampleApp() override;
};
```
-------------------------------------------
### ImGui Notes
ImGui is weird with DLLs. The application has a callback `void SandboxApp::setUpImGui(ImGuiContext* context)` that will
be sent the current ImGui context. You can then implement another callback which can set this context
everywhere (every DLL or regular EXE) that you plan to use ImGui
## CMake
Please make sure to include the following as part of your cmake
root.
```
set(PATH_TO_GEOGL Dependencies/GEOGL)
add_subdirectory(${PATH_TO_GEOGL})
```

## Using GEOGL in multiple files
All you need to use GEOGL in another project
is to `#include <GEOGL/Core.h>`, after setting up
your build system to have the correct include directories.
Additional components can be included with the following paths:
```
#include <GEOGL/Core.hpp>
#include <GEOGL/Events.hpp>
#include <GEOGL/IO.hpp>
#include <GEOGL/Layers.hpp>
#include <GEOGL/Renderer.hpp>
```

## Windows GUI Application
There is no need to create a separate main class for a windows
GUI Application. Simply `#define GEOGL_INCLUDE_WIN_MAIN` and
GEOGL will include a wWinMain function that will forward argc and argv
to your real main function, courtesy of stackoverflow.com. This is automatically
only included on Windows, so no need to guard it for cross-platform builds.
