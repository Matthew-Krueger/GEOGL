/*******************************************************************************
 * Copyright (c) 2020 Matthew Krueger                                          *
 *                                                                             *
 * This software is provided 'as-is', without any express or implied           *
 * warranty. In no event will the authors be held liable for any damages       *
 * arising from the use of this software.                                      *
 *                                                                             *
 * Permission is granted to anyone to use this software for any purpose,       *
 * including commercial applications, and to alter it and redistribute it      *
 * freely, subject to the following restrictions:                              *
 *                                                                             *
 * 1. The origin of this software must not be misrepresented; you must not     *
 *    claim that you wrote the original software. If you use this software     *
 *    in a product, an acknowledgment in the product documentation would       *
 *    be appreciated but is not required.                                      *
 *                                                                             *
 * 2. Altered source versions must be plainly marked as such, and must not     *
 *    be misrepresented as being the original software.                        *
 *                                                                             *
 * 3. This notice may not be removed or altered from any source                *
 *    distribution.                                                            *
 *                                                                             *
 *******************************************************************************/

#define GEOGL_INCLUDE_MAIN
#define GEOGL_INCLUDE_WIN_MAIN
#include <GEOGL/MainCreator.hpp>

#include "ExampleApplication.hpp"
#include "3DLayer.hpp"

static bool show = true;

namespace Example{

    ExampleApp::ExampleApp() : GEOGL::Application(
            GEOGL::WindowProps(
                    "GEOGL Example",
                    1920,
                    1080,
                    "Resources/Runtime-Icon.png"
            )) {


        GEOGL_INFO_NOSTRIP("Starting Sandbox Application.");
        pushLayer(new TwoDLayer());
        GEOGL::Renderer::setClearColor({0.1f,0.1f,0.1f,1.0f});

        //getWindow().setVSync(false);

    }

    ExampleApp::~ExampleApp(){

        GEOGL_INFO_NOSTRIP("Closing Sandbox Application.");

    }

}

GEOGL::Application* GEOGL::createApplication(){

    GEOGL::Log::Init("example-app-log.txt", "Example App");
    return new Example::ExampleApp();

}