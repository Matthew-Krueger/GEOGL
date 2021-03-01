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


#ifndef GEOGL_3DLAYER_HPP
#define GEOGL_3DLAYER_HPP

#include <GEOGL/Core.hpp>

namespace Example{

    class TwoDLayer : public GEOGL::Layer{
    public:
        TwoDLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;

        void onEvent(GEOGL::Event& event);

        void pollCameraMovement();

    private:
        std::shared_ptr<GEOGL::VertexArray> m_VertexArrayTrianglePerVColor;
        std::shared_ptr<GEOGL::VertexArray> m_VertexArraySquare;

        std::shared_ptr<GEOGL::Shader> m_PerVertexShader;
        std::shared_ptr<GEOGL::Shader> m_BlueShader;

        GEOGL::OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition;
        float m_CameraRotation = 0.0f;
        float m_CameraSpeed = .01f;
        float m_CameraRotSpeed = 1.0f;

    };

}

#endif //GEOGL_3DLAYER_HPP
