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

#include "SandboxApplication.hpp"
#include "../../Dependencies/imgui-docking/include/ImGui/imgui.h"

namespace Sandbox{

class ExampleLayer: public GEOGL::Layer{

public:
    ExampleLayer():Layer("Example Layer"){};

    void onImGuiRender() override{
        //static bool show = true;
        //ImGui::ShowDemoWindow(&show);
        ImGui::Begin("Test");
        ImGui::Text("Hello World!");
        ImGui::End();
    }

};



    SandboxApp::SandboxApp() {
        GEOGL::APIManager manager;


        GEOGL_INFO_NOSTRIP("Starting Sandbox Application.");
        pushLayer(new ExampleLayer);

    }

    SandboxApp::~SandboxApp(){

        GEOGL_INFO_NOSTRIP("Closing Sandbox Application.");

    }

}

GEOGL::Application* GEOGL::createApplication(){

    GEOGL::Log::Init("log.txt", "Sandbox");
    return new Sandbox::SandboxApp();

}