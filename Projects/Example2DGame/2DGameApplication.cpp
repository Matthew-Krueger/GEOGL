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

#include "2DGameApplication.hpp"
#include "DebugLayer.hpp"
#include "GameLayer.hpp"

#define GEOGL_INCLUDE_MAIN
#define GEOGL_INCLUDE_WIN_MAIN
#include <GEOGL/MainCreator.hpp>

namespace TwoDGame{


    TwoDGameApp::TwoDGameApp() :
            GEOGL::Application(
                    {
                            "2D Game Example",
                            "2d-game-example-log.txt",
                            1920,
                            1080,
                            "Example2DGameResources", // application resource directory
                            "Runtime-Icon.png" // window Icon
                    }
       ){
        GEOGL_PROFILE_FUNCTION();

        m_DebugLayer = new DebugLayer();
        m_DebugLayerOnStack = false;
        pushLayer(new GameLayer());

    }

    TwoDGameApp::~TwoDGameApp() {

        if(!m_DebugLayerOnStack)
        delete m_DebugLayer;

    }

    void TwoDGameApp::onEvent(GEOGL::Event &event) {
        GEOGL_PROFILE_FUNCTION();

        GEOGL::EventDispatcher dispatcher(event);

        dispatcher.dispatch<GEOGL::KeyPressedEvent>(GEOGL_BIND_EVENT_FN(TwoDGameApp::onKeyPressedEvent));

    }

    void TwoDGameApp::setUpImGui(ImGuiContext *context) {
        ImGui::SetCurrentContext(context);
    }

    bool TwoDGameApp::onKeyPressedEvent(GEOGL::KeyPressedEvent &event) {
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

        /* quit if escape */
        if(event.getKeyCode() == GEOGL::Key::Escape){
            GEOGL_INFO("Close requested via escape key. Notifying engine to shutdown on next refresh.");
            setRunning(false);
        }

        return false;

    }

}

GEOGL::Application* GEOGL::createApplication() {
    GEOGL_PROFILE_FUNCTION();

    return new TwoDGame::TwoDGameApp();

}