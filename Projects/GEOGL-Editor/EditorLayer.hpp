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


#ifndef GEOGL_EDITORLAYER_HPP
#define GEOGL_EDITORLAYER_HPP

namespace GEOGLEditor{

    class EditorLayer : public GEOGL::Layer{
    public:
        EditorLayer();
        EditorLayer(const EditorLayer&) = delete;
        ~EditorLayer() override = default;


        void onAttach() override;
        void onDetach() override;
        void onUpdate(GEOGL::TimeStep timeStep) override;
        void onImGuiRender(GEOGL::TimeStep timeStep) override;

        void onEvent(GEOGL::Event& event) override;

    private:

        /* Camera utils */
        GEOGL::OrthographicCameraController m_OrthographicCameraController;

        /* framebuffer */
        GEOGL::Ref<GEOGL::Framebuffer> m_EditorViewportFramebuffer;

        /* Textures */
        GEOGL::Ref<GEOGL::Texture2D> m_Checkerboard;
        GEOGL::Ref<GEOGL::Texture2D> m_TextureAtlas;
        GEOGL::Ref<GEOGL::SubTexture2D> m_TextureTree;

        glm::vec2 m_ViewportSize;


    };

}

#endif //GEOGL_EDITORLAYER_HPP
