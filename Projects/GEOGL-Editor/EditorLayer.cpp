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


#include "EditorLayer.hpp"

namespace GEOGLEditor{


    EditorLayer::EditorLayer() {

    }

    void EditorLayer::onAttach() {

        /* Create the framebuffer */
        {
            GEOGL::FramebufferSpecification framebufferSpecification{};
            framebufferSpecification.width = GEOGL::Application::get().getWindow().getWidth();
            framebufferSpecification.height = GEOGL::Application::get().getWindow().getHeight();
            m_EditorViewportFramebuffer = GEOGL::Framebuffer::create(framebufferSpecification);
        }

        /* Create the orthographic controler and set the debug name for this layer */
        {
            m_OrthographicCameraController = GEOGL::OrthographicCameraController(
                    GEOGL::Application::get().getWindow().getDimensions());
            m_DebugName = "GEOGL Editor Layer";
        }

        /* Set up ImGui */
        {
            ImGui::SetCurrentContext(GEOGL::ImGuiLayer::getImGuiContext());
            ImGuiIO &io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("SandboxResources/Fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
            io.Fonts->Build();
            io.Fonts->AddFontDefault();
            io.IniFilename = "GEOGL-Editor-Resources/imgui-config.ini";
        }

        /* load some textures */
        {

            m_TextureAtlas = GEOGL::Texture2D::create("GEOGL-Editor-Resources/Textures/Kenny-RPG-Pack/RPGpack_sheet_2X.png");
            m_TextureTree = GEOGL::SubTexture2D::createFromCoords(m_TextureAtlas, {2,1}, {128,128}, {1,2});
        }
    }

    void EditorLayer::onDetach() {


    }

    void EditorLayer::onUpdate(GEOGL::TimeStep timeStep) {

        m_OrthographicCameraController.onUpdate(timeStep);

        GEOGL::Renderer2D::resetStats();
        m_EditorViewportFramebuffer->bind();
        GEOGL::Renderer::setClearColor({0.1f,0.1f,0.1f,1.0f});
        GEOGL::RenderCommand::clear();

        GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());

        GEOGL::Renderer2D::drawQuad({{0,0,0},{1,2}}, m_TextureTree);

        GEOGL::Renderer2D::endScene();
        m_EditorViewportFramebuffer->unbind();

    }

    void EditorLayer::onImGuiRender(GEOGL::TimeStep timeStep) {

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen){
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("GEOGL Editor", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
            ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar()){

            /* File Menu */
            if (ImGui::BeginMenu("File")){
                if(ImGui::MenuItem("Exit")){
                    GEOGL::Application::get().close();
                };
                ImGui::EndMenu();
            }

            /* About Menu */
            if(ImGui::BeginMenu("About")){
                if(ImGui::MenuItem("Licenses")){
                    GEOGL_INFO_NOSTRIP("Licenses coming Soon!");
                }
                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }

        uint32_t textureID = m_EditorViewportFramebuffer->getColorAttachmentRendererID();
        ImVec2 framebufferDimensions = {(float)m_EditorViewportFramebuffer->getFramebufferSpecification().width, (float)m_EditorViewportFramebuffer->getFramebufferSpecification().height};

#pragma warning (push)
#pragma warning (disable:4312)
        ImGui::Begin("Window");
        ImGui::Image(reinterpret_cast<void*>(textureID),framebufferDimensions, {0,1}, {1,0});
        ImGui::End();
#pragma warning (pop)
        ImGui::End();

    }

    void EditorLayer::onEvent(GEOGL::Event &event) {

        m_OrthographicCameraController.onEvent(event);


    }
}
