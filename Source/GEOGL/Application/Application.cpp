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


#include "Application.hpp"
#include <GLFW/glfw3.h>
namespace GEOGL{

#define BIND_EVENT_FN(function) std::bind(&function, this, std::placeholders::_1)

    Application::Application() {

        /* Create window */
        m_Window = std::unique_ptr<Window>(Window::create());
        m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent)); // NOLINT(modernize-avoid-bind)

    }

    Application::~Application() = default;

    void Application::run(){

        while(m_Running){
            glClearColor(1,0,1,1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->onUpdate();
        }
// while(true);

    }

    void Application::onEvent(Event& event){

        EventDispatcher dispatcher(event);

        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose)); // NOLINT(modernize-avoid-bind)


        //GEOGL_CORE_INFO("{}", event.toString());

    }

    bool Application::onWindowClose(WindowCloseEvent& event){
        m_Running = false;
        return true;
    }


}