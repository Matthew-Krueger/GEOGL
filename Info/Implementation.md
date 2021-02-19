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
which returns a pointer to the application. This also MUST contain an initialization of
the logging engine, or launch will fail with a SIGSEV bad address. This MUST be called
before creating a new sandbox window. An example of the full function is:
```
GEOGL::Application* GEOGL::createApplication(){
    GEOGL::Log::Init("log.txt", "Sandbox");
    return new Sandbox::SandboxApp();
}
```

-------------------------------------------
To get this pointer, you need to define a subclass of `GEOGL::Application`,
like this:
```
class SandboxApp : public GEOGL::Application {
public:
    SandboxApp();
    ~SandboxApp() override;
};
```
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

## Windows GUI Application
There is no need to create a separate main class for a windows
GUI Application. Simply `#define GEOGL_INCLUDE_WIN_MAIN` and
GEOGL will include a wWinMain function that will forward argc and argv
to your real main function, courtesy of stackoverflow.com. This is automatically
only included on Windows, so no need to guard it for cross platform builds.
