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

/*******************************************************************************
 *                                                                             *
 * This code was based heavily off the Cherno game engine series               *
 *                                                                             *
 *******************************************************************************/



#include "ImGuiLayer.hpp"
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>
#include "../Application/Application.hpp"
#include "../Modules/Platform/OpenGL/imgui_impl_opengl3.h"

namespace GEOGL{


    ImGuiLayer::ImGuiLayer()
    : Layer("ImGui Layer"){



    }

    ImGuiLayer::~ImGuiLayer() {

    }

    void ImGuiLayer::onAttach() {

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // TEMPORARY: should eventually use Hazel key codes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

#if GEOGL_BUILD_WITH_OPENGL
        ImGui_ImplOpenGL3_Init("#version 410");
#endif
        GEOGL_CORE_INFO("Attached ImGui layer.");

    }

    void ImGuiLayer::onDetach() {

    }

    void ImGuiLayer::onUpdate() {

#if GEOGL_BUILD_WITH_OPENGL
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();

        io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()), static_cast<float>(app.getWindow().getHeight()));

        auto time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time-m_Time) : (1.0f/60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

    }

    void ImGuiLayer::onEvent(Event &event){

        EventDispatcher dispatcher(event);

        dispatcher.dispatch<MouseButtonPressedEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onMouseButtonPressedEvent));
        dispatcher.dispatch<MouseButtonReleasedEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onMouseButtonReleasedEvent));
        dispatcher.dispatch<MouseMovedEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onMouseMovedEvent));
        dispatcher.dispatch<MouseScrolledEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onMouseScrolledEvent));
        dispatcher.dispatch<KeyPressedEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onKeyPressedEvent));
        dispatcher.dispatch<KeyReleasedEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onKeyReleasedEvent));
        dispatcher.dispatch<KeyTypedEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onKeyTypedEvent));
        dispatcher.dispatch<WindowResizeEvent>(GEOGL_BIND_EVENT_FN(ImGuiLayer::onWindowResizedEvent));

    }

    bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = true;

        return false;

    }

    bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = false;

        return false;

    }

    bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.getX(), event.getY());

        return false;

    }

    bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += event.getXOffset();
        io.MouseWheel += event.getYOffset();

        return false;

    }

    bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.getKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return false;

    }

    bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.getKeyCode()] = false;
        return false;

    }

    bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        int keycode = event.getKeyCode();
        if(keycode>0 && keycode<0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;

    }

    bool ImGuiLayer::onWindowResizedEvent(WindowResizeEvent& event) {

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(event.getWidth()), static_cast<float>(event.getHeight()));
        io.DisplayFramebufferScale = ImVec2(1.0f,1.0f);

        //glViewport(0,0,event.getWidth(), event.getHeight());

        return false;

    }
}
