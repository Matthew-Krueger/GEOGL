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
#include "../Rendering/Renderer2D.hpp"
#include "../IO/Input.hpp"
#include <fstream>

namespace GEOGL{

    Application* Application::s_Instance = nullptr;



    Application::Application(const ApplicationProperties& props){

        if(!GEOGL::Log::isInitialized()){
            GEOGL::Log::init(props.logFileName, props.appName);
        }

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
                m_Settings.data["RenderingAPI"]["API"] = (RendererAPI::RENDERING_OPENGL_DESKTOP);
                m_Settings.flush();
            }

            m_Settings.data["RenderingAPI"]["Info"] = "The RenderingAPI is a bit switched flag, such that 0 is invalid, 1 is OpenGL, 2 is Vulkan, 4 is DirectX11, 8 is DirectX12, 16 is Metal, and so on. This is done to automatically detect the best way to open Windows.";

        }
        /* if doesn't have api */
        Ref<RendererAPI> rendererAPI;
        if(m_Settings.data["RenderingAPI"].contains("API")){
            rendererAPI = RendererAPI::create(m_Settings.data["RenderingAPI"]["API"]);
        }else{
            GEOGL_CORE_WARN_NOSTRIP("Unable to find RenderingAPI in settings.json. Setting a new one.");
            rendererAPI = RendererAPI::create(RendererAPI::RENDERING_INVALID);
        }

        /* set the rendering API */
        Renderer::setRendererAPI(rendererAPI);

        /* now, make sure the data is stored */
        {
            auto renderAPI = Renderer::getRendererAPI();
            m_Settings.data["RenderingAPI"]["API"] = (Renderer::getRendererAPI()->getRenderingAPI());
            m_Settings.flush();
        }

        GEOGL_CORE_INFO_NOSTRIP("Selected Graphics API: {}", RendererAPI::getRenderingAPIName(Renderer::getRendererAPI()->getRenderingAPI()));
        GEOGL_CORE_INFO_NOSTRIP("Calculated best Windowing API: {}", RendererAPI::getWindowingAPIName(Renderer::getRendererAPI()->getWindowingAPI()));

        GEOGL_CORE_ASSERT_NOSTRIP(!s_Instance,"An application already exists.");
        s_Instance = this;

        /* Create window */
        m_Window = Scope<Window>(Window::create(WindowProps(props.appName, props.width, props.height, props.appVersionMajor, props.appVersionMinor, props.appVersionPatch)));
        m_Window->setEventCallback(GEOGL_BIND_EVENT_FN(Application::eventCallback)); // NOLINT(modernize-avoid-bind)
        if(!props.appIconPath.empty())
            m_Window->setWindowIcon(props.applicationResourceDirectory + "/" + props.appIconPath);

        /* Initialize Renderer */
        Renderer::init(props.applicationResourceDirectory);

        /* Initialize ImGuiLayer */
        m_ImGuiLayer = new ImGuiLayer;
        pushOverlay(m_ImGuiLayer);

    }

    Application::~Application(){

        s_Instance = nullptr;
        Renderer2D::shutdown();
        Renderer::shutdown();

    };

    void Application::run(){
        GEOGL_PROFILE_FUNCTION();
        setUpImGui(ImGuiLayer::getImGuiContext());

        GEOGL::Renderer::setClearColor({0.1f,0.1f,0.1f,1.0f});
        GEOGL_CORE_INFO_NOSTRIP("Successfully started application. Entering Loop.");

        while(m_Running){
            GEOGL_PROFILE_SCOPE("Run Loop");

            Renderer::clear();
            /* Measure platform time */
            float time = m_Window->getCurrentPlatformTime();
            TimeStep timeStep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if(!m_Minimized) {
                onUpdate(timeStep);
                GEOGL_PROFILE_SCOPE("Layer Stack Propagation");

                for (Layer *layer : m_LayerStack) {
                    layer->onUpdate(timeStep);
                }

                //std::this_thread::sleep_for(std::chrono::milliseconds(1000));


            }else{
                /* Since we are not running, slow down the loop */
                std::this_thread::sleep_for(std::chrono::milliseconds(1000/5));
            }

            {
                GEOGL_PROFILE_SCOPE("ImGui Render and propagation");
                m_ImGuiLayer->begin();
                for (Layer *layer : m_LayerStack) {
                    layer->onImGuiRender(timeStep);
                }
                m_ImGuiLayer->end();
            }

            m_Window->onUpdate();

        }

    }

    void Application::eventCallback(Event& event){
        GEOGL_PROFILE_FUNCTION();

        /* Firstly, call the application's on event function, which may do other things, such as push or pop layers */
        onEvent(event);
        if(event.Handled)
            return;

        EventDispatcher dispatcher(event);

        /* Bind a Window Close Event to Application::onWindowClose() */
        dispatcher.dispatch<WindowCloseEvent>(GEOGL_BIND_EVENT_FN(Application::onWindowClose)); // NOLINT(modernize-avoid-bind)
        dispatcher.dispatch<WindowResizeEvent>(GEOGL_BIND_EVENT_FN(Application::onWindowResize));
        dispatcher.dispatch<KeyPressedEvent>(GEOGL_BIND_EVENT_FN(Application::onKeyPressedEvent));

        if(event.Handled)
            return;
        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();){
            (*--it)->onEvent(event);
            if(event.Handled){
                break;
            }
        }


    }

    bool Application::onWindowClose(WindowCloseEvent& event){
        GEOGL_PROFILE_FUNCTION();

        m_Running = false;
        event.Handled = true;
        return true;
    }

    bool Application::onKeyPressedEvent(KeyPressedEvent& event){

        if(
                event.getKeyCode() == GEOGL::Key::R &&
                (GEOGL::Input::isKeyPressed(GEOGL::Key::LeftShift) || GEOGL::Input::isKeyPressed(GEOGL::Key::RightShift)) &&
                (GEOGL::Input::isKeyPressed(GEOGL::Key::LeftControl) || GEOGL::Input::isKeyPressed(GEOGL::Key::RightControl))){

            m_Running = false;
            m_ShouldRestart = true;
            event.Handled = true;
            return true;

        }

        return false;

    }

    bool Application::onWindowResize(WindowResizeEvent &event){
        GEOGL_PROFILE_FUNCTION();

        /* Guard against resizing to zero and rendering */
        if(event.getWidth() == 0 || event.getHeight() == 0){
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::onWindowResize({event.getWidth(), event.getHeight()});

        return false;

    }

}