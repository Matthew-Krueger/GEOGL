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
#include <fstream>

namespace GEOGL{

    Application* Application::s_Instance = nullptr;

    Application::Application() {

#ifdef NDEBUG
        std::string releaseMode("Release");
#else
        std::string releaseMode("Debug");
#endif

        GEOGL_CORE_INFO_NOSTRIP("Starting {}, compiled in {} mode.", GEOGL_ENGINE_NAME, releaseMode);

        /* Open settings */
        {
            if(!m_Settings.open("settings.json")) {
                m_Settings.data()["api"] = determineLowestAPI();
                m_Settings.flush();
            }

        }

        /* Make sure the API works */
        switch ((WindowAPIType) m_Settings.data()["api"]){
            case WindowAPIType::WINDOW_OPENGL_DESKTOP:
                GEOGL_CORE_ASSERT_NOSTRIP(GEOGL_BUILD_WITH_OPENGL, "OpenGL Selected, but not supported.");
                break;
            case WindowAPIType::WINDOW_VULKAN_DESKTOP:
                GEOGL_CORE_ASSERT_NOSTRIP(GEOGL_BUILD_WITH_VULKAN, "Vulkan Selected, but not supported.");
                break;
            default:
                GEOGL_CORE_ASSERT_NOSTRIP(false, "No API Selected. Exiting. Set api flag in settings.json");
        }

        GEOGL_CORE_INFO_NOSTRIP("Selected Graphics API: {}", apiPrettyPrint(m_Settings.data()["api"]));

        GEOGL_CORE_ASSERT_NOSTRIP(!s_Instance,"An application already exists.");
        s_Instance = this;

        /* Create window */
        m_Window = std::unique_ptr<Window>(Window::create((WindowAPIType)m_Settings.data()["api"]));
        m_Window->setEventCallback(GEOGL_BIND_EVENT_FN(Application::onEvent)); // NOLINT(modernize-avoid-bind)

        /* Initialize input class */

    }

    Application::~Application() = default;

    void Application::run(){

        GEOGL_CORE_INFO_NOSTRIP("Successfully started application. Entering Loop.");

        while(m_Running){

            m_Window->clearColor();

            for(Layer* layer : m_LayerStack){
                layer->onUpdate();
            }

            m_Window->onUpdate();
        }
// while(true);

    }

    void Application::onEvent(Event& event){

        EventDispatcher dispatcher(event);

        /* Bind a Window Close Event to Application::onWindowClose() */
        dispatcher.dispatch<WindowCloseEvent>(GEOGL_BIND_EVENT_FN(Application::onWindowClose)); // NOLINT(modernize-avoid-bind)

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();){
            (*--it)->onEvent(event);
            if(event.Handled)
                break;
        }

        //GEOGL_CORE_INFO("{}", event.toString());

    }

    void Application::pushLayer(Layer *layer) {
        m_LayerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer *layer) {
        m_LayerStack.pushOverlay(layer);
    }

    bool Application::onWindowClose(WindowCloseEvent& event){
        m_Running = false;
        return true;
    }


}