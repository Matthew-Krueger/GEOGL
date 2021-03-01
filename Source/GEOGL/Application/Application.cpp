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
#include "../Rendering/Renderer.hpp"
#include <fstream>

namespace GEOGL{

    Application* Application::s_Instance = nullptr;



    Application::Application(const WindowProps& props){

#ifdef NDEBUG
        std::string releaseMode("Release");
#else
        std::string releaseMode("Debug");
#endif

        GEOGL_CORE_INFO_NOSTRIP("Starting {}, compiled in {} mode.", GEOGL_ENGINE_NAME, releaseMode);

        /* Open settings */
        {
            if(!m_Settings.open("settings.json")) {
                GEOGL_CORE_WARN_NOSTRIP("Settings file not found. Treating OpenGL as the lowest API");
                m_Settings.data["RenderingAPI"] = (RendererAPI::RENDERING_OPENGL_DESKTOP);
                m_Settings.flush();
            }

        }
        /* if doesn't have api */
        std::shared_ptr<RendererAPI> rendererAPI;
        if(m_Settings.data.contains("RenderingAPI")){
            rendererAPI = RendererAPI::create(m_Settings.data["RenderingAPI"]);
        }else{
            GEOGL_CORE_WARN_NOSTRIP("Unable to find RenderingAPI in settings.json. Setting a new one.");
            rendererAPI = RendererAPI::create(RendererAPI::RENDERING_INVALID);
        }

        /* set the rendering API */
        Renderer::setRendererAPI(rendererAPI);

        /* now, make sure the data is stored */
        {
            auto renderAPI = Renderer::getRendererAPI();
            m_Settings.data["RenderingAPI"] = (Renderer::getRendererAPI()->getRenderingAPI());
            m_Settings.flush();
        }

        GEOGL_CORE_INFO_NOSTRIP("Selected Graphics API: {}", RendererAPI::getRenderingAPIName(Renderer::getRendererAPI()->getRenderingAPI()));
        GEOGL_CORE_INFO_NOSTRIP("Calculated best Windowing API: {}", RendererAPI::getWindowingAPIName(Renderer::getRendererAPI()->getWindowingAPI()));

        GEOGL_CORE_ASSERT_NOSTRIP(!s_Instance,"An application already exists.");
        s_Instance = this;

        /* Create window */
        m_Window = std::unique_ptr<Window>(Window::create(props));
        m_Window->setEventCallback(GEOGL_BIND_EVENT_FN(Application::onEvent)); // NOLINT(modernize-avoid-bind)

        /* Initialize ImGuiLayer */
        m_ImGuiLayer = new ImGuiLayer;
        pushOverlay(m_ImGuiLayer);

    }

    Application::~Application() = default;

    void Application::run(){

        GEOGL_CORE_INFO_NOSTRIP("Successfully started application. Entering Loop.");

        while(m_Running){

            GEOGL::Renderer::clear();
            onUpdate();

            for(Layer* layer : m_LayerStack){
                layer->onUpdate();
            }

            m_ImGuiLayer->begin();
            for(Layer* layer : m_LayerStack){
                layer->onImGuiRender();
            }
            m_ImGuiLayer->end();

            m_Window->onUpdate();

        }

    }

    void Application::onEvent(Event& event){

        EventDispatcher dispatcher(event);

        /* Bind a Window Close Event to Application::onWindowClose() */
        dispatcher.dispatch<WindowCloseEvent>(GEOGL_BIND_EVENT_FN(Application::onWindowClose)); // NOLINT(modernize-avoid-bind)

        if(event.Handled)
            return;
        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();){
            (*--it)->onEvent(event);
            if(event.Handled){
                break;
            }
        }

        //GEOGL_CORE_INFO("{}", event.toString());

    }

    bool Application::onWindowClose(WindowCloseEvent& event){
        m_Running = false;
        event.Handled = true;
        return true;
    }


}