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


#ifndef GEOGL_TWODLAYER_HPP
#define GEOGL_TWODLAYER_HPP

/* If not using a pch, the following include MUST be used in every file that uses GEOGL */
/* #include <GEOGL/Core.hpp> */

namespace Example{

    class TwoDLayer : public GEOGL::Layer{
    public:
        TwoDLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate(GEOGL::TimeStep timeStep) override;
        void onImGuiRender(GEOGL::TimeStep timeStep) override;

        void onEvent(GEOGL::Event& event) override;
        bool onKeyPressedEvent(GEOGL::KeyPressedEvent& event);
        bool onWindowResizeEvent(GEOGL::WindowResizeEvent& windowResizeEvent);

        void pollCameraMovement(GEOGL::TimeStep& timeStep);

    private:
        GEOGL::Ref<GEOGL::VertexArray> m_VertexArrayTrianglePerVColor;
        GEOGL::Ref<GEOGL::VertexArray> m_VertexArraySquare;

        GEOGL::ShaderLibrary m_ShaderLibrary;

        /* The cherno logo is from the cherno game engine series. Copyright belongs with him */
        GEOGL::Ref<GEOGL::Texture2D> m_CheckerboardTexture, m_ChernoLogo;
        glm::vec3 m_SquareColor = {0.2f,0.3f,0.8f};

        GEOGL::OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition;
        float m_CameraRotation = 0.0f;
        float m_CameraSpeed = 3.5f;
        float m_CameraRotSpeed = 180.0f;

        glm::mat4 m_Scalepointoneone;

    };

}

#endif //GEOGL_TWODLAYER_HPP
