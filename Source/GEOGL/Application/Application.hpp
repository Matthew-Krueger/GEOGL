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


#ifndef GEOGL_APPLICATION_HPP
#define GEOGL_APPLICATION_HPP

#include "../Rendering/Shader.hpp"


#include "../IO/Window.hpp"
#include "../IO/Events/ApplicationEvent.hpp"
#include "../Layers/LayerStack.hpp"
#include "../ImGui/ImGuiLayer.hpp"
#include "../Rendering/Buffer.hpp"
#include "../Rendering/VertexArray.hpp"
#include "../Rendering/RendererAPI.hpp"
#include "../Rendering/Camera.hpp"
#include <GEOGL/Utils.hpp>


namespace GEOGL{

    /**
     * \brief An abstract representation of an application. To be extended in
     * the client.
     *
     * This class represents the abstract Application, and defines the interfaces
     * that an application must have, as well as stores the variables that an
     * application must support, such as a LayerStack, a Window, or a running variable.
     */
    class GEOGL_API Application{

    public:
        explicit Application(const WindowProps& props = WindowProps());
        virtual ~Application();

        /**
         * \brief Runs the update code for anything that must happen exactly once a frame from an application.
         *
         * Application guarantees that this is called before any other on update functions.
         */
        virtual void onUpdate(TimeStep timeStep){};

        /**
         * Contains the main run loop for the game
         */
        void run();

        /**
         * Handles events for the game
         * @param e
         */
        void onEvent(Event& event);

        inline void pushLayer(Layer* layer) { m_LayerStack.pushLayer(layer); };
        inline void pushOverlay(Layer* layer) { m_LayerStack.pushOverlay(layer); };
        inline void popLayer(Layer* layer) { m_LayerStack.popLayer(layer); };
        inline void popOverlay(Layer* layer) { m_LayerStack.popOverlay(layer); };

        static inline Application& get() { return *Application::s_Instance; };
        inline Window& getWindow() { return *m_Window; };

    private:
        /**
         * Is a callback for when the window is closed
         * @param event The event that asks for the window to be closed
         * @return returns true if successful
         */
        bool onWindowClose(WindowCloseEvent& event);
        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        Settings m_Settings;
        ImGuiLayer* m_ImGuiLayer;

    private:
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;

    };

    // To be defined in the client.
    /**
     * Returns a pointer to an application
     * \note MUST BE IMPLEMENTED IN CLIENT
     * @return Application pointer
     */
    Application* createApplication();

}

#endif //GEOGL_APPLICATION_HPP
