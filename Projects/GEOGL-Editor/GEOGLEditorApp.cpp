/*******************************************************************************
 * Copyright (c) 2021 Matthew Krueger                                          *
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


#include "GEOGLEditorApp.hpp"
#include "EditorLayer.hpp"

#define GEOGL_INCLUDE_MAIN
#define GEOGL_INCLUDE_WIN_MAIN
#include <GEOGL/MainCreator.hpp>

namespace GEOGLEditor{


    GEOGLEditor::GEOGLEditor() : GEOGL::Application({
                "GEOGL Editor",
                "GEOGL-Editor-log.txt",
                1280,
                720,
                "GEOGL-Editor-Resources", // application resource directory
                "Runtime-Icon.png" // window Icon
    }){

        pushLayer(new EditorLayer());

    }

    GEOGLEditor::~GEOGLEditor() {

    }

    void GEOGLEditor::onEvent(GEOGL::Event &event) {

        GEOGL::EventDispatcher dispatcher(event);

        dispatcher.dispatch<GEOGL::KeyPressedEvent>(GEOGL_BIND_EVENT_FN(GEOGLEditor::onKeyPressedEvent));

    }

    void GEOGLEditor::setUpImGui(ImGuiContext *context) {

        ImGui::SetCurrentContext(context);

    }

    bool GEOGLEditor::onKeyPressedEvent(GEOGL::KeyPressedEvent &event) {

        if(event.getKeyCode() == GEOGL::Key::F1){
            getWindow().setVSync(!getWindow().isVSync());
        }

        return false;
    }

}

GEOGL::Application* GEOGL::createApplication() {
    GEOGL_PROFILE_FUNCTION();

    return new GEOGLEditor::GEOGLEditor();

}