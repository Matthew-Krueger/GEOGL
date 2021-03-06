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
#include <ImGui/imgui.h>

#include <GEOGL/IO.hpp>

#include "SandboxApp.hpp"
#include "Layer2D.hpp"

namespace SandboxApp{

    SandboxApp::SandboxApp():
            GEOGL::Application(
                    {
                            "Sandbox Application",
                            "sandbox-app-log.txt",
                            1280,
                            720,
                            "SandboxResources", // application resource directory
                            "Runtime-Icon.png" // window Icon
                    }
    ){
        GEOGL_PROFILE_FUNCTION();

        /* Since I want to load fonts in my layers, I need to explicitly set the context right now */
        ImGui::SetCurrentContext(GEOGL::ImGuiLayer::getImGuiContext());

        m_DebugLayer = new DebugLayer();
        m_DebugLayerOnStack = false;
        pushLayer(new Layer2D());

    }

    SandboxApp::~SandboxApp() {

        if(!m_DebugLayerOnStack)
        delete m_DebugLayer;

    }

    void SandboxApp::onEvent(GEOGL::Event &event) {
        GEOGL_PROFILE_FUNCTION();

        GEOGL::EventDispatcher dispatcher(event);

        dispatcher.dispatch<GEOGL::KeyPressedEvent>(GEOGL_BIND_EVENT_FN(SandboxApp::onKeyPressedEvent));

    }

    void SandboxApp::setUpImGui(ImGuiContext* context){
        ImGui::SetCurrentContext(context);
    }

    bool SandboxApp::onKeyPressedEvent(GEOGL::KeyPressedEvent &event) {
        GEOGL_PROFILE_FUNCTION();

        if(event.getKeyCode() == GEOGL::Key::F5) {
            if (m_DebugLayerOnStack) {
                GEOGL::Application::get().popOverlay(m_DebugLayer);
                m_DebugLayerOnStack = false;
            } else {
                GEOGL::Application::get().pushOverlay(m_DebugLayer);
                m_DebugLayerOnStack = true;
            }

            return true;

        }


        if(event.getKeyCode() == GEOGL::Key::F1){
            getWindow().setVSync(!getWindow().isVSync());
        }

        return false;
    }

}

GEOGL::Application* GEOGL::createApplication() {
    GEOGL_PROFILE_FUNCTION();

    return new SandboxApp::SandboxApp();

}