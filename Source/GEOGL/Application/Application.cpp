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
#include <glad/glad.h>

namespace GEOGL{

    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type){
        switch(type){
            case ShaderDataType::FLOAT:
            case ShaderDataType::FLOAT2:
            case ShaderDataType::FLOAT3:
            case ShaderDataType::FLOAT4:
            case ShaderDataType::MAT3:
            case ShaderDataType::MAT4:
                return GL_FLOAT;
            case ShaderDataType::INT:
            case ShaderDataType::INT2:
            case ShaderDataType::INT3:
            case ShaderDataType::INT4:
                return GL_INT;
            case ShaderDataType::BOOLEAN:
                return GL_BOOL;
            default:
            GEOGL_CORE_ASSERT(false, "Unknown shader DataType");
                return 0;
        }
    }

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
                RenderingAPIType api = determineLowestAPI();
                GEOGL_CORE_WARN_NOSTRIP("Settings file not found. Setting lowest API");
                m_Settings.data["api"] = determineLowestAPI();
                m_Settings.flush();
            }

        }

        m_APIManager = std::make_unique<RendererAPI>(RendererAPI(static_cast<RenderingAPIType>(m_Settings.data["api"])));
        if(static_cast<RenderingAPIType>(m_Settings.data["api"]) != m_APIManager->getRenderAPIType()) {
            GEOGL_CORE_WARN_NOSTRIP("API Stored as {} does not match engine selected api {}. Setting property.", apiPrettyPrint(m_Settings.data["api"]), apiPrettyPrint(m_APIManager->getRenderAPIType()));
            m_Settings.data["api"] = m_APIManager->getRenderAPIType();
            m_Settings.flush();
        }

        GEOGL_CORE_INFO_NOSTRIP("Selected Graphics API: {}", apiPrettyPrint(m_APIManager->getRenderAPIType()));
        GEOGL_CORE_INFO_NOSTRIP("Calculated best Windowing API: {}", windowingPrettyPrint(m_APIManager->getWindowingType()));

        GEOGL_CORE_ASSERT_NOSTRIP(!s_Instance,"An application already exists.");
        s_Instance = this;

        /* Create window */
        m_Window = std::unique_ptr<Window>(Window::create(*m_APIManager.get(), props));
        m_Window->setEventCallback(GEOGL_BIND_EVENT_FN(Application::onEvent)); // NOLINT(modernize-avoid-bind)

        /* Initialize ImGuiLayer */
        m_ImGuiLayer = new ImGuiLayer;
        pushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        //glGenBuffers(1, &m_VertexBuffer);
        //glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        std::vector<float> vertices = {-0.5f,-0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f,
                                         0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 1.0f,
                                         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.8f, 1.0f};

        m_VertexBuffer = VertexBuffer::create(vertices);
        {
            BufferLayout layout = {
                    {ShaderDataType::FLOAT3, "a_Position"},
                    {ShaderDataType::FLOAT4, "a_Color"}
            };

            m_VertexBuffer->setLayout(layout);
        }


        uint32_t index = 0;
        for(const BufferElement& element : m_VertexBuffer->getLayout()){

            /* Hopefully this doesn't crash */
            void* elementOffsetPtr = reinterpret_cast<void*>(element.offset);

            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                    index,
                    element.getComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.dataType),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    m_VertexBuffer->getLayout().getStride(),
                    elementOffsetPtr);

            index++;
        }


        std::vector<uint32_t> indices = {0,1,2};
        m_IndexBuffer = IndexBuffer::create(indices);

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

        m_Shader = Shader::create(vertexSrc, fragmentSrc);
    }

    Application::~Application() = default;

    void Application::run(){

        GEOGL_CORE_INFO_NOSTRIP("Successfully started application. Entering Loop.");

        while(m_Running){

            m_Window->clearColor();

            m_Shader->bind();
            //glBindVertexArray(m_VertexArray);
            //m_VertexBuffer->bind();
            //m_IndexBuffer->bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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