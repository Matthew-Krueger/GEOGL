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



    Application::Application(const WindowProps& props) {

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
                m_Settings.setRenderingAPI(RendererAPI::RENDERING_OPENGL_DESKTOP);
                m_Settings.flush();
            }

        }

        Renderer::setRendererAPI(RendererAPI::create(m_Settings.getRenderingAPI()));
        auto renderAPI = Renderer::getRendererAPI();

        if(m_Settings.getRenderingAPI() != Renderer::getRendererAPI()->getRenderingAPI()) {
            GEOGL_CORE_WARN_NOSTRIP("API Stored as {} does not match engine selected api {}. Setting property.", RendererAPI::getRenderingAPIName(m_Settings.getRenderingAPI()), RendererAPI::getRenderingAPIName(Renderer::getRendererAPI()->getRenderingAPI()));
            m_Settings.setRenderingAPI(Renderer::getRendererAPI()->getRenderingAPI());
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


        /* set up the triangle */
        {

            /* Create a vertex array */
            m_VertexArrayTrianglePerVColor = VertexArray::create();

            std::vector<float> vertices = {-0.5f,-0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f,
                                           0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 1.0f,
                                           0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.8f, 1.0f};

            auto vertexBuffer = VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                  {ShaderDataType::FLOAT3, "a_Position"},
                                                  {ShaderDataType::FLOAT4, "a_Color"}
                                          });
            }

            std::vector<uint32_t> indices = {0,1,2};
            auto indexBuffer = IndexBuffer::create(indices);

            m_VertexArrayTrianglePerVColor->addVertexBuffer(vertexBuffer);
            m_VertexArrayTrianglePerVColor->setIndexBuffer(indexBuffer);

        }

        /* set up the square */
        {

            /* Create a vertex array */
            m_VertexArraySquare = VertexArray::create();

            std::vector<float> vertices = {-0.75f, -0.75f, 0.0f,
                                            0.75f, -0.75f, 0.0f,
                                            0.75f,  0.75f, 0.0f,
                                           -0.75f,  0.75f, 0.0f};

            auto vertexBuffer = VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                {ShaderDataType::FLOAT3, "a_Position"},
                                        });
            }

            std::vector<uint32_t> indices = {0,1,2,0,2,3};
            auto indexBuffer = IndexBuffer::create(indices);

            m_VertexArraySquare->addVertexBuffer(vertexBuffer);
            m_VertexArraySquare->setIndexBuffer(indexBuffer);

        }

        {
            std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
			out vec4 v_Position;
			void main()
			{
				v_Position = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

            std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec4 v_Position;
			void main()
			{
				color = v_Position;
			}
		)";

            m_PerVertexShader = Shader::create(vertexSrc, fragmentSrc);
        }

        {
            std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

            std::string fragmentSrc = R"(
			#version 330 core


			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.03,0.03,0.2,1.0);
			}
		)";

            m_BlueShader = Shader::create(vertexSrc, fragmentSrc);
        }

        Renderer::setClearColor({0.1f,0.1f,0.1f,1.0f});

    }

    Application::~Application() = default;

    void Application::run(){

        GEOGL_CORE_INFO_NOSTRIP("Successfully started application. Entering Loop.");

        while(m_Running){

            Renderer::beginScene();

            m_BlueShader->bind();
            Renderer::submit(m_VertexArraySquare);

            m_PerVertexShader->bind();
            Renderer::submit(m_VertexArrayTrianglePerVColor);


            Renderer::endScene();


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
// while(true);

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